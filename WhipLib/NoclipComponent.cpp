#include "NoclipComponent.h"
#include "GameClock.h"
#include "GameInput.h"
#include "Entity.h"
#include "Camera.h"
#include "MathHelpers.h"
#include "gtx\transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
float CNoclipComponent::s_fMovementSpeed = 30000.0f;
//-------------------------------------------------------------------------------------------------

CNoclipComponent::CNoclipComponent(bool bRequiresClick)
  : m_bRequiresClick(bRequiresClick)
  , m_strafeDirection(0.0f, 0.0f, 0.0f)
  , m_oldMousePos(0.0f, 0.0f)
{
}

//-------------------------------------------------------------------------------------------------

void CNoclipComponent::Update()
{
  if (!m_pContainingEntity->m_bAcceptControls)
    return;

  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_FORWARD)
    MoveForward();
  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_LEFT)
    StrafeLeft();
  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_BACKWARD)
    MoveBackward();
  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_RIGHT)
    StrafeRight();
  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_UP)
    MoveUp();
  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_DOWN)
    MoveDown();
  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_CLICK
      || !m_bRequiresClick) {
    MouseUpdate(CGameInput::GetGameInput().GetMousePos());
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
  m_pContainingEntity->m_fYaw += mouseDelta.x * ROTATIONAL_SPEED;
  m_pContainingEntity->m_fPitch += mouseDelta.y * ROTATIONAL_SPEED;
  m_pContainingEntity->m_fYaw = (float)MathHelpers::ConstrainAngle(m_pContainingEntity->m_fYaw);
  m_pContainingEntity->m_fPitch = (float)MathHelpers::ConstrainAngle(m_pContainingEntity->m_fPitch);
  m_strafeDirection = glm::cross(m_pContainingEntity->GetOrientation(), CCamera::s_UP);

  m_oldMousePos = newMousePos;
}

//-------------------------------------------------------------------------------------------------

void CNoclipComponent::MoveForward()
{
  m_pContainingEntity->m_position += s_fMovementSpeed * m_pContainingEntity->GetOrientation() * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------

void CNoclipComponent::MoveBackward()
{
  m_pContainingEntity->m_position -= s_fMovementSpeed * m_pContainingEntity->GetOrientation() * CGameClock::GetGameClock().DeltaTimeLastFrame();
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
  m_pContainingEntity->m_position += s_fMovementSpeed * CCamera::s_UP * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------

void CNoclipComponent::MoveDown()
{
  m_pContainingEntity->m_position -= s_fMovementSpeed * CCamera::s_UP * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------