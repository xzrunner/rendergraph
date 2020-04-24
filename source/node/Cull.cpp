#include "rendergraph/node/Cull.h"
#include "rendergraph/RenderContext.h"

namespace rendergraph
{
namespace node
{

void Cull::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    rc->ur_ds.render_state.facet_culling.enabled = true;
    switch (m_mode)
    {
    case Mode::Off:
        rc->ur_ds.render_state.facet_culling.enabled = false;
        break;
    case Mode::Front:
        rc->ur_ds.render_state.facet_culling.face = ur2::CullFace::Front;
        break;
    case Mode::Back:
        rc->ur_ds.render_state.facet_culling.face = ur2::CullFace::Back;
        break;
    case Mode::FrontAndBack:
        rc->ur_ds.render_state.facet_culling.face = ur2::CullFace::FrontAndBack;
        break;
    }
}

}
}