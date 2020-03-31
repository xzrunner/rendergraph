#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(ClearType, std::vector<rendergraph::node::Clear::Type>, type,  m_clear_type, ())
PARAM_INFO(Color,     pt0::Color,                                  color, m_col,        (0, 0, 0, 0))
