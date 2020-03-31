#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Left,   float, left,   m_left,   (-10.0f))
PARAM_INFO(Right,  float, right,  m_right,  (10.0f))
PARAM_INFO(Bottom, float, bottom, m_bottom, (-10.0f))
PARAM_INFO(Top,    float, top,    m_top,    (10.0f))
PARAM_INFO(ZNear,  float, znear,  m_znear,  (1.0f))
PARAM_INFO(ZFar,   float, zfar,   m_zfar,   (7.5f))
