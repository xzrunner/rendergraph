#include "rendergraph/node/Viewport.h"

#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void Viewport::Execute(ur::RenderContext& rc)
{
    rc.SetViewport(m_x, m_y, m_w, m_h);
}

}
}