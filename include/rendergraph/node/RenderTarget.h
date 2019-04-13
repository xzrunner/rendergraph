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
    virtual ~RenderTarget();

    virtual void Execute(const RenderContext& rc) override;

    unsigned int GetID() const { return m_id; }

    bool IsBinded() const { return m_binded; }
    void SetBinded(bool binded) { m_binded = binded; }

private:
    unsigned int m_id = 0;

    bool m_binded = false;

    RTTR_ENABLE(Node)

}; // RenderTarget

}
}