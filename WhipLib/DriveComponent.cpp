#include "DriveComponent.h"
#include "GameClock.h"
#include "GameInput.h"
#include "Entity.h"
#include "MathHelpers.h"
#include "Track.h"
#include "glm.hpp"
#include "gtx\transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CDriveComponent::CDriveComponent()
  : m_fMovementSpeed(15000.0f)
  , m_fRotateSpeed(100.0f)
  , m_pTrack(NULL)
{
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::Update()
{
  if (!m_pContainingEntity->m_bAcceptControls)
    return;

  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_FORWARD)
    MoveForward();
  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_LEFT)
    TurnLeft();
  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_BACKWARD)
    MoveBackward();
  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_RIGHT)
    TurnRight();
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::MoveForward()
{
  m_pContainingEntity->m_position += m_fMovementSpeed * m_pContainingEntity->GetOrientation() * CGameClock::GetGameClock().DeltaTimeLastFrame();
  if (m_pTrack) {
    m_pTrack->ProjectToTrack(m_pContainingEntity->m_position, m_pContainingEntity->m_rotationMat, m_pContainingEntity->GetUp());
  }
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::MoveBackward()
{
  m_pContainingEntity->m_position -= m_fMovementSpeed * m_pContainingEntity->GetOrientation() * CGameClock::GetGameClock().DeltaTimeLastFrame();
  if (m_pTrack) {
    m_pTrack->ProjectToTrack(m_pContainingEntity->m_position, m_pContainingEntity->m_rotationMat, m_pContainingEntity->GetUp());
  }
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::TurnLeft()
{
  float fYaw = m_fRotateSpeed * CGameClock::GetGameClock().DeltaTimeLastFrame() * -1.0f;
  glm::mat4 rotation = glm::rotate(glm::radians(fYaw), m_pContainingEntity->GetUp());
  m_pContainingEntity->m_rotationMat = rotation * m_pContainingEntity->m_rotationMat;

  //m_pContainingEntity->m_fYaw += m_fRotateSpeed * CGameClock::GetGameClock().DeltaTimeLastFrame();
  //m_pContainingEntity->m_fYaw = (float)MathHelpers::ConstrainAngle(m_pContainingEntity->m_fYaw);
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::TurnRight()
{
  float fYaw = m_fRotateSpeed * CGameClock::GetGameClock().DeltaTimeLastFrame();
  glm::mat4 rotation = glm::rotate(glm::radians(fYaw), m_pContainingEntity->GetUp());
  m_pContainingEntity->m_rotationMat = rotation * m_pContainingEntity->m_rotationMat;

  //m_pContainingEntity->m_fYaw -= m_fRotateSpeed * CGameClock::GetGameClock().DeltaTimeLastFrame();
  //m_pContainingEntity->m_fYaw = (float)MathHelpers::ConstrainAngle(m_pContainingEntity->m_fYaw);
}

//-------------------------------------------------------------------------------------------------