#pragma once

#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class Unbind : public Node
{
public:
    Unbind()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }},
            {{ VariableType::Any,  "obj" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(ur::RenderContext& rc) override;

    RTTR_ENABLE(Node)

}; // Unbind

}
}