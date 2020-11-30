#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(BarrierTypes, std::vector<rendergraph::node::Barrier::Type>, barrier_types,  m_barrier_types, ())
