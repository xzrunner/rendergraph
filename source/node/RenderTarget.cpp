#include "rendergraph/node/RenderTarget.h"
#include "rendergraph/node/Texture.h"
#include "rendergraph/node/Input.h"
#include "rendergraph/node/SubGraph.h"
#include "rendergraph/RenderContext.h"

#include <unirender/Device.h>
#include <unirender/Context.h>
#include <unirender/Framebuffer.h>

namespace rendergraph
{
namespace node
{

void RenderTarget::Eval(const RenderContext& rc, size_t port_idx,
	                    ShaderVariant& var, uint32_t& flags) const
{
	switch (port_idx)
	{
	case O_RT:
		var.type = VariableType::RenderTarget;
		var.p = m_frame_buffer.get();
		break;

	case O_COLOR_TEX0:
	{
		auto& conns = m_imports[I_COLOR_TEX0].conns;
		if (!conns.empty()) {
			assert(conns.size() == 1);
			auto prev_node = conns[0].node.lock();
			std::static_pointer_cast<Node>(prev_node)->Eval(rc, conns[0].idx, var, flags);
		}
	}
		break;
	}
}

void RenderTarget::Bind(const RenderContext& rc)
{
	Setup(rc);

    rc.ur_ctx->GetViewport(m_prev_vp_x, m_prev_vp_y, m_prev_vp_w, m_prev_vp_h);
    m_prev_fbo = rc.ur_ctx->GetFramebuffer();

    rc.ur_ctx->SetViewport(0, 0, m_width, m_height);
    rc.ur_ctx->SetFramebuffer(m_frame_buffer);
}

void RenderTarget::Unbind(const RenderContext& rc)
{
    rc.ur_ctx->SetViewport(m_prev_vp_x, m_prev_vp_y, m_prev_vp_w, m_prev_vp_h);
    rc.ur_ctx->SetFramebuffer(m_prev_fbo);
}

void RenderTarget::SetSize(uint32_t width, uint32_t height)
{
    if (m_width == width && m_height == height) {
        return;
    }

    m_width  = width;
    m_height = height;
}

void RenderTarget::Setup(const RenderContext& rc)
{
	if (!m_frame_buffer) {
		m_frame_buffer = rc.ur_dev->CreateFramebuffer();
	}

    // init texture
    SetupTexture(I_COLOR_TEX0, rc);
    SetupTexture(I_COLOR_TEX1, rc);
    SetupTexture(I_COLOR_TEX2, rc);
    SetupTexture(I_COLOR_TEX3, rc);
    SetupTexture(I_DEPTH_TEX, rc);

    // init rbo
    if (m_enable_rbo_depth && m_width != 0 && m_height != 0)
    {
        auto type = ur::AttachmentType::Depth;
        auto rbo = rc.ur_dev->CreateRenderBuffer(m_width, m_height, ur::InternalFormat::DepthComponent, type);
        m_frame_buffer->SetAttachment(type, ur::TextureTarget::Texture2D, nullptr, rbo);
    }
    if (m_enable_rbo_color && m_width != 0 && m_height != 0)
    {
        auto type = ur::AttachmentType::Color0;
        auto rbo = rc.ur_dev->CreateRenderBuffer(m_width, m_height, ur::InternalFormat::RGB, type);
        m_frame_buffer->SetAttachment(type, ur::TextureTarget::Texture2D, nullptr, rbo);
    }
}

void RenderTarget::SetupTexture(int input_idx, const RenderContext& rc)
{
    auto& conns = GetImports()[input_idx].conns;
    if (conns.empty()) {
        return;
    }
    auto tex_node = conns[0].node.lock();
    if (!tex_node || m_binded_textures[input_idx - I_COLOR_TEX0] == tex_node) {
        return;
    }

    if (tex_node->get_type() == rttr::type::get<node::Input>()) {
        if (!rc.sub_graph_stack.empty()) {
            auto input = std::static_pointer_cast<node::Input>(tex_node);
            auto sub_graph = rc.sub_graph_stack.back();
            for (auto& in : sub_graph->GetImports()) {
                if (in.var.type.name == input->GetVarName()) {
                    if (!in.conns.empty()) {
                        tex_node = in.conns[0].node.lock();
                        break;
                    }
                }
            }
        }
    }

    if (tex_node->get_type() != rttr::type::get<node::Texture>()) {
        return;
    }

	m_binded_textures[input_idx - I_COLOR_TEX0] = std::static_pointer_cast<Node>(tex_node);

    auto tex = std::static_pointer_cast<node::Texture>(tex_node);
    tex->Init(rc);

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

    ur::AttachmentType atta_type;
    if (input_idx >= I_COLOR_TEX0 && input_idx <= I_COLOR_TEX3) {
        atta_type = static_cast<ur::AttachmentType>(input_idx);
    } else {
        assert(input_idx == I_DEPTH_TEX);
        atta_type = ur::AttachmentType::Depth;
    }
    m_frame_buffer->SetAttachment(atta_type, ur::TextureTarget::Texture2D, tex->GetTexture(), nullptr);
}

}
}