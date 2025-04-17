#include "Clock.h"
#include <sys/timeb.h>
#include <time.h>
#include <ctime>
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

bool CClock::Init()
{
#ifdef IS_WINDOWS
  LARGE_INTEGER frequency;
  bool bSuccess = (0 != QueryPerformanceFrequency(&frequency));
  m_llCpuTimerFrequency = frequency.QuadPart;
  if (!bSuccess) { return false; }

  LARGE_INTEGER now;
  bSuccess = (1 == QueryPerformanceCounter(&now));
  m_llLastFrameTimeCounter = now.QuadPart;
  if (!bSuccess) { return false; }
#else
  m_llCpuTimerFrequency = 1000000000;

  timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  m_llLastFrameTimeCounter = spec.tv_sec * 1000000000 * spec.tv_nsec;
#endif

  return true;
}

//-------------------------------------------------------------------------------------------------

void CClock::NewFrame()
{
#ifdef IS_WINDOWS
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);

  LARGE_INTEGER delta;
  delta.QuadPart = now.QuadPart - m_llLastFrameTimeCounter;
  m_fDeltaTime = ((float)delta.QuadPart) / m_llCpuTimerFrequency;

  m_llLastFrameTimeCounter = now.QuadPart;
#else
  timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  int64 llNow = spec.tv_sec * 1000000000 * spec.tv_nsec;

  int64 llDelta;
  llDelta = llNow - m_llLastFrameTimeCounter;
  m_fDeltaTime = ((float)llDelta) / m_llCpuTimerFrequency;

  m_llLastFrameTimeCounter = llNow;
#endif
}

//-------------------------------------------------------------------------------------------------

float CClock::DeltaTimeLastFrame() const
{
  return m_fDeltaTime;
}

//-------------------------------------------------------------------------------------------------
