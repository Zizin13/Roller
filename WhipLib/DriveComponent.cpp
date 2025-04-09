#include "DriveComponent.h"
#include "GameClock.h"
#include "GameInput.h"
#include "Entity.h"
#include "MathHelpers.h"
#include "PhysicsComponent.h"
#include "gtx/transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CDriveComponent::CDriveComponent()
  : m_fMovementSpeed(15000.0f)
  , m_fRotateSpeed(100.0f)
{
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::Update()
{
  if (!m_pContainingEntity->m_bAcceptControls)
    return;

  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_BACKWARD)
    MoveBackward();
  else if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_FORWARD)
    MoveForward();
  else
    Coast();

  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_LEFT)
    TurnLeft();
  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_RIGHT)
    TurnRight();
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::MoveForward()
{
  CPhysicsComponent *pPhysicsComponent = m_pContainingEntity->GetComponent<CPhysicsComponent>();
  if (pPhysicsComponent) {
    pPhysicsComponent->m_fLinearAccel = 10000.0f;
  }
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::MoveBackward()
{
  CPhysicsComponent *pPhysicsComponent = m_pContainingEntity->GetComponent<CPhysicsComponent>();
  if (pPhysicsComponent) {
    pPhysicsComponent->m_fLinearAccel = -10000.0f;
  }
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::Coast()
{
  CPhysicsComponent *pPhysicsComponent = m_pContainingEntity->GetComponent<CPhysicsComponent>();
  if (pPhysicsComponent) {
    pPhysicsComponent->m_fLinearAccel = 0.0f;
  }
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::TurnLeft()
{
  float fYaw = m_fRotateSpeed * CGameClock::GetGameClock().DeltaTimeLastFrame() * -1.0f;
  glm::mat4 rotation = glm::rotate(glm::radians(fYaw), m_pContainingEntity->GetUp());
  m_pContainingEntity->m_rotationMat = m_pContainingEntity->m_rotationMat * rotation;
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::TurnRight()
{
  float fYaw = m_fRotateSpeed * CGameClock::GetGameClock().DeltaTimeLastFrame();
  glm::mat4 rotation = glm::rotate(glm::radians(fYaw), m_pContainingEntity->GetUp());
  m_pContainingEntity->m_rotationMat = m_pContainingEntity->m_rotationMat * rotation;
}

//-------------------------------------------------------------------------------------------------
