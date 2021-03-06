#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class SetUniform : public Node
{
public:
    enum InputID
    {
        I_PREV = 0,
        I_SHADER,
        I_VALUE,

        I_MAX_NUM
    };

    enum OutputID
    {
        O_NEXT = 0,

        O_MAX_NUM
    };

public:
    SetUniform()
    {
        m_imports.resize(I_MAX_NUM);
        m_imports[I_PREV]   = {{ VariableType::Port,   "prev" }};
        m_imports[I_SHADER] = {{ VariableType::Shader, "shader" }};
        m_imports[I_VALUE]  = {{ VariableType::Any,    "value" }};

        m_exports.resize(O_MAX_NUM);
        m_exports[O_NEXT]   = {{ VariableType::Port,   "next" }};
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/SetUniform.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // SetUniform

}
}