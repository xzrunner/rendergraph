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
    size_t m_width = 128, m_height = 128;

    uint32_t m_vao = 0, m_vbo = 0, m_ebo = 0;

    RTTR_ENABLE(Node)

}; // Heightfield

}
}