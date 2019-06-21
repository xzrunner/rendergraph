#include "rendergraph/node/Shader.h"
#include "rendergraph/node/ShaderParser.h"
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
        CU_VEC<ur::VertexAttrib> va_list;

        auto vert = m_vert, frag = m_frag;
        cpputil::StringHelper::ReplaceAll(vert, "\\n", "\n");
        cpputil::StringHelper::ReplaceAll(frag, "\\n", "\n");
        m_shader = std::make_shared<ur::Shader>(
            &rc.rc, vert.c_str(), frag.c_str(), m_textures, va_list, true
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
    m_textures.clear();

    m_imports.erase(m_imports.begin() + 1, m_imports.end());
    std::vector<Variable> uniforms;
    GetCodeUniforms(m_vert, uniforms);
    GetCodeUniforms(m_frag, uniforms);
    for (auto& u : uniforms)
    {
        m_imports.push_back(u);
        if (u.type == VariableType::Sampler2D ||
            u.type == VariableType::SamplerCube) {
            m_textures.push_back(u.name);
        }
    }
}

void Shader::Bind(const RenderContext& rc)
{
    if (!m_shader || !m_shader->IsValid()) {
        return;
    }

    m_shader->Use();

    std::vector<uint32_t> texture_ids;
    for (int i = 1, n = m_imports.size(); i < n; ++i)
    {
        auto& var = m_imports[i].var;

        uint32_t flags = 0;
        auto v = Evaluator::Calc(rc, m_imports[i], var.type, flags);

        switch (var.type)
        {
        case VariableType::Bool:
            m_shader->SetInt(var.name, v.b ? 1 : 0);
            break;
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
        case VariableType::Sampler2D:
        case VariableType::SamplerCube:
            texture_ids.push_back(v.id);
            break;
        }

        if (flags & Evaluator::FLAG_MODEL_MAT) {
            m_unif_names.Add(pt0::UniformTypes::ModelMat, var.name);
        }
    }

    if (!texture_ids.empty()) {
        m_shader->SetUsedTextures(texture_ids);
        m_shader->Use();    // fixme: to call BindTexture()
    }
}

std::shared_ptr<ur::Shader> Shader::GetShader(const RenderContext& rc)
{
    if (!m_shader) {
        Execute(rc);
    }
    return m_shader;
}

void Shader::GetCodeUniforms(const std::string& code, std::vector<Variable>& uniforms)
{
    auto fixed = code;
    cpputil::StringHelper::ReplaceAll(fixed, "\\n", "\n");

    ShaderParser parser(fixed);
    parser.Parse();

    auto& unifs = parser.GetUniforms();
    std::copy(unifs.begin(), unifs.end(), std::back_inserter(uniforms));
}

}
}