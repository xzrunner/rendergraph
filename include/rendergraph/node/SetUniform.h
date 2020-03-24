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

    virtual void Execute(const RenderContext& rc) override;

    void SetVarName(const std::string& name) { m_var_name = name; }
    void SetVarType(VariableType type) { m_var_type = type; }

private:
    std::string m_var_name;

    VariableType m_var_type = VariableType::Any;

    RTTR_ENABLE(Node)

}; // SetUniform

}
}