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

    void Draw(const RenderContext& rc) const;

    enum class Type
    {
        Quad = 0,
        Cube,
    };

    Type GetType() const { return m_type; }
    void SetType(Type type) { m_type = type; }

private:
    Type m_type = Type::Quad;

    RTTR_ENABLE(Node)

}; // PrimitiveShape

}
}