#include "glm.hpp"
#include "ShapeGenerator.h"
#include "Vertex.h"
#include "TeapotData.h"
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
#pragma warning(disable:4018)
#pragma warning(disable:4244)
//-------------------------------------------------------------------------------------------------
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)
#define PI 3.14159265359
//-------------------------------------------------------------------------------------------------

glm::vec3 RandomColor()
{
  glm::vec3 ret;
  ret.x = rand() / (float)RAND_MAX;
  ret.y = rand() / (float)RAND_MAX;
  ret.z = rand() / (float)RAND_MAX;
  return ret;
}

//-------------------------------------------------------------------------------------------------

tShapeData ShapeGenerator::MakeTriangle()
{
  tShapeData ret;
  tVertex myTri[] = {
    glm::vec3(+0.0f, +1.0f, +0.0f),
    glm::vec3(+1.0f, +0.0f, +0.0f),
    glm::vec3(+0.0f, +0.0f, +1.0f),

    glm::vec3(-1.0f, -1.0f, +0.0f),
    glm::vec3(+0.0f, +1.0f, +0.0f),
    glm::vec3(+0.0f, +0.0f, +1.0f),

    glm::vec3(+1.0f, -1.0f, +0.0f),
    glm::vec3(+0.0f, +0.0f, +1.0f),
    glm::vec3(+0.0f, +0.0f, +1.0f),
  };
  ret.pVertexBuf = new CVertexBuffer(myTri, NUM_ARRAY_ELEMENTS(myTri));

  GLuint indices[] = { 0, 1, 2 };
  ret.pIndexBuf = new CIndexBuffer(indices, NUM_ARRAY_ELEMENTS(indices));

  return ret;
}

//-------------------------------------------------------------------------------------------------

