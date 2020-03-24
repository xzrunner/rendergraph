#include "rendergraph/node/AlphaTest.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rendergraph
{
namespace node
{

void AlphaTest::Execute(const RenderContext& rc)
{
    ur::ALPHA_FUNC func;
    switch (m_func)
    {
    case Func::Off:
        func = ur::ALPHA_DISABLE;
        break;
    case Func::Never:
        func = ur::ALPHA_NEVER;
        break;
    case Func::Less:
        func = ur::ALPHA_LESS;
        break;
    case Func::Equal:
        func = ur::ALPHA_EQUAL;
        break;
    case Func::LEqual:
        func = ur::ALPHA_LEQUAL;
        break;
    case Func::Greater:
        func = ur::ALPHA_GREATER;
        break;
    case Func::NotEqual:
        func = ur::ALPHA_NOTEQUAL;
        break;
    case Func::GEqual:
        func = ur::ALPHA_GEQUAL;
        break;
    case Func::Always:
        func = ur::ALPHA_ALWAYS;
        break;
    }
    rc.rc.SetAlphaTest(func);
}

}
}