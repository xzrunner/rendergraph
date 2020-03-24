#pragma once

#include "rendergraph/Node.h"

#include <painting0/Material.h>

namespace model { struct Model; class ModelInstance; }

namespace rendergraph
{
namespace node
{

class Shader;

class Model : public Node
{
public:
    Model()
    {
        m_exports = {
            {{ VariableType::Model, "out" }}
        };
    }

    void Draw(const RenderContext& rc, std::shared_ptr<Shader>& shader) const;

    void SetModel(const std::shared_ptr<model::ModelInstance>& model);

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