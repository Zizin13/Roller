#include "Camera.h"
#include "Entity.h"
#include "GameClock.h"
#include "gtx\transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
#define CAMERA_TIMER 0.024f
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
  //m_fTimer += CGameClock::GetGameClock().DeltaTimeLastFrame();
  //
  //if (m_fTimer > CAMERA_TIMER) {
  //  m_fTimer = m_fTimer - CAMERA_TIMER;

    glm::vec3 desiredViewDirection = m_pContainingEntity->GetOrientation();
    glm::mat4 translateMat = glm::translate(m_pContainingEntity->m_position);
    glm::vec3 useOffset = glm::vec4(m_offset, 1.0f) * m_pContainingEntity->m_rotationMat;
    glm::vec3 desiredPosition = m_pContainingEntity->m_position + useOffset;

    m_viewDirection = 0.9f * m_viewDirection + 0.1f * desiredViewDirection;
    m_position = 0.9f * m_position + 0.1f * desiredPosition;
  //}
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