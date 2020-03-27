#include "rendergraph/node/Viewport.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rendergraph
{
namespace node
{

void Viewport::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    rc->rc.SetViewport(m_x, m_y, m_w, m_h);
}

void Viewport::SetParams(int x, int y, int w, int h)
{
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;
}

}
}