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
  : m_offset(0.0f, 0.0f, 0.0f)
  , m_viewDirection(0.0f, 0.0f, 1.0f)
{
}

//-------------------------------------------------------------------------------------------------

void CCamera::Update()
{
  m_viewDirection = m_pContainingEntity->m_orientation;
}

//-------------------------------------------------------------------------------------------------

glm::mat4 CCamera::GetWorldToViewMatrix() const
{
  return glm::lookAt(GetPosition(), GetPosition() + m_viewDirection, CCamera::s_UP);
}

//-------------------------------------------------------------------------------------------------

glm::vec3 CCamera::GetPosition() const
{
  return m_pContainingEntity->m_position + m_offset;
}

//-------------------------------------------------------------------------------------------------