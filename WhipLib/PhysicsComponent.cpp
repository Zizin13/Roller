#include "PhysicsComponent.h"
#include "GameClock.h"
#include "Entity.h"
#include "Track.h"
#include "Renderer.h"
#include "gtx\transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CPhysicsComponent::CPhysicsComponent()
  : m_fLinearAccel(0.0f)
  , m_linearVelocity(0, 0, 0)
  , m_pTrack(NULL)
  , m_pRenderer(NULL)
  , m_pTex(NULL)
  , m_pDebugLine(NULL)
  , m_pDebugTri(NULL)
{

}

//-------------------------------------------------------------------------------------------------
#define DRAG_CONSTANT 0.0f
#define ROLLING_RESISTANCE -.018f
#define CAR_MASS 1.0f
#define CORNERING_STIFFNESS 1.0f

void CPhysicsComponent::Update()
{
  float fDeltaTime = CGameClock::GetGameClock().DeltaTimeLastFrame();
  glm::vec3 orientation = m_pContainingEntity->GetOrientation();
  glm::vec3 traction = orientation * m_fLinearAccel;
  glm::vec3 drag = m_linearVelocity * glm::length(m_linearVelocity) * DRAG_CONSTANT;
  glm::vec3 rollingResistance = m_linearVelocity * ROLLING_RESISTANCE;
  glm::vec3 longitudinal = traction + drag + rollingResistance;
  glm::vec3 accel = longitudinal / CAR_MASS;
  m_linearVelocity += accel * fDeltaTime;

  float fCosTheta = glm::dot(glm::normalize(m_linearVelocity), glm::normalize(m_pContainingEntity->GetOrientation()));
  if (fCosTheta > 1.0f)
    fCosTheta = 1.0f;
  float fTheta = glm::acos(fCosTheta);
  glm::vec3 lateralVel = m_linearVelocity * glm::sin(fTheta);
  if (!glm::any(glm::isnan(lateralVel)))
    m_linearVelocity -= lateralVel * fDeltaTime;

  glm::vec3 newPos = m_pContainingEntity->m_position + m_linearVelocity * fDeltaTime;
  if (m_pTrack) {
    glm::vec3 p0, p1, p2, peg1, peg2;

    //make peg
    glm::mat4 translateMat = glm::translate(m_pContainingEntity->m_position);
    peg1 = glm::vec3(translateMat * glm::vec4(m_pContainingEntity->GetUp() * 500.0f, 1.0f));
    peg2 = glm::vec3(translateMat * glm::vec4(m_pContainingEntity->GetUp() * -500.0f, 1.0f));

    //project
    m_pTrack->ProjectToTrack(newPos, m_pContainingEntity->m_rotationMat, m_pContainingEntity->GetUp(), p0, p1, p2, peg1, peg2);
    m_pContainingEntity->m_position = newPos;

    //update debug visualizations
    if (m_pRenderer && m_pTex) {
      m_pRenderer->MakeDebugTri(&m_pDebugTri, m_pTex, p0, p1, p2);
      m_pRenderer->MakeDebugLine(&m_pDebugLine, m_pTex, peg1, peg2);
    }
  }
}

//-------------------------------------------------------------------------------------------------