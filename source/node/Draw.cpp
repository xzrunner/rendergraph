#include "rendergraph/node/Draw.h"
#include "rendergraph/node/PrimitiveShape.h"
#include "rendergraph/node/Texture.h"
#include "rendergraph/node/VertexArray.h"
#include "rendergraph/node/Model.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>
#include <painting2/RenderSystem.h>
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
    auto& conns = m_imports[ID_OBJ].conns;
    if (conns.empty()) {
        return;
    }

    std::shared_ptr<ur::Shader> shader = nullptr;
    pt0::UniformNames uniforms;
    if (!m_imports[ID_SHADER].conns.empty())
    {
        auto node = m_imports[ID_SHADER].conns[0].node.lock();
        if (node->get_type() == rttr::type::get<Shader>()) {
            auto snode = std::static_pointer_cast<Shader>(node);
            shader = snode->GetShader(rc);
            uniforms = snode->GetUniformNames();
        }
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
            case PrimitiveShape::Type::Quad:
                rc.rc.RenderQuad();
                break;
            case PrimitiveShape::Type::Cube:
                rc.rc.RenderCube();
                break;
            }
        }
        else if (type == rttr::type::get<Texture>())
        {
            auto tex = std::static_pointer_cast<Texture>(node);
            pt2::RenderSystem::DrawTexture(
                tex->GetWidth(), tex->GetHeight(), tex->GetTexID(), sm::rect(100, 100), sm::Matrix2D()
            );
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
            ctx.AddVar(
                pt3::MaterialMgr::PositionUniforms::cam_pos.name,
                pt0::RenderVariant(rc.cam_position)
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
                *model->GetModel(), model->GetMaterials(), params, ctx, shader, uniforms
            );
        }
    }
}

}
}