tShapeData ShapeGenerator::MakeCube()
{
  tShapeData ret;
  tVertex stackVerts[] = {
    glm::vec3(-1.0f, +1.0f, +1.0f),  // 0
    glm::vec3(+1.0f, +0.0f, +0.0f),	// Color
    glm::vec3(+0.0f, +1.0f, +0.0f),  // Normal
    glm::vec3(+1.0f, +1.0f, +1.0f),  // 1
    glm::vec3(+0.0f, +1.0f, +0.0f),	// Color
    glm::vec3(+0.0f, +1.0f, +0.0f),  // Normal
    glm::vec3(+1.0f, +1.0f, -1.0f),  // 2
    glm::vec3(+0.0f, +0.0f, +1.0f),  // Color
    glm::vec3(+0.0f, +1.0f, +0.0f),  // Normal
    glm::vec3(-1.0f, +1.0f, -1.0f),  // 3
    glm::vec3(+1.0f, +1.0f, +1.0f),  // Color
    glm::vec3(+0.0f, +1.0f, +0.0f),  // Normal

    glm::vec3(-1.0f, +1.0f, -1.0f),  // 4
    glm::vec3(+1.0f, +0.0f, +1.0f),  // Color
    glm::vec3(+0.0f, +0.0f, -1.0f),  // Normal
    glm::vec3(+1.0f, +1.0f, -1.0f),  // 5
    glm::vec3(+0.0f, +0.5f, +0.2f),  // Color
    glm::vec3(+0.0f, +0.0f, -1.0f),  // Normal
    glm::vec3(+1.0f, -1.0f, -1.0f),  // 6
    glm::vec3(+0.8f, +0.6f, +0.4f),  // Color
    glm::vec3(+0.0f, +0.0f, -1.0f),  // Normal
    glm::vec3(-1.0f, -1.0f, -1.0f),  // 7
    glm::vec3(+0.3f, +1.0f, +0.5f),  // Color
    glm::vec3(+0.0f, +0.0f, -1.0f),  // Normal

    glm::vec3(+1.0f, +1.0f, -1.0f),  // 8
    glm::vec3(+0.2f, +0.5f, +0.2f),  // Color
    glm::vec3(+1.0f, +0.0f, +0.0f),  // Normal
    glm::vec3(+1.0f, +1.0f, +1.0f),  // 9
    glm::vec3(+0.9f, +0.3f, +0.7f),  // Color
    glm::vec3(+1.0f, +0.0f, +0.0f),  // Normal
    glm::vec3(+1.0f, -1.0f, +1.0f),  // 10
    glm::vec3(+0.3f, +0.7f, +0.5f),  // Color
    glm::vec3(+1.0f, +0.0f, +0.0f),  // Normal
    glm::vec3(+1.0f, -1.0f, -1.0f),  // 11
    glm::vec3(+0.5f, +0.7f, +0.5f),  // Color
    glm::vec3(+1.0f, +0.0f, +0.0f),  // Normal

    glm::vec3(-1.0f, +1.0f, +1.0f),  // 12
    glm::vec3(+0.7f, +0.8f, +0.2f),  // Color
    glm::vec3(-1.0f, +0.0f, +0.0f),  // Normal
    glm::vec3(-1.0f, +1.0f, -1.0f),  // 13
    glm::vec3(+0.5f, +0.7f, +0.3f),  // Color
    glm::vec3(-1.0f, +0.0f, +0.0f),  // Normal
    glm::vec3(-1.0f, -1.0f, -1.0f),  // 14
    glm::vec3(+0.4f, +0.7f, +0.7f),  // Color
    glm::vec3(-1.0f, +0.0f, +0.0f),  // Normal
    glm::vec3(-1.0f, -1.0f, +1.0f),  // 15
    glm::vec3(+0.2f, +0.5f, +1.0f),  // Color
    glm::vec3(-1.0f, +0.0f, +0.0f),  // Normal

    glm::vec3(+1.0f, +1.0f, +1.0f),  // 16
    glm::vec3(+0.6f, +1.0f, +0.7f),  // Color
    glm::vec3(+0.0f, +0.0f, +1.0f),  // Normal
    glm::vec3(-1.0f, +1.0f, +1.0f),  // 17
    glm::vec3(+0.6f, +0.4f, +0.8f),  // Color
    glm::vec3(+0.0f, +0.0f, +1.0f),  // Normal
    glm::vec3(-1.0f, -1.0f, +1.0f),  // 18
    glm::vec3(+0.2f, +0.8f, +0.7f),  // Color
    glm::vec3(+0.0f, +0.0f, +1.0f),  // Normal
    glm::vec3(+1.0f, -1.0f, +1.0f),  // 19
    glm::vec3(+0.2f, +0.7f, +1.0f),  // Color
    glm::vec3(+0.0f, +0.0f, +1.0f),  // Normal

    glm::vec3(+1.0f, -1.0f, -1.0f),  // 20
    glm::vec3(+0.8f, +0.3f, +0.7f),  // Color
    glm::vec3(+0.0f, -1.0f, +0.0f),  // Normal
    glm::vec3(-1.0f, -1.0f, -1.0f),  // 21
    glm::vec3(+0.8f, +0.9f, +0.5f),  // Color
    glm::vec3(+0.0f, -1.0f, +0.0f),  // Normal
    glm::vec3(-1.0f, -1.0f, +1.0f),  // 22
    glm::vec3(+0.5f, +0.8f, +0.5f),  // Color
    glm::vec3(+0.0f, -1.0f, +0.0f),  // Normal
    glm::vec3(+1.0f, -1.0f, +1.0f),  // 23
    glm::vec3(+0.9f, +1.0f, +0.2f),  // Color
    glm::vec3(+0.0f, -1.0f, +0.0f),  // Normal
  };
  ret.pVertexBuf = new CVertexBuffer(stackVerts, NUM_ARRAY_ELEMENTS(stackVerts));

  GLuint indices[] = { 
     0,  1,  2,  0,  2,  3, //top
     4,  5,  6,  4,  6,  7, //front
     8,  9, 10,  8, 10, 11, //right
    12, 13, 14, 12, 14, 15, //left
    16, 17, 18, 16, 18, 19, //back
    20, 22, 21, 20, 23, 22, //bottom
  };
  ret.pIndexBuf = new CIndexBuffer(indices, NUM_ARRAY_ELEMENTS(indices));

  return ret;
}

