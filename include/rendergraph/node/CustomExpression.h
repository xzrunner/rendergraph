#pragma once


#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class CustomExpression : public Node
{
public:
    enum InputID
    {
        I_PREV = 0,

        I_MAX_NUM
    };

    enum OutputID
    {
        O_NEXT = 0,

        O_MAX_NUM
    };

public:
    CustomExpression() 
    {
        m_imports.resize(I_MAX_NUM);
        m_imports[I_PREV] = {{ VariableType::Port, "prev" }};

        m_exports.resize(O_MAX_NUM);
        m_exports[O_NEXT] = {{ VariableType::Port, "next" }};
    }

    virtual void Execute(const RenderContext& rc) override;

    void SetCode(const std::string& code);

private:
    std::string m_body;
    
    RTTR_ENABLE(Node)

}; // CustomExpression

}
}