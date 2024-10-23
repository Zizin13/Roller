#include "Camera.h"
#include "Entity.h"
#include "GameClock.h"
#include "gtx\transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
const glm::vec3 CCamera::s_UP(0.0f, 1.0f, 0.0f);
//-------------------------------------------------------------------------------------------------
CCamera::CCamera()
  : m_offset(0, 0, 0)
  , m_position(0, 0, 0)
  , m_viewDirection(0, 0, 1)
  , m_fTimer(0)
{
}

//-------------------------------------------------------------------------------------------------

void CCamera::Update()
{
  glm::vec3 desiredViewDirection = m_pContainingEntity->GetOrientation();
  glm::mat4 translateMat = glm::translate(m_pContainingEntity->m_position);
  glm::vec3 useOffset = glm::vec4(m_offset, 1.0f) * m_pContainingEntity->m_rotationMat;
  glm::vec3 desiredPosition = m_pContainingEntity->m_position + useOffset;\

  m_viewDirection += (desiredViewDirection - m_viewDirection) * 25.0f * CGameClock::GetGameClock().DeltaTimeLastFrame();
  m_position += (desiredPosition - m_position) * 25.0f * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------

glm::mat4 CCamera::GetWorldToViewMatrix() const
{
  return glm::lookAt(GetPosition(), GetPosition() + GetViewDirection(), CCamera::s_UP);
}

//-------------------------------------------------------------------------------------------------

glm::vec3 CCamera::GetPosition() const
{
  return m_position;
}

//-------------------------------------------------------------------------------------------------

glm::vec3 CCamera::GetViewDirection() const
{
  return m_viewDirection;
}

//-------------------------------------------------------------------------------------------------