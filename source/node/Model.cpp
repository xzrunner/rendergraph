#include "rendergraph/node/Model.h"

#include <model/Model.h>
#include <model/ModelInstance.h>
#include <painting0/Material.h>
#include <painting3/MaterialMgr.h>

namespace rg
{
namespace node
{

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