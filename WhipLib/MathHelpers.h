#ifndef _WHIPLIB_MATHHELPERS_H
#define _WHIPLIB_MATHHELPERS_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------

namespace MathHelpers
{
  extern double ConstrainAngle(double dAngle);
  extern float Dist(const glm::vec3 &pos1, const glm::vec3 &pos2);
  extern glm::vec3 ProjectPointOntoPlane(const glm::vec3 &pos, const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2);
  extern float GetProjectionPercentageAlongSegment(const glm::vec3 &pos, const glm::vec3 &begin, const glm::vec3 &end);
}

//-------------------------------------------------------------------------------------------------
#endif