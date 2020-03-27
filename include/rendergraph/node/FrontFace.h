#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class FrontFace : public Node
{
public:
    FrontFace()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr);

    void SetClockwise(bool clockwise) { m_clockwise = clockwise; }

private:
    bool m_clockwise = false;

    RTTR_ENABLE(Node)

}; // FrontFace

}
}