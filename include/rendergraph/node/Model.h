#pragma once

#include "rendergraph/Node.h"

#include <painting0/Material.h>

namespace model { struct Model; class ModelInstance; }

namespace rg
{
namespace node
{

class Model : public Node
{
public:
    Model()
    {
        m_exports = {
            {{ VariableType::Model, "out" }}
        };
    }

    virtual void Execute(const RenderContext& rc) override {}

    auto GetModel() const { return m_model; }
    void SetModel(const std::shared_ptr<model::ModelInstance>& model);

    auto& GetMaterials() const { return m_materials; }

private:
    // todo: copy from CompModel::InitMaterials()
    void InitMaterials();

private:
    std::shared_ptr<model::ModelInstance> m_model = nullptr;

    std::vector<pt0::Material> m_materials;

    RTTR_ENABLE(Node)

}; // Model

}
}