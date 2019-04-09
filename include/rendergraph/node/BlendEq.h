#pragma once

#include "rendergraph/Node.h"

namespace rg
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

    virtual void Execute(const RenderContext& rc) override;

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