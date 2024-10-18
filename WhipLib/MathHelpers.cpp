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

float MathHelpers::Dist(glm::vec3 &pos1, glm::vec3 &pos2)
{
  return sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) +
              (pos1.y - pos2.y) * (pos1.y - pos2.y) +
              (pos1.z - pos2.z) * (pos1.z - pos2.z));
}

//-------------------------------------------------------------------------------------------------

glm::vec3 MathHelpers::ProjectPointOntoPlane(const glm::vec3 &pos, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3)
{
  //get plane normal
  glm::vec3 tl1 = p3 - p1;
  glm::vec3 tl2 = p2 - p1;
  glm::vec3 normal = glm::normalize(glm::cross(tl1, tl2));

  glm::vec3 v = pos - p1;

  //project point onto plane
  float fDist = glm::dot(v, normal);
  return (pos - fDist * normal);
}

//-------------------------------------------------------------------------------------------------