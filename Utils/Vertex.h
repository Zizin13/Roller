#ifndef _UTILS_VERTEX_H
#define _UTILS_VERTEX_H
//-------------------------------------------------------------------------------------------------
#include "glm.hpp"
#include "Types.h"
//-------------------------------------------------------------------------------------------------
static const uint32 NUM_FLOATS_PER_VERTICE = 11;
static const uint32 VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
//-------------------------------------------------------------------------------------------------
struct tVertex
{
  glm::vec3 position;
  glm::vec3 color;
  glm::vec3 normal;
  glm::vec2 texCoords;
};
//-------------------------------------------------------------------------------------------------
#endif