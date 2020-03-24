#include "rendergraph/node/Model.h"
#include "rendergraph/node/Shader.h"
#include "rendergraph/RenderContext.h"

#include <model/Model.h>
#include <model/ModelInstance.h>
#include <painting0/Material.h>
#include <painting3/MaterialMgr.h>
#include <painting3/RenderSystem.h>
#include <painting3/Blackboard.h>
#include <painting3/WindowContext.h>

namespace rendergraph
{
namespace node
{

void Model::Draw(const RenderContext& rc, std::shared_ptr<Shader>& shader) const
{
	pt3::RenderParams params;

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

    std::shared_ptr<ur::Shader> ur_shader = nullptr;
    pt0::UniformNames uniforms;
    if (shader) {
        ur_shader = shader->GetShader(rc.rc);
        uniforms  = shader->GetUniformNames();
    }
    pt3::RenderSystem::Instance()->DrawModel(
        *m_model, m_materials, params, ctx, ur_shader, uniforms
    );
}

void Model::SetModel(const std::shared_ptr<model::ModelInstance>& model)
{
    m_model = model;
    InitMaterials();
}

void Model::InitMaterials()
{
    m_materials.clear();

    if (!m_model) {
        return;
    }

    typedef pt3::MaterialMgr::PhongUniforms UNIFORMS;

    auto model = m_model->GetModel();
    m_materials.resize(model->materials.size());
    for (size_t i = 0, n = model->materials.size(); i < n; ++i)
    {
        auto& src = model->materials[i];
        auto& dst = m_materials[i];

        dst.AddVar(UNIFORMS::ambient.name,     pt0::RenderVariant(src->ambient));
        dst.AddVar(UNIFORMS::diffuse.name,     pt0::RenderVariant(src->diffuse));
        dst.AddVar(UNIFORMS::specular.name,    pt0::RenderVariant(src->specular));
        dst.AddVar(UNIFORMS::shininess.name,   pt0::RenderVariant(src->shininess));

        ur::Texture* tex = nullptr;
        if (src->diffuse_tex >= 0) {
            tex = model->textures[src->diffuse_tex].second.get();
            if (tex) {
                dst.AddVar(UNIFORMS::diffuse_tex.name, pt0::RenderVariant(tex));
            }
        }
    }
}

}
}