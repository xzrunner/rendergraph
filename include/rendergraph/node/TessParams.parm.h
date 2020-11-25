#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(VertNum,    int, vert_num,    m_vert_num,    (3))
PARAM_INFO(OuterLevel, int, outer_level, m_outer_level, (1))
PARAM_INFO(InnerLevel, int, inner_level, m_inner_level, (1))