//-------------------------------------------------------------------------------------------------

tShapeData ShapeGenerator::MakeArrow()
{
  tShapeData ret;

  // Vert + Color
  tVertex verts[] =
  {
    // Top side of arrow head
    glm::vec3(+0.00f, +0.25f, -0.25f),         // 0
    glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
    glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
    glm::vec3(+0.50f, +0.25f, -0.25f),         // 1
    glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
    glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
    glm::vec3(+0.00f, +0.25f, -1.00f),         // 2
    glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
    glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
    glm::vec3(-0.50f, +0.25f, -0.25f),         // 3
    glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
    glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
    // Bottom side of arrow head
    glm::vec3(+0.00f, -0.25f, -0.25f),         // 4
    glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
    glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
    glm::vec3(+0.50f, -0.25f, -0.25f),         // 5
    glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
    glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
    glm::vec3(+0.00f, -0.25f, -1.00f),         // 6
    glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
    glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
    glm::vec3(-0.50f, -0.25f, -0.25f),         // 7
    glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
    glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
    // Right side of arrow tip
    glm::vec3(+0.50f, +0.25f, -0.25f),         // 8
    glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
    glm::vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
    glm::vec3(+0.00f, +0.25f, -1.00f),         // 9
    glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
    glm::vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
    glm::vec3(+0.00f, -0.25f, -1.00f),         // 10
    glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
    glm::vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
    glm::vec3(+0.50f, -0.25f, -0.25f),         // 11
    glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
    glm::vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
    // Left side of arrow tip
    glm::vec3(+0.00f, +0.25f, -1.00f),         // 12
    glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
    glm::vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
    glm::vec3(-0.50f, +0.25f, -0.25f),         // 13
    glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
    glm::vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
    glm::vec3(+0.00f, -0.25f, -1.00f),         // 14
    glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
    glm::vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
    glm::vec3(-0.50f, -0.25f, -0.25f),         // 15
    glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
    glm::vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
    // Back side of arrow tip
    glm::vec3(-0.50f, +0.25f, -0.25f),         // 16
    glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
    glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
    glm::vec3(+0.50f, +0.25f, -0.25f),         // 17
    glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
    glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
    glm::vec3(-0.50f, -0.25f, -0.25f),         // 18
    glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
    glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
    glm::vec3(+0.50f, -0.25f, -0.25f),         // 19
    glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
    glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
    // Top side of back of arrow
    glm::vec3(+0.25f, +0.25f, -0.25f),         // 20
    glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
    glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
    glm::vec3(+0.25f, +0.25f, +1.00f),         // 21
    glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
    glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
    glm::vec3(-0.25f, +0.25f, +1.00f),         // 22
    glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
    glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
    glm::vec3(-0.25f, +0.25f, -0.25f),         // 23
    glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
    glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
    // Bottom side of back of arrow
    glm::vec3(+0.25f, -0.25f, -0.25f),         // 24
    glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
    glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
    glm::vec3(+0.25f, -0.25f, +1.00f),         // 25
    glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
    glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
    glm::vec3(-0.25f, -0.25f, +1.00f),         // 26
    glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
    glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
    glm::vec3(-0.25f, -0.25f, -0.25f),         // 27
    glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
    glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
    // Right side of back of arrow
    glm::vec3(+0.25f, +0.25f, -0.25f),         // 28
    glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
    glm::vec3(+1.00f, +0.00f, +0.00f),         // Normal
    glm::vec3(+0.25f, -0.25f, -0.25f),         // 29
    glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
    glm::vec3(+1.00f, +0.00f, +0.00f),         // Normal
    glm::vec3(+0.25f, -0.25f, +1.00f),         // 30
    glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
    glm::vec3(+1.00f, +0.00f, +0.00f),         // Normal
    glm::vec3(+0.25f, +0.25f, +1.00f),         // 31
    glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
    glm::vec3(+1.00f, +0.00f, +0.00f),         // Normal
    // Left side of back of arrow
    glm::vec3(-0.25f, +0.25f, -0.25f),         // 32
    glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
    glm::vec3(-1.00f, +0.00f, +0.00f),         // Normal
    glm::vec3(-0.25f, -0.25f, -0.25f),         // 33
    glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
    glm::vec3(-1.00f, +0.00f, +0.00f),         // Normal
    glm::vec3(-0.25f, -0.25f, +1.00f),         // 34
    glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
    glm::vec3(-1.00f, +0.00f, +0.00f),         // Normal
    glm::vec3(-0.25f, +0.25f, +1.00f),         // 35
    glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
    glm::vec3(-1.00f, +0.00f, +0.00f),         // Normal
    // Back side of back of arrow
    glm::vec3(-0.25f, +0.25f, +1.00f),         // 36
    glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
    glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
    glm::vec3(+0.25f, +0.25f, +1.00f),         // 37
    glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
    glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
    glm::vec3(-0.25f, -0.25f, +1.00f),         // 38
    glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
    glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
    glm::vec3(+0.25f, -0.25f, +1.00f),         // 39
    glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
    glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
  };

  ret.pVertexBuf = new CVertexBuffer(verts, NUM_ARRAY_ELEMENTS(verts));

  GLuint indices[] = {
    0, 1, 2, // Top
    0, 2, 3,
    4, 6, 5, // Bottom
    4, 7, 6,
    8, 10, 9, // Right side of arrow tip
    8, 11, 10,
    12, 15, 13, // Left side of arrow tip
    12, 14, 15,
    16, 19, 17, // Back side of arrow tip
    16, 18, 19,
    20, 22, 21, // Top side of back of arrow
    20, 23, 22,
    24, 25, 26, // Bottom side of back of arrow
    24, 26, 27,
    28, 30, 29, // Right side of back of arrow
    28, 31, 30,
    32, 33, 34, // Left side of back of arrow
    32, 34, 35,
    36, 39, 37, // Back side of back of arrow
    36, 38, 39,
  };

  ret.pIndexBuf = new CIndexBuffer(indices, NUM_ARRAY_ELEMENTS(indices));

  return ret;
}

