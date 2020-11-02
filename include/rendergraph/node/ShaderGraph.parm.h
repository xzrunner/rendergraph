#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Filepath,   std::string, filepath,    m_filepath, ())
PARAM_INFO(ShaderCode, std::string, shader_code, m_shader_code, ())
