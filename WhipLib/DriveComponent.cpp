#include "DriveComponent.h"
#include "GameClock.h"
#include "GameInput.h"
#include "Entity.h"
#include "MathHelpers.h"
#include "Track.h"
#include "DebugTriComponent.h"
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

  glm::vec3 newPos = m_pContainingEntity->m_position;
  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_FORWARD)
    MoveForward(newPos);
  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_LEFT)
    TurnLeft();
  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_BACKWARD)
    MoveBackward(newPos);
  if (CGameInput::GetGameInput().GetActionsPressed() & ACTION_RIGHT)
    TurnRight();

  m_pContainingEntity->m_position += (newPos - m_pContainingEntity->m_position) * 0.9f;// 300.0f * CGameClock::GetGameClock().DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::MoveForward(glm::vec3 &newPos)
{
  newPos += m_fMovementSpeed * m_pContainingEntity->GetOrientation() * CGameClock::GetGameClock().DeltaTimeLastFrame();
  if (m_pTrack) {
    glm::vec3 p0, p1, p2;
    m_pTrack->ProjectToTrack(newPos, m_pContainingEntity->m_rotationMat, m_pContainingEntity->GetUp(), p0, p1, p2);

    CDebugTriComponent *pDebugTri = m_pContainingEntity->GetComponent<CDebugTriComponent>();
    if (pDebugTri) {
      pDebugTri->p0 = p0;
      pDebugTri->p1 = p1;
      pDebugTri->p2 = p2;
    }
  }
}

//-------------------------------------------------------------------------------------------------

void CDriveComponent::MoveBackward(glm::vec3 &newPos)
{
  newPos -= m_fMovementSpeed * m_pContainingEntity->GetOrientation() * CGameClock::GetGameClock().DeltaTimeLastFrame();
  if (m_pTrack) {
    glm::vec3 p0, p1, p2;
    m_pTrack->ProjectToTrack(newPos, m_pContainingEntity->m_rotationMat, m_pContainingEntity->GetUp(), p0, p1, p2);

    CDebugTriComponent *pDebugTri = m_pContainingEntity->GetComponent<CDebugTriComponent>();
    if (pDebugTri) {
      pDebugTri->p0 = p0;
      pDebugTri->p1 = p1;
      pDebugTri->p2 = p2;
    }
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