#include "rendergraph/node/AlphaTest.h"
#include "rendergraph/RenderContext.h"

namespace rendergraph
{
namespace node
{

void AlphaTest::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    ur2::AlphaTest alpha;
    alpha.enabled = true;

    switch (m_func)
    {
    case Func::Off:
        alpha.enabled = false;
        break;
    case Func::Never:
        alpha.function = ur2::AlphaTestFunc::Never;
        break;
    case Func::Less:
        alpha.function = ur2::AlphaTestFunc::Less;
        break;
    case Func::Equal:
        alpha.function = ur2::AlphaTestFunc::Equal;
        break;
    case Func::LEqual:
        alpha.function = ur2::AlphaTestFunc::LessThanOrEqual;
        break;
    case Func::Greater:
        alpha.function = ur2::AlphaTestFunc::Greater;
        break;
    case Func::NotEqual:
        alpha.function = ur2::AlphaTestFunc::NotEqual;
        break;
    case Func::GEqual:
        alpha.function = ur2::AlphaTestFunc::GreaterThanOrEqual;
        break;
    case Func::Always:
        alpha.function = ur2::AlphaTestFunc::Always;
        break;
    }

    alpha.ref = m_ref;

    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    rc->ur_ds.render_state.alpha_test = alpha;
}

}
}