#include "rendergraph/node/RenderTarget.h"

#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void RenderTarget::Execute(ur::RenderContext& rc)
{
    m_id = rc.CreateRenderTarget(m_id);
}

}
}