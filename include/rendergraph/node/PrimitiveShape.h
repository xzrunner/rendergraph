#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
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

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/PrimitiveShape.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // PrimitiveShape

}
}