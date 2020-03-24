#include "rendergraph/node/Heightfield.h"
#include "rendergraph/RenderContext.h"

#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>
#include <renderpipeline/UniformNames.h>

namespace
{

class Vertex
{
public:
    Vertex() {}
    Vertex(size_t ix, size_t iz, size_t size_x, size_t size_z)
    {
        float x = ix / static_cast<float>(size_x);
        float z = iz / static_cast<float>(size_z);
        position.Set(x, 0, z);
        texcoords.Set(x, z);
    }

    sm::vec3 position;
    sm::vec2 texcoords;
};

}

namespace rendergraph
{
namespace node
{

Heightfield::~Heightfield()
{
    ur::Blackboard::Instance()->GetRenderContext().ReleaseVAO(m_vao, m_vbo, m_ebo);
}

void Heightfield::Draw(const RenderContext& rc) const
{
    assert(m_vao > 0 && m_vbo > 0 && m_ebo > 0);
    size_t n = (m_width - 1) * (m_height - 1) * 6;
    ur::Blackboard::Instance()->GetRenderContext().DrawElementsVAO(ur::DRAW_TRIANGLES, 0, n, m_vao, false);
}

void Heightfield::SetSize(size_t width, size_t height)
{
    if (m_width == width || m_height == height) {
        return;
    }

    m_width  = width;
    m_height = height;

    InitVertexBuf();
}

void Heightfield::InitVertexBuf()
{
    std::vector<Vertex> verts(m_width * m_height);
    size_t ptr = 0;
    for (size_t y = 0; y < m_height; ++y) {
        for (size_t x = 0; x < m_width; ++x) {
            verts[ptr++] = Vertex(x, y, m_width, m_height);
        }
    }

    ptr = 0;
    std::vector<uint32_t> indices((m_width - 1) * (m_height - 1) * 6);
    for (size_t y = 0; y < m_height - 1; ++y) {
        for (size_t x = 0; x < m_width - 1; ++x) {
            size_t ll = y * m_width + x;
            size_t rl = ll + 1;
            size_t lh = ll + m_width;
            size_t rh = lh + 1;
            indices[ptr++] = ll;
            indices[ptr++] = lh;
            indices[ptr++] = rh;
            indices[ptr++] = ll;
            indices[ptr++] = rh;
            indices[ptr++] = rl;
        }
    }

    ur::RenderContext::VertexInfo vi;

    const size_t stride = 5;    // float number
    vi.vn        = verts.size();
    vi.vertices  = verts.data();
    vi.stride    = stride * sizeof(float);
    vi.indices   = indices.data();
    vi.in        = indices.size();
    vi.idx_short = false;

    vi.va_list.push_back(ur::VertexAttrib(rp::VERT_POSITION_NAME, 3, 4, vi.stride, 0));   // vertices
    vi.va_list.push_back(ur::VertexAttrib(rp::VERT_TEXCOORD_NAME, 2, 4, vi.stride, 12));  // texcoord

    ur::Blackboard::Instance()->GetRenderContext().CreateVAO(vi, m_vao, m_vbo, m_ebo);
}

}
}