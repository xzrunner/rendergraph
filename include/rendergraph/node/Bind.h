#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class Bind : public Node
{
public:
    Bind()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }},
            {{ VariableType::Any,  "obj" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Bind.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Bind

}
}