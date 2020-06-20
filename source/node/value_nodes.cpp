#include "rendergraph/node/value_nodes.h"
#include "rendergraph/Evaluator.h"

namespace rendergraph
{
namespace node
{

void Matrix3::Eval(const RenderContext& rc, size_t port_idx,
                   ShaderVariant& var, uint32_t& flags) const
{
    // mat4 to mat3
    if (!m_imports[0].conns.empty()) 
    {
        auto v = Evaluator::Calc(rc, m_imports[0], Evaluator::DefaultValue(var.type), flags);
        assert(v.type == VariableType::Matrix4);
        var.type = VariableType::Matrix3;
        var.mat3 = v.mat4;
    } 
    else if (var.type == VariableType::Matrix3) 
    {
        var.mat3 = m_val;
    }
}

void Matrix4::Eval(const RenderContext& rc, size_t port_idx, ShaderVariant& var, uint32_t& flags) const
{
    // mat3 to mat4
    if (!m_imports[0].conns.empty())
    {
        auto v = Evaluator::Calc(rc, m_imports[0], Evaluator::DefaultValue(var.type), flags);
        assert(v.type == VariableType::Matrix3);
        var.type = VariableType::Matrix4;
        var.mat4 = v.mat3;
    }
    else if (var.type == VariableType::Matrix4) 
    {
        var.mat4 = m_val;
    }
}

}
}