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
    auto& rc = ur::Blackboard::Instance()->GetRenderContext();
    if (m_fbo != 0) {
        rc.ReleaseRenderTarget(m_fbo);
    }
    if (m_rbo_depth != 0) {
        rc.ReleaseRenderbufferObject(m_rbo_depth);
    }
    if (m_rbo_color != 0) {
        rc.ReleaseRenderbufferObject(m_rbo_color);
    }
}

void RenderTarget::Execute(const RenderContext& rc)
{
    auto& ur_rc = rc.rc;

    auto& conns = GetImports()[1].conns;
    if (!conns.empty())
    {
        auto tex_node = conns[0].node.lock();
        if (tex_node && tex_node->get_type() == rttr::type::get<node::Texture>())
        {
            auto tex = std::static_pointer_cast<node::Texture>(tex_node);
            if (tex->GetTexID() == 0) {
                tex->Execute(rc);
            }

            m_width  = tex->GetWidth();
            m_height = tex->GetHeight();
        }
    }

    if (m_fbo != 0) {
        ur_rc.ReleaseRenderTarget(m_fbo);
    }
    m_fbo = ur_rc.CreateRenderTarget(m_fbo);

    if (m_enable_rbo_depth && m_width != 0 && m_height != 0)
    {
        if (m_rbo_depth != 0) {
            ur_rc.ReleaseRenderbufferObject(m_rbo_depth);
        }
        m_rbo_depth = ur_rc.CreateRenderbufferObject(m_fbo, ur::FMT_DEPTH_COMPONENT, m_width, m_height);
    }
    if (m_enable_rbo_color && m_rbo_color == 0 && m_width != 0 && m_height != 0)
    {
        if (m_rbo_color != 0) {
            ur_rc.ReleaseRenderbufferObject(m_rbo_color);
        }
        m_rbo_color = ur_rc.CreateRenderbufferObject(m_fbo, ur::FMT_RGB8, m_width, m_height);
    }
}

void RenderTarget::Bind(const RenderContext& rc)
{
    auto& ur_rc = rc.rc;

    ur_rc.BindRenderTarget(m_fbo);
    if (m_rbo_depth != 0) {
        ur_rc.BindRenderbufferObject(m_rbo_depth, ur::ATTACHMENT_DEPTH);
    }
    if (m_rbo_color != 0) {
        ur_rc.BindRenderbufferObject(m_rbo_color, ur::ATTACHMENT_COLOR0);
    }

    auto& conns = GetImports()[1].conns;
    if (!conns.empty())
    {
        auto tex_node = conns[0].node.lock();
        if (tex_node && tex_node->get_type() == rttr::type::get<node::Texture>())
        {
            auto tex = std::static_pointer_cast<node::Texture>(tex_node);
            assert(tex->GetTexID() != 0);
    
            ur::ATTACHMENT_TYPE att_type = ur::ATTACHMENT_COLOR0;
            if (tex->GetFormat() == node::Texture::Format::Depth) {
                att_type = ur::ATTACHMENT_DEPTH;
            }
            ur_rc.BindRenderTargetTex(tex->GetTexID(), att_type);

            ur_rc.GetViewport(m_vp_x, m_vp_y, m_vp_w, m_vp_h);
            ur_rc.SetViewport(0, 0, m_width, m_height);
        }
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

}
}