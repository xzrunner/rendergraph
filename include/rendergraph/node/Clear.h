#pragma once

#include <painting0/Color.h>

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class Clear : public Node
{
public:
    enum class Type
    {
        Color,
        Depth,
        Stencil,
    };

public:
    Clear()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }},
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr);

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Clear.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Clear

}
}