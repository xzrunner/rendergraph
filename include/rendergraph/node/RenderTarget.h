#pragma once

#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class RenderTarget : public Node
{
public:
    RenderTarget()
    {
        m_imports = {
            {{ VariableType::Port,    "prev" }},
            {{ VariableType::Texture, "tex" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(ur::RenderContext& rc) override;

    unsigned int GetID() const { return m_id; }

private:
    unsigned int m_id = 0;

    RTTR_ENABLE(Node)

}; // RenderTarget

}
}