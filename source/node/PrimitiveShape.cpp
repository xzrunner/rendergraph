#include "rendergraph/node/PrimitiveShape.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void PrimitiveShape::Draw(const RenderContext& rc) const
{
    ur::RenderContext::VertLayout layout;
    switch (m_vert_layout)
    {
    case VertLayout::Pos:
        layout = ur::RenderContext::VertLayout::VL_POS;
        break;
    case VertLayout::PosTex:
        layout = ur::RenderContext::VertLayout::VL_POS_TEX;
        break;
    case VertLayout::PosNormTex:
        layout = ur::RenderContext::VertLayout::VL_POS_NORM_TEX;
        break;
    case VertLayout::PosNormTexTB:
        layout = ur::RenderContext::VertLayout::VL_POS_NORM_TEX_TB;
        break;
    default:
        assert(0);
    }

    switch (m_type)
    {
    case PrimitiveShape::Type::Quad:
        rc.rc.RenderQuad(layout);
        break;
    case PrimitiveShape::Type::Cube:
        rc.rc.RenderCube(layout);
        break;
    }
}

}
}