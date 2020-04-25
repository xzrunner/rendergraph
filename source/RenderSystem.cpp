#include "rendergraph/RenderSystem.h"
#include "rendergraph/RenderContext.h"

#include <unirender2/Context.h>
#include <unirender2/DrawState.h>
#include <unirender2/Device.h>

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
                                       const ur2::Texture& tex) const
{
    if (!m_prog) {
        m_prog = rc.ur_dev->CreateShaderProgram(vert, frag);
        assert(m_prog);
    }

    ur2::DrawState ds = rc.ur_ds;
    ds.render_state = rc.ur_ds.render_state;
    ds.program      = m_prog;
    ds.vertex_array = rc.ur_dev->GetVertexArray(ur2::Device::PrimitiveType::Cube, ur2::VertexLayoutType::PosTex);
    rc.ur_ctx->Draw(ur2::PrimitiveType::Triangles, ds, nullptr);
}

}