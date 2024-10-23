#include "DebugLineComponent.h"
#include "Renderer.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CDebugLineComponent::CDebugLineComponent()
  : m_pShapeData(NULL)
  , m_pTex(NULL)
  , m_pRenderer(NULL)
  , p0(100, 0, 0)
  , p1(100, 5000, 0)
{
}

//-------------------------------------------------------------------------------------------------

void CDebugLineComponent::Update()
{
  if (!m_pRenderer || !m_pTex)
    return;

  m_pRenderer->MakeDebugLine(&m_pShapeData, m_pTex, p0, p1);
}

//-------------------------------------------------------------------------------------------------