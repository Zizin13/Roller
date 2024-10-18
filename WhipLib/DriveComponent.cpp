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
  , m_fRotateSpeed(300.0f)
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
  glm::vec3 newPos = m_pContainingEntity->m_position + m_fMovementSpeed * m_pContainingEntity->GetOrientation() * CGameClock::GetGameClock().DeltaTimeLastFrame();
  if (m_pTrack) {
    m_pTrack->ProjectToTrack(m_pContainingEntity->m_position, newPos);
  }
  m_pContainingEntity->m_position = newPos;
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::MoveBackward()
{
  glm::vec3 newPos = m_pContainingEntity->m_position -= m_fMovementSpeed * m_pContainingEntity->GetOrientation() * CGameClock::GetGameClock().DeltaTimeLastFrame();
  if (m_pTrack) {
    m_pTrack->ProjectToTrack(m_pContainingEntity->m_position, newPos);
  }
  m_pContainingEntity->m_position = newPos;
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::TurnLeft()
{
  m_pContainingEntity->m_fYaw += m_fRotateSpeed * CGameClock::GetGameClock().DeltaTimeLastFrame();
  m_pContainingEntity->m_fYaw = (float)MathHelpers::ConstrainAngle(m_pContainingEntity->m_fYaw);
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::TurnRight()
{
  m_pContainingEntity->m_fYaw -= m_fRotateSpeed * CGameClock::GetGameClock().DeltaTimeLastFrame();
  m_pContainingEntity->m_fYaw = (float)MathHelpers::ConstrainAngle(m_pContainingEntity->m_fYaw);
}

//-------------------------------------------------------------------------------------------------