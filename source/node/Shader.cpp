#include "rendergraph/node/Shader.h"
#include "rendergraph/Evaluator.h"
#include "rendergraph/Variable.h"
#include "rendergraph/RenderContext.h"

#include <unirender/Shader.h>
#include <unirender/VertexAttrib.h>
#include <cpputil/StringHelper.h>

namespace rg
{
namespace node
{

void Shader::Execute(const RenderContext& rc)
{
    if (!m_shader && !m_vert.empty() && !m_frag.empty())
    {
        std::vector<std::string> textures;
        CU_VEC<ur::VertexAttrib> va_list;

        auto vert = m_vert, frag = m_frag;
        cpputil::StringHelper::ReplaceAll(vert, "\\n", "\n");
        cpputil::StringHelper::ReplaceAll(frag, "\\n", "\n");
        m_shader = std::make_shared<ur::Shader>(
            &rc.rc, vert.c_str(), frag.c_str(), textures, va_list, true
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

    m_imports.erase(m_imports.begin() + 1, m_imports.end());
    std::vector<Variable> uniforms;
    GetCodeUniforms(m_vert, uniforms);
    GetCodeUniforms(m_frag, uniforms);
    for (auto& u : uniforms) {
        m_imports.push_back(u);
    }
}

void Shader::Bind(const RenderContext& rc)
{
    if (!m_shader) {
        return;
    }

    m_shader->Use();

    for (int i = 1, n = m_imports.size(); i < n; ++i)
    {
        auto& var = m_imports[i].var;
        auto v = Evaluator::Calc(rc, m_imports[i], var.type);
        switch (var.type)
        {
        case VariableType::Vector1:
            m_shader->SetFloat(var.name, v.vec1);
            break;
        case VariableType::Vector2:
            m_shader->SetVec2(var.name, v.vec2.xy);
            break;
        case VariableType::Vector3:
            m_shader->SetVec3(var.name, v.vec3.xyz);
            break;
        case VariableType::Vector4:
            m_shader->SetVec4(var.name, v.vec4.xyzw);
            break;
        case VariableType::Matrix2:
            break;
        case VariableType::Matrix3:
            m_shader->SetMat3(var.name, v.mat3.x);
            break;
        case VariableType::Matrix4:
            m_shader->SetMat4(var.name, v.mat4.x);
            break;
        }
    }
}

void Shader::GetCodeUniforms(const std::string& code, std::vector<Variable>& uniforms)
{
    std::vector<std::string> tokens;

    auto fixed = code;
    cpputil::StringHelper::ReplaceAll(fixed, "\\n", "\n");
    cpputil::StringHelper::Split(fixed, "\n; ", tokens);

    if (tokens.empty()) {
        return;
    }

    int ptr = 0;
    do {
        if (tokens[ptr] != "uniform") {
            continue;
        }

        assert(ptr < static_cast<int>(tokens.size() - 2));
        ++ptr;
        auto& type_str = tokens[ptr];
        VariableType type;
        if (type_str == "float") {
            type = VariableType::Vector1;
        } else if (type_str == "vec2") {
            type = VariableType::Vector2;
        } else if (type_str == "vec3") {
            type = VariableType::Vector3;
        } else if (type_str == "vec4") {
            type = VariableType::Vector4;
        } else if (type_str == "mat2") {
            type = VariableType::Matrix2;
        } else if (type_str == "mat3") {
            type = VariableType::Matrix3;
        } else if (type_str == "mat4") {
            type = VariableType::Matrix4;
        } else if (type_str == "sampler2D") {
            type = VariableType::Sampler2D;
        }

        ++ptr;
        auto& name = tokens[ptr];

        uniforms.push_back(Variable({ type, name }));
    } while (++ptr != tokens.size());
}

}
}