#include "rendergraph/node/Shader.h"
#include "rendergraph/Evaluator.h"

#include <unirender/Shader.h>
#include <unirender/VertexAttrib.h>
#include <cpputil/StringHelper.h>

namespace rg
{
namespace node
{

void Shader::Execute(ur::RenderContext& rc)
{
    if (!m_shader && !m_vert.empty() && !m_frag.empty())
    {
        std::vector<std::string> textures;
        CU_VEC<ur::VertexAttrib> va_list;

        auto vert = m_vert, frag = m_frag;
        cpputil::StringHelper::ReplaceAll(vert, "\\n", "\n");
        cpputil::StringHelper::ReplaceAll(frag, "\\n", "\n");
        m_shader = std::make_shared<ur::Shader>(
            &rc, vert.c_str(), frag.c_str(), textures, va_list, true
        );
    }
}

void Shader::SetCodes(const std::string& vert, const std::string& frag)
{
    if (m_vert == vert && m_frag == frag) {
        return;
    }

    m_vert = vert;
    m_frag = frag;

    m_shader.reset();
}

void Shader::Bind()
{
    if (!m_shader) {
        return;
    }

    m_shader->Use();

    for (int i = 1, n = m_imports.size(); i < n; ++i)
    {
        auto& var = m_imports[i].var;
        switch (var.type)
        {
        case VariableType::Vector1:
        {
            auto v = Evaluator::Calc(m_imports[i], VariableType::Vector1);
            m_shader->SetFloat(var.name, v.vec1);
        }
            break;
        case VariableType::Vector2:
        {
            auto v = Evaluator::Calc(m_imports[i], VariableType::Vector2);
            m_shader->SetVec2(var.name, v.vec2.xy);
        }
            break;
        case VariableType::Vector3:
        {
            auto v = Evaluator::Calc(m_imports[i], VariableType::Vector3);
            m_shader->SetVec3(var.name, v.vec3.xyz);
        }
            break;
        case VariableType::Vector4:
        {
            auto v = Evaluator::Calc(m_imports[i], VariableType::Vector4);
            m_shader->SetVec4(var.name, v.vec4.xyzw);
        }
            break;
        case VariableType::Matrix2:
            break;
        case VariableType::Matrix3:
        {
            auto v = Evaluator::Calc(m_imports[i], VariableType::Matrix3);
            m_shader->SetMat3(var.name, v.mat3.x);
        }
            break;
        case VariableType::Matrix4:
        {
            auto v = Evaluator::Calc(m_imports[i], VariableType::Vector4);
            m_shader->SetVec4(var.name, v.mat4.x);
        }
            break;
        }
    }
}

}
}