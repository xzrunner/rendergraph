#include "rendergraph/node/Shader.h"
#include "rendergraph/node/ShaderParser.h"
#include "rendergraph/Evaluator.h"
#include "rendergraph/Variable.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/Utility.h"

#include <unirender/Device.h>
#include <unirender/Context.h>
#include <unirender/ShaderProgram.h>
#include <unirender/Uniform.h>
#include <painting0/ModelMatUpdater.h>
#include <painting0/CamPosUpdater.h>

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
        SetUniformValue(ip.var.type, val);

        if (flags & Evaluator::FLAG_MODEL_MAT) {
            auto up = std::make_shared<pt0::ModelMatUpdater>(*m_prog, ip.var.type.name);
            m_prog->AddUniformUpdater(up);
        }
        if (flags & Evaluator::FLAG_CAMERA_POS) {
            auto up = std::make_shared<pt0::CamPosUpdater>(*m_prog, ip.var.type.name);
            m_prog->AddUniformUpdater(up);
        }
        if (val.type == VariableType::Sampler2D ||
            val.type == VariableType::SamplerCube)
        {
            if (val.p)
            {
                const int slot = m_prog->QueryTexSlot(ip.var.type.name);
                SetUniformValue(rc, ip.var.type.name, ShaderVariant(slot));
                ur::TexturePtr tex = *reinterpret_cast<const ur::TexturePtr*>(val.p);
                rc.ur_ctx->SetTexture(slot, tex);
            }
        }
    }
}

std::shared_ptr<ur::ShaderProgram> Shader::GetShader(const RenderContext& rc)
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
        if (m_imports[i].var.type.name == key) {
            key_idx = i;
            break;
        }
    }

    if (key_idx < 0) {
        return;
    }

    SetUniformValue(m_imports[key_idx].var.type, val);
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

void Shader::SetUniformValue(const Variable& k, const ShaderVariant& v)
{
    auto uniform = m_prog->QueryUniform(k.name);
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
        uniform->SetValue(v.vec2.xy, 2);
        break;
    case VariableType::Vector3:
        uniform->SetValue(v.vec3.xyz, 3);
        break;
    case VariableType::Vector4:
        uniform->SetValue(v.vec4.xyzw, 4);
        break;
    case VariableType::Matrix2:
        uniform->SetValue(v.mat2.x, 2 * 2);
        break;
    case VariableType::Matrix3:
        uniform->SetValue(v.mat3.x, 3 * 3);
        break;
    case VariableType::Matrix4:
        uniform->SetValue(v.mat4.x, 4 * 4);
        break;
    case VariableType::Sampler2D:
    case VariableType::SamplerCube:
        break;
    case VariableType::Vec1Array:
        uniform->SetValue(v.vec1_array.data(), v.vec1_array.size());
        break;
    case VariableType::Vec2Array:
        uniform->SetValue(v.vec2_array[0].xy, v.vec2_array.size() * 2);
        break;
    case VariableType::Vec3Array:
        uniform->SetValue(v.vec3_array[0].xyz, v.vec3_array.size() * 3);
        break;
    case VariableType::Vec4Array:
        uniform->SetValue(v.vec4_array[0].xyzw, v.vec4_array.size() * 4);
        break;
    }
}

}
}