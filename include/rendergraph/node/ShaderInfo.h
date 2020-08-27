#pragma once

#include "rendergraph/Variable.h"
#include "rendergraph/node/Shader.h"

#include <shadertrans/ShaderStage.h>

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
	static void GetCodeUniforms(shadertrans::ShaderStage stage, const std::string& code, node::Shader::Language lang,
		std::vector<rendergraph::Variable>& uniforms, std::set<std::string>& unique_names, std::ostream& out);

}; // ShaderInfo

}
}