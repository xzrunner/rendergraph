#include "rendergraph/node/FrontFace.h"
#include "rendergraph/RenderContext.h"

namespace rendergraph
{
namespace node
{

void FrontFace::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    rc->ur_ds.render_state.facet_culling.front_face_winding_order
        = m_clockwise ? ur::WindingOrder::Clockwise : ur::WindingOrder::Counterclockwise;
}

}
}