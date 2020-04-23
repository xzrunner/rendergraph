#include "rendergraph/node/Clear.h"
#include "rendergraph/RenderContext.h"

#include <unirender2/ClearState.h>
#include <unirender2/Context.h>

namespace rendergraph
{
namespace node
{

void Clear::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    if (m_clear_type.empty()) {
        return;
    }

    ur2::ClearState clear;
    int clear_mask = 0;
    for (auto& type : m_clear_type)
    {
        switch (type)
        {
        case Type::Color:
        {
            clear_mask |= static_cast<int>(ur2::ClearBuffers::ColorBuffer);

            clear.color.r = m_col.r;
            clear.color.g = m_col.g;
            clear.color.b = m_col.b;
            clear.color.a = m_col.a;
        }
            break;
        case Type::Depth:
            clear_mask |= static_cast<int>(ur2::ClearBuffers::DepthBuffer);
            break;
        case Type::Stencil:
            clear_mask |= static_cast<int>(ur2::ClearBuffers::StencilBuffer);
            break;
        }
    }

    clear.buffers = static_cast<ur2::ClearBuffers>(clear_mask);

    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    rc->ur_ctx->Clear(clear);
}

}
}