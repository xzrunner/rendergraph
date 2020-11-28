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
	ShaderInfo() {}

	void Parse(shadertrans::ShaderStage stage, const std::string& code,
		node::Shader::Language lang, std::ostream& out);

	auto& GetUniforms() const { return m_uniforms; }

	auto& GetProps() const { return m_props; }

private:
	std::vector<Variable> m_uniforms;
	std::set<std::string> m_unique_names;

	std::map<std::string, ShaderVariant> m_props;

}; // ShaderInfo

}
}