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
  m_ullCpuTimerFrequency = frequency.QuadPart;
  if (!bSuccess) { return false; }

  LARGE_INTEGER now;
  bSuccess = (1 == QueryPerformanceCounter(&now));
  m_ullLastFrameTimeCounter = now.QuadPart;
  if (!bSuccess) { return false; }
#else
  m_ullCpuTimerFrequency = 1000000000;

  timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  m_ullLastFrameTimeCounter = spec.tv_sec * 1000000000 + spec.tv_nsec;
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
  delta.QuadPart = now.QuadPart - m_ullLastFrameTimeCounter;
  m_fDeltaTime = ((float)delta.QuadPart) / m_ullCpuTimerFrequency;

  m_ullLastFrameTimeCounter = now.QuadPart;
#else
  timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  uint64 ullNow = spec.tv_sec * 1000000000 + spec.tv_nsec;

  uint64 ullDelta;
  ullDelta = ullNow - m_ullLastFrameTimeCounter;
  m_fDeltaTime = ((float)ullDelta) / m_ullCpuTimerFrequency;

  m_ullLastFrameTimeCounter = ullNow;
#endif
}

//-------------------------------------------------------------------------------------------------

float CClock::DeltaTimeLastFrame() const
{
  return m_fDeltaTime;
}

//-------------------------------------------------------------------------------------------------
