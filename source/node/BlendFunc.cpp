#include "rendergraph/node/BlendFunc.h"
#include "rendergraph/RenderContext.h"

namespace
{

ur2::BlendingFactor trans_blend_factor(rendergraph::node::BlendFunc::Factor factor)
{
    ur2::BlendingFactor ret;
    switch (factor)
    {
    case rendergraph::node::BlendFunc::Factor::Zero:
        ret = ur2::BlendingFactor::Zero;
        break;
    case rendergraph::node::BlendFunc::Factor::One:
        ret = ur2::BlendingFactor::One;
        break;
    case rendergraph::node::BlendFunc::Factor::SrcColor:
        ret = ur2::BlendingFactor::SrcColor;
        break;
    case rendergraph::node::BlendFunc::Factor::OneMinusSrcColor:
        ret = ur2::BlendingFactor::OneMinusSrcColor;
        break;
    case rendergraph::node::BlendFunc::Factor::DstColor:
        ret = ur2::BlendingFactor::DstColor;
        break;
    case rendergraph::node::BlendFunc::Factor::OneMinusDstColor:
        ret = ur2::BlendingFactor::OneMinusDstColor;
        break;
    case rendergraph::node::BlendFunc::Factor::SrcAlpha:
        ret = ur2::BlendingFactor::SrcAlpha;
        break;
    case rendergraph::node::BlendFunc::Factor::OneMinusSrcAlpha:
        ret = ur2::BlendingFactor::OneMinusSrcAlpha;
        break;
    case rendergraph::node::BlendFunc::Factor::DstAlpha:
        ret = ur2::BlendingFactor::DstAlpha;
        break;
    case rendergraph::node::BlendFunc::Factor::OneMinusDstAlpha:
        ret = ur2::BlendingFactor::OneMinusDstAlpha;
        break;
    }
    return ret;
}

}

namespace rendergraph
{
namespace node
{

void BlendFunc::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    ur2::Blending blend;
    blend.enabled = true;
    blend.separately = false;
    if (m_src == Factor::Off || m_dst == Factor::Off)
    {
        blend.enabled = false;
    }
    else
    {
        blend.src = trans_blend_factor(m_src);
        blend.dst = trans_blend_factor(m_dst);
    }

    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    rc->ur_ds.render_state.blending = blend;
}

}
}