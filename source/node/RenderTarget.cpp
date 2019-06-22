#include "rendergraph/node/RenderTarget.h"
#include "rendergraph/node/Texture.h"
#include "rendergraph/RenderContext.h"

#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

RenderTarget::~RenderTarget()
{
    ReleaseRes();
}

void RenderTarget::Execute(const RenderContext& rc)
{
    auto& ur_rc = rc.rc;

    // create fbo
    if (m_fbo == 0) {
        m_fbo = ur_rc.CreateRenderTarget(m_fbo);
    }

    // create texture
    ExecuteTexture(ID_COLOR_TEX, ur_rc);
    ExecuteTexture(ID_DEPTH_TEX, ur_rc);

    // create rbo
    if (m_enable_rbo_depth && m_width != 0 && m_height != 0 && m_rbo_depth == 0) {
        m_rbo_depth = ur_rc.CreateRenderbufferObject(m_fbo, ur::FMT_DEPTH_COMPONENT, m_width, m_height);
    }
    if (m_enable_rbo_color && m_rbo_color == 0 && m_width != 0 && m_height != 0 && m_rbo_color == 0) {
        m_rbo_color = ur_rc.CreateRenderbufferObject(m_fbo, ur::FMT_RGB8, m_width, m_height);
    }
}

void RenderTarget::Bind(const RenderContext& rc)
{
    auto& ur_rc = rc.rc;

    // bind fbo
    ur_rc.BindRenderTarget(m_fbo);
    // bind texture
    ur_rc.GetViewport(m_vp_x, m_vp_y, m_vp_w, m_vp_h);
    BindTexture(ID_COLOR_TEX, ur_rc);
    BindTexture(ID_DEPTH_TEX, ur_rc);
    ur_rc.SetViewport(0, 0, m_width, m_height);
    // bind rbo
    if (m_rbo_depth != 0) {
        ur_rc.BindRenderbufferObject(m_rbo_depth, ur::ATTACHMENT_DEPTH);
    }
    if (m_rbo_color != 0) {
        ur_rc.BindRenderbufferObject(m_rbo_color, ur::ATTACHMENT_COLOR0);
    }

    if (ur_rc.CheckRenderTargetStatus() == 0) {
        ur_rc.UnbindRenderTarget();
        m_binded = false;
    } else {
        m_binded = true;
    }
}

void RenderTarget::Unbind(const RenderContext& rc)
{
    if (!m_binded) {
        return;
    }

    m_binded = false;

    auto& ur_rc = rc.rc;
    ur_rc.UnbindRenderTarget();

    ur_rc.SetViewport(m_vp_x, m_vp_y, m_vp_w, m_vp_h);
}

void RenderTarget::SetSize(uint32_t width, uint32_t height)
{
    if (m_width == width && m_height == height) {
        return;
    }

    ReleaseRes();

    m_width  = width;
    m_height = height;
}

void RenderTarget::EnableDepthRBO()
{
    if (m_enable_rbo_depth) {
        return;
    }

    m_enable_rbo_depth = true;
}

void RenderTarget::EnableColorRBO()
{
    if (m_enable_rbo_color) {
        return;
    }

    m_enable_rbo_color = true;
}

void RenderTarget::ExecuteTexture(int input_idx, ur::RenderContext& rc)
{
    auto& conns = GetImports()[input_idx].conns;
    if (conns.empty()) {
        return;
    }
    auto tex_node = conns[0].node.lock();
    if (!tex_node || tex_node->get_type() != rttr::type::get<node::Texture>()) {
        return;
    }

    auto tex = std::static_pointer_cast<node::Texture>(tex_node);
    if (tex->GetTexID() == 0) {
        tex->Execute(rc);
    }

    uint32_t width  = tex->GetWidth();
    uint32_t height = tex->GetHeight();
    if (width != 0 && height != 0)
    {
        if (m_width != 0) {
            assert(width == m_width && height == m_height);
        } else {
            m_width  = width;
            m_height = height;
        }
    }
}

void RenderTarget::BindTexture(int input_idx, ur::RenderContext& rc)
{
    auto& conns = GetImports()[input_idx].conns;
    if (conns.empty()) {
        return;
    }

    auto tex_node = conns[0].node.lock();
    if (!tex_node || tex_node->get_type() != rttr::type::get<node::Texture>()) {
        return;
    }

    auto tex = std::static_pointer_cast<node::Texture>(tex_node);
    assert(tex->GetTexID() != 0);

    ur::ATTACHMENT_TYPE att_type = ur::ATTACHMENT_COLOR0;
    if (tex->GetFormat() == node::Texture::Format::Depth) {
        att_type = ur::ATTACHMENT_DEPTH;
    }
    rc.BindRenderTargetTex(tex->GetTexID(), att_type);
}

void RenderTarget::ReleaseRes()
{
    auto& rc = ur::Blackboard::Instance()->GetRenderContext();
    if (m_fbo != 0) {
        rc.ReleaseRenderTarget(m_fbo);
        m_fbo = 0;
    }
    if (m_rbo_depth != 0) {
        rc.ReleaseRenderbufferObject(m_rbo_depth);
        m_rbo_depth = 0;
    }
    if (m_rbo_color != 0) {
        rc.ReleaseRenderbufferObject(m_rbo_color);
        m_rbo_color = 0;
    }
}

}
}