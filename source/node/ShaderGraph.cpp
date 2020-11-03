#include "rendergraph/node/ShaderGraph.h"
#include "rendergraph/RenderContext.h"

#include <unirender/Device.h>
#include <shadertrans/ShaderTrans.h>

namespace rendergraph
{
namespace node
{

void ShaderGraph::Execute(const std::shared_ptr<dag::Context>& ctx)
{
	auto rc = std::static_pointer_cast<RenderContext>(ctx);
	rc->ur_ds.program = m_prog;
}

void ShaderGraph::Init(const ur::Device& dev, const std::string& fs,
	                   const std::vector<std::pair<size_t, ur::TexturePtr>>& textures)
{
	if (fs.empty()) {
		return;
	}
	
	std::string vs;
	switch (m_vert_shader)
	{
	case VertexShader::Image:
		vs = R"(
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
		vs = R"(
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
		break;
	}

	std::vector<unsigned int> _vs, _fs;
	shadertrans::ShaderTrans::GLSL2SpirV(shadertrans::ShaderStage::VertexShader, vs, _vs);
	shadertrans::ShaderTrans::GLSL2SpirV(shadertrans::ShaderStage::PixelShader, fs, _fs);

	m_prog = dev.CreateShaderProgram(_vs, _fs);	
}

}
}