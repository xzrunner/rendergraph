#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Value, std::string,                     val,  m_val,  ())
PARAM_INFO(Type,  rendergraph::node::String::Type, type, m_type, (rendergraph::node::String::Type::Data))
