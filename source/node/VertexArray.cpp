#include "rendergraph/node/VertexArray.h"
#include "rendergraph/RenderContext.h"

#include <unirender2/Context.h>
#include <unirender2/Device.h>
#include <unirender2/VertexArray.h>
#include <unirender2/VertexBuffer.h>
#include <unirender2/IndexBuffer.h>
#include <unirender2/VertexBufferAttribute.h>
#include <unirender2/DrawState.h>

namespace rendergraph
{
namespace node
{

void VertexArray::Draw(const RenderContext& rc) const
{
    if (!m_vertex_array) {
        Init(rc);
    }

    if (m_vertex_array)
    {
        ur2::DrawState draw;
        draw.render_state = rc.ur_rs;
        draw.program = rc.ur_ctx->GetShaderProgram();
        draw.vertex_array = m_vertex_array;

        rc.ur_ctx->Draw(ur2::PrimitiveType::Triangles, draw, nullptr);
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
}

void VertexArray::Init(const RenderContext& rc) const
{
    assert(!m_vertex_array);
    m_vertex_array = rc.ur_dev->CreateVertexArray();

    auto usage = ur2::BufferUsageHint::StaticDraw;

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

    std::vector<std::shared_ptr<ur2::VertexBufferAttribute>> vbuf_attrs;
    vbuf_attrs.resize(m_va_list.size());

    int stride_in_bytes = 0;
    for (auto& attr : m_va_list) {
        stride_in_bytes += attr.size * attr.num;
    }
    int offset_in_bytes = 0;

    for (size_t i = 0, n = m_va_list.size(); i < n; ++i)
    {
        auto& attr = m_va_list[i];

        ur2::ComponentDataType type;
        bool normalized;
        switch (attr.size)
        {
        case 1:
            type = ur2::ComponentDataType::UnsignedByte;
            normalized = true;
            break;
        case 2:
            type = ur2::ComponentDataType::UnsignedShort;
            normalized = true;
            break;
        case 4:
            type = ur2::ComponentDataType::Float;
            normalized = false;
            break;
        default:
            assert(0);
        }

        vbuf_attrs[i] = std::make_shared<ur2::VertexBufferAttribute>(
            type, attr.num, offset_in_bytes, stride_in_bytes
        );
        offset_in_bytes += attr.num * attr.size;
    }

    m_vertex_array->SetVertexBufferAttrs(vbuf_attrs);
}

}
}