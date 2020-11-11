#include "rendergraph/node/Viewport.h"
#include "rendergraph/RenderContext.h"
#include "rendergraph/Evaluator.h"

#include <unirender/Context.h>

namespace rendergraph
{
namespace node
{

void Viewport::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    if (!m_imports[I_SIZE].conns.empty())
    {
        auto rc = std::static_pointer_cast<RenderContext>(ctx);

        sm::vec2 sz(static_cast<float>(m_w), static_cast<float>(m_h));
        uint32_t flags = 0;
        auto v_sz = Evaluator::Calc(*rc, m_imports[I_SIZE], ShaderVariant(sz), flags);
        if (v_sz.type != VariableType::Any) {
            rc->ur_ctx->SetViewport(m_x, m_y, static_cast<int>(v_sz.vec2.x), static_cast<int>(v_sz.vec2.y));
        }
    }
}

}
}