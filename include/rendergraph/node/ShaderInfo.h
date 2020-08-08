#pragma once

#include "rendergraph/Variable.h"

#include <unirender/ShaderType.h>

#include <string>
#include <vector>
#include <set>

namespace rendergraph
{
namespace node
{

class ShaderInfo
{
public:
	static void GetCodeUniforms(ur::ShaderType type, const std::string& glsl,
		std::vector<rendergraph::Variable>& uniforms, std::set<std::string>& unique_names);

}; // ShaderInfo

}
}