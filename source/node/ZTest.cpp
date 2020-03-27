#include "rendergraph/node/ZTest.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rendergraph
{
namespace node
{

void ZTest::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    ur::DEPTH_FORMAT fmt;
    switch (m_func)
    {
    case Func::Off:
        fmt = ur::DEPTH_DISABLE;
        break;
    case Func::Never:
        fmt = ur::DEPTH_NEVER;
        break;
    case Func::Less:
        fmt = ur::DEPTH_LESS;
        break;
    case Func::Equal:
        fmt = ur::DEPTH_EQUAL;
        break;
    case Func::LEqual:
        fmt = ur::DEPTH_LESS_EQUAL;
        break;
    case Func::Greater:
        fmt = ur::DEPTH_GREATER;
        break;
    case Func::NotEqual:
        fmt = ur::DEPTH_NOT_EQUAL;
        break;
    case Func::GEqual:
        fmt = ur::DEPTH_GREATER_EQUAL;
        break;
    case Func::Always:
        fmt = ur::DEPTH_ALWAYS;
        break;
    }

    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    rc->rc.SetZTest(fmt);
}

}
}