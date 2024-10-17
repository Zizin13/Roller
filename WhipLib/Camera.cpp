#include "Camera.h"
#include "Entity.h"
#include "gtx\transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
const glm::vec3 CCamera::s_UP(0.0f, 1.0f, 0.0f);
//-------------------------------------------------------------------------------------------------
CCamera::CCamera()
  : m_viewDirection(0.0f, -0.3f, 1.0f)
  , m_position(0.0f, 4000.0f, -5000.0f)
{
}

//-------------------------------------------------------------------------------------------------

void CCamera::Update()
{
  m_position = m_pContainingEntity->m_position;
  m_viewDirection = m_pContainingEntity->m_orientation;
}

//-------------------------------------------------------------------------------------------------

glm::mat4 CCamera::GetWorldToViewMatrix() const
{
  return glm::lookAt(m_position, m_position + m_viewDirection, CCamera::s_UP);
}

//-------------------------------------------------------------------------------------------------