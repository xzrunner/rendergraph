#include "rendergraph/node/ZWrite.h"
#include "rendergraph/RenderContext.h"

namespace rendergraph
{
namespace node
{

void ZWrite::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    rc->ur_rs.depth_mask = m_enable;
}

}
}