#include "rendergraph/node/RenderTarget.h"

#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void RenderTarget::Execute(ur::RenderContext& rc)
{
    if (m_id != 0) {
        return;
    }

    m_id = rc.CreateRenderTarget(m_id);
}

}
}