#include "rendergraph/node/PrimitiveShape.h"
#include "rendergraph/RenderContext.h"

#include <unirender2/Device.h>
#include <unirender2/Context.h>
#include <unirender2/DrawState.h>

namespace rendergraph
{
namespace node
{

void PrimitiveShape::Draw(const RenderContext& rc) const
{
    ur2::VertexLayoutType layout;
    switch (m_vert_layout)
    {
    case VertLayout::Pos:
        layout = ur2::VertexLayoutType::Pos;
        break;
    case VertLayout::PosTex:
        layout = ur2::VertexLayoutType::PosTex;
        break;
    case VertLayout::PosNormTex:
        layout = ur2::VertexLayoutType::PosNormTex;
        break;
    case VertLayout::PosNormTexTB:
        layout = ur2::VertexLayoutType::PosNormTexTB;
        break;
    default:
        assert(0);
    }

    ur2::DrawState ds = rc.ur_ds;
    switch (m_type)
    {
    case PrimitiveShape::Type::Quad:
        ds.vertex_array = rc.ur_dev->GetVertexArray(ur2::Device::PrimitiveType::Quad, layout);
        break;
    case PrimitiveShape::Type::Cube:
        break;
    }
    rc.ur_ctx->Draw(ur2::PrimitiveType::TriangleStrip, ds, nullptr);
}

}
}