//-------------------------------------------------------------------------------------------------

tShapeData ShapeGenerator::MakePlane(unsigned int uiDimensions)
{
  tShapeData ret;

  uint32 uiNumVerts;
  struct tVertex *vertices = MakePlaneVerts(uiNumVerts, uiDimensions);
  ret.pVertexBuf = new CVertexBuffer(vertices, uiNumVerts);
  delete[] vertices;

  uint32 uiNumIndices;
  uint32 *indices = MakePlaneIndices(uiNumIndices, uiDimensions);
  ret.pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
  delete[] indices;

  return ret;
}

//-------------------------------------------------------------------------------------------------

tVertex *ShapeGenerator::MakePlaneVerts(uint32 &numVertices, uint32 uiDimensions)
{
  numVertices = uiDimensions * uiDimensions;
  int half = uiDimensions / 2;
  tVertex *vertices = new tVertex[numVertices];
  for (int i = 0; i < uiDimensions; i++) {
    for (int j = 0; j < uiDimensions; j++) {
      tVertex &thisVert = vertices[i * uiDimensions + j];
      thisVert.position.x = j - half;
      thisVert.position.z = i - half;
      thisVert.position.y = 0;
      thisVert.normal = glm::vec3(0,-1,0);
      thisVert.color = RandomColor();
    }
  }
  return vertices;
}

//-------------------------------------------------------------------------------------------------

