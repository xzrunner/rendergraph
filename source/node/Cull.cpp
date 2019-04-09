#include "rendergraph/node/Cull.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void Cull::Execute(const RenderContext& rc)
{
    ur::CULL_MODE cull;
    switch (m_type)
    {
    case CullType::Off:
        cull = ur::CULL_DISABLE;
        break;
    case CullType::Back:
        cull = ur::CULL_BACK;
        break;
    case CullType::Front:
        cull = ur::CULL_FRONT;
        break;
    }
    rc.rc.SetCull(cull);
}

}
}