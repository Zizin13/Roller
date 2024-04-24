#include "DebugShapes.h"
#include "ShapeData.h"
#include "Shader.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

tVertex *MakeVertsAxes(uint32 &uiNumVerts)
{
  uiNumVerts = 4;
  tVertex *vertices = new tVertex[uiNumVerts];

  vertices[0].position = glm::vec3(1, 0, 0);
  vertices[1].position = glm::vec3(0, 1, 0);
  vertices[2].position = glm::vec3(0, 0, 1);
  vertices[3].position = glm::vec3(0, 0, 0);
  vertices[0].color = glm::vec3(1, 0, 0);
  vertices[1].color = glm::vec3(0, 1, 0);
  vertices[2].color = glm::vec3(0, 0, 1);
  vertices[3].color = glm::vec3(1, 1, 1);
  vertices[0].flags.x = 1.0f;
  vertices[1].flags.x = 1.0f;
  vertices[2].flags.x = 1.0f;
  vertices[3].flags.x = 1.0f;

  return vertices;
}

//-------------------------------------------------------------------------------------------------

uint32 *MakeIndicesAxes(uint32 &uiNumIndices)
{
  uiNumIndices = 6;
  uint32 *indices = new uint32[uiNumIndices];
  memset(indices, 0, uiNumIndices * sizeof(uint32));
  
  indices[0] = 3;
  indices[1] = 0;
  indices[2] = 3;
  indices[3] = 1;
  indices[4] = 3;
  indices[5] = 2;

  return indices;
}

//-------------------------------------------------------------------------------------------------

CShapeData *DebugShapes::MakeAxes(CShader *pShader)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVertsAxes(uiNumVerts);
  uint32 uiNumIndices;
  uint32 *indices = MakeIndicesAxes(uiNumIndices);
  GLenum drawType = GL_LINES;

  CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts);
  CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
  CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

  CShapeData *pRet = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, NULL, drawType);

  if (vertices)
    delete[] vertices;
  if (indices)
    delete[] indices;

  return pRet;
}

//-------------------------------------------------------------------------------------------------