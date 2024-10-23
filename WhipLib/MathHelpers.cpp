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

bool MathHelpers::RayCollisionTriangle(const glm::vec3 &pos1, const glm::vec3 &pos2, const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2)
{
  constexpr float fEpsilon = std::numeric_limits<float>::epsilon();
  glm::vec3 edge1;
  glm::vec3 edge2;
  glm::vec3 p, q, tv;
  float det, invDet, u, v, t;

  edge1 = p1 - p0;
  edge2 = p2 - p0;
  p = glm::cross(pos2, edge2);
  det = glm::dot(edge1, p);

  //parallel to triangle
  if ((det > -fEpsilon) && (det < fEpsilon)) return false;

  invDet = 1.0f / det;
  //distance from p0 to ray origin
  tv = pos2 - p1;
  u = glm::dot(tv, p) * invDet;
  //not in triangle
  if ((u < 0.0f) || (u > 1.0f)) return false;

  q = glm::cross(tv, edge1);
  v = glm::dot(pos2, q) * invDet;
  //intersects but outside triangle
  if ((v < 0.0f) || ((u + v) > 1.0f)) return false;

  t = glm::dot(edge2, q) * invDet;
  if (t > fEpsilon) {
    //ray intersection
    return true;
  }
  //line intersection but not ray intersection
  return false;
}
//-------------------------------------------------------------------------------------------------