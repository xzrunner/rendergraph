#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

#define NOT_GEN_SETTER_GETTER_FUNC
#define PROP_EDIT_CODE

PARAM_INFO(Vert, std::string, vert, m_vert, ())
PARAM_INFO(Frag, std::string, frag, m_frag, ())

#undef NOT_GEN_SETTER_GETTER_FUNC
#undef PROP_EDIT_CODE

PARAM_INFO(Language, rendergraph::node::Shader::Language, lang, m_lang, (rendergraph::node::Shader::Language::GLSL))
