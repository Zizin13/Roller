#include "Entity.h"
#include "Component.h"
#include <assert.h>
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CEntity::CEntity()
  : m_iNumComponents(0)
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