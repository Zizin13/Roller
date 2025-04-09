#ifndef _WHIPLIB_CLOCK_H
#define _WHIPLIB_CLOCK_H
//-------------------------------------------------------------------------------------------------
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
#if defined(IS_WINDOWS)
  LARGE_INTEGER m_lastFrameTimeCounter;
  LARGE_INTEGER m_cpuTimerFrequency;
#endif
  float m_fDeltaTime;
};
//-------------------------------------------------------------------------------------------------
#endif
