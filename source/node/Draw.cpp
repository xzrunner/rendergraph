#include "rendergraph/node/Draw.h"
#include "rendergraph/node/PrimitiveShape.h"
#include "rendergraph/node/Texture.h"
#include "rendergraph/node/VertexArray.h"
#include "rendergraph/node/Model.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/node/Heightfield.h"
#include "rendergraph/RenderContext.h"

namespace rendergraph
{
namespace node
{

void Draw::Execute(const std::shared_ptr<dag::Context>& ctx)
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

    auto rc = std::static_pointer_cast<RenderContext>(ctx);
    std::shared_ptr<ur::ShaderProgram> prog = nullptr;
    if (shader) {
        prog = shader->GetShader(*rc);
    }

    if (prog) {
        rc->ur_ds.program = prog;
    }

    for (auto& c : conns)
    {
        auto node = c.node.lock();
        if (!node) {
            continue;
        }

        auto type = node->get_type();
        if (type == rttr::type::get<PrimitiveShape>()) {
            std::static_pointer_cast<PrimitiveShape>(node)->Draw(*rc);
        } else if (type == rttr::type::get<Texture>()) {
            std::static_pointer_cast<Texture>(node)->Draw(*rc, shader);
        } else if (type == rttr::type::get<VertexArray>()) {
            std::static_pointer_cast<VertexArray>(node)->Draw(*rc);
        } else if (type == rttr::type::get<Model>()) {
            std::static_pointer_cast<Model>(node)->Draw(*rc, shader);
        } else if (type == rttr::type::get<Heightfield>()) {
            std::static_pointer_cast<Heightfield>(node)->Draw(*rc);
        }
    }
}

}
}