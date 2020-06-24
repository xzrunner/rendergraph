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

    RTTR_ENABLE(Node)

#define PARM_FILEPATH "rendergraph/node/Output.parm.h"
#include <dag/node_parms_gen.h>
#undef PARM_FILEPATH

}; // Output

}
}