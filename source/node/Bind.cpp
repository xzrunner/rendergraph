#include "rendergraph/node/Bind.h"
#include "rendergraph/node/Texture.h"
#include "rendergraph/node/RenderTarget.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>
#include <unirender/Shader.h>

namespace rg
{
namespace node
{

void Bind::Execute(const RenderContext& rc)
{
    if (m_imports.empty() || m_imports[0].conns.empty()) {
        return;
    }

    auto node = m_imports[1].conns[0].node.lock();
    if (node)
    {
        node->Execute(rc);

        auto type = node->get_type();
        if (type == rttr::type::get<node::Texture>())
        {
            rc.rc.BindTexture(std::static_pointer_cast<node::Texture>(node)->GetTexID(), m_channel);
        }
        else if (type == rttr::type::get<node::RenderTarget>())
        {
            auto rt = std::static_pointer_cast<node::RenderTarget>(node);
            rc.rc.BindRenderTarget(rt->GetID());
            auto& conns = rt->GetImports()[0].conns;
            if (!conns.empty()) {
                auto tex_node = conns[0].node.lock();
                if (tex_node && tex_node->get_type() == rttr::type::get<node::Texture>()) {
                    rc.rc.BindRenderTargetTex(std::static_pointer_cast<node::Texture>(tex_node)->GetTexID());
                }
            }
        }
        else if (type == rttr::type::get<node::Shader>())
        {
            std::static_pointer_cast<node::Shader>(node)->Bind(rc);
        }
    }
}

}
}