#include <glew.h>
#include "CarData.h"
#include "ZizinPlans.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "ShapeData.h"
#include "ShapeGenerator.h"
#include "Texture.h"
#include "Palette.h"
#include "TrackData.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

class CCarDataPrivate
{
public:
  CCarDataPrivate() {};
  ~CCarDataPrivate() {};

  CPalette m_pal;
  CTexture m_tex;
};

//-------------------------------------------------------------------------------------------------

CCarData::CCarData()
  : m_fScale(1000.0f)
{
  p = new CCarDataPrivate;
}

//-------------------------------------------------------------------------------------------------

CCarData::~CCarData()
{
  if (p) {
    delete p;
    p = NULL;
  }
}

//-------------------------------------------------------------------------------------------------

void CCarData::LoadTexture(const std::string &sPal, const std::string &sTex, bool bMangled)
{
  p->m_pal.LoadPalette(sPal);
  p->m_tex.LoadTexture(sTex, p->m_pal, bMangled);
}

//-------------------------------------------------------------------------------------------------

CShapeData *CCarData::MakeCar(CShader *pShader)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVerts(uiNumVerts);
  uint32 uiNumIndices;
  uint32 *indices = MakeIndices(uiNumIndices);

  //for (uint32 i = 0; i < uiNumVerts; ++i) {
  //  vertices[i].flags.x = 1.0f; //use color rather than tex
  //  vertices[i].color = ShapeGenerator::RandomColor();
  //}

  CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts);
  CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
  CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

  CShapeData *pRet = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, &p->m_tex);

  if (vertices)
    delete[] vertices;
  if (indices)
    delete[] indices;

  return pRet;
}

//-------------------------------------------------------------------------------------------------

tVertex *CCarData::MakeVerts(uint32 &numVertices)
{
  //first turn float array into vertex array
  uint32 uiNumCoords = g_xzizinCoordsCount / 3;
  tVertex *coordAy = new tVertex[uiNumCoords];
  int iVertIndex = 0;
  for (int i = 0; i < g_xzizinCoordsCount; ++i) {
    coordAy[i / 3].position[iVertIndex] = g_xzizinCoords[i] / m_fScale;
    iVertIndex++;
    if (iVertIndex == 3)
      iVertIndex = 0;
  }

  //then get vertices for polygons
  numVertices = g_xzizinPolsCount * 4;  
  tVertex *vertices = new tVertex[numVertices];
  for (int i = 0; i < g_xzizinPolsCount; ++i) {
    vertices[i * 4 + 0] = coordAy[g_xzizinPols[i].byVert1];
    vertices[i * 4 + 1] = coordAy[g_xzizinPols[i].byVert2];
    vertices[i * 4 + 2] = coordAy[g_xzizinPols[i].byVert3];
    vertices[i * 4 + 3] = coordAy[g_xzizinPols[i].byVert4];

    uint32 uiUseTex = g_xzizinPols[i].uiTex;
    if (uiUseTex & SURFACE_FLAG_9) {
      uiUseTex = g_xzizinAnms[uiUseTex & SURFACE_TEXTURE_INDEX].framesAy[0];
    }
    p->m_tex.GetTextureCoordinates(uiUseTex,
                                   vertices[i * 4 + 1],
                                   vertices[i * 4 + 0],
                                   vertices[i * 4 + 2],
                                   vertices[i * 4 + 3]);
  }

  delete[] coordAy;
  return vertices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CCarData::MakeIndices(uint32 &numIndices)
{
  int iNumIndicesPerPol = 6;
  int iNumVertsPerPol = 4;
  numIndices = g_xzizinPolsCount * iNumIndicesPerPol;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  int i = 0;
  for (; i < g_xzizinPolsCount; i++) {
    indices[i * iNumIndicesPerPol + 0] = (i * iNumVertsPerPol) + 0;
    indices[i * iNumIndicesPerPol + 1] = (i * iNumVertsPerPol) + 1;
    indices[i * iNumIndicesPerPol + 2] = (i * iNumVertsPerPol) + 2;
    indices[i * iNumIndicesPerPol + 3] = (i * iNumVertsPerPol) + 0;
    indices[i * iNumIndicesPerPol + 4] = (i * iNumVertsPerPol) + 2;
    indices[i * iNumIndicesPerPol + 5] = (i * iNumVertsPerPol) + 3;
  }

  return indices;
}

//-------------------------------------------------------------------------------------------------