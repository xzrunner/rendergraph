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

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/BlendEq.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // BlendEq

}
}