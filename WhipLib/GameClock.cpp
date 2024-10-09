#include "GameClock.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CGameClock &CGameClock::GetGameClock()
{
  static CGameClock s_gameClock;

  return s_gameClock;
}

//-------------------------------------------------------------------------------------------------

bool CGameClock::Init()
{
  return m_clock.Init();
}

//-------------------------------------------------------------------------------------------------

void CGameClock::NewFrame()
{
  m_clock.NewFrame();
}

//-------------------------------------------------------------------------------------------------

float CGameClock::DeltaTimeLastFrame() const
{
  return m_clock.DeltaTimeLastFrame();
}

//-------------------------------------------------------------------------------------------------