#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class AlphaTest : public Node
{
public:
    AlphaTest()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr);

    enum class Func
    {
        Off = 0,
        Never,
        Less,
        Equal,
        LEqual,
        Greater,
        NotEqual,
        GEqual,
        Always
    };

    void SetFunc(Func func) { m_func = func; }

private:
    Func m_func = Func::Always;

    float m_ref = 0;

    RTTR_ENABLE(Node)

}; // AlphaTest

}
}