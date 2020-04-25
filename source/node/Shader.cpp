#include "rendergraph/node/Shader.h"
#include "rendergraph/node/ShaderParser.h"
#include "rendergraph/Evaluator.h"
#include "rendergraph/Variable.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/Utility.h"

#include <unirender2/Device.h>
#include <unirender2/Context.h>
#include <unirender2/ShaderProgram.h>
#include <unirender2/Uniform.h>

namespace rendergraph
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

    m_prog.reset();
    m_textures.clear();

    m_imports.clear();
    std::vector<Variable> uniforms;
    std::set<std::string> names;
    GetCodeUniforms(m_vert, uniforms, names);
    GetCodeUniforms(m_frag, uniforms, names);
    for (auto& u : uniforms)
    {
        dag::Node<rendergraph::Variable>::Port port;
        port.var.type = u;
        m_imports.push_back(port);

        if (u.type == VariableType::Sampler2D ||
            u.type == VariableType::SamplerCube) {
            m_textures.push_back(u.name);
        }
    }
}

void Shader::Bind(RenderContext& rc)
{
    if (!m_prog) {
        Init(rc);
    }
    if (!m_prog || !m_prog->CheckStatus()) {
        return;
    }

    rc.ur_ds.program = m_prog;
    for (auto& ip : m_imports)
    {
        uint32_t flags = 0;
        auto val = Evaluator::Calc(rc, ip, ip.var.type.type, ip.var.type.count, flags);
        std::vector<uint32_t> texture_ids;
        SetUniformValue(ip.var.type, val, texture_ids);

        if (val.type == VariableType::Sampler2D ||
            val.type == VariableType::SamplerCube)
        {
            ur2::TexturePtr tex = *reinterpret_cast<const ur2::TexturePtr*>(val.p);
            rc.ur_ctx->SetTexture(m_prog->QueryTexSlot(ip.var.type.name), tex);
        }
    }

//    m_prog->Bind();

    //std::vector<uint32_t> texture_ids;
    //for (int i = 0, n = m_imports.size(); i < n; ++i)
    //{
    //    auto& key = m_imports[i].var;

    //    uint32_t flags = 0;
    //    auto val = Evaluator::Calc(rc, m_imports[i], key.type.type, key.type.count, flags);
    //    SetUniformValue(key.type, val, texture_ids);

    //    if (flags & Evaluator::FLAG_MODEL_MAT) {
    //        m_unif_names.Add(pt0::UniformTypes::ModelMat, key.type.GetDisplayName());
    //    }
    //}

    //if (!texture_ids.empty()) {
    //    assert(m_textures.size() == texture_ids.size());
    //    m_prog->SetUsedTextures(texture_ids);
    //    m_prog->Use();    // fixme: to call BindTexture()
    //}
}

std::shared_ptr<ur2::ShaderProgram> Shader::GetShader(const RenderContext& rc)
{
    if (!m_prog) {
        Init(rc);
    }
    return m_prog;
}

void Shader::SetUniformValue(const RenderContext& rc, const std::string& key,
                             const ShaderVariant& val)
{
    if (!m_prog) {
        Init(rc);
    }
    if (!m_prog || !m_prog->CheckStatus()) {
        return;
    }

    int key_idx = -1;
    for (int i = 0, n = m_imports.size(); i < n; ++i) {
        if (m_imports[i].var.full_name == key) {
            key_idx = i;
            break;
        }
    }

    if (key_idx < 0) {
        return;
    }

    std::vector<uint32_t> texture_ids;
    SetUniformValue(m_imports[key_idx].var.type, val, texture_ids);



    //if (!texture_ids.empty())
    //{
    //    assert(texture_ids.size() == 1);
    //    int tex_channel = -1;
    //    for (int i = 0, n = m_textures.size(); i < n; ++i) {
    //        if (m_textures[i] == m_imports[key_idx].var.full_name) {
    //            tex_channel = i;
    //            break;
    //        }
    //    }
    //    assert(tex_channel != -1);
    //    ur_rc.BindTexture(texture_ids[0], tex_channel);
    //}
}

void Shader::GetCodeUniforms(const std::string& code, std::vector<Variable>& uniforms,
                             std::set<std::string>& unique_names)
{
    auto formated = Utility::FormatCode(code);
    ShaderParser parser(formated);
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
    if (!m_prog && !m_vert.empty() && !m_frag.empty())
    {
        auto vert = Utility::FormatCode(m_vert);
        auto frag = Utility::FormatCode(m_frag);
        m_prog = rc.ur_dev->CreateShaderProgram(vert, frag);
    }
}

void Shader::SetUniformValue(const Variable& k, const ShaderVariant& v,
                             std::vector<uint32_t>& texture_ids)
{
    auto uniform = m_prog->QueryUniform(k.name);
    assert(uniform);
    if (!uniform) {
        return;
    }

    switch (v.type)
    {
    case VariableType::Int:
        uniform->SetValue(&v.i, 1);
        break;
    case VariableType::Bool:
    {
        int i = v.b ? 1 : 0;
        uniform->SetValue(&i, 1);
    }
        break;
    case VariableType::Vector1:
        uniform->SetValue(&v.vec1, 1);
        break;
    case VariableType::Vector2:
        uniform->SetValue(v.vec2.xy, 1);
        break;
    case VariableType::Vector3:
        uniform->SetValue(v.vec3.xyz, 1);
        break;
    case VariableType::Vector4:
        uniform->SetValue(v.vec4.xyzw, 1);
        break;
    case VariableType::Matrix2:
        uniform->SetValue(v.mat2.x, 1);
        break;
    case VariableType::Matrix3:
        uniform->SetValue(v.mat3.x, 1);
        break;
    case VariableType::Matrix4:
        uniform->SetValue(v.mat4.x, 1);
        break;
    case VariableType::Sampler2D:
    case VariableType::SamplerCube:
        //texture_ids.push_back(v.res_id);
        break;
    case VariableType::Vec1Array:
        uniform->SetValue(v.vec1_array.data(), v.vec1_array.size());
        //for (int i = 0, n = v.vec1_array.size(); i < n; ++i) {
        //    auto name = k.user_type + "[" + std::to_string(i) + "]." + k.name;
        //    m_prog->SetFloat(name, v.vec1_array[i]);
        //}
        break;
    case VariableType::Vec2Array:
        uniform->SetValue(v.vec2_array[0].xy, v.vec2_array.size());
        //for (int i = 0, n = v.vec2_array.size(); i < n; ++i) {
        //    auto name = k.user_type + "[" + std::to_string(i) + "]." + k.name;
        //    m_prog->SetVec2(name, v.vec2_array[i].xy);
        //}
        break;
    case VariableType::Vec3Array:
        uniform->SetValue(v.vec3_array[0].xyz, v.vec3_array.size());
        //for (int i = 0, n = v.vec3_array.size(); i < n; ++i) {
        //    auto name = k.user_type + "[" + std::to_string(i) + "]." + k.name;
        //    m_prog->SetVec3(name, v.vec3_array[i].xyz);
        //}
        break;
    case VariableType::Vec4Array:
        uniform->SetValue(v.vec4_array[0].xyzw, v.vec4_array.size());
        //for (int i = 0, n = v.vec4_array.size(); i < n; ++i) {
        //    auto name = k.user_type + "[" + std::to_string(i) + "]." + k.name;
        //    m_prog->SetVec4(name, v.vec4_array[i].xyzw);
        //}
        break;
    }
}

}
}