#include "rendergraph/node/AlphaTest.h"
#include "rendergraph/RenderContext.h"

namespace rendergraph
{
namespace node
{

void AlphaTest::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    ur::AlphaTest alpha;
    alpha.enabled = true;

    switch (m_func)
    {
    case Func::Off:
        alpha.enabled = false;
        break;
    case Func::Never:
        alpha.function = ur::AlphaTestFunc::Never;
        break;
    case Func::Less:
        alpha.function = ur::AlphaTestFunc::Less;
        break;
    case Func::Equal:
        alpha.function = ur::AlphaTestFunc::Equal;
        break;
    case Func::LEqual:
        alpha.function = ur::AlphaTestFunc::LessThanOrEqual;
        break;
    case Func::Greater:
        alpha.function = ur::AlphaTestFunc::Greater;
        break;
    case Func::NotEqual:
        alpha.function = ur::AlphaTestFunc::NotEqual;
        break;
    case Func::GEqual:
        alpha.function = ur::AlphaTestFunc::GreaterThanOrEqual;
        break;
    case Func::Always:
        alpha.function = ur::AlphaTestFunc::Always;
        break;
    }

    alpha.ref = m_ref;

    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    rc->ur_ds.render_state.alpha_test = alpha;
}

}
}