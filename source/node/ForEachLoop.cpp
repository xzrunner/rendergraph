#include "rendergraph/node/ForEachLoop.h"
#include "rendergraph/node/UserScript.h"
#include "rendergraph/ScriptEnv.h"

namespace rg
{
namespace node
{

void ForEachLoop::Execute(const RenderContext& rc)
{
    if (m_imports[I_ARRAY].conns.empty()) {
        return;
    }

    auto array_node = m_imports[I_ARRAY].conns[0].node.lock();
    auto array_node_type = array_node->get_type();
    if (array_node_type == rttr::type::get<UserScript>())
    {
        ShaderVariant var;
        auto ret_type = std::static_pointer_cast<UserScript>(array_node)->GetRetType();
        switch (ret_type)
        {
        case UserScript::ReturnType::Vec3Array:
            var.type = VariableType::Vec3Array;
            break;
        case UserScript::ReturnType::Vec4Array:
            var.type = VariableType::Vec4Array;
            break;
        default:
            assert(0);
        }

        uint32_t flags;
        array_node->Eval(rc, UserScript::O_OUT, var, flags);
        switch (var.type)
        {
        case VariableType::Vec3Array:
        {
            m_index_curr = 0;
            for (int n = var.vec3_array.size(); m_index_curr < n; ++m_index_curr) {
                for (auto& conn : m_exports[O_LOOP_BODY].conns) {
                    conn.node.lock()->Execute(rc);
                }
            }
        }
            break;
        default:
            assert(0);
        }
    }

}

void ForEachLoop::Eval(const RenderContext& rc, size_t port_idx,
                       ShaderVariant& var, uint32_t& flags) const
{

}

}
}