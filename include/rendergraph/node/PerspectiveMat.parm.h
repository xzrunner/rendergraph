#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(FovY,   float, fovy,   m_fovy,   (45.0f))
PARAM_INFO(Aspect, float, aspect, m_aspect, (1.0f))
PARAM_INFO(ZNear,  float, znear,  m_znear,  (0.1f))
PARAM_INFO(ZFar,   float, zfar,   m_zfar,   (100.0f))
