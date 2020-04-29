#include "rendergraph/node/Heightfield.h"
#include "rendergraph/RenderContext.h"

#include <unirender/DrawState.h>
#include <unirender/Context.h>
#include <unirender/Device.h>
#include <unirender/VertexArray.h>
#include <unirender/ComponentDataType.h>
#include <unirender/VertexBufferAttribute.h>
#include <unirender/IndexBuffer.h>
#include <unirender/VertexBuffer.h>
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

void Heightfield::Draw(RenderContext& rc) const
{
    if (!m_vertex_array) {
        Init(rc);
    }

    rc.ur_ds.vertex_array = m_vertex_array;
    rc.ur_ctx->Draw(ur::PrimitiveType::Triangles, rc.ur_ds, nullptr);

}

void Heightfield::SetSize(size_t width, size_t height)
{
    if (m_width == width || m_height == height) {
        return;
    }

    m_width  = width;
    m_height = height;

    //InitVertexBuf();
}

void Heightfield::Init(const RenderContext& rc) const
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

    assert(!m_vertex_array);
    m_vertex_array = rc.ur_dev->CreateVertexArray();

    auto usage = ur::BufferUsageHint::StaticDraw;

    auto ibuf_sz = sizeof(unsigned int) * indices.size();
    auto ibuf = rc.ur_dev->CreateIndexBuffer(usage, ibuf_sz);
    ibuf->ReadFromMemory(indices.data(), ibuf_sz, 0);
    m_vertex_array->SetIndexBuffer(ibuf);

    auto vbuf_sz = sizeof(Vertex) * verts.size();
    auto vbuf = rc.ur_dev->CreateVertexBuffer(usage, vbuf_sz);
    vbuf->ReadFromMemory(verts.data(), vbuf_sz, 0);
    m_vertex_array->SetVertexBuffer(vbuf);

    std::vector<std::shared_ptr<ur::VertexBufferAttribute>> vbuf_attrs;
    vbuf_attrs.resize(2);
    vbuf_attrs[0] = std::make_shared<ur::VertexBufferAttribute>(
        ur::ComponentDataType::Float, 2, 0, 4 * 5
    );
    vbuf_attrs[1] = std::make_shared<ur::VertexBufferAttribute>(
        ur::ComponentDataType::Float, 2, 4 * 3, 4 * 5
    );
    m_vertex_array->SetVertexBufferAttrs(vbuf_attrs);
}

}
}