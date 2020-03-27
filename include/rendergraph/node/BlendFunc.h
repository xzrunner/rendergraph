#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class BlendFunc : public Node
{
public:
    BlendFunc()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr);

    enum class Factor
    {
        Off = 0,
        Zero, 
        One, 
        SrcColor, 
        OneMinusSrcColor, 
        DstColor, 
        OneMinusDstColor, 
        SrcAlpha, 
        OneMinusSrcAlpha, 
        DstAlpha, 
        OneMinusDstAlpha
    };

    void SetSrcFactor(Factor factor) { m_sfactor = factor; }
    void SetDstFactor(Factor factor) { m_dfactor = factor; }

private:
    Factor m_sfactor = Factor::One;
    Factor m_dfactor = Factor::Zero;

    RTTR_ENABLE(Node)

}; // BlendFunc

}
}