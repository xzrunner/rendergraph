#include "rendergraph/node/VertexArray.h"
#include "rendergraph/RenderContext.h"

#include <unirender/Context.h>
#include <unirender/Device.h>
#include <unirender/VertexArray.h>
#include <unirender/VertexBuffer.h>
#include <unirender/IndexBuffer.h>
#include <unirender/VertexInputAttribute.h>
#include <unirender/DrawState.h>

namespace rendergraph
{
namespace node
{

void VertexArray::Draw(RenderContext& rc) const
{
    if (!m_vertex_array || m_dirty) {
        Rebuild(rc);
    }

    if (m_vertex_array)
    {
        rc.ur_ds.vertex_array = m_vertex_array;
        rc.ur_ctx->Draw(ur::PrimitiveType::Triangles, rc.ur_ds, nullptr);
    }
}

void VertexArray::SetVertList(const std::vector<VertexAttrib>& va_list)
{
    m_va_list = va_list;

    m_stride = 0;
    for (auto& va : va_list) {
        m_stride += va.num * va.size;
    }
    m_stride /= sizeof(float);

    m_dirty = true;
}

void VertexArray::Rebuild(const RenderContext& rc) const
{
    m_vertex_array = rc.ur_dev->CreateVertexArray();

    auto usage = ur::BufferUsageHint::StaticDraw;

    if (!m_index_buf.empty()) {
        auto ibuf_sz = sizeof(unsigned short) * m_index_buf.size();
        auto ibuf = rc.ur_dev->CreateIndexBuffer(usage, ibuf_sz);
        ibuf->ReadFromMemory(m_index_buf.data(), ibuf_sz, 0);
        m_vertex_array->SetIndexBuffer(ibuf);
    }

    auto vbuf_sz = sizeof(float) * m_vertex_buf.size();
    auto vbuf = rc.ur_dev->CreateVertexBuffer(usage, vbuf_sz);
    vbuf->ReadFromMemory(m_vertex_buf.data(), vbuf_sz, 0);
    m_vertex_array->SetVertexBuffer(vbuf);

    std::vector<std::shared_ptr<ur::VertexInputAttribute>> vbuf_attrs;
    vbuf_attrs.resize(m_va_list.size());

    int stride_in_bytes = 0;
    for (auto& attr : m_va_list) {
        stride_in_bytes += attr.size * attr.num;
    }
    int offset_in_bytes = 0;

    for (size_t i = 0, n = m_va_list.size(); i < n; ++i)
    {
        auto& attr = m_va_list[i];

        ur::ComponentDataType type;
        bool normalized;
        switch (attr.size)
        {
        case 1:
            type = ur::ComponentDataType::UnsignedByte;
            normalized = true;
            break;
        case 2:
            type = ur::ComponentDataType::UnsignedShort;
            normalized = true;
            break;
        case 4:
            type = ur::ComponentDataType::Float;
            normalized = false;
            break;
        default:
            assert(0);
        }

        vbuf_attrs[i] = std::make_shared<ur::VertexInputAttribute>(
            i, type, attr.num, offset_in_bytes, stride_in_bytes
        );
        offset_in_bytes += attr.num * attr.size;
    }

    m_vertex_array->SetVertexBufferAttrs(vbuf_attrs);

    m_dirty = false;
}

}
}