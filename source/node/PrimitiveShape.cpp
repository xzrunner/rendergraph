#include "rendergraph/node/PrimitiveShape.h"
#include "rendergraph/RenderContext.h"

#include <unirender/Device.h>
#include <unirender/Context.h>
#include <unirender/DrawState.h>

namespace rendergraph
{
namespace node
{

void PrimitiveShape::Draw(const RenderContext& rc) const
{
    ur::VertexLayoutType layout;
    switch (m_vert_layout)
    {
    case VertLayout::Pos:
        layout = ur::VertexLayoutType::Pos;
        break;
    case VertLayout::PosTex:
        layout = ur::VertexLayoutType::PosTex;
        break;
    case VertLayout::PosNormTex:
        layout = ur::VertexLayoutType::PosNormTex;
        break;
    case VertLayout::PosNormTexTB:
        layout = ur::VertexLayoutType::PosNormTexTB;
        break;
    default:
        assert(0);
    }

    ur::DrawState ds = rc.ur_ds;
    switch (m_type)
    {
    case PrimitiveShape::Type::Quad:
        ds.vertex_array = rc.ur_dev->GetVertexArray(ur::Device::PrimitiveType::Quad, layout);
        break;
    case PrimitiveShape::Type::Cube:
        ds.vertex_array = rc.ur_dev->GetVertexArray(ur::Device::PrimitiveType::Cube, layout);
        break;
    }
    rc.ur_ctx->Draw(ur::PrimitiveType::TriangleStrip, ds, nullptr);
}

}
}