uint32 *ShapeGenerator::MakePlaneIndices(uint32 &numIndices, uint32 uiDimensions)
{
  numIndices = (uiDimensions - 1) * (uiDimensions - 1) * 2 * 3; // 2 triangles per square, 3 indices per triangle
  uint32 *indices = new uint32[numIndices];
  int runner = 0;
  for (int row = 0; row < uiDimensions - 1; row++) {
    for (int col = 0; col < uiDimensions - 1; col++) {
      indices[runner++] = uiDimensions * row + col;
      indices[runner++] = uiDimensions * row + col + uiDimensions;
      indices[runner++] = uiDimensions * row + col + uiDimensions + 1;

      indices[runner++] = uiDimensions * row + col;
      indices[runner++] = uiDimensions * row + col + uiDimensions + 1;
      indices[runner++] = uiDimensions * row + col + 1;
    }
  }
  assert(runner == numIndices);
  return indices;
}

//-------------------------------------------------------------------------------------------------

CIndexBuffer *ShapeGenerator::MakePlaneUnseamedIndices(GLuint tesselation)
{
  GLuint dimensions = tesselation * tesselation;
  uint32 numIndices = dimensions * 2 * 3; // 2 triangles per square, 3 indices per triangle
  uint32 *indices = new uint32[numIndices];
  int runner = 0;
  for (int row = 0; row < tesselation; row++) {
          // This code is crap but works, and I'm not in the mood right now to clean it up
    for (int col = 0; col < tesselation; col++) {
            // Bottom left triangle
      indices[runner++] = tesselation * row + col;
      // One row down unless it's the bottom row, 
      indices[runner++] = (row + 1 == tesselation ? 0 : tesselation * row + tesselation) + col;
      // Move to vert right of this one unless it's the last vert,
      // which we connect to the first vert in the row
      // the % dimensions at the end accounts for the last row hooking to the first row
      indices[runner++] = (tesselation * row + col + tesselation + (col + 1 == tesselation ? (tesselation * -1) + 1 : 1)) % dimensions;

      // Upper right triangle
      indices[runner++] = tesselation * row + col;
      if (col + 1 == tesselation && row + 1 == tesselation) {
              // Very last vert
        indices[runner++] = 0;
      } else if (col + 1 == tesselation) {
              // Last vert on this row
              // Check if we need to connect it to zeroeth row or the next row
        if (row + 1 == tesselation) {
                // Tie to zeroeth row
          indices[runner++] = col + 1;
        } else {
                // Tie to next row
          indices[runner++] = tesselation * row + col + 1;
        }
      } else {
              // Regular interior vert
              // the % dimensions at the end accounts for the last row hooking to the first row
        indices[runner++] = (tesselation * row + col + tesselation + 1) % dimensions;
      }

      indices[runner++] = tesselation * row + col + (col + 1 == tesselation ? -col : 1);
    }
  }

  CIndexBuffer *pRetBuf = new CIndexBuffer(indices, numIndices);
  delete[] indices;
  return pRetBuf;
}

//-------------------------------------------------------------------------------------------------

tShapeData ShapeGenerator::MakeTeapot(unsigned int uiTesselation, const glm::mat4 &lidTransform)
{
  uint32 numVertices = 32 * (uiTesselation + 1) * (uiTesselation + 1);
  GLuint faces = uiTesselation * uiTesselation * 32;
  float *vertices = new float[numVertices * 3];
  float *normals = new float[numVertices * 3];
  float *textureCoordinates = new float[numVertices * 2];
  uint32 numIndices = faces * 6;
  unsigned short *indices = new unsigned short[numIndices];

  generatePatches(vertices, normals, textureCoordinates, indices, uiTesselation);
  //moveLid(uiTesselation, vertices, lidTransform);

  // Adapt/convert their data format to mine
  tVertex *retVertices = new tVertex[numVertices];
  for (uint32 i = 0; i < numVertices; i++) {
    tVertex &v = retVertices[i];
    v.position.x = vertices[i * 3 + 0];
    v.position.y = vertices[i * 3 + 1];
    v.position.z = vertices[i * 3 + 2];
    v.normal.x = normals[i * 3 + 0];
    v.normal.y = normals[i * 3 + 1];
    v.normal.z = normals[i * 3 + 2];
    v.color = RandomColor();
  }
  uint32 *retIndices = new uint32[numIndices];
  for (uint32 i = 0; i < numIndices; ++i) {
    retIndices[i] = indices[i];
  }

  tShapeData ret;
  ret.pVertexBuf = new CVertexBuffer(retVertices, numVertices);
  ret.pIndexBuf = new CIndexBuffer(retIndices, numIndices);

  delete[] vertices;
  delete[] normals;
  delete[] textureCoordinates;
  delete[] indices;
  delete[] retVertices;
  delete[] retIndices;

  return ret;
}

