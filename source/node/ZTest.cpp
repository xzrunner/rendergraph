#include "rendergraph/node/ZTest.h"
#include "rendergraph/RenderContext.h"

namespace rendergraph
{
namespace node
{

void ZTest::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    ur::DepthTest depth;
    depth.enabled = true;

    switch (m_func)
    {
    case Func::Off:
        depth.enabled = false;
        break;
    case Func::Never:
        depth.function = ur::DepthTestFunc::Never;
        break;
    case Func::Less:
        depth.function = ur::DepthTestFunc::Less;
        break;
    case Func::Equal:
        depth.function = ur::DepthTestFunc::Equal;
        break;
    case Func::LEqual:
        depth.function = ur::DepthTestFunc::LessThanOrEqual;
        break;
    case Func::Greater:
        depth.function = ur::DepthTestFunc::Greater;
        break;
    case Func::NotEqual:
        depth.function = ur::DepthTestFunc::NotEqual;
        break;
    case Func::GEqual:
        depth.function = ur::DepthTestFunc::GreaterThanOrEqual;
        break;
    case Func::Always:
        depth.function = ur::DepthTestFunc::Always;
        break;
    }

    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    rc->ur_ds.render_state.depth_test = depth;
}
}
}