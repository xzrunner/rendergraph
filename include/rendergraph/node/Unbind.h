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
            {{ VariableType::Any,  "obj" }}
        };
    }

    virtual void Execute(ur::RenderContext& rc) override;

    RTTR_ENABLE(Node)

}; // Unbind

}
}