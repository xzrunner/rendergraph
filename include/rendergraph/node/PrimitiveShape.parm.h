#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Type,       rendergraph::node::PrimitiveShape::Type,       type,   m_type,        (rendergraph::node::PrimitiveShape::Type::Quad))
PARAM_INFO(VertLayout, rendergraph::node::PrimitiveShape::VertLayout, layout, m_vert_layout, (rendergraph::node::PrimitiveShape::VertLayout::Pos))
