#pragma once

#include <string>

namespace ur { class ShaderProgram; }

namespace rendergraph
{

struct Variable;
class ShaderVariant;

class ShaderHelper
{
public:
    static void SetUniformValue(const std::shared_ptr<ur::ShaderProgram>& prog, 
        const Variable& key, const ShaderVariant& val);

private:
    static void SetUniformValue(const std::shared_ptr<ur::ShaderProgram>& prog,
        const std::string& base_name, const std::string& name, const ShaderVariant& val);

}; // ShaderHelper

}