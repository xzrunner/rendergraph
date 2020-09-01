#pragma once

#include "rendergraph/Node.h"

#include <string>

namespace rendergraph
{
namespace node
{

class String : public Node
{
public:
    enum class Type
    {
        Data,
        Code,
    };

public:
    String();

    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/String.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // String

}
}