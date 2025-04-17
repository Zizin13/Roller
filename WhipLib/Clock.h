#ifndef _WHIPLIB_CLOCK_H
#define _WHIPLIB_CLOCK_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#if defined(IS_WINDOWS)
#include <Windows.h>
#endif
//-------------------------------------------------------------------------------------------------

class CClock
{
public:
  bool Init();
  void NewFrame();
  float DeltaTimeLastFrame() const;

private:
  uint64 m_ullStart;
  uint64 m_ullLastFrameTimeCounter;
  uint64 m_ullCpuTimerFrequency;
  float m_fDeltaTime;
};
//-------------------------------------------------------------------------------------------------
#endif
