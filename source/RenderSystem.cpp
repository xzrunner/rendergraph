#include "rendergraph/RenderSystem.h"

#include <unirender/Shader.h>
#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>

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

namespace rg
{

CU_SINGLETON_DEFINITION(RenderSystem);

RenderSystem::RenderSystem()
{
    InitShader();
}

void RenderSystem::DrawTextureToScreen(unsigned int tex_id) const
{
    if (!m_shader) {
        return;
    }

    auto& rc = ur::Blackboard::Instance()->GetRenderContext();

    rc.BindTexture(tex_id, 0);

    m_shader->Use();
    rc.RenderCube();
}

void RenderSystem::InitShader()
{
    auto& rc = ur::Blackboard::Instance()->GetRenderContext();

    std::vector<std::string> textures;
    textures.push_back("texture1");
    CU_VEC<ur::VertexAttrib> va_list;    
    m_shader = std::make_shared<ur::Shader>(&rc, vert.c_str(), frag.c_str(), textures, va_list, true);
}

}