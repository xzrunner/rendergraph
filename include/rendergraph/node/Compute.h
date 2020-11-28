#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class Compute : public Node
{
public:
    Compute()
    {
        m_imports = {
            {{ VariableType::Port,   "prev" }},
            {{ VariableType::Shader, "shader" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;

    enum InputID
    {
        ID_SHADER = 1,
    };

    RTTR_ENABLE(Node)

}; // Compute

}
}