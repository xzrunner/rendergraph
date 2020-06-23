#pragma once


#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class CustomFunction : public Node
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
    CustomFunction()
    {
        m_imports.resize(I_MAX_NUM);
        m_imports[I_PREV] = {{ VariableType::Port, "prev" }};

        m_exports.resize(O_MAX_NUM);
        m_exports[O_NEXT] = {{ VariableType::Port, "next" }};
    }

    virtual void Execute(const std::shared_ptr<dag::Context>& ctx = nullptr) override;

    void SetCode(const std::string& code);

private:
    std::string m_body;

    RTTR_ENABLE(Node)

}; // CustomFunction

}
}