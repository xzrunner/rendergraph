#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

#define NOT_GEN_SETTER_GETTER_FUNC
PARAM_INFO(Width,  size_t, width,  m_width,  (128))
PARAM_INFO(Height, size_t, height, m_height, (128))
#undef NOT_GEN_SETTER_GETTER_FUNC