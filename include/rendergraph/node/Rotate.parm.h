#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Angle, float,    angle, m_angle, (0))       // rad
PARAM_INFO(Axis,  sm::vec3, axis,  m_axis,  (1, 0, 0)) // normalized
