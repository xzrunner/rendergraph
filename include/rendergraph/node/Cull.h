#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class Cull : public Node
{
public:
    Cull()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;

    enum class Mode
    {
        Off = 0,
        Front,
        Back,
        FrontAndBack,
    };

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Cull.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Cull

}
}