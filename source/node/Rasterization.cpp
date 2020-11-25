#include "rendergraph/node/Rasterization.h"
#include "rendergraph/RenderContext.h"

namespace rendergraph
{
namespace node
{

void Rasterization::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    switch (m_mode)
    {
    case Mode::Point:
        rc->ur_ds.render_state.rasterization_mode = ur::RasterizationMode::Point;
        break;
    case Mode::Line:
        rc->ur_ds.render_state.rasterization_mode = ur::RasterizationMode::Line;
        break;
    case Mode::Fill:
        rc->ur_ds.render_state.rasterization_mode = ur::RasterizationMode::Fill;
        break;
    default:
        assert(0);
    }
}

}
}