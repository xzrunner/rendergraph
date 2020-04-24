#include "rendergraph/node/BlendEq.h"
#include "rendergraph/RenderContext.h"

namespace rendergraph
{
namespace node
{

void BlendEq::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    auto& blend = std::static_pointer_cast<RenderContext>(ctx)->ur_ds.render_state.blending;
    switch (m_mode)
    {
    case Mode::FuncAdd:
        blend.equation = ur2::BlendEquation::Add;
        break;
    case Mode::FuncSubtract:
        blend.equation = ur2::BlendEquation::Subtract;
        break;
    case Mode::FuncReverseSubtract:
        blend.equation = ur2::BlendEquation::ReverseSubtract;
        break;
    case Mode::Min:
        blend.equation = ur2::BlendEquation::Minimum;
        break;
    case Mode::Max:
        blend.equation = ur2::BlendEquation::Maximum;
        break;
    }
}

}
}