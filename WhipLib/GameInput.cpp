#include "GameInput.h"
#include "KeyMapper.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CGameInput &CGameInput::GetGameInput()
{
  static CGameInput s_gameInput;

  return s_gameInput;
}

//-------------------------------------------------------------------------------------------------

bool CGameInput::Init(IKeyMapper *pKeyMapper)
{
  m_pKeyMapper = pKeyMapper;
  m_ullActionsPressed = 0;
  m_mousePos = glm::vec2(0.0f, 0.0f);
  return m_pKeyMapper;
}

//-------------------------------------------------------------------------------------------------

void CGameInput::Update()
{
  if (m_pKeyMapper)
    m_pKeyMapper->Update(m_ullActionsPressed, m_mousePos);
}

//-------------------------------------------------------------------------------------------------