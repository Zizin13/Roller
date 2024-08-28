#ifndef _WHIPLIB_VERTEX_H
#define _WHIPLIB_VERTEX_H
//-------------------------------------------------------------------------------------------------
#include "glm.hpp"
#include "Types.h"
//-------------------------------------------------------------------------------------------------
static const uint32 NUM_FLOATS_PER_VERTICE = 22;
static const uint32 VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
//-------------------------------------------------------------------------------------------------
struct tVertex
{
  tVertex()
  {
    position = glm::vec3(0);
    color = glm::vec4(0);
    normal = glm::vec3(0);
    texCoords = glm::vec2(0);
    flags = glm::vec4(0);
    backColor = glm::vec4(0);
    backTexCoords = glm::vec2(0);
  }
  glm::vec3 position;
  glm::vec4 color;
  glm::vec3 normal;
  glm::vec2 texCoords;
  glm::vec4 flags;
  glm::vec4 backColor;
  glm::vec2 backTexCoords;
};
//-------------------------------------------------------------------------------------------------
#endif