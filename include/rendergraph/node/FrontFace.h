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

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/FrontFace.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // FrontFace

}
}