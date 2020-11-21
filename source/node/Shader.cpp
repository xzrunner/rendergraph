#include "rendergraph/node/Shader.h"
#include "rendergraph/node/ShaderParser.h"
#include "rendergraph/node/Texture.h"
#include "rendergraph/node/ShaderInfo.h"
#include "rendergraph/Evaluator.h"
#include "rendergraph/Variable.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/ValueImpl.h"
#include "rendergraph/ShaderHelper.h"

#include <unirender/Device.h>
#include <unirender/Context.h>
#include <unirender/ShaderProgram.h>
#include <unirender/Uniform.h>
#include <painting0/ModelMatUpdater.h>
#include <painting0/CamPosUpdater.h>
#include <cpputil/StringHelper.h>
#include <shadertrans/ShaderTrans.h>

namespace rendergraph
{
namespace node
{

void Shader::SetCodes(const std::string& vert, const std::string& frag, std::ostream& out)
{
    if (m_vert == vert && m_frag == frag) {
        return;
    }

    m_vert = vert;
    m_frag = frag;

    m_prog.reset();

    std::vector<Variable> uniforms;
    std::set<std::string> names;
    ShaderInfo::GetCodeUniforms(shadertrans::ShaderStage::VertexShader, m_vert, m_lang, uniforms, names, out);
    ShaderInfo::GetCodeUniforms(shadertrans::ShaderStage::PixelShader, m_frag, m_lang, uniforms, names, out);

    m_imports.reserve(m_imports.size() + uniforms.size());
    for (auto& u : uniforms)
    {
        dag::Node<rendergraph::Variable>::Port port;

        port.var.type = u;

        for (auto& p : m_imports) {
            if (p.var.type.type == u.type &&
                p.var.type.name == u.name) {
                port.conns = p.conns;
                break;
            }
        }

        m_imports.push_back(port);
    }
}

void Shader::Bind(RenderContext& rc)
{
    if (!m_prog && rc.ur_dev) {
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
        ShaderHelper::SetUniformValue(m_prog, ip.var.type, val);

        if (flags & Evaluator::FLAG_MODEL_MAT) {
            auto up = std::make_shared<pt0::ModelMatUpdater>(*m_prog, ip.var.type.name);
            m_prog->AddUniformUpdater(up);
        }
        if (flags & Evaluator::FLAG_CAMERA_POS) {
            auto up = std::make_shared<pt0::CamPosUpdater>(*m_prog, ip.var.type.name);
            m_prog->AddUniformUpdater(up);
        }
        if (val.type == VariableType::Texture && val.p)
        {
            const int slot = m_prog->QueryTexSlot(ip.var.type.name);
            SetUniformValue(rc, ip.var.type.name, ShaderVariant(slot));
            auto tex = reinterpret_cast<const TextureVal*>(val.p);
			if (tex->texture) {
				rc.ur_ctx->SetTexture(slot, tex->texture);
			}
			if (tex->sampler) {
				rc.ur_ctx->SetTextureSampler(slot, tex->sampler);
			}
        }
    }
}

std::shared_ptr<ur::ShaderProgram> Shader::GetShader(const RenderContext& rc)
{
    if (!m_prog && rc.ur_dev) {
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

    ShaderHelper::SetUniformValue(m_prog, m_imports[key_idx].var.type, val);
}

void Shader::Init(const RenderContext& rc)
{
    std::string vert = m_vert;
    std::string frag = m_frag;

    auto expect = ShaderVariant(std::string(""));
    uint32_t flags = 0;
    auto prev_vert = Evaluator::Calc(rc, m_imports[I_VS], expect, flags);
    if (prev_vert.type == VariableType::String) {
        vert = static_cast<const char*>(prev_vert.p) + vert;
    }
    auto prev_frag = Evaluator::Calc(rc, m_imports[I_FS], expect, flags);
    if (prev_frag.type == VariableType::String) {
        frag = static_cast<const char*>(prev_frag.p) + frag;
    }

    if (m_prog || vert.empty() || frag.empty()) {
        return;
    }

    std::vector<unsigned int> vs, fs;
    switch (m_lang)
    {
    case Language::GLSL:
        shadertrans::ShaderTrans::GLSL2SpirV(shadertrans::ShaderStage::VertexShader, vert, vs);
        shadertrans::ShaderTrans::GLSL2SpirV(shadertrans::ShaderStage::PixelShader, frag, fs);
        break;
    case Language::HLSL:
        shadertrans::ShaderTrans::HLSL2SpirV(shadertrans::ShaderStage::VertexShader, vert, vs);
        shadertrans::ShaderTrans::HLSL2SpirV(shadertrans::ShaderStage::PixelShader, frag, fs);
        break;
    default:
        assert(0);
    }
    m_prog = rc.ur_dev->CreateShaderProgram(vs, fs);
}

}
}