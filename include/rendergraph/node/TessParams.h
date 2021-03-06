#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class TessParams : public Node
{
public:
    TessParams()
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

#define PARM_FILEPATH "rendergraph/node/TessParams.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // TessParams

}
}