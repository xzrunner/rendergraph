#include "rendergraph/node/Unbind.h"
#include "rendergraph/node/RenderTarget.h"
#include "rendergraph/RenderContext.h"

namespace rendergraph
{
namespace node
{

void Unbind::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    if (m_imports.empty() || m_imports[0].conns.empty()) {
        return;
    }

    auto node = m_imports[1].conns[0].node.lock();
    if (node)
    {
        auto type = node->get_type();
        if (type == rttr::type::get<node::RenderTarget>()) {
            auto rc = std::static_pointer_cast<RenderContext>(ctx);
            std::static_pointer_cast<node::RenderTarget>(node)->Unbind(*rc);
        }
    }
}

}
}