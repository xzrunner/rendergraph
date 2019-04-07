#pragma once

#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class DrawCube : public Node
{
public:
    DrawCube()
    {
        m_imports = {
            {{ VariableType::Port, "prev" }}
        };
        m_exports = {
            {{ VariableType::Port, "next" }}
        };
    }

    virtual void Execute(ur::RenderContext& rc) override;

    RTTR_ENABLE(Node)

}; // DrawCube

}
}