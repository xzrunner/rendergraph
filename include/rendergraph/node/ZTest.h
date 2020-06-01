#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class ZTest : public Node
{
public:
    ZTest()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;

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

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/ZTest.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // ZTest

}
}