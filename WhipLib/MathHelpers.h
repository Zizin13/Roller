#ifndef _WHIPLIB_MATHHELPERS_H
#define _WHIPLIB_MATHHELPERS_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------

namespace MathHelpers
{
  extern double ConstrainAngle(double dAngle);
  extern glm::vec3 ProjectPointOntoPlane(const glm::vec3 &pos, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3);
}

//-------------------------------------------------------------------------------------------------
#endif