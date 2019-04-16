#include "rendergraph/node/Draw.h"
#include "rendergraph/node/PrimitiveShape.h"
#include "rendergraph/node/VertexArray.h"
#include "rendergraph/node/Model.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>
#include <painting3/RenderSystem.h>
#include <painting3/MaterialMgr.h>
#include <painting3/Blackboard.h>
#include <painting3/WindowContext.h>

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
        else if (type == rttr::type::get<VertexArray>())
        {
            std::static_pointer_cast<VertexArray>(node)->Draw(rc);
        }
        else if (type == rttr::type::get<Model>())
        {
            auto model = std::static_pointer_cast<Model>(node);

	        pt3::RenderParams params;
	        params.type = pt3::RenderParams::DRAW_MESH;

            pt0::RenderContext ctx;
            ctx.AddVar(
                pt3::MaterialMgr::PositionUniforms::light_pos.name,
                pt0::RenderVariant(sm::vec3(0, 2, -4))
            );
            auto& wc = pt3::Blackboard::Instance()->GetWindowContext();
            assert(wc);
            ctx.AddVar(
                pt3::MaterialMgr::PosTransUniforms::view.name,
                pt0::RenderVariant(wc->GetViewMat())
            );
            ctx.AddVar(
                pt3::MaterialMgr::PosTransUniforms::projection.name,
                pt0::RenderVariant(wc->GetProjMat())
            );

            pt3::RenderSystem::Instance()->DrawModel(
                *model->GetModel(), model->GetMaterials(), params, ctx
            );
        }
    }
}

}
}