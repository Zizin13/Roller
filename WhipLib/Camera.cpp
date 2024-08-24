#include "Camera.h"
#include "gtx\transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
const float Camera::MOVEMENT_SPEED = 1000.0f;
//-------------------------------------------------------------------------------------------------

Camera::Camera()
  : m_viewDirection(0.0f, 0.0f, -1.0f)
  , m_UP(0.0f, 1.0f, 0.0f)
{
  m_position.x = 4.0f;
  m_position.z = 4.0f;
  m_position.y = 3.0f;
}

//-------------------------------------------------------------------------------------------------

void Camera::MouseUpdate(const glm::vec2 &newMousePos)
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

glm::mat4 Camera::GetWorldToViewMatrix() const
{
  return glm::lookAt(m_position, m_position + m_viewDirection, m_UP);
}

//-------------------------------------------------------------------------------------------------

void Camera::MoveForward()
{
  m_position += MOVEMENT_SPEED * m_viewDirection;
}

//-------------------------------------------------------------------------------------------------

void Camera::MoveBackward()
{
  m_position -= MOVEMENT_SPEED * m_viewDirection;
}

//-------------------------------------------------------------------------------------------------

void Camera::StrafeLeft()
{
  m_position -= MOVEMENT_SPEED * m_strafeDirection;
}

//-------------------------------------------------------------------------------------------------

void Camera::StrafeRight()
{
  m_position += MOVEMENT_SPEED * m_strafeDirection;
}

//-------------------------------------------------------------------------------------------------

void Camera::MoveUp()
{
  m_position += MOVEMENT_SPEED * m_UP;
}

//-------------------------------------------------------------------------------------------------

void Camera::MoveDown()
{
  m_position -= MOVEMENT_SPEED * m_UP;
}

//-------------------------------------------------------------------------------------------------