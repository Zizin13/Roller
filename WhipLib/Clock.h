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
  int64 m_llLastFrameTimeCounter;
  int64 m_llCpuTimerFrequency;
  float m_fDeltaTime;
};
//-------------------------------------------------------------------------------------------------
#endif
