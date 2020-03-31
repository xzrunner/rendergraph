#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Filepath, std::string, filepath, m_filepath, ())

PARAM_INFO(Type, rendergraph::node::Texture::Type, type, m_type, (rendergraph::node::Texture::Type::Tex2D))

PARAM_INFO(Width,  int, width,  m_width,  (0))
PARAM_INFO(Height, int, height, m_height, (0))
PARAM_INFO(Format, rendergraph::node::Texture::Format, format, m_format, (rendergraph::node::Texture::Format::RGBA8))

PARAM_INFO(Wrapping,  rendergraph::node::Texture::Wrapping,  wrap,   m_wrap,   (rendergraph::node::Texture::Wrapping::Repeat))
PARAM_INFO(Filtering, rendergraph::node::Texture::Filtering, filter, m_filter, (rendergraph::node::Texture::Filtering::Linear))