//-------------------------------------------------------------------------------------------------

void ShapeGenerator::generatePatches(float *v, float *n, float *tc, unsigned short *el, int grid)
{
  float *B = new float[4 * (grid + 1)];  // Pre-computed Bernstein basis functions
  float *dB = new float[4 * (grid + 1)]; // Pre-computed derivitives of basis functions

  int idx = 0, elIndex = 0, tcIndex = 0;

  // Pre-compute the basis functions  (Bernstein polynomials)
  // and their derivatives
  computeBasisFunctions(B, dB, grid);

  // Build each patch
  // The rim
  buildPatchReflect(0, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  // The body
  buildPatchReflect(1, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  buildPatchReflect(2, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  // The lid
  buildPatchReflect(3, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  buildPatchReflect(4, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  // The bottom
  buildPatchReflect(5, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  // The handle
  buildPatchReflect(6, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
  buildPatchReflect(7, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
  // The spout
  buildPatchReflect(8, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
  buildPatchReflect(9, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);

  delete[] B;
  delete[] dB;
}

//-------------------------------------------------------------------------------------------------

void ShapeGenerator::moveLid(int grid, float *v, glm::mat4 lidTransform)
{

  int start = 3 * 12 * (grid + 1) * (grid + 1);
  int end = 3 * 20 * (grid + 1) * (grid + 1);

  for (int i = start; i < end; i += 3) {
    glm::vec4 vert = glm::vec4(v[i], v[i + 1], v[i + 2], 1.0f);
    vert = lidTransform * vert;
    v[i] = vert.x;
    v[i + 1] = vert.y;
    v[i + 2] = vert.z;
  }
}

//-------------------------------------------------------------------------------------------------

void ShapeGenerator::buildPatchReflect(int patchNum,
        float *B, float *dB,
        float *v, float *n,
        float *tc, unsigned short *el,
        int &index, int &elIndex, int &tcIndex, int grid,
        bool reflectX, bool reflectY)
{
  glm::vec3 patch[4][4];
  glm::vec3 patchRevV[4][4];
  getPatch(patchNum, patch, false);
  getPatch(patchNum, patchRevV, true);

  // Patch without modification
  buildPatch(patch, B, dB, v, n, tc, el,
          index, elIndex, tcIndex, grid, glm::mat3(1.0f), true);

  // Patch reflected in x
  if (reflectX) {
    buildPatch(patchRevV, B, dB, v, n, tc, el,
            index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
                                                     glm::vec3(0.0f, 1.0f, 0.0f),
                                                     glm::vec3(0.0f, 0.0f, 1.0f)), false);
  }

  // Patch reflected in y
  if (reflectY) {
    buildPatch(patchRevV, B, dB, v, n, tc, el,
            index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(1.0f, 0.0f, 0.0f),
                                                     glm::vec3(0.0f, -1.0f, 0.0f),
                                                     glm::vec3(0.0f, 0.0f, 1.0f)), false);
  }

  // Patch reflected in x and y
  if (reflectX && reflectY) {
    buildPatch(patch, B, dB, v, n, tc, el,
            index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
                                                     glm::vec3(0.0f, -1.0f, 0.0f),
                                                     glm::vec3(0.0f, 0.0f, 1.0f)), true);
  }
}

//-------------------------------------------------------------------------------------------------

void ShapeGenerator::buildPatch(glm::vec3 patch[][4],
        float *B, float *dB,
        float *v, float *n, float *tc,
        unsigned short *el,
        int &index, int &elIndex, int &tcIndex, int grid, glm::mat3 reflect,
        bool invertNormal)
{
  int startIndex = index / 3;
  float tcFactor = 1.0f / grid;

  for (int i = 0; i <= grid; i++) {
    for (int j = 0; j <= grid; j++) {
      glm::vec3 pt = reflect * evaluate(i, j, B, patch);
      glm::vec3 norm = reflect * evaluateNormal(i, j, B, dB, patch);
      if (invertNormal)
        norm = -norm;

      v[index] = pt.x;
      v[index + 1] = pt.y;
      v[index + 2] = pt.z;

      n[index] = norm.x;
      n[index + 1] = norm.y;
      n[index + 2] = norm.z;

      tc[tcIndex] = i * tcFactor;
      tc[tcIndex + 1] = j * tcFactor;

      index += 3;
      tcIndex += 2;
    }
  }

  for (int i = 0; i < grid; i++) {
    int iStart = i * (grid + 1) + startIndex;
    int nextiStart = (i + 1) * (grid + 1) + startIndex;
    for (int j = 0; j < grid; j++) {
      el[elIndex] = iStart + j;
      el[elIndex + 1] = nextiStart + j + 1;
      el[elIndex + 2] = nextiStart + j;

      el[elIndex + 3] = iStart + j;
      el[elIndex + 4] = iStart + j + 1;
      el[elIndex + 5] = nextiStart + j + 1;

      elIndex += 6;
    }
  }
}

//-------------------------------------------------------------------------------------------------

void ShapeGenerator::getPatch(int patchNum, glm::vec3 patch[][4], bool reverseV)
{
  for (int u = 0; u < 4; u++) {          // Loop in u direction
    for (int v = 0; v < 4; v++) {     // Loop in v direction
      if (reverseV) {
        patch[u][v] = glm::vec3(
                Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + (3 - v)]][0],
                Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + (3 - v)]][1],
                Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + (3 - v)]][2]
        );
      } else {
        patch[u][v] = glm::vec3(
                Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + v]][0],
                Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + v]][1],
                Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + v]][2]
        );
      }
    }
  }
}

