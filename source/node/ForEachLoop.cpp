#include "rendergraph/node/ForEachLoop.h"
#include "rendergraph/node/CustomData.h"
#include "rendergraph/ScriptEnv.h"
#include "rendergraph/typedef.h"
#include "rendergraph/DrawList.h"
#include "rendergraph/RenderContext.h"

#include <queue>

namespace rendergraph
{
namespace node
{

ForEachLoop::ForEachLoop()
{
    m_imports.resize(I_MAX_NUM);
    m_imports[I_PREV]      = {{ VariableType::Port, "prev" }};
    m_imports[I_ARRAY]     = {{ VariableType::Any,  "array" }};

    m_exports.resize(O_MAX_NUM);
    m_exports[O_NEXT]          = {{ VariableType::Port, "next" }};
    m_exports[O_ARRAY_ELEMENT] = {{ VariableType::Any,  "array_element" }};
    m_exports[O_ARRAY_INDEX]   = {{ VariableType::Any,  "array_index" }};
    m_exports[O_LOOP_BODY]     = {{ VariableType::Port, "loop_body" }};
}

ForEachLoop::~ForEachLoop()
{
}

void ForEachLoop::Execute(const std::shared_ptr<dag::Context>& ctx)
{
    if (m_imports[I_ARRAY].conns.empty() ||
        m_exports[O_LOOP_BODY].conns.empty()) {
        return;
    }

    // Build Body DList
    std::vector<NodePtr> nodes;
    DrawList::GetSubsequentNodes(m_exports[O_LOOP_BODY], nodes);
    m_body_dlist = std::make_unique<DrawList>(nodes);

    auto array_node = m_imports[I_ARRAY].conns[0].node.lock();
    auto array_node_type = array_node->get_type();
    if (array_node_type == rttr::type::get<CustomData>())
    {
        auto ret_type = std::static_pointer_cast<CustomData>(array_node)->GetRetType();
        switch (ret_type)
        {
        case CustomData::ReturnType::Vec1Array:
            m_array_var.type = VariableType::Vec1Array;
            break;
        case CustomData::ReturnType::Vec2Array:
            m_array_var.type = VariableType::Vec2Array;
            break;
        case CustomData::ReturnType::Vec3Array:
            m_array_var.type = VariableType::Vec3Array;
            break;
        case CustomData::ReturnType::Vec4Array:
            m_array_var.type = VariableType::Vec4Array;
            break;
        default:
            assert(0);
        }

        uint32_t flags;
        auto rc = std::static_pointer_cast<RenderContext>(ctx);
        std::static_pointer_cast<Node>(array_node)->Eval(*rc, CustomData::O_OUT, m_array_var, flags);
        switch (m_array_var.type)
        {
        case VariableType::Vec1Array:
        {
            m_index_curr = 0;
            for (int n = m_array_var.vec1_array.size(); m_index_curr < n; ++m_index_curr) {
                m_body_dlist->Draw(ctx);
            }
        }
            break;
        case VariableType::Vec2Array:
        {
            m_index_curr = 0;
            for (int n = m_array_var.vec2_array.size(); m_index_curr < n; ++m_index_curr) {
                m_body_dlist->Draw(ctx);
            }
        }
            break;
        case VariableType::Vec3Array:
        {
            m_index_curr = 0;
            for (int n = m_array_var.vec3_array.size(); m_index_curr < n; ++m_index_curr) {
                m_body_dlist->Draw(ctx);
            }
        }
            break;
        case VariableType::Vec4Array:
        {
            m_index_curr = 0;
            for (int n = m_array_var.vec4_array.size(); m_index_curr < n; ++m_index_curr) {
                m_body_dlist->Draw(ctx);
            }
        }
            break;
        default:
            assert(0);
        }
    }

    m_index_curr = 0;
}

void ForEachLoop::Eval(const RenderContext& rc, size_t port_idx,
                       ShaderVariant& var, uint32_t& flags) const
{
    switch (port_idx)
    {
    case O_ARRAY_ELEMENT:
        switch (m_array_var.type)
        {
        case VariableType::Vec1Array:
            assert(m_index_curr >= 0 && m_index_curr < static_cast<int>(m_array_var.vec1_array.size()));
            var.type = VariableType::Vector1;
            var.vec1 = m_array_var.vec1_array[m_index_curr];
            break;
        case VariableType::Vec2Array:
            assert(m_index_curr >= 0 && m_index_curr < static_cast<int>(m_array_var.vec2_array.size()));
            var.type = VariableType::Vector2;
            var.vec2 = m_array_var.vec2_array[m_index_curr];
            break;
        case VariableType::Vec3Array:
            assert(m_index_curr >= 0 && m_index_curr < static_cast<int>(m_array_var.vec3_array.size()));
            var.type = VariableType::Vector3;
            var.vec3 = m_array_var.vec3_array[m_index_curr];
            break;
        case VariableType::Vec4Array:
            assert(m_index_curr >= 0 && m_index_curr < static_cast<int>(m_array_var.vec4_array.size()));
            var.type = VariableType::Vector4;
            var.vec4 = m_array_var.vec4_array[m_index_curr];
            break;
        default:
            assert(0);
        }
        break;
    case O_ARRAY_INDEX:
        var.type = VariableType::Int;
        var.i = m_index_curr;
        break;
    }
}

}
}