#ifndef _WHIPLIB_VERTEX_H
#define _WHIPLIB_VERTEX_H
//-------------------------------------------------------------------------------------------------
#include "glm.hpp"
#include "Types.h"
//-------------------------------------------------------------------------------------------------
static const uint32 NUM_FLOATS_PER_VERTICE = 11;
static const uint32 VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
//-------------------------------------------------------------------------------------------------
struct tVertex
{
  tVertex()
  {
    position = glm::vec3(0);
    normal = glm::vec3(0);
    texCoords = glm::vec2(0);
    backTexCoords = glm::vec2(0);
    fAlpha = 1.0f;
  }
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
  glm::vec2 backTexCoords;
  float fAlpha;
};
//-------------------------------------------------------------------------------------------------
#endif