#pragma once

#include "rendergraph/Node.h"

namespace ur { class VertexArray; }

namespace rendergraph
{
namespace node
{

class VertexArray : public Node
{
public:
    VertexArray()
    {
        m_exports = {
           {{ VariableType::Model, "out" }}
        };
    }

    void Draw(RenderContext& rc) const;

    struct VertexAttrib
    {
        std::string name;
        size_t num;
        size_t size;
    };

    void SetVertList(const std::vector<VertexAttrib>& va_list);
    void SetVertexBuf(const std::vector<float>& vertex_buf) {
        m_vertex_buf = vertex_buf;
        m_dirty = true;
    }
    void SetIndexBuf(const std::vector<unsigned short>& index_buf) {
        m_index_buf = index_buf;
        m_dirty = true;
    }

private:
    void Rebuild(const RenderContext& rc) const;

private:
    mutable std::shared_ptr<ur::VertexArray> m_vertex_array = nullptr;

    std::vector<VertexAttrib> m_va_list;

    size_t m_stride = 0;
    std::vector<float> m_vertex_buf;
    std::vector<unsigned short> m_index_buf;

    mutable bool m_dirty = false;

    RTTR_ENABLE(Node)

}; // VertexArray

}
}