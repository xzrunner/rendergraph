#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class Rasterization : public Node
{
public:
    Rasterization()
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
        Point = 0,
        Line,
        Fill
    };

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Rasterization.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Rasterization

}
}