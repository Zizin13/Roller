#include <glew.h>
#include "CarData.h"
#include "CarPlans.h"
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
  : m_fScale(10000.0f)
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
  return NULL;
}

//-------------------------------------------------------------------------------------------------

uint32 *CCarData::MakeIndices(uint32 &numIndices)
{
  return NULL;
}

//-------------------------------------------------------------------------------------------------