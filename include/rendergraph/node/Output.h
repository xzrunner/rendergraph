#pragma once

#include "rendergraph/Node.h"

namespace rendergraph
{
namespace node
{

class Output : public Node
{
public:
    Output()
    {
		m_imports = {
			{{ VariableType::Any, "_in" }}
		};
    }

    virtual void Eval(const RenderContext& rc, size_t port_idx,
        ShaderVariant& var, uint32_t& flags) const override;

    void SetVarType(const VariableType& type) { m_var_type = type; }
    const VariableType& GetVarType() const { return m_var_type; }

    void SetVarName(const std::string& name) {
        m_var_name = name;
        m_imports[0].var.type.name = name;
    }
    const std::string& GetVarName() const { return m_var_name; }

    RTTR_ENABLE(Node)

#define NOT_GEN_SETTER_GETTER_FUNC
#define PARM_FILEPATH "rendergraph/node/Output.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH
#undef NOT_GEN_SETTER_GETTER_FUNC

}; // Output

}
}