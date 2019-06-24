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

void Shader::SetCodes(const std::string& vert, const std::string& frag)
{
    if (m_vert == vert && m_frag == frag) {
        return;
    }

    m_vert = vert;
    m_frag = frag;

    m_shader.reset();
    m_textures.clear();

    m_imports.clear();
    std::vector<Variable> uniforms;
    std::set<std::string> names;
    GetCodeUniforms(m_vert, uniforms, names);
    GetCodeUniforms(m_frag, uniforms, names);
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
    if (!m_shader) {
        Init(rc);
    }
    if (!m_shader || !m_shader->IsValid()) {
        return;
    }

    m_shader->Use();

    std::vector<uint32_t> texture_ids;
    for (int i = 0, n = m_imports.size(); i < n; ++i)
    {
        auto& key = m_imports[i].var;

        uint32_t flags = 0;
        auto val = Evaluator::Calc(rc, m_imports[i], key.type, key.count, flags);
        SetUniformValue(key, val, texture_ids);

        if (flags & Evaluator::FLAG_MODEL_MAT) {
            m_unif_names.Add(pt0::UniformTypes::ModelMat, key.GetDisplayName());
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
        Init(rc);
    }
    return m_shader;
}

void Shader::SetUniformValue(const RenderContext& rc, const std::string& key, 
                             const ShaderVariant& val)
{
    if (!m_shader) {
        Init(rc);
    }
    if (!m_shader || !m_shader->IsValid()) {
        return;
    }

    int key_idx = -1;
    for (int i = 0, n = m_imports.size(); i < n; ++i) {
        if (m_imports[i].var.name == key) {
            key_idx = i;
            break;
        }
    }

    if (key_idx < 0) {
        return;
    }

    std::vector<uint32_t> texture_ids;
    SetUniformValue(m_imports[key_idx].var, val, texture_ids);
}

void Shader::GetCodeUniforms(const std::string& code, std::vector<Variable>& uniforms,
                             std::set<std::string>& unique_names)
{
    auto fixed = code;
    cpputil::StringHelper::ReplaceAll(fixed, "\\n", "\n");

    ShaderParser parser(fixed);
    parser.Parse();

    auto& unifs = parser.GetUniforms();
    for (auto& u : unifs) {
        if (unique_names.find(u.name) == unique_names.end()) {
            uniforms.push_back(u);
            unique_names.insert(u.name);
        }
    }
}

void Shader::Init(const RenderContext& rc)
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

void Shader::SetUniformValue(const Variable& k, const ShaderVariant& v,
                             std::vector<uint32_t>& texture_ids)
{
    switch (v.type)
    {
    case VariableType::Int:
        m_shader->SetInt(k.name, v.i);
        break;
    case VariableType::Bool:
        m_shader->SetInt(k.name, v.b ? 1 : 0);
        break;
    case VariableType::Vector1:
        m_shader->SetFloat(k.name, v.vec1);
        break;
    case VariableType::Vector2:
        m_shader->SetVec2(k.name, v.vec2.xy);
        break;
    case VariableType::Vector3:
        m_shader->SetVec3(k.name, v.vec3.xyz);
        break;
    case VariableType::Vector4:
        m_shader->SetVec4(k.name, v.vec4.xyzw);
        break;
    case VariableType::Matrix2:
        break;
    case VariableType::Matrix3:
        m_shader->SetMat3(k.name, v.mat3.x);
        break;
    case VariableType::Matrix4:
        m_shader->SetMat4(k.name, v.mat4.x);
        break;
    case VariableType::Sampler2D:
    case VariableType::SamplerCube:
        texture_ids.push_back(v.res_id);
        break;
    case VariableType::Vec3Array:
        for (int i = 0, n = v.vec3_array.size(); i < n; ++i) {
            auto name = k.user_type + "[" + std::to_string(i) + "]." + k.name;
            m_shader->SetVec3(name, v.vec3_array[i].xyz);
        }
        break;
    case VariableType::Vec4Array:
        for (int i = 0, n = v.vec4_array.size(); i < n; ++i) {
            auto name = k.user_type + "[" + std::to_string(i) + "]." + k.name;
            m_shader->SetVec4(name, v.vec4_array[i].xyzw);
        }
        break;
    }
}

}
}