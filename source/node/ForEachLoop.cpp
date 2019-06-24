#include "rendergraph/node/ForEachLoop.h"
#include "rendergraph/node/UserScript.h"
#include "rendergraph/ScriptEnv.h"
#include "rendergraph/typedef.h"
#include "rendergraph/DrawList.h"

#include <queue>

namespace rg
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

void ForEachLoop::Execute(const RenderContext& rc)
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
    if (array_node_type == rttr::type::get<UserScript>())
    {
        auto ret_type = std::static_pointer_cast<UserScript>(array_node)->GetRetType();
        switch (ret_type)
        {
        case UserScript::ReturnType::Vec3Array:
            m_array_var.type = VariableType::Vec3Array;
            break;
        case UserScript::ReturnType::Vec4Array:
            m_array_var.type = VariableType::Vec4Array;
            break;
        default:
            assert(0);
        }

        uint32_t flags;
        array_node->Eval(rc, UserScript::O_OUT, m_array_var, flags);
        switch (m_array_var.type)
        {
        case VariableType::Vec3Array:
        {
            m_index_curr = 0;
            for (int n = m_array_var.vec3_array.size(); m_index_curr < n; ++m_index_curr) {
                m_body_dlist->Draw(rc);
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