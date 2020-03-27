#include "rendergraph/node/Bind.h"
#include "rendergraph/node/Texture.h"
#include "rendergraph/node/RenderTarget.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/RenderContext.h"

namespace rendergraph
{
namespace node
{

void Bind::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    if (m_imports.empty() || m_imports[1].conns.empty()) {
        return;
    }

    auto node = m_imports[1].conns[0].node.lock();
    if (node)
    {
        node->Execute(ctx);

        auto rc = std::static_pointer_cast<RenderContext>(ctx);

        auto type = node->get_type();
        if (type == rttr::type::get<node::Texture>()) {
            std::static_pointer_cast<node::Texture>(node)->Bind(*rc, m_channel);
        } else if (type == rttr::type::get<node::RenderTarget>()) {
            std::static_pointer_cast<node::RenderTarget>(node)->Bind(*rc);
        } else if (type == rttr::type::get<node::Shader>()) {
            std::static_pointer_cast<node::Shader>(node)->Bind(*rc);
        }
    }
}

}
}