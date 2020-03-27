#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
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

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr);

    void SetDepthWrite(bool enable) { m_enable = enable; }

private:
    bool m_enable = false;

    RTTR_ENABLE(Node)

}; // ZWrite

}
}