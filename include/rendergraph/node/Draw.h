#pragma once

#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class Draw : public Node
{
public:
    Draw()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }},
            {{ VariableType::Any,  "obj" }},
            {{ VariableType::Any,  "shader" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const RenderContext& rc) override;

    enum InputID
    {
        ID_OBJ = 1,
        ID_SHADER
    };

    RTTR_ENABLE(Node)

}; // Draw

}
}