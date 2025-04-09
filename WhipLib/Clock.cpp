#include "Clock.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
#if defined(IS_WINDOWS)
//-------------------------------------------------------------------------------------------------
bool CClock::Init()
{
  bool bSuccess = (0 != QueryPerformanceFrequency(&m_cpuTimerFrequency));
  if (!bSuccess) { return false; }

  bSuccess = (1 == QueryPerformanceCounter(&m_lastFrameTimeCounter));
  if (!bSuccess) { return false; }

  return true;
}

//-------------------------------------------------------------------------------------------------

void CClock::NewFrame()
{
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);

  LARGE_INTEGER delta;
  delta.QuadPart = now.QuadPart - m_lastFrameTimeCounter.QuadPart;
  m_fDeltaTime = ((float)delta.QuadPart) / m_cpuTimerFrequency.QuadPart;

  m_lastFrameTimeCounter.QuadPart = now.QuadPart;
}

//-------------------------------------------------------------------------------------------------

float CClock::DeltaTimeLastFrame() const
{
  return m_fDeltaTime;
}

//-------------------------------------------------------------------------------------------------
#else
//-------------------------------------------------------------------------------------------------
bool CClock::Init()
{
  return true;
}

//-------------------------------------------------------------------------------------------------

void CClock::NewFrame()
{
}

//-------------------------------------------------------------------------------------------------

float CClock::DeltaTimeLastFrame() const
{
  return m_fDeltaTime;
}

//-------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------
