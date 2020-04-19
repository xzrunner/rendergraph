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

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/AlphaTest.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // AlphaTest

}
}