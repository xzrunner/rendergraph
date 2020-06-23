#pragma once

#include "rendergraph/Node.h"

#include <unirender/PrimitiveType.h>

namespace ur { class VertexArray; class Device; }
namespace model { class ParametricSurface; }

namespace rendergraph
{
namespace node
{

class PrimitiveShape : public Node
{
public:
    enum class Type
    {
        Quad = 0,
        Cube,
        Sphere,
    };

    enum class VertLayout
    {
        Pos = 0,
        PosTex,
        PosNorm,
        PosNormTex,
        PosNormTexTB,
    };

public:
    PrimitiveShape()
    {
        m_exports = {
            {{ VariableType::Model, "out" }}
        };
    }

    void Draw(const RenderContext& rc) const;

    void SetType(Type type);
    Type GetType() const { return m_type; }

    void SetVertLayout(VertLayout layout);
    VertLayout GetVertLayout() const { return m_vert_layout; }

private:
    void SetupVertices(const RenderContext& rc) const;

    static std::shared_ptr<ur::VertexArray> BuildVertexArray(
        const ur::Device& dev, const model::ParametricSurface& surface, VertLayout layout
    );

private:
    mutable bool m_dirty = false;

    mutable std::shared_ptr<ur::VertexArray> m_va = nullptr;
    mutable ur::PrimitiveType m_prim_type;

    RTTR_ENABLE(Node)

#define NOT_GEN_SETTER_GETTER_FUNC
#define PARM_FILEPATH "rendergraph/node/PrimitiveShape.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH
#undef NOT_GEN_SETTER_GETTER_FUNC

}; // PrimitiveShape

}
}