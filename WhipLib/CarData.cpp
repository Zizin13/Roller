#include <glew.h>
#include "CarData.h"
#include "ZizinPlans.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "ShapeData.h"
#include "ShapeGenerator.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CCarData::CCarData()
  : m_fScale(1000.0f)
{

}

//-------------------------------------------------------------------------------------------------

CCarData::~CCarData()
{

}

//-------------------------------------------------------------------------------------------------

CShapeData *CCarData::MakeCar(CShader *pShader)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVerts(uiNumVerts);
  uint32 uiNumIndices;
  uint32 *indices = MakeIndices(uiNumIndices);
  GLenum drawType = GL_LINES;

  for (uint32 i = 0; i < uiNumVerts; ++i) {
    vertices[i].flags.x = 1.0f; //use color rather than tex
    vertices[i].color = ShapeGenerator::RandomColor();
  }

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

tVertex *CCarData::MakeVerts(uint32 &numVertices)
{
  numVertices = g_xzizinCoordsCount / 3;
  tVertex *vertices = new tVertex[numVertices];
  int iVertIndex = 0;
  for (uint32 i = 0; i < g_xzizinCoordsCount; ++i) {
    vertices[i / 3].position[iVertIndex] = g_xzizinCoords[i] / m_fScale;
    iVertIndex++;
    if (iVertIndex == 3)
      iVertIndex = 0;
  }

  return vertices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CCarData::MakeIndices(uint32 &numIndices)
{
  int iNumIndicesPerPol = 8;
  numIndices = g_xzizinPolsCount * iNumIndicesPerPol;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  uint32 i = 0;
  for (; i < g_xzizinPolsCount; i++) {
    indices[i * iNumIndicesPerPol + 0] = g_xzizinPols[i].byVert1;
    indices[i * iNumIndicesPerPol + 1] = g_xzizinPols[i].byVert2;
    indices[i * iNumIndicesPerPol + 2] = g_xzizinPols[i].byVert2;
    indices[i * iNumIndicesPerPol + 3] = g_xzizinPols[i].byVert3;
    indices[i * iNumIndicesPerPol + 4] = g_xzizinPols[i].byVert3;
    indices[i * iNumIndicesPerPol + 5] = g_xzizinPols[i].byVert4;
    indices[i * iNumIndicesPerPol + 6] = g_xzizinPols[i].byVert4;
    indices[i * iNumIndicesPerPol + 7] = g_xzizinPols[i].byVert1;
  }

  return indices;
}

//-------------------------------------------------------------------------------------------------