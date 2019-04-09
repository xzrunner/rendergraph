#include "rendergraph/node/BlendFunc.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace
{

ur::BLEND_FORMAT trans_factor_to_ur(rg::node::BlendFunc::Factor factor)
{
    ur::BLEND_FORMAT ret = ur::BLEND_DISABLE;
    switch (factor)
    {
    case rg::node::BlendFunc::Factor::Off:
        ret = ur::BLEND_DISABLE;
        break;
    case rg::node::BlendFunc::Factor::Zero:
        ret = ur::BLEND_ZERO;
        break;
    case rg::node::BlendFunc::Factor::One:
        ret = ur::BLEND_ONE;
        break;
    case rg::node::BlendFunc::Factor::SrcColor:
        ret = ur::BLEND_SRC_COLOR;
        break;
    case rg::node::BlendFunc::Factor::OneMinusSrcColor:
        ret = ur::BLEND_ONE_MINUS_SRC_COLOR;
        break;
    case rg::node::BlendFunc::Factor::DstColor:
        ret = ur::BLEND_DST_COLOR;
        break;
    case rg::node::BlendFunc::Factor::OneMinusDstColor:
        ret = ur::BLEND_ONE_MINUS_DST_COLOR;
        break;
    case rg::node::BlendFunc::Factor::SrcAlpha:
        ret = ur::BLEND_SRC_ALPHA;
        break;
    case rg::node::BlendFunc::Factor::OneMinusSrcAlpha:
        ret = ur::BLEND_ONE_MINUS_SRC_ALPHA;
        break;
    case rg::node::BlendFunc::Factor::DstAlpha:
        ret = ur::BLEND_DST_ALPHA;
        break;
    case rg::node::BlendFunc::Factor::OneMinusDstAlpha:
        ret = ur::BLEND_ONE_MINUS_DST_ALPHA;
        break;
    }
    return ret;
}

}

namespace rg
{
namespace node
{

void BlendFunc::Execute(const RenderContext& rc)
{
    ur::BLEND_FORMAT src = trans_factor_to_ur(m_sfactor);
    ur::BLEND_FORMAT dst = trans_factor_to_ur(m_dfactor);
    rc.rc.SetBlend(src, dst);
}

}
}