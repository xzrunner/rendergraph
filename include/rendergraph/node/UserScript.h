#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
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
        Vec1Array,
        Vec2Array,
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

    const ShaderVariant& GetCachedVar(VariableType type) const;

private:
    static ShaderVariant Run(const std::string& code, VariableType type);

private:
    mutable ShaderVariant m_cached;

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/UserScript.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // UserScript

}
}