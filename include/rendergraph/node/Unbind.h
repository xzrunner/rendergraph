#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
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

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr);

    RTTR_ENABLE(Node)

}; // Unbind

}
}