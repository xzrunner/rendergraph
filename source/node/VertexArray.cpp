#include "rendergraph/node/VertexArray.h"
#include "rendergraph/RenderContext.h"

#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>

namespace rg
{
namespace node
{

VertexArray::~VertexArray()
{
    // fixme
    auto& rc = ur::Blackboard::Instance()->GetRenderContext();
    if (m_vao != 0) {
		rc.ReleaseVAO(m_vao, m_vbo, m_ebo);
	} else {
        if (m_vbo != 0) {
            rc.ReleaseBuffer(ur::VERTEXBUFFER, m_vbo);
        }
		if (m_ebo != 0) {
			rc.ReleaseBuffer(ur::INDEXBUFFER, m_ebo);
		}
	}
}

void VertexArray::Draw(const RenderContext& rc) const
{
    if (m_vao == 0) {
        Init(rc);
    }

    if (m_vao != 0)
    {
        if (m_index_buf.empty()) {
            rc.rc.DrawArraysVAO(ur::DRAW_TRIANGLES, 0, m_vertex_buf.size() / m_stride, m_vao);
        } else {
            rc.rc.DrawElementsVAO(ur::DRAW_TRIANGLES, 0, m_index_buf.size(), m_vao);
        }
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
    ur::RenderContext::VertexInfo vi;

    int stride = 0;
    for (auto& va : m_va_list) {
        stride += va.size * va.num;
    }
    vi.stride = stride;

    vi.vn       = m_vertex_buf.size() / m_stride;
    vi.vertices = m_vertex_buf.data();
    vi.in       = m_index_buf.size();
    vi.indices  = m_index_buf.data();

    int offset = 0;
    for (auto& va : m_va_list) {
        vi.va_list.push_back(ur::VertexAttrib(va.name, va.num, va.size, stride, offset));
        offset += va.num * va.size;
    }

    rc.rc.CreateVAO(vi, m_vao, m_vbo, m_ebo);
}

}
}