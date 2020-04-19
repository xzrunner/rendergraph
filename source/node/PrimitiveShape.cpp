#include "rendergraph/node/PrimitiveShape.h"
#include "rendergraph/RenderContext.h"

#include <unirender2/Context.h>
#include <unirender2/DrawState.h>

namespace rendergraph
{
namespace node
{

void PrimitiveShape::Draw(const RenderContext& rc) const
{
    ur2::Context::VertexLayout layout;
    switch (m_vert_layout)
    {
    case VertLayout::Pos:
        layout = ur2::Context::VertexLayout::Pos;
        break;
    case VertLayout::PosTex:
        layout = ur2::Context::VertexLayout::PosTex;
        break;
    case VertLayout::PosNormTex:
        layout = ur2::Context::VertexLayout::PosNormTex;
        break;
    case VertLayout::PosNormTexTB:
        layout = ur2::Context::VertexLayout::PosNormTexTB;
        break;
    default:
        assert(0);
    }

    ur2::DrawState ds;
    switch (m_type)
    {
    case PrimitiveShape::Type::Quad:
        rc.ur_ctx->DrawQuad(layout, ds);
        break;
    case PrimitiveShape::Type::Cube:
        rc.ur_ctx->DrawCube(layout, ds);
        break;
    }
}

}
}