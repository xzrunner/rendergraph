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
    switch (m_mode)
    {
    case Mode::Off:
        cull = ur::CULL_DISABLE;
        break;
    case Mode::Front:
        cull = ur::CULL_FRONT;
        break;
    case Mode::Back:
        cull = ur::CULL_BACK;
        break;
    case Mode::FrontAndBack:
        cull = ur::CULL_FRONT_AND_BACK;
        break;
    }
    rc.rc.SetCull(cull);
}

}
}