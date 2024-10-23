#include "DebugTriComponent.h"
#include "Renderer.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CDebugTriComponent::CDebugTriComponent()
  : m_pShapeData(NULL)
  , m_pTex(NULL)
  , m_pRenderer(NULL)
  , p0(0, 0, 0)
  , p1(0, 5000, 0)
  , p2(0, 0, 5000)
{
}

//-------------------------------------------------------------------------------------------------

void CDebugTriComponent::Update()
{
  if (!m_pRenderer || !m_pTex)
    return;

  m_pShapeData = m_pRenderer->MakeDebugTri(m_pShapeData, m_pTex, p0, p1, p2);
}

//-------------------------------------------------------------------------------------------------