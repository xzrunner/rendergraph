#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Code, std::string, code, m_code, ())
PARAM_INFO(RetType, rendergraph::node::CustomData::ReturnType, ret_type, m_ret_type, (rendergraph::node::CustomData::ReturnType::Void))
