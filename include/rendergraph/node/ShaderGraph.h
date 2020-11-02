#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class ShaderGraph : public Node
{
public:
    ShaderGraph()
    {
        m_exports = {
            {{ VariableType::String, "out" }}
        };
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override {
        var = ShaderVariant(m_shader_code);
    }
    
    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/ShaderGraph.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // ShaderGraph

}
}