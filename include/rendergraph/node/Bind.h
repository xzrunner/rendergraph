#pragma once

#include "rendergraph/Node.h"

namespace rg
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

    virtual void Execute(ur::RenderContext& rc) override;

    void SetChannel(int channel) { m_channel = channel; }

private:
    int m_channel = 0;

    RTTR_ENABLE(Node)

}; // Bind

}
}