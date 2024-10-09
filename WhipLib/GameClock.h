#ifndef WHIPLIB_GAMECLOCK_H
#define WHIPLIB_GAMECLOCK_H
//-------------------------------------------------------------------------------------------------
#include "Clock.h"
//-------------------------------------------------------------------------------------------------

class CGameClock
{
public:
  static CGameClock &GetGameClock();
  CGameClock(const CGameClock &) = delete;
  CGameClock &operator=(const CGameClock &) = delete;

  bool Init();
  void NewFrame();
  float DeltaTimeLastFrame() const;

private:
  CGameClock() {}

  CClock m_clock;
};

//-------------------------------------------------------------------------------------------------
#endif