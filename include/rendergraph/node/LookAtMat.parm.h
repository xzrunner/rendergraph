#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Eye,    sm::vec3, eye,    m_eye,    ())
PARAM_INFO(Center, sm::vec3, center, m_center, ())
PARAM_INFO(Up,     sm::vec3, up,     m_up,     (0, 1, 0))
