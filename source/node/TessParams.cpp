#include "rendergraph/node/TessParams.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderState.h>

namespace rendergraph
{
namespace node
{

void TessParams::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    auto& dst = rc->ur_ds.render_state.tess_params;
    dst.vert_num    = m_vert_num;
    dst.outer_level = m_outer_level;
    dst.inner_level = m_inner_level;
}

}
}