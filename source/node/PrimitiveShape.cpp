#include "rendergraph/node/PrimitiveShape.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void PrimitiveShape::Draw(const RenderContext& rc) const
{
    switch (m_type)
    {
    case PrimitiveShape::Type::Quad:
        rc.rc.RenderQuad();
        break;
    case PrimitiveShape::Type::Cube:
        rc.rc.RenderCube();
        break;
    }
}

}
}