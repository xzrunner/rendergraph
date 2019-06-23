#pragma once

#include "rendergraph/Node.h"

namespace rg
{
namespace node
{

class UserScript : public Node
{
public:
    enum OutputID
    {
        O_OUT = 0,

        O_MAX_NUM
    };

    enum class ReturnType
    {
        Void = 0,
        Vec3Array,
        Vec4Array,
    };

public:
    UserScript()
    {
        m_exports.resize(O_MAX_NUM);
        m_exports[O_OUT] = {{ VariableType::UserType, "out" }};
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override;

    void SetValue(const std::string& code) { m_code = code; }

    void SetRetType(ReturnType type) { m_ret_type = type; }
    auto GetRetType() const { return m_ret_type; }

private:
    std::string m_code;

    ReturnType m_ret_type = ReturnType::Void;

    RTTR_ENABLE(Node)

}; // UserScript

}
}