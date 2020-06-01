#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class BlendFunc : public Node
{
public:
    BlendFunc()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;

    enum class Factor
    {
        Off = 0,
        Zero,
        One,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha
    };

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/BlendFunc.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // BlendFunc

}
}