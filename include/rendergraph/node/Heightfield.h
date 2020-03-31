#pragma once

#include "rendergraph/Node.h"

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

        InitVertexBuf();
    }
    virtual ~Heightfield();

    void Draw(const RenderContext& rc) const;

    void SetSize(size_t width, size_t height);

private:
    void InitVertexBuf();

private:
    uint32_t m_vao = 0, m_vbo = 0, m_ebo = 0;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Heightfield.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Heightfield

}
}