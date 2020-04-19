#pragma once

#include "rendergraph/Node.h"

namespace ur2 { class VertexArray; }

namespace rendergraph
{
namespace node
{

class Heightfield : public Node
{
public:
    Heightfield()
    {
        m_exports = {
            {{ VariableType::Model, "out" }}
        };
    }

    void Draw(const RenderContext& rc) const;

    void SetSize(size_t width, size_t height);

private:
    void Init(const RenderContext& rc) const;

private:
    mutable std::shared_ptr<ur2::VertexArray> m_vertex_array;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Heightfield.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Heightfield

}
}