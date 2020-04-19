#include "rendergraph/node/Viewport.h"
#include "rendergraph/RenderContext.h"

#include <unirender2/Context.h>

namespace rendergraph
{
namespace node
{

void Viewport::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    auto rc = std::static_pointer_cast<RenderContext>(ctx)->ur_ctx;
    rc->SetViewport(m_x, m_y, m_w, m_h);
}

}
}