//-------------------------------------------------------------------------------------------------

void ShapeGenerator::computeBasisFunctions(float *B, float *dB, int grid)
{
  float inc = 1.0f / grid;
  for (int i = 0; i <= grid; i++) {
    float t = i * inc;
    float tSqr = t * t;
    float oneMinusT = (1.0f - t);
    float oneMinusT2 = oneMinusT * oneMinusT;

    B[i * 4 + 0] = oneMinusT * oneMinusT2;
    B[i * 4 + 1] = 3.0f * oneMinusT2 * t;
    B[i * 4 + 2] = 3.0f * oneMinusT * tSqr;
    B[i * 4 + 3] = t * tSqr;

    dB[i * 4 + 0] = -3.0f * oneMinusT2;
    dB[i * 4 + 1] = -6.0f * t * oneMinusT + 3.0f * oneMinusT2;
    dB[i * 4 + 2] = -3.0f * tSqr + 6.0f * t * oneMinusT;
    dB[i * 4 + 3] = 3.0f * tSqr;
  }
}

//-------------------------------------------------------------------------------------------------

glm::vec3 ShapeGenerator::evaluate(int gridU, int gridV, float *B, glm::vec3 patch[][4])
{
  glm::vec3 p(0.0f, 0.0f, 0.0f);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      p += patch[i][j] * B[gridU * 4 + i] * B[gridV * 4 + j];
    }
  }
  return p;
}

//-------------------------------------------------------------------------------------------------

glm::vec3 ShapeGenerator::evaluateNormal(int gridU, int gridV, float *B, float *dB, glm::vec3 patch[][4])
{
  glm::vec3 du(0.0f, 0.0f, 0.0f);
  glm::vec3 dv(0.0f, 0.0f, 0.0f);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      du += patch[i][j] * dB[gridU * 4 + i] * B[gridV * 4 + j];
      dv += patch[i][j] * B[gridU * 4 + i] * dB[gridV * 4 + j];
    }
  }
  return glm::normalize(glm::cross(du, dv));
}

//-------------------------------------------------------------------------------------------------

tShapeData ShapeGenerator::MakeTorus(GLuint tesselation)
{
  GLuint dimensions = tesselation * tesselation;
  uint32 numVertices = dimensions;
  tVertex *vertices = new tVertex[numVertices];
  float sliceAngle = 360 / tesselation;
  const float torusRadius = 1.0f;
  const float pipeRadius = 0.5f;
  for (GLuint round1 = 0; round1 < tesselation; round1++) {
          // Generate a circle on the xy plane, then
          // translate then rotate it into position
    glm::mat4 transform =
      glm::rotate(glm::mat4(1.f), (float)glm::radians(round1 * sliceAngle), glm::vec3(0.0f, 1.0f, 0.0f)) *
      glm::translate(glm::mat4(1.f), glm::vec3(torusRadius, 0.0f, 0.0f));
    glm::mat3 normalTransform = (glm::mat3)(transform);
    for (GLuint round2 = 0; round2 < tesselation; round2++) {
      tVertex &v = vertices[round1 * tesselation + round2];
      glm::vec4 glmVert = glm::vec4(
              pipeRadius * cos(glm::radians(sliceAngle * round2)),
              pipeRadius * sin(glm::radians(sliceAngle * round2)),
              0,
              1.0f);
      glm::vec4 glmVertPrime = glm::vec4(transform * glmVert);
      v.position = (glm::vec3)(glmVertPrime);
      v.normal = glm::normalize(normalTransform * (glm::vec3)(glmVert));
      v.color = RandomColor();
    }
  }

  tShapeData ret;
  ret.pVertexBuf = new CVertexBuffer(vertices, numVertices);
  ret.pIndexBuf = MakePlaneUnseamedIndices(tesselation);
  delete[] vertices;

  return ret;
}

//-------------------------------------------------------------------------------------------------

tShapeData ShapeGenerator::MakeSphere(GLuint tesselation)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakePlaneVerts(uiNumVerts, tesselation);
  uint32 uiNumIndices;
  uint32 *indices = MakePlaneIndices(uiNumIndices, tesselation);

  GLuint dimensions = tesselation;
  const float RADIUS = 1.0f;
  const double CIRCLE = PI * 2;
  const double SLICE_ANGLE = CIRCLE / (dimensions - 1);
  for (size_t col = 0; col < dimensions; col++) {
    double phi = -SLICE_ANGLE * col;
    for (size_t row = 0; row < dimensions; row++) {
      double theta = -(SLICE_ANGLE / 2.0) * row;
      size_t vertIndex = col * dimensions + row;
      tVertex &v = vertices[vertIndex];
      v.position.x = RADIUS * cos(phi) * sin(theta);
      v.position.y = RADIUS * sin(phi) * sin(theta);
      v.position.z = RADIUS * cos(theta);
      v.normal = glm::normalize(v.position);
    }
  }

  tShapeData ret;
  ret.pVertexBuf = new CVertexBuffer(vertices, uiNumVerts);
  ret.pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
  delete[] vertices;
  delete[] indices;
  return ret;
}

//-------------------------------------------------------------------------------------------------

//tShapeData ShapeGenerator::GenerateNormals(const tShapeData &data)
//{
//  tShapeData ret;
//  ret.numVertices = data.numVertices * 2;
//  ret.vertices = new tVertex[ret.numVertices];
//  glm::vec3 white(1.0f, 1.0f, 1.0f);
//  for (int i = 0; i < data.numVertices; i++) {
//    GLuint vertIndex = i * 2;
//    tVertex &v1 = ret.vertices[vertIndex];
//    tVertex &v2 = ret.vertices[vertIndex + 1];
//    const tVertex &sourceVertex = data.vertices[i];
//    v1.position = sourceVertex.position;
//    v2.position = sourceVertex.position + sourceVertex.normal;
//    v1.color = v2.color = white;
//  }
//
//  ret.numIndices = ret.numVertices;
//  ret.indices = new GLushort[ret.numIndices];
//  for (int i = 0; i < ret.numIndices; i++)
//    ret.indices[i] = i;
//  return ret;
//}

//-------------------------------------------------------------------------------------------------