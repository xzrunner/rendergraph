#include "rendergraph/node/Shader.h"
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
#include <painting3/ViewMatUpdater.h>
#include <painting3/ProjectMatUpdater.h>
#include <cpputil/StringHelper.h>
#include <shadertrans/ShaderTrans.h>

#include <sstream>

namespace rendergraph
{
namespace node
{

void Shader::SetCodes(const std::string& vert, const std::string& tess_ctrl, 
                      const std::string& tess_eval, const std::string& frag, std::ostream& out)
{
    if (m_vert == vert && 
        m_tess_ctrl == tess_ctrl && 
        m_tess_eval == tess_eval &&
        m_frag == frag) {
        return;
    }

    m_vert = vert;
    m_tess_ctrl = tess_ctrl;
    m_tess_eval = tess_eval;
    m_frag = frag;

    m_prog.reset();

    ShaderInfo si;
    si.Parse(shadertrans::ShaderStage::VertexShader, m_vert, m_lang, out);
    si.Parse(shadertrans::ShaderStage::TessCtrlShader, m_tess_ctrl, m_lang, out);
    si.Parse(shadertrans::ShaderStage::TessEvalShader, m_tess_eval, m_lang, out);
    si.Parse(shadertrans::ShaderStage::PixelShader, m_frag, m_lang, out);

    UpdateImports(si.GetUniforms());

    // disconnect old pins
//    for (auto& old : cache_old_pins) 
    //for (size_t i = 0, n = cache_old_pins.size(); i < n; ++i)
    //{
    //    auto& old = cache_old_pins[i];

    //    bool find = false;
    //    for (auto& _new : m_imports) {
    //        if (&_new == &old) {
    //            find = true;
    //            break;
    //        }
    //    }
    //    if (!find) {
    //        for (auto& conn : old.conns) {
    //            dag::disconnect<Variable>(*conn.node.lock(), conn.idx, *this, i);
    //        }
    //    }
    //}
}

void Shader::SetCodes(const std::string& compute, std::ostream& out)
{
    if (m_compute == compute) {
        return;
    }

    m_compute = compute;

    m_prog.reset();

    ShaderInfo si;
    si.Parse(shadertrans::ShaderStage::ComputeShader, m_compute, m_lang, out);

    m_props = si.GetProps();

    UpdateImports(si.GetUniforms());
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
        if (flags & Evaluator::FLAG_VIEW_MAT) {
            auto up = std::make_shared<pt3::ViewMatUpdater>(*m_prog, ip.var.type.name);
            m_prog->AddUniformUpdater(up);
        }
        if (flags & Evaluator::FLAG_PROJ_MAT) {
            auto up = std::make_shared<pt3::ProjectMatUpdater>(*m_prog, ip.var.type.name);
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

ShaderVariant Shader::QueryProp(const std::string& name) const
{
    ShaderVariant ret;

    auto itr = m_props.find(name);
    if (itr != m_props.end()) {
        ret = itr->second;
    }

    return ret;
}

void Shader::Init(const RenderContext& rc)
{
    if (!m_compute.empty()) {
        InitComputeShader(rc);
    } else {
        InitRenderShader(rc);
    }
}

void Shader::InitRenderShader(const RenderContext& rc)
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

    std::vector<unsigned int> vs, fs, tcs, tes;
    auto& logger = rc.ur_dev->GetLogger();
    switch (m_lang)
    {
    case Language::GLSL:
        shadertrans::ShaderTrans::GLSL2SpirV(shadertrans::ShaderStage::VertexShader, vert, vs, logger);
        shadertrans::ShaderTrans::GLSL2SpirV(shadertrans::ShaderStage::PixelShader, frag, fs, logger);
        if (!m_tess_ctrl.empty()) {
            shadertrans::ShaderTrans::GLSL2SpirV(shadertrans::ShaderStage::TessCtrlShader, m_tess_ctrl, tcs, logger);
        }
        if (!m_tess_eval.empty()) {
            shadertrans::ShaderTrans::GLSL2SpirV(shadertrans::ShaderStage::TessEvalShader, m_tess_eval, tes, logger);
        }
        break;
    case Language::HLSL:
        shadertrans::ShaderTrans::HLSL2SpirV(shadertrans::ShaderStage::VertexShader, vert, vs, logger);
        shadertrans::ShaderTrans::HLSL2SpirV(shadertrans::ShaderStage::PixelShader, frag, fs, logger);
        if (!m_tess_ctrl.empty()) {
            shadertrans::ShaderTrans::HLSL2SpirV(shadertrans::ShaderStage::TessCtrlShader, m_tess_ctrl, tcs, logger);
        }
        if (!m_tess_eval.empty()) {
            shadertrans::ShaderTrans::HLSL2SpirV(shadertrans::ShaderStage::TessEvalShader, m_tess_eval, tes, logger);
        }
        break;
    default:
        assert(0);
    }

    m_prog = rc.ur_dev->CreateShaderProgram(vs, fs, tcs, tes);
}

void Shader::InitComputeShader(const RenderContext& rc)
{
    if (m_prog || m_compute.empty()) {
        return;
    }

    
    std::vector<unsigned int> cs;
    auto& logger = rc.ur_dev->GetLogger();
    switch (m_lang)
    {
    case Language::GLSL:
        shadertrans::ShaderTrans::GLSL2SpirV(shadertrans::ShaderStage::ComputeShader, m_compute, cs, logger);
        break;
    case Language::HLSL:
        shadertrans::ShaderTrans::HLSL2SpirV(shadertrans::ShaderStage::ComputeShader, m_compute, cs, logger);
        break;
    default:
        assert(0);
    }

    m_prog = rc.ur_dev->CreateShaderProgram(cs);
}

void Shader::UpdateImports(const std::vector<Variable>& uniforms)
{
    std::vector<Port> cache_old_pins = m_imports;
    m_imports.erase(m_imports.begin() + I_MAX_NUM, m_imports.end());
    for (auto& u : uniforms)
    {
        bool find = false;
        for (auto& old : cache_old_pins)
        {
            if (old.var.type.type == u.type &&
                old.var.type.name == u.name) 
            {
                find = true;
                m_imports.push_back(old);
                break;
            }
        }

        if (!find)
        {
            dag::Node<rendergraph::Variable>::Port port;
            port.var.full_name = u.name;
            port.var.type = u;

            m_imports.push_back(port);
        }
    }
}

}
}