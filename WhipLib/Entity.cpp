#include "Entity.h"
#include "Component.h"
#include "gtx/transform.hpp"
#include <assert.h>
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CEntity::CEntity()
  : m_iNumComponents(0)
  , m_position(0.0f, 0.0f, 0.0f)
  , m_bAcceptControls(false)
  , m_rotationMat(glm::mat4(1))
{
}

//-------------------------------------------------------------------------------------------------

bool CEntity::Init()
{
  for (int i = 0; i < m_iNumComponents; ++i) {
    if (!m_components[i]->Init()) {
      return false;
    }
  }

  return true;
}

//-------------------------------------------------------------------------------------------------

bool CEntity::Shutdown()
{
  for (int i = 0; i < m_iNumComponents; ++i) {
    if (!m_components[i]->Shutdown()) {
      return false;
    }
  }

  return true;
}

//-------------------------------------------------------------------------------------------------

void CEntity::AddComponent(IComponent *pComponent)
{
  assert(m_iNumComponents != s_iMaxComponents);
  m_components[m_iNumComponents++] = pComponent;
  pComponent->m_pContainingEntity = this;
}

//-------------------------------------------------------------------------------------------------

void CEntity::Update()
{
  for (int i = 0; i < m_iNumComponents; i++) {
    m_components[i]->Update();
  }
}

//-------------------------------------------------------------------------------------------------

glm::vec3 CEntity::GetOrientation()
{
  glm::vec3 start = glm::vec3(0, 0, 1);
  glm::vec3 rotated = glm::vec4(start, 1) * m_rotationMat;
  glm::vec3 normalized = glm::vec3(glm::normalize(rotated));
  return normalized;
}

//-------------------------------------------------------------------------------------------------

glm::vec3 CEntity::GetUp()
{
  glm::vec3 start = glm::vec3(0, 1, 0);
  glm::vec3 rotated = glm::vec4(start, 1) * m_rotationMat;
  glm::vec3 normalized = glm::vec3(glm::normalize(rotated));
  return normalized;
}

//-------------------------------------------------------------------------------------------------
