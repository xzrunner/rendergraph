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
            {{ VariableType::Any,  "obj" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const RenderContext& rc) override;

    RTTR_ENABLE(Node)

}; // Draw

}
}