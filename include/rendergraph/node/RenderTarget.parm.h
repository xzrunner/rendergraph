#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

#define NOT_GEN_SETTER_GETTER_FUNC
PARAM_INFO(Width,  size_t, width,  m_width,  (0))
PARAM_INFO(Height, size_t, height, m_height, (0))
#undef NOT_GEN_SETTER_GETTER_FUNC

PARAM_INFO(DepthBuf, bool, depth_buf, m_enable_rbo_depth, (false))
PARAM_INFO(ColorBuf, bool, color_buf, m_enable_rbo_color, (false))
