#include "rendergraph/node/RenderTarget.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void RenderTarget::Execute(const RenderContext& rc)
{
    if (m_id != 0) {
        return;
    }

    m_id = rc.rc.CreateRenderTarget(m_id);
}

}
}