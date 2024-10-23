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

float MathHelpers::Dist(const glm::vec3 &pos1, const glm::vec3 &pos2)
{
  return sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) +
              (pos1.y - pos2.y) * (pos1.y - pos2.y) +
              (pos1.z - pos2.z) * (pos1.z - pos2.z));
}

//-------------------------------------------------------------------------------------------------

glm::vec3 MathHelpers::ProjectPointOntoPlane(const glm::vec3 &pos, const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2)
{
  //get plane normal
  glm::vec3 sub1 = p2 - p0;
  glm::vec3 sub2 = p1 - p0;
  glm::vec3 normal = glm::normalize(glm::cross(sub1, sub2));
  glm::vec3 v = pos - p0;
  //project point onto plane
  float fDist = glm::dot(v, normal);
  glm::vec3 ret = (pos - fDist * normal);
  return ret;
}

//-------------------------------------------------------------------------------------------------

float MathHelpers::GetProjectionPercentageAlongSegment(const glm::vec3 &pos, const glm::vec3 &begin, const glm::vec3 &end)
{
  glm::vec3 endMinusBegin = end - begin;
  glm::vec3 posMinusBegin = pos - begin;
  float fScalarProjection = glm::dot(posMinusBegin, glm::normalize(endMinusBegin));
  float fL2 = glm::length(endMinusBegin);
  float fPercent = fScalarProjection / fL2;
  return fPercent;
}

//-------------------------------------------------------------------------------------------------

bool MathHelpers::RayCollisionTriangle(const glm::vec3 &rayOrig, const glm::vec3 &rayVec, const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2)
{
  constexpr float epsilon = std::numeric_limits<float>::epsilon();

  glm::vec3 edge1 = p1 - p0;
  glm::vec3 edge2 = p2 - p0;
  glm::vec3 ray_cross_e2 = glm::cross(rayVec, edge2);
  float det = glm::dot(edge1, ray_cross_e2);

  if (det > -epsilon && det < epsilon)
    return false;    // This ray is parallel to this triangle.

  float inv_det = 1.0f / det;
  glm::vec3 s = rayOrig - p0;
  float u = inv_det * glm::dot(s, ray_cross_e2);

  if (u < 0 || u > 1)
    return false;

  glm::vec3 s_cross_e1 = glm::cross(s, edge1);
  float v = inv_det * glm::dot(rayVec, s_cross_e1);

  if (v < 0 || u + v > 1)
    return false;

// At this stage we can compute t to find out where the intersection point is on the line.
  float t = inv_det * glm::dot(edge2, s_cross_e1);

  if (t > epsilon) // ray intersection
  {
    return true;// glm::vec3(ray_origin + ray_vector * t);
  } else // This means that there is a line intersection but not a ray intersection.
    return false;
}
//-------------------------------------------------------------------------------------------------