#include "rendergraph/node/Unbind.h"
#include "rendergraph/node/RenderTarget.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void Unbind::Execute(const RenderContext& rc)
{
    if (m_imports.empty() || m_imports[0].conns.empty()) {
        return;
    }

    auto node = m_imports[0].conns[0].node.lock();
    if (node)
    {
        auto type = node->get_type();
        if (type == rttr::type::get<node::RenderTarget>())
        {
            //auto rt = std::static_pointer_cast<node::RenderTarget>(node);
            rc.rc.UnbindRenderTarget();
        }
    }
}

}
}