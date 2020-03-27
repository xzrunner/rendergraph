#include "rendergraph/node/Cull.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rendergraph
{
namespace node
{

void Cull::Execute(const std::shared_ptr<dag::Context>& ctx)
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

    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    rc->rc.SetCullMode(cull);
}

}
}