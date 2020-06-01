#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class Draw : public Node
{
public:
    Draw()
    {
        m_imports = {
            {{ VariableType::Port,   "prev" }},
            {{ VariableType::Any,    "obj" }},
            {{ VariableType::Shader, "shader" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;

    enum InputID
    {
        ID_OBJ = 1,
        ID_SHADER
    };

    RTTR_ENABLE(Node)

}; // Draw

}
}