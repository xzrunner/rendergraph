#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class Viewport : public Node
{
public:
    enum InputID
    {
        I_SIZE = 1,
    };

public:
    Viewport()
    {
        m_imports = {
            {{ VariableType::Port,    "prev" }},
            {{ VariableType::Vector2, "size" }},
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Viewport.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Viewport

}
}
