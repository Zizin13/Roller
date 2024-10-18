#include "MathHelpers.h"
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

double MathHelpers::ConstrainAngle(double dAngle)
{
  dAngle = fmod(dAngle, 360);
  if (dAngle < 0)
    dAngle += 360;
  return dAngle;
}

//-------------------------------------------------------------------------------------------------