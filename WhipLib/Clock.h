#ifndef _WHIPLIB_CLOCK_H
#define _WHIPLIB_CLOCK_H
//-------------------------------------------------------------------------------------------------
#include <Windows.h>
//-------------------------------------------------------------------------------------------------

class CClock
{
public:
  bool Init();
  void NewFrame();
  float DeltaTimeLastFrame() const;

private:
  LARGE_INTEGER m_lastFrameTimeCounter;
  LARGE_INTEGER m_cpuTimerFrequency;
  float m_fDeltaTime;
};

//-------------------------------------------------------------------------------------------------
#endif