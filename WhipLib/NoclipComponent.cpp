#include "NoclipComponent.h"
#include "GameClock.h"
#include "Entity.h"
#include "gtx\transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
float CNoclipComponent::s_fMovementSpeed = 30000.0f;
//-------------------------------------------------------------------------------------------------

CNoclipComponent::CNoclipComponent()
  : m_UP(0.0f, 1.0f, 0.0f)
{
}

//-------------------------------------------------------------------------------------------------

void CNoclipComponent::Update()
{
  if (GetAsyncKeyState(0x57)) //W
    MoveForward();
  if (GetAsyncKeyState(0x41)) //A
    StrafeLeft();
  if (GetAsyncKeyState(0x53)) //S
    MoveBackward();
  if (GetAsyncKeyState(0x44)) //D
    StrafeRight();
  if (GetAsyncKeyState(0x52) //R
      || GetAsyncKeyState(0x45)) //E
    MoveUp();
  if (GetAsyncKeyState(0x46) //F
      || GetAsyncKeyState(0x51)) //Q
    MoveDown();
  POINT mousePos;
  if (GetCursorPos(&mousePos)) {
    MouseUpdate(glm::vec2(mousePos.x, mousePos.y));
  }
}

//-------------------------------------------------------------------------------------------------

void CNoclipComponent::MouseUpdate(const glm::vec2 &newMousePos)
{
  glm::vec2 mouseDelta = newMousePos - m_oldMousePos;
  if (glm::length(mouseDelta) > 50.0f) {
    m_oldMousePos = newMousePos;
    return;
  }
  const float ROTATIONAL_SPEED = -0.3f;
  m_strafeDirection = glm::cross(m_pContainingEntity->m_orientation, m_UP);
  glm::mat4 rotator = glm::rotate(glm::radians(mouseDelta.x * ROTATIONAL_SPEED), m_UP) *
    glm::rotate(glm::radians(mouseDelta.y * ROTATIONAL_SPEED), m_strafeDirection);
  m_pContainingEntity->m_orientation = glm::mat3(rotator) * m_pContainingEntity->m_orientation;

  m_oldMousePos = newMousePos;
}

//-------------------------------------------------------------------------------------------------

void CNoclipComponent::MoveForward()
{
  m_pContainingEntity->m_position += s_fMovementSpeed * m_pContainingEntity->m_orientation * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------

void CNoclipComponent::MoveBackward()
{
  m_pContainingEntity->m_position -= s_fMovementSpeed * m_pContainingEntity->m_orientation * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------

void CNoclipComponent::StrafeLeft()
{
  m_pContainingEntity->m_position -= s_fMovementSpeed * m_strafeDirection * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------

void CNoclipComponent::StrafeRight()
{
  m_pContainingEntity->m_position += s_fMovementSpeed * m_strafeDirection * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------

void CNoclipComponent::MoveUp()
{
  m_pContainingEntity->m_position += s_fMovementSpeed * m_UP * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------

void CNoclipComponent::MoveDown()
{
  m_pContainingEntity->m_position -= s_fMovementSpeed * m_UP * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------