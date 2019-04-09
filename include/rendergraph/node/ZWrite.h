#pragma once

#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class ZWrite : public Node
{
public:
    ZWrite()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const RenderContext& rc) override;

    void SetDepthWrite(bool enable) { m_enable = enable; }

private:
    bool m_enable = false;

    RTTR_ENABLE(Node)

}; // ZWrite

}
}