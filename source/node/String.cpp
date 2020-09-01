#include "rendergraph/node/String.h"

namespace rendergraph
{
namespace node
{

String::String()
{
    m_imports = {
        {{ VariableType::String, "in" }}
    };
    m_exports = {
        {{ VariableType::String, "out" }}
    };
}

void String::Eval(const RenderContext& rc, size_t port_idx,
                  ShaderVariant& var, uint32_t& flags) const
{
    if (var.type != VariableType::String) {
        return;
    }

    if (var.p) {
        delete static_cast<const char*>(var.p);
    }

    auto& conns = m_imports[0].conns;
    if (!conns.empty()) 
    {
        auto node = conns[0].node.lock();
        if (node) 
        {
            auto ret = ShaderVariant(std::string(""));
            std::static_pointer_cast<Node>(node)->Eval(rc, conns[0].idx, ret, flags);
            std::string str = static_cast<const char*>(ret.p);
            str += m_val;
            var.p = _strdup(str.c_str());
            return;
        }
    }

    var.p = _strdup(static_cast<const char*>(m_val.c_str()));
}

}
}