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

    enum class VertLayout
    {
        Pos = 0,
        PosTex,
        PosNormTex,
        PosNormTexTB,
    };

    Type GetType() const { return m_type; }
    void SetType(Type type) { m_type = type; }

    VertLayout GetVertLayout() const { return m_vert_layout; }
    void SetVertLayout(VertLayout layout) { m_vert_layout = layout; }

private:
    Type m_type = Type::Quad;

    VertLayout m_vert_layout = VertLayout::Pos;

    RTTR_ENABLE(Node)

}; // PrimitiveShape

}
}