#pragma once

#include "rendergraph/Node.h"

namespace rg
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

    virtual void Execute(const RenderContext& rc) override;

    enum class CullType
    {
        Off = 0,
        Back,
        Front
    };

    void SetCullType(CullType type) { m_type = type; }

private:
    CullType m_type = CullType::Off;

    RTTR_ENABLE(Node)

}; // Cull

}
}