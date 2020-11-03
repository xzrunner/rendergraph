#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Filepath, std::string, filepath, m_filepath, ())
PARAM_INFO(VertexShader, rendergraph::node::ShaderGraph::VertexShader, vert_shader, m_vert_shader, (rendergraph::node::ShaderGraph::VertexShader::Image))
