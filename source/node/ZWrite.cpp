#include "rendergraph/node/ZWrite.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void ZWrite::Execute(const RenderContext& rc)
{
    rc.rc.SetZWrite(m_enable);
}

}
}