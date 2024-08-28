#ifndef _WHIPLIB_VERTEX_H
#define _WHIPLIB_VERTEX_H
//-------------------------------------------------------------------------------------------------
#include "glm.hpp"
#include "Types.h"
//-------------------------------------------------------------------------------------------------
static const uint32 NUM_FLOATS_PER_VERTICE = 18;
static const uint32 NUM_BYTES_PER_VERTICE = 4;
static const uint32 VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float) + NUM_BYTES_PER_VERTICE * sizeof(uint8);
//-------------------------------------------------------------------------------------------------
struct tVertex
{
  tVertex()
  {
    position = glm::vec3(0);
    normal = glm::vec3(0);
    color = glm::vec4(0);
    texCoords = glm::vec2(0);
    backColor = glm::vec4(0);
    backTexCoords = glm::vec2(0);
    byUseColor = 0;
    byPadding1 = 0;
    byBackUseColor = 0;
    byPadding2 = 0;
  }
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec4 color;
  glm::vec2 texCoords;
  glm::vec4 backColor;
  glm::vec2 backTexCoords;
  uint8 byUseColor;
  uint8 byBackUseColor;
  uint8 byPadding1;
  uint8 byPadding2;
};
//-------------------------------------------------------------------------------------------------
#endif