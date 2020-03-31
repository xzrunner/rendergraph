#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(SrcFactor, rendergraph::node::BlendFunc::Factor, src_factor, m_src, (rendergraph::node::BlendFunc::Factor::One))
PARAM_INFO(DstFactor, rendergraph::node::BlendFunc::Factor, dst_factor, m_dst, (rendergraph::node::BlendFunc::Factor::Zero))
