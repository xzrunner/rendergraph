#include "rendergraph/node/BlendEq.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rendergraph
{
namespace node
{

void BlendEq::Execute(const RenderContext& rc)
{
    ur::BLEND_FUNC mode;
    switch (m_mode)
    {
    case Mode::FuncAdd:
        mode = ur::BLEND_FUNC_ADD;
        break;
    case Mode::FuncSubtract:
        mode = ur::BLEND_FUNC_SUBTRACT;
        break;
    case Mode::FuncReverseSubtract:
        mode = ur::BLEND_FUNC_REVERSE_SUBTRACT;
        break;
    case Mode::Min:
        mode = ur::BLEND_MIN;
        break;
    case Mode::Max:
        mode = ur::BLEND_MAX;
        break;
    }
    rc.rc.SetBlendEquation(mode);
}

}
}