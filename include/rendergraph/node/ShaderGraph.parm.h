#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Filepath, std::string, filepath, m_filepath, ())
PARAM_INFO(VertexShader, rendergraph::node::ShaderGraph::VertexShader, vert_shader, m_vert_shader, (rendergraph::node::ShaderGraph::VertexShader::Image))

PARAM_INFO(Vert, std::string, vert, m_vert, ())
PARAM_INFO(Frag, std::string, frag, m_frag, ())
