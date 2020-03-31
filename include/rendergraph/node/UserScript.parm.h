#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Code, std::string, code, m_code, ())
PARAM_INFO(RetType, rendergraph::node::UserScript::ReturnType, ret_type, m_ret_type, (rendergraph::node::UserScript::ReturnType::Void))
