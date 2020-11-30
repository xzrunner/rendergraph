#include "rendergraph/node/Barrier.h"
#include "rendergraph/RenderContext.h"

#include <unirender/ClearState.h>
#include <unirender/Context.h>

namespace rendergraph
{
namespace node
{

void Barrier::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    if (m_barrier_types.empty()) {
        return;
    }

    std::vector<ur::BarrierType> barriers;
    barriers.reserve(m_barrier_types.size());
    for (auto& b : m_barrier_types) {
        barriers.push_back(static_cast<ur::BarrierType>(b));
    }
    
    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    rc->ur_ctx->MemoryBarrier(barriers);
}

}
}