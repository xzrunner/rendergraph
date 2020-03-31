#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(VarName, std::string,               var_name, m_var_name, ())
PARAM_INFO(VarType, rendergraph::VariableType, var_type, m_var_type, (rendergraph::VariableType::Any))
