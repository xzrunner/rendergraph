#pragma once

#include "rendergraph/Node.h"

namespace rg
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
    virtual ~VertexArray();

    virtual void Execute(const RenderContext& rc) override {}

    void Draw(const RenderContext& rc) const;

    struct VertexAttrib
    {
        std::string name;
        size_t num;
        size_t size;
    };

    void SetVertList(const std::vector<VertexAttrib>& va_list);
    void SetVertexBuf(const std::vector<float>& vertex_buf) {
        m_vertex_buf = vertex_buf;
    }
    void SetIndexBuf(const std::vector<unsigned short>& index_buf) {
        m_index_buf = index_buf;
    }

private:
    void Init(const RenderContext& rc) const;

private:
    mutable unsigned int m_vao = 0;
    mutable unsigned int m_vbo = 0;
    mutable unsigned int m_ebo = 0;

    std::vector<VertexAttrib> m_va_list;

    size_t m_stride = 0;
    std::vector<float> m_vertex_buf;
    std::vector<unsigned short> m_index_buf;

    RTTR_ENABLE(Node)

}; // VertexArray

}
}