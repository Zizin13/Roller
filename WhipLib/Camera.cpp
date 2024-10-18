#include "Camera.h"
#include "Entity.h"
#include "gtx\transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
const glm::vec3 CCamera::s_UP(0.0f, 1.0f, 0.0f);
//-------------------------------------------------------------------------------------------------
CCamera::CCamera()
  : m_offset(0.0f, 0.0f, 0.0f)
  , m_viewDirection(0.0f, 0.0f, 1.0f)
{
}

//-------------------------------------------------------------------------------------------------

void CCamera::Update()
{
  glm::vec3 startVec = glm::vec3(0, 0, 1);
  glm::mat4 yawMat = glm::rotate(glm::radians(m_pContainingEntity->m_fYaw), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 yawedVec = glm::vec3(yawMat * glm::vec4(startVec, 1.0f));
  //glm::vec3 pitchAxis = glm::normalize(glm::cross(yawedVec, glm::vec3(0.0f, 1.0f, 0.0f)));
  //glm::mat4 pitchMat = glm::rotate(glm::radians(m_pContainingEntity->m_fPitch), pitchAxis);

  m_viewDirection = yawedVec;// glm::vec3(pitchMat * glm::vec4(yawedVec, 1.0f));
}

//-------------------------------------------------------------------------------------------------

glm::mat4 CCamera::GetWorldToViewMatrix() const
{
  return glm::lookAt(GetPosition(), GetPosition() + m_viewDirection, CCamera::s_UP);
}

//-------------------------------------------------------------------------------------------------

glm::vec3 CCamera::GetPosition() const
{
  glm::mat4 translateMat = glm::translate(m_pContainingEntity->m_position);
  glm::vec3 startVec = glm::vec3(0, 0, 1);
  glm::mat4 yawMat = glm::rotate(glm::radians(m_pContainingEntity->m_fYaw * -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 yawedVec = glm::vec3(yawMat * glm::vec4(startVec, 1.0f));
  //glm::vec3 pitchAxis = glm::normalize(glm::cross(yawedVec, glm::vec3(0.0f, 1.0f, 0.0f)));
  //glm::mat4 pitchMat = glm::rotate(glm::radians(m_pContainingEntity->m_fPitch), pitchAxis);

  glm::vec3 positionRotated = glm::vec4(m_offset, 1.0f) * yawMat;// *pitchMat;
  glm::vec3 positionTranslated = glm::vec3(translateMat * glm::vec4(positionRotated, 1.0f));
  return positionTranslated;
}

//-------------------------------------------------------------------------------------------------