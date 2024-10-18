#include "ShapeComponent.h"
#include "ShapeData.h"
#include "Palette.h"
#include "Texture.h"
#include "ShapeFactory.h"
#include "Entity.h"
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CShapeComponent::CShapeComponent()
  : m_pShapeData(NULL)
  , m_rotationOffset(glm::mat4(1))
{

}

//-------------------------------------------------------------------------------------------------

CShapeComponent::~CShapeComponent()
{
}

//-------------------------------------------------------------------------------------------------

void CShapeComponent::Update()
{
  if (!m_pShapeData)
    return;

  glm::vec3 startVec = glm::vec3(0, 0, 1);
  glm::mat4 yawMat = glm::rotate(glm::radians(m_pContainingEntity->m_fYaw), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 yawedVec = glm::vec3(yawMat * glm::vec4(startVec, 1.0f));
  glm::vec3 pitchAxis = glm::normalize(glm::cross(yawedVec, glm::vec3(0.0f, 1.0f, 0.0f)));
  glm::mat4 pitchMat = glm::rotate(glm::radians(m_pContainingEntity->m_fPitch), pitchAxis);
  glm::mat4 rollMat = glm::rotate(glm::radians(m_pContainingEntity->m_fRoll), glm::normalize(m_pContainingEntity->GetOrientation()));

  m_pShapeData->m_modelToWorldMatrix = glm::translate(m_pContainingEntity->m_position) *
    rollMat * pitchMat * yawMat *
    m_rotationOffset;
}

//-------------------------------------------------------------------------------------------------