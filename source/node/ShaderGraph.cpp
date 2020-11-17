#include "rendergraph/node/ShaderGraph.h"
#include "rendergraph/RenderContext.h"

#include <unirender/Device.h>
#include <unirender/Context.h>
#include <unirender/ShaderProgram.h>
#include <shadertrans/ShaderTrans.h>
#include <shadergraph/VarNames.h>
#include <shadergraph/block/Time.h>
#include <cpputil/StringHelper.h>
#include <painting0/ModelMatUpdater.h>

namespace rendergraph
{
namespace node
{

void ShaderGraph::Execute(const std::shared_ptr<dag::Context>& ctx)
{
	if (!m_prog) {
		return;
	}

	auto model_updater = m_prog->QueryUniformUpdater(ur::GetUpdaterTypeID<pt0::ModelMatUpdater>());
	if (model_updater) {
		std::static_pointer_cast<pt0::ModelMatUpdater>(model_updater)->Update(sm::mat4());
	}

	auto rc = std::static_pointer_cast<RenderContext>(ctx);
	rc->ur_ds.program = m_prog;

	for (auto& t : m_textures) {
		rc->ur_ctx->SetTexture(t.first, t.second);
	}
}

void ShaderGraph::Init(const ur::Device& dev, const std::string& vs, const std::string& fs,
	                   const std::vector<std::pair<std::string, ur::TexturePtr>>& textures)
{
	if (fs.empty()) {
		return;
	}

	m_frag = fs;
	m_vert = vs;
	if (m_vert.empty())
	{
		switch (m_vert_shader)
		{
		case VertexShader::Image:
			m_vert = R"(
	#version 330 core
	layout (location = 0) in vec2 aPos;
	layout (location = 1) in vec2 aTexCoord;

	out vec2 TexCoord;

	void main()
	{
		gl_Position = vec4(aPos, 0.0, 1.0);
		TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	}
	)";
			break;
		case VertexShader::Model:
			m_vert = R"(
	#version 330 core
	layout (location = 0) in vec3 position;
	layout (location = 1) in vec2 texcoord;

	out vec2 TexCoord;
	out vec4 #frag_pos#;

	layout(std140) uniform UBO_VS
	{
		mat4 projection;
		mat4 view;
		mat4 model;
	} ubo_vs;

	void main()
	{
		TexCoord = vec2(texcoord.x, texcoord.y);

		#frag_pos# = vec4(position, 1.0);
		gl_Position = ubo_vs.projection * ubo_vs.view * ubo_vs.model * #frag_pos#;
	}
	)";
			cpputil::StringHelper::ReplaceAll(m_vert, "#frag_pos#", shadergraph::VarNames::FragInputs::frag_pos);
			break;
		}
	}

	std::vector<unsigned int> _vs, _fs;
	shadertrans::ShaderTrans::GLSL2SpirV(shadertrans::ShaderStage::VertexShader, m_vert, _vs);
	shadertrans::ShaderTrans::GLSL2SpirV(shadertrans::ShaderStage::PixelShader, m_frag, _fs);

	m_prog = dev.CreateShaderProgram(_vs, _fs);	

	m_textures.clear();
	for (auto& tex : textures)
	{
		auto slot = m_prog->QueryTexSlot(tex.first);
		if (slot >= 0) {
			m_textures.push_back({ slot, tex.second });
		}
	}
}

}
}