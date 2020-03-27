#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class BlendEq : public Node
{
public:
    BlendEq()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr);

    enum class Mode
    {
        FuncAdd, 
        FuncSubtract, 
        FuncReverseSubtract, 
        Min, 
        Max
    };

    void SetMode(Mode mode) { m_mode = mode; }

private:
    Mode m_mode = Mode::FuncAdd;

    RTTR_ENABLE(Node)

}; // BlendEq

}
}