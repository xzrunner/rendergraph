#include "rendergraph/node/Unbind.h"
#include "rendergraph/node/RenderTarget.h"

namespace rendergraph
{
namespace node
{

void Unbind::Execute(const RenderContext& rc)
{
    if (m_imports.empty() || m_imports[0].conns.empty()) {
        return;
    }

    auto node = m_imports[1].conns[0].node.lock();
    if (node)
    {
        auto type = node->get_type();
        if (type == rttr::type::get<node::RenderTarget>()) {
            std::static_pointer_cast<node::RenderTarget>(node)->Unbind(rc);
        }
    }
}

}
}