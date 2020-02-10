#include "rendergraph/node/Draw.h"
#include "rendergraph/node/PrimitiveShape.h"
#include "rendergraph/node/Texture.h"
#include "rendergraph/node/VertexArray.h"
#include "rendergraph/node/Model.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/node/Heightfield.h"

namespace rg
{
namespace node
{

void Draw::Execute(const RenderContext& rc)
{
    auto& conns = m_imports[ID_OBJ].conns;
    if (conns.empty()) {
        return;
    }

    std::shared_ptr<Shader> shader = nullptr;
    if (!m_imports[ID_SHADER].conns.empty())
    {
        auto node = m_imports[ID_SHADER].conns[0].node.lock();
        if (node->get_type() == rttr::type::get<Shader>()) {
            shader = std::static_pointer_cast<Shader>(node);
        }
    }

    for (auto& c : conns)
    {
        auto node = c.node.lock();
        if (!node) {
            continue;
        }

        auto type = node->get_type();
        if (type == rttr::type::get<PrimitiveShape>()) {
            std::static_pointer_cast<PrimitiveShape>(node)->Draw(rc);
        } else if (type == rttr::type::get<Texture>()) {
            std::static_pointer_cast<Texture>(node)->Draw(rc, shader);
        } else if (type == rttr::type::get<VertexArray>()) {
            std::static_pointer_cast<VertexArray>(node)->Draw(rc);
        } else if (type == rttr::type::get<Model>()) {
            std::static_pointer_cast<Model>(node)->Draw(rc, shader);
        } else if (type == rttr::type::get<Heightfield>()) {
            std::static_pointer_cast<Heightfield>(node)->Draw(rc);
        }
    }
}

}
}