#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class Bind : public Node
{
public:
    Bind()
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

    void SetChannel(int channel) { m_channel = channel; }

private:
    int m_channel = 0;

    RTTR_ENABLE(Node)

}; // Bind

}
}