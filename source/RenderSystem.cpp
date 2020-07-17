#include "rendergraph/RenderSystem.h"
#include "rendergraph/RenderContext.h"

#include <unirender/Context.h>
#include <unirender/DrawState.h>
#include <unirender/Device.h>
#include <shadertrans/ShaderTrans.h>

#include <string>

namespace
{

const std::string vert = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
)" + 1;

const std::string frag = R"(
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoord);
}
)" + 1;

}

namespace rendergraph
{

CU_SINGLETON_DEFINITION(RenderSystem);

RenderSystem::RenderSystem()
{
}

void RenderSystem::DrawTextureToScreen(const RenderContext& rc,
                                       const ur::Texture& tex) const
{
    if (!m_prog) 
    {
        std::vector<unsigned int> vs, fs;
        shadertrans::ShaderTrans::GLSL2SpirV(shadertrans::ShaderStage::VertexShader, vert, vs);
        shadertrans::ShaderTrans::GLSL2SpirV(shadertrans::ShaderStage::PixelShader, frag, fs);
        m_prog = rc.ur_dev->CreateShaderProgram(vs, fs);
        assert(m_prog);
    }

    ur::DrawState ds = rc.ur_ds;
    ds.render_state = rc.ur_ds.render_state;
    ds.program      = m_prog;
    ds.vertex_array = rc.ur_dev->GetVertexArray(ur::Device::PrimitiveType::Cube, ur::VertexLayoutType::PosTex);
    rc.ur_ctx->Draw(ur::PrimitiveType::Triangles, ds, nullptr);
}

}