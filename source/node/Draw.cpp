#include "rendergraph/node/Draw.h"
#include "rendergraph/node/PrimitiveShape.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

void Draw::Execute(const RenderContext& rc)
{
    const int OBJ_ID = 1;
    auto& conns = m_imports[OBJ_ID].conns;
    if (conns.empty()) {
        return;
    }

    for (auto& c : conns)
    {
        auto node = c.node.lock();
        if (!node) {
            continue;
        }

        auto type = node->get_type();
        if (type == rttr::type::get<PrimitiveShape>())
        {
            auto prim = std::static_pointer_cast<PrimitiveShape>(node);
            switch (prim->GetType())
            {
            case PrimitiveShape::Type::Cube:
                rc.rc.RenderCube();
                break;
            }
        }
    }

    rc.rc.RenderCube();
}

}
}