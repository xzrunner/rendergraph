#include "rendergraph/node/ZTest.h"
#include "rendergraph/RenderContext.h"

namespace rendergraph
{
namespace node
{

void ZTest::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    ur2::DepthTest depth;
    depth.enabled = true;

    switch (m_func)
    {
    case Func::Off:
        depth.enabled = false;
        break;
    case Func::Never:
        depth.function = ur2::DepthTestFunc::Never;
        break;
    case Func::Less:
        depth.function = ur2::DepthTestFunc::Less;
        break;
    case Func::Equal:
        depth.function = ur2::DepthTestFunc::Equal;
        break;
    case Func::LEqual:
        depth.function = ur2::DepthTestFunc::LessThanOrEqual;
        break;
    case Func::Greater:
        depth.function = ur2::DepthTestFunc::Greater;
        break;
    case Func::NotEqual:
        depth.function = ur2::DepthTestFunc::NotEqual;
        break;
    case Func::GEqual:
        depth.function = ur2::DepthTestFunc::GreaterThanOrEqual;
        break;
    case Func::Always:
        depth.function = ur2::DepthTestFunc::Always;
        break;
    }

    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    rc->ur_ds.render_state.depth_test = depth;
}
}
}