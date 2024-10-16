#include "Camera.h"
#include "GameClock.h"
#include "gtx\transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
float CCamera::s_fMovementSpeed = 30000.0f;
//-------------------------------------------------------------------------------------------------

CCamera::CCamera()
  : m_viewDirection(0.0f, -0.3f, 1.0f)
  , m_UP(0.0f, 1.0f, 0.0f)
{
  m_position.x = 0.0f;
  m_position.z = -5000.0f;
  m_position.y = 4000.0f;
}

//-------------------------------------------------------------------------------------------------

void CCamera::MouseUpdate(const glm::vec2 &newMousePos)
{
  glm::vec2 mouseDelta = newMousePos - m_oldMousePos;
  if (glm::length(mouseDelta) > 50.0f) {
    m_oldMousePos = newMousePos;
    return;
  }
  const float ROTATIONAL_SPEED = -0.3f;
  m_strafeDirection = glm::cross(m_viewDirection, m_UP);
  glm::mat4 rotator = glm::rotate(glm::radians(mouseDelta.x * ROTATIONAL_SPEED), m_UP) *
    glm::rotate(glm::radians(mouseDelta.y * ROTATIONAL_SPEED), m_strafeDirection);
  m_viewDirection = glm::mat3(rotator) * m_viewDirection;

  m_oldMousePos = newMousePos;
}

//-------------------------------------------------------------------------------------------------

glm::mat4 CCamera::GetWorldToViewMatrix() const
{
  return glm::lookAt(m_position, m_position + m_viewDirection, m_UP);
}

//-------------------------------------------------------------------------------------------------

void CCamera::MoveForward()
{
  m_position += s_fMovementSpeed * m_viewDirection * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------

void CCamera::MoveBackward()
{
  m_position -= s_fMovementSpeed * m_viewDirection * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------

void CCamera::StrafeLeft()
{
  m_position -= s_fMovementSpeed * m_strafeDirection * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------

void CCamera::StrafeRight()
{
  m_position += s_fMovementSpeed * m_strafeDirection * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------

void CCamera::MoveUp()
{
  m_position += s_fMovementSpeed * m_UP * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------

void CCamera::MoveDown()
{
  m_position -= s_fMovementSpeed * m_UP * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------