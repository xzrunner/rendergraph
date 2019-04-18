#pragma once

#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class PrimitiveShape : public Node
{
public:
    PrimitiveShape()
    {
        m_exports = {
            {{ VariableType::Model, "out" }}
        };
    }

    enum class Type
    {
        Cube = 0,
    };

    Type GetType() const { return m_type; }
    void SetType(Type type) { m_type = type; }

private:
    Type m_type = Type::Cube;

    RTTR_ENABLE(Node)

}; // PrimitiveShape

}
}