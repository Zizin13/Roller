#include <glew.h>
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
#include "ShapeFactory.h"
#include "ShapeData.h"
#include "Shader.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Track.h"
#include "SignType.h"
#include "CarHelpers.h"
//-------------------------------------------------------------------------------------------------
#include "AutoPlans.h"
#include "DesilvaPlans.h"
#include "F1WackPlans.h"
#include "GlobalPlans.h"
#include "MillionPlans.h"
#include "MissionPlans.h"
#include "PulsePlans.h"
#include "ReisePlans.h"
#include "ZizinPlans.h"
#include "Advert2Plans.h"
#include "Advert3Plans.h"
#include "AdvertPlans.h"
#include "Balloon2Plans.h"
#include "BalloonPlans.h"
#include "Bld0Plans.h"
#include "Build1Plans.h"
#include "Build2Plans.h"
#include "Build3Plans.h"
#include "BuildPlans.h"
#include "HeelbarPlans.h"
#include "QuadBldPlans.h"
#include "Sign01Plans.h"
#include "Sign02Plans.h"
#include "Tower2Plans.h"
#include "TowerPlans.h"
#include "TreePlans.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
#if defined (WHIPLIB_DLL)
  #define OGL_DEFAULT false
#else
  #define OGL_DEFAULT true
#endif
//-------------------------------------------------------------------------------------------------

CShapeFactory &CShapeFactory::GetShapeFactory()
{
  static CShapeFactory s_shapeFactory;

  return s_shapeFactory;
}

//-------------------------------------------------------------------------------------------------

CShapeFactory::CShapeFactory()
  : m_bOglRunning(OGL_DEFAULT)
{

}

//-------------------------------------------------------------------------------------------------

CShapeFactory::~CShapeFactory()
{

}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::MakeAxes(CShapeData **pShape, CShader *pShader, CTexture *pTexture)
{
  uint32 uiNumVerts = 4;
  tVertex *vertices = new tVertex[uiNumVerts];
  vertices[0].position = glm::vec3(500, 0, 0);
  vertices[1].position = glm::vec3(0, 500, 0);
  vertices[2].position = glm::vec3(0, 0, 500);
  vertices[3].position = glm::vec3(0, 0, 0);
  vertices[0].texCoords = pTexture->GetColorCenterCoordinates(0xe7); //red
  vertices[1].texCoords = pTexture->GetColorCenterCoordinates(0xff); //green
  vertices[2].texCoords = pTexture->GetColorCenterCoordinates(0xf3); //blue
  vertices[2].texCoords = pTexture->GetColorCenterCoordinates(0x8f); //white
  uint32 uiNumIndices = 6;
  uint32 *indices = new uint32[uiNumIndices];
  memset(indices, 0, uiNumIndices * sizeof(uint32));
  indices[0] = 3;
  indices[1] = 0;
  indices[2] = 3;
  indices[3] = 1;
  indices[4] = 3;
  indices[5] = 2;
  GLenum drawType = GL_LINES;

  if (!*pShape) {
    CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts, GL_DYNAMIC_DRAW);
    CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices, GL_DYNAMIC_DRAW);
    CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

    *pShape = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, pTexture, drawType);
  } else {
    (*pShape)->m_pVertexBuf->Update(vertices, uiNumVerts);
    (*pShape)->m_pIndexBuf->Update(indices, uiNumIndices);
  }

  if (vertices)
    delete[] vertices;
  if (indices)
    delete[] indices;
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::MakeDebugTri(CShapeData **pShape, CShader *pShader, CTexture *pTexture, const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2)
{
  uint32 uiNumVerts = 3;
  struct tVertex *vertices = new tVertex[uiNumVerts];
  vertices[0].position = p0;
  vertices[1].position = p1;
  vertices[2].position = p2;
  vertices[0].texCoords = pTexture->GetColorCenterCoordinates(0xff);
  vertices[1].texCoords = pTexture->GetColorCenterCoordinates(0xff);
  vertices[2].texCoords = pTexture->GetColorCenterCoordinates(0xff);
  uint32 uiNumIndices = 6;
  uint32 *indices = new uint32[uiNumIndices];
  indices[0] = 0;
  indices[1] = 1;
  indices[2] = 1;
  indices[3] = 2;
  indices[4] = 2;
  indices[5] = 0;
  GLenum drawType = GL_LINES;

  if (!*pShape) {
    CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts, GL_DYNAMIC_DRAW);
    CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices, GL_DYNAMIC_DRAW);
    CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

    *pShape = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, pTexture, drawType);
  } else {
    (*pShape)->m_pVertexBuf->Update(vertices, uiNumVerts);
    (*pShape)->m_pIndexBuf->Update(indices, uiNumIndices);
  }

  if (vertices)
    delete[] vertices;
  if (indices)
    delete[] indices;
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::MakeDebugLine(CShapeData **pShape, CShader *pShader, CTexture *pTexture, const glm::vec3 &p0, const glm::vec3 &p1)
{
  uint32 uiNumVerts = 2;
  struct tVertex *vertices = new tVertex[uiNumVerts];
  vertices[0].position = p0;
  vertices[1].position = p1;
  vertices[0].texCoords = pTexture->GetColorCenterCoordinates(0xb7);
  vertices[1].texCoords = pTexture->GetColorCenterCoordinates(0xb7);
  uint32 uiNumIndices = 2;
  uint32 *indices = new uint32[uiNumIndices];
  indices[0] = 0;
  indices[1] = 1;
  GLenum drawType = GL_LINES;

  if (!*pShape) {
    CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts, GL_DYNAMIC_DRAW);
    CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices, GL_DYNAMIC_DRAW);
    CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

    *pShape = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, pTexture, drawType);
  } else {
    (*pShape)->m_pVertexBuf->Update(vertices, uiNumVerts);
    (*pShape)->m_pIndexBuf->Update(indices, uiNumIndices);
  }

  if (vertices)
    delete[] vertices;
  if (indices)
    delete[] indices;
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::MakeModel(CShapeData **pShape, CShader *pShader, CTexture *pTexture, eWhipModel model, int iSignSurfaceType)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeModelVerts(uiNumVerts, pTexture, model, iSignSurfaceType);
  GLenum drawType = GL_TRIANGLES;

  if (vertices) {
    if (!*pShape) {
      uint32 uiNumIndices;
      uint32 *indices = MakeModelIndices(uiNumIndices, model);

      CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts, GL_DYNAMIC_DRAW);
      CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices, GL_DYNAMIC_DRAW);
      CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

      *pShape = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, pTexture, drawType);
      (*pShape)->m_vertices = vertices;
      (*pShape)->m_uiNumVerts = uiNumVerts;
      (*pShape)->m_indices = indices;
      (*pShape)->m_uiNumIndices = uiNumIndices;
    } else {
      (*pShape)->m_pVertexBuf->Update(vertices, uiNumVerts);
      if ((*pShape)->m_vertices)
        delete[](*pShape)->m_vertices;
      (*pShape)->m_vertices = vertices;
      (*pShape)->m_uiNumVerts = uiNumVerts;
    }
  }
}

//-------------------------------------------------------------------------------------------------

tVertex *CShapeFactory::MakeModelVerts(uint32 &numVertices, CTexture *pTexture, eWhipModel model, int iSignSurfaceType)
{
  if (!pTexture)
    return NULL;

  //first turn float array into vertex array
  uint32 uiNumCoords = GetCoordsCount(model) / 3;
  tVertex *coordAy = new tVertex[uiNumCoords];
  int iVertIndex = 0;
  for (int i = 0; i < GetCoordsCount(model); ++i) {
    coordAy[i / 3].position[iVertIndex] = GetCoords(model)[i];
    iVertIndex++;
    if (iVertIndex == 3)
      iVertIndex = 0;
  }

  //then get vertices for polygons
  numVertices = GetPolsCount(model) * 8;
  tVertex *vertices = new tVertex[numVertices];
  for (int i = 0; i < GetPolsCount(model); ++i) {
    vertices[i * 8 + 0] = coordAy[GetPols(model)[i].byVert1];
    vertices[i * 8 + 1] = coordAy[GetPols(model)[i].byVert2];
    vertices[i * 8 + 2] = coordAy[GetPols(model)[i].byVert3];
    vertices[i * 8 + 3] = coordAy[GetPols(model)[i].byVert4];
    vertices[i * 8 + 4] = coordAy[GetPols(model)[i].byVert1];
    vertices[i * 8 + 5] = coordAy[GetPols(model)[i].byVert2];
    vertices[i * 8 + 6] = coordAy[GetPols(model)[i].byVert3];
    vertices[i * 8 + 7] = coordAy[GetPols(model)[i].byVert4];
    MakeNormals(vertices[i * 8 + 1],
                vertices[i * 8 + 0],
                vertices[i * 8 + 2],
                vertices[i * 8 + 3]);
    MakeNormals(vertices[i * 8 + 0 + 4],
                vertices[i * 8 + 1 + 4],
                vertices[i * 8 + 3 + 4],
                vertices[i * 8 + 2 + 4]);

    uint32 uiUseTex = GetPols(model)[i].uiTex;
    if (uiUseTex & SURFACE_FLAG_ANMS_LOOKUP && GetAnms(model)) {
      uiUseTex = GetAnms(model)[uiUseTex & SURFACE_MASK_TEXTURE_INDEX].framesAy[0];
    }
    if (IsSign(model)) {
      if (GetSignTypeFromModel(model).bCanHaveTexture) {
        if (GetPols(model)[i].uiTex &SURFACE_FLAG_ANMS_LOOKUP) {
          uint32 uiSignSurfaceType = CTrack::GetSignedBitValueFromInt(iSignSurfaceType);
          uiUseTex = uiSignSurfaceType;
        } else {
          uiUseTex &= ~SURFACE_FLAG_APPLY_TEXTURE;
        }
      }
    }
    CarHelpers::RemapColor(model, uiUseTex);
    pTexture->GetTextureCoordinates(uiUseTex,
                                    vertices[i * 8 + 1],
                                    vertices[i * 8 + 0],
                                    vertices[i * 8 + 2],
                                    vertices[i * 8 + 3]);
    pTexture->GetTextureCoordinates(uiUseTex,
                                    vertices[i * 8 + 1 + 4],
                                    vertices[i * 8 + 0 + 4],
                                    vertices[i * 8 + 2 + 4],
                                    vertices[i * 8 + 3 + 4]);

    if (GetBacks(model)) {
      uint32 uiBackTex = GetBacks(model)[i];
      if (uiBackTex) {
        pTexture->GetTextureCoordinates(uiBackTex,
                                        vertices[i * 8 + 0 + 4],
                                        vertices[i * 8 + 1 + 4],
                                        vertices[i * 8 + 3 + 4],
                                        vertices[i * 8 + 2 + 4]);
      }
    }
  }

  delete[] coordAy;
  return vertices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeModelIndices(uint32 &numIndices, eWhipModel model)
{
  int iNumIndicesPerPol = 12;
  int iNumVertsPerPol = 8;
  numIndices = GetPolsCount(model) * iNumIndicesPerPol;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  int i = 0;
  for (; i < GetPolsCount(model); i++) {
    indices[i * iNumIndicesPerPol + 0] = (i * iNumVertsPerPol) + 0 + 4;
    indices[i * iNumIndicesPerPol + 1] = (i * iNumVertsPerPol) + 3 + 4;
    indices[i * iNumIndicesPerPol + 2] = (i * iNumVertsPerPol) + 2 + 4;
    indices[i * iNumIndicesPerPol + 3] = (i * iNumVertsPerPol) + 0 + 4;
    indices[i * iNumIndicesPerPol + 4] = (i * iNumVertsPerPol) + 2 + 4;
    indices[i * iNumIndicesPerPol + 5] = (i * iNumVertsPerPol) + 1 + 4;

    indices[i * iNumIndicesPerPol + 6]  = (i * iNumVertsPerPol) + 0;
    indices[i * iNumIndicesPerPol + 7]  = (i * iNumVertsPerPol) + 1;
    indices[i * iNumIndicesPerPol + 8]  = (i * iNumVertsPerPol) + 2;
    indices[i * iNumIndicesPerPol + 9]  = (i * iNumVertsPerPol) + 0;
    indices[i * iNumIndicesPerPol + 10] = (i * iNumVertsPerPol) + 2;
    indices[i * iNumIndicesPerPol + 11] = (i * iNumVertsPerPol) + 3;
  }

  return indices;
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::MakeAudioMarker(CShapeData **pShape, CShader *pShader, CTexture *pTexture)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVertsAudioMarker(uiNumVerts, pTexture);
  uint32 uiNumIndices;
  uint32 *indices = MakeIndicesAudioMarker(uiNumIndices);
  GLenum drawType = GL_TRIANGLES;

  if (vertices && indices) {
    if (!*pShape) {
      CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts, GL_DYNAMIC_DRAW);
      CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices, GL_DYNAMIC_DRAW);
      CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

      *pShape = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, pTexture, drawType);
    } else {
      (*pShape)->m_pVertexBuf->Update(vertices, uiNumVerts);
      (*pShape)->m_pIndexBuf->Update(indices, uiNumIndices);
    }

    delete[] vertices;
    delete[] indices;
  }
}

//-------------------------------------------------------------------------------------------------

tVertex *CShapeFactory::MakeVertsAudioMarker(uint32 &uiNumVerts, CTexture *pTexture)
{
  uiNumVerts = 12;
  tVertex *vertices = new tVertex[uiNumVerts];

  vertices[0].position = glm::vec3(  +0.0f,   +0.0f, +0.0f);
  vertices[1].position = glm::vec3(+500.0f,   +0.0f, +0.0f);
  vertices[2].position = glm::vec3(+500.0f, +500.0f, +0.0f);
  vertices[3].position = glm::vec3(  +0.0f, +500.0f, +0.0f);
  vertices[4].position = glm::vec3(-500.0f, +800.0f, +0.0f);
  vertices[5].position = glm::vec3(-500.0f, -300.0f, +0.0f);
  vertices[0].texCoords = pTexture->GetColorCenterCoordinates(0x8f);
  vertices[1].texCoords = pTexture->GetColorCenterCoordinates(0x8f);
  vertices[2].texCoords = pTexture->GetColorCenterCoordinates(0x8f);
  vertices[3].texCoords = pTexture->GetColorCenterCoordinates(0x8f);
  vertices[4].texCoords = pTexture->GetColorCenterCoordinates(0x8f);
  vertices[5].texCoords = pTexture->GetColorCenterCoordinates(0x8f);

  vertices[0 + 6].position = glm::vec3(+0.0f, +0.0f, +0.0f);
  vertices[1 + 6].position = glm::vec3(+500.0f, +0.0f, +0.0f);
  vertices[2 + 6].position = glm::vec3(+500.0f, +500.0f, +0.0f);
  vertices[3 + 6].position = glm::vec3(+0.0f, +500.0f, +0.0f);
  vertices[4 + 6].position = glm::vec3(-500.0f, +800.0f, +0.0f);
  vertices[5 + 6].position = glm::vec3(-500.0f, -300.0f, +0.0f);
  vertices[0 + 6].texCoords = pTexture->GetColorCenterCoordinates(0x8f);
  vertices[1 + 6].texCoords = pTexture->GetColorCenterCoordinates(0x8f);
  vertices[2 + 6].texCoords = pTexture->GetColorCenterCoordinates(0x8f);
  vertices[3 + 6].texCoords = pTexture->GetColorCenterCoordinates(0x8f);
  vertices[4 + 6].texCoords = pTexture->GetColorCenterCoordinates(0x8f);
  vertices[5 + 6].texCoords = pTexture->GetColorCenterCoordinates(0x8f);

  return vertices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesAudioMarker(uint32 &uiNumIndices)
{
  uiNumIndices = 24;
  uint32 *indices = new uint32[uiNumIndices];
  memset(indices, 0, uiNumIndices * sizeof(uint32));

  indices[0] = 1;
  indices[1] = 0;
  indices[2] = 3;
  indices[3] = 1;
  indices[4] = 3;
  indices[5] = 2;
  indices[6] = 0;
  indices[7] = 5;
  indices[8] = 4;
  indices[9] = 0;
  indices[10] = 4;
  indices[11] = 3;

  indices[0 + 12] = 1;
  indices[1 + 12] = 3;
  indices[2 + 12] = 0;
  indices[3 + 12] = 1;
  indices[4 + 12] = 2;
  indices[5 + 12] = 3;
  indices[6 + 12] = 0;
  indices[7 + 12] = 4;
  indices[8 + 12] = 5;
  indices[9 + 12] = 0;
  indices[10 + 12] = 3;
  indices[11 + 12] = 4;

  return indices;
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::MakeStuntMarker(CShapeData **pShape, CShader *pShader, CTexture *pTexture)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVertsStuntMarker(uiNumVerts, pTexture);
  uint32 uiNumIndices;
  uint32 *indices = MakeIndicesStuntMarker(uiNumIndices);
  GLenum drawType = GL_TRIANGLES;

  if (vertices && indices) {
    if (!*pShape) {
      CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts, GL_DYNAMIC_DRAW);
      CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices, GL_DYNAMIC_DRAW);
      CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

      *pShape = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, pTexture, drawType);
    } else {
      (*pShape)->m_pVertexBuf->Update(vertices, uiNumVerts);
      (*pShape)->m_pIndexBuf->Update(indices, uiNumIndices);
    }

    delete[] vertices;
    delete[] indices;
  }
}

//-------------------------------------------------------------------------------------------------

tVertex *CShapeFactory::MakeVertsStuntMarker(uint32 &uiNumVerts, CTexture *pTexture)
{
  uiNumVerts = 14;
  tVertex *vertices = new tVertex[uiNumVerts];

  glm::vec4 color = glm::vec4(0, 1, 0, 1);
  vertices[0].position = glm::vec3(+0.0f  , +100.0f, +0.0f);
  vertices[1].position = glm::vec3(+100.0f, +0.0f  , +0.0f);
  vertices[2].position = glm::vec3(+400.0f, -300.0f, +0.0f);
  vertices[3].position = glm::vec3(+300.0f, -400.0f, +0.0f);
  vertices[4].position = glm::vec3(+0.0f  , -100.0f, +0.0f);
  vertices[5].position = glm::vec3(-300.0f, -400.0f, +0.0f);
  vertices[6].position = glm::vec3(-400.0f, -300.0f, +0.0f);
  vertices[0].texCoords = pTexture->GetColorCenterCoordinates(0xff);
  vertices[1].texCoords = pTexture->GetColorCenterCoordinates(0xff);
  vertices[2].texCoords = pTexture->GetColorCenterCoordinates(0xff);
  vertices[3].texCoords = pTexture->GetColorCenterCoordinates(0xff);
  vertices[4].texCoords = pTexture->GetColorCenterCoordinates(0xff);
  vertices[5].texCoords = pTexture->GetColorCenterCoordinates(0xff);
  vertices[6].texCoords = pTexture->GetColorCenterCoordinates(0xff);

  vertices[0 + 7].position = glm::vec3(+0.0f, +100.0f, +0.0f);
  vertices[1 + 7].position = glm::vec3(+100.0f, +0.0f, +0.0f);
  vertices[2 + 7].position = glm::vec3(+400.0f, -300.0f, +0.0f);
  vertices[3 + 7].position = glm::vec3(+300.0f, -400.0f, +0.0f);
  vertices[4 + 7].position = glm::vec3(+0.0f, -100.0f, +0.0f);
  vertices[5 + 7].position = glm::vec3(-300.0f, -400.0f, +0.0f);
  vertices[6 + 7].position = glm::vec3(-400.0f, -300.0f, +0.0f);
  vertices[0 + 7].texCoords = pTexture->GetColorCenterCoordinates(0xff);
  vertices[1 + 7].texCoords = pTexture->GetColorCenterCoordinates(0xff);
  vertices[2 + 7].texCoords = pTexture->GetColorCenterCoordinates(0xff);
  vertices[3 + 7].texCoords = pTexture->GetColorCenterCoordinates(0xff);
  vertices[4 + 7].texCoords = pTexture->GetColorCenterCoordinates(0xff);
  vertices[5 + 7].texCoords = pTexture->GetColorCenterCoordinates(0xff);
  vertices[6 + 7].texCoords = pTexture->GetColorCenterCoordinates(0xff);

  return vertices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesStuntMarker(uint32 &uiNumIndices)
{
  uiNumIndices = 24;
  uint32 *indices = new uint32[uiNumIndices];
  memset(indices, 0, uiNumIndices * sizeof(uint32));

  indices[0]  = 4;
  indices[1]  = 3;
  indices[2]  = 2;
  indices[3]  = 4;
  indices[4]  = 2;
  indices[5]  = 1;
  indices[6]  = 5;
  indices[7]  = 1;
  indices[8]  = 0;
  indices[9]  = 6;
  indices[10] = 5;
  indices[11] = 0;

  indices[0 + 12] = 4;
  indices[1 + 12] = 2;
  indices[2 + 12] = 3;
  indices[3 + 12] = 4;
  indices[4 + 12] = 1;
  indices[5 + 12] = 2;
  indices[6 + 12] = 5;
  indices[7 + 12] = 0;
  indices[8 + 12] = 1;
  indices[9 + 12] = 6;
  indices[10 + 12] = 0;
  indices[11 + 12] = 5;

  return indices;
}

//-------------------------------------------------------------------------------------------------

float *CShapeFactory::GetCoords(eWhipModel model)
{
  switch (model) {
    case eWhipModel::CAR_F1WACK:
      return g_f1wackCoords;
    case eWhipModel::CAR_XAUTO:
    case eWhipModel::CAR_YAUTO:
      return g_xautoCoords;
    case eWhipModel::CAR_XDESILVA:
    case eWhipModel::CAR_YDESILVA:
      return g_xdesilvaCoords;
    case eWhipModel::CAR_XPULSE:
    case eWhipModel::CAR_YPULSE:
      return g_xpulseCoords;
    case eWhipModel::CAR_XGLOBAL:
    case eWhipModel::CAR_YGLOBAL:
      return g_xglobalCoords;
    case eWhipModel::CAR_XMILLION:
    case eWhipModel::CAR_YMILLION:
      return g_xmillionCoords;
    case eWhipModel::CAR_XMISSION:
    case eWhipModel::CAR_YMISSION:
      return g_xmissionCoords;
    case eWhipModel::CAR_XZIZIN:
    case eWhipModel::CAR_YZIZIN:
      return g_xzizinCoords;
    case eWhipModel::CAR_XREISE:
    case eWhipModel::CAR_YREISE:
    case eWhipModel::CAR_DEATH:
      return g_xreiseCoords;
    case eWhipModel::SIGN_TOWER:
      return g_towerCoords;
    case eWhipModel::SIGN_TOWER2:
      return g_tower2Coords;
    case eWhipModel::SIGN_SIGN01:
      return g_sign01Coords;
    case eWhipModel::SIGN_SIGN02:
      return g_sign02Coords;
    case eWhipModel::SIGN_BUILD:
      return g_buildCoords;
    case eWhipModel::SIGN_BUILD1:
      return g_build1Coords;
    case eWhipModel::SIGN_BUILD2:
      return g_build2Coords;
    case eWhipModel::SIGN_BUILD3:
      return g_build3Coords;
    case eWhipModel::SIGN_HEELBAR:
      return g_heelbarCoords;
    case eWhipModel::SIGN_BALLOON:
      return g_balloonCoords;
    case eWhipModel::SIGN_BALLOON2:
      return g_balloon2Coords;
    case eWhipModel::SIGN_TREE:
      return g_treeCoords;
    case eWhipModel::SIGN_ADVERT:
      return g_advertCoords;
    case eWhipModel::SIGN_ADVERT2:
      return g_advert2Coords;
    case eWhipModel::SIGN_ADVERT3:
      return g_advert3Coords;
    case eWhipModel::SIGN_QUADBLD:
      return g_quadbldCoords;
    case eWhipModel::SIGN_BLD0:
      return g_bld0Coords;
    default:
      assert(0);
      return NULL;
  }
}

//-------------------------------------------------------------------------------------------------

int CShapeFactory::GetCoordsCount(eWhipModel model)
{
  switch (model) {
    case eWhipModel::CAR_F1WACK:
      return g_f1wackCoordsCount;
    case eWhipModel::CAR_XAUTO:
    case eWhipModel::CAR_YAUTO:
      return g_xautoCoordsCount;
    case eWhipModel::CAR_XDESILVA:
    case eWhipModel::CAR_YDESILVA:
      return g_xdesilvaCoordsCount;
    case eWhipModel::CAR_XPULSE:
    case eWhipModel::CAR_YPULSE:
      return g_xpulseCoordsCount;
    case eWhipModel::CAR_XGLOBAL:
    case eWhipModel::CAR_YGLOBAL:
      return g_xglobalCoordsCount;
    case eWhipModel::CAR_XMILLION:
    case eWhipModel::CAR_YMILLION:
      return g_xmillionCoordsCount;
    case eWhipModel::CAR_XMISSION:
    case eWhipModel::CAR_YMISSION:
      return g_xmissionCoordsCount;
    case eWhipModel::CAR_XZIZIN:
    case eWhipModel::CAR_YZIZIN:
      return g_xzizinCoordsCount;
    case eWhipModel::CAR_XREISE:
    case eWhipModel::CAR_YREISE:
    case eWhipModel::CAR_DEATH:
      return g_xreiseCoordsCount;
    case eWhipModel::SIGN_TOWER:
      return g_towerCoordsCount;
    case eWhipModel::SIGN_TOWER2:
      return g_tower2CoordsCount;
    case eWhipModel::SIGN_SIGN01:
      return g_sign01CoordsCount;
    case eWhipModel::SIGN_SIGN02:
      return g_sign02CoordsCount;
    case eWhipModel::SIGN_BUILD:
      return g_buildCoordsCount;
    case eWhipModel::SIGN_BUILD1:
      return g_build1CoordsCount;
    case eWhipModel::SIGN_BUILD2:
      return g_build2CoordsCount;
    case eWhipModel::SIGN_BUILD3:
      return g_build3CoordsCount;
    case eWhipModel::SIGN_HEELBAR:
      return g_heelbarCoordsCount;
    case eWhipModel::SIGN_BALLOON:
      return g_balloonCoordsCount;
    case eWhipModel::SIGN_BALLOON2:
      return g_balloon2CoordsCount;
    case eWhipModel::SIGN_TREE:
      return g_treeCoordsCount;
    case eWhipModel::SIGN_ADVERT:
      return g_advertCoordsCount;
    case eWhipModel::SIGN_ADVERT2:
      return g_advert2CoordsCount;
    case eWhipModel::SIGN_ADVERT3:
      return g_advert3CoordsCount;
    case eWhipModel::SIGN_QUADBLD:
      return g_quadbldCoordsCount;
    case eWhipModel::SIGN_BLD0:
      return g_bld0CoordsCount;
    default:
      assert(0);
      return NULL;
  }
}

//-------------------------------------------------------------------------------------------------

tPolygon *CShapeFactory::GetPols(eWhipModel model)
{
  switch (model) {
    case eWhipModel::CAR_F1WACK:
      return g_f1wackPols;
    case eWhipModel::CAR_XAUTO:
    case eWhipModel::CAR_YAUTO:
      return g_xautoPols;
    case eWhipModel::CAR_XDESILVA:
    case eWhipModel::CAR_YDESILVA:
      return g_xdesilvaPols;
    case eWhipModel::CAR_XPULSE:
    case eWhipModel::CAR_YPULSE:
      return g_xpulsePols;
    case eWhipModel::CAR_XGLOBAL:
    case eWhipModel::CAR_YGLOBAL:
      return g_xglobalPols;
    case eWhipModel::CAR_XMILLION:
    case eWhipModel::CAR_YMILLION:
      return g_xmillionPols;
    case eWhipModel::CAR_XMISSION:
    case eWhipModel::CAR_YMISSION:
      return g_xmissionPols;
    case eWhipModel::CAR_XZIZIN:
    case eWhipModel::CAR_YZIZIN:
      return g_xzizinPols;
    case eWhipModel::CAR_XREISE:
    case eWhipModel::CAR_YREISE:
    case eWhipModel::CAR_DEATH:
      return g_xreisePols;
    case eWhipModel::SIGN_TOWER:
      return g_towerPols;
    case eWhipModel::SIGN_TOWER2:
      return g_tower2Pols;
    case eWhipModel::SIGN_SIGN01:
      return g_sign01Pols;
    case eWhipModel::SIGN_SIGN02:
      return g_sign02Pols;
    case eWhipModel::SIGN_BUILD:
      return g_buildPols;
    case eWhipModel::SIGN_BUILD1:
      return g_build1Pols;
    case eWhipModel::SIGN_BUILD2:
      return g_build2Pols;
    case eWhipModel::SIGN_BUILD3:
      return g_build3Pols;
    case eWhipModel::SIGN_HEELBAR:
      return g_heelbarPols;
    case eWhipModel::SIGN_BALLOON:
      return g_balloonPols;
    case eWhipModel::SIGN_BALLOON2:
      return g_balloon2Pols;
    case eWhipModel::SIGN_TREE:
      return g_treePols;
    case eWhipModel::SIGN_ADVERT:
      return g_advertPols;
    case eWhipModel::SIGN_ADVERT2:
      return g_advert2Pols;
    case eWhipModel::SIGN_ADVERT3:
      return g_advert3Pols;
    case eWhipModel::SIGN_QUADBLD:
      return g_quadbldPols;
    case eWhipModel::SIGN_BLD0:
      return g_bld0Pols;
    default:
      assert(0);
      return NULL;
  }
}

//-------------------------------------------------------------------------------------------------

int CShapeFactory::GetPolsCount(eWhipModel model)
{
  switch (model) {
    case eWhipModel::CAR_F1WACK:
      return g_f1wackPolsCount;
    case eWhipModel::CAR_XAUTO:
    case eWhipModel::CAR_YAUTO:
      return g_xautoPolsCount;
    case eWhipModel::CAR_XDESILVA:
    case eWhipModel::CAR_YDESILVA:
      return g_xdesilvaPolsCount;
    case eWhipModel::CAR_XPULSE:
    case eWhipModel::CAR_YPULSE:
      return g_xpulsePolsCount;
    case eWhipModel::CAR_XGLOBAL:
    case eWhipModel::CAR_YGLOBAL:
      return g_xglobalPolsCount;
    case eWhipModel::CAR_XMILLION:
    case eWhipModel::CAR_YMILLION:
      return g_xmillionPolsCount;
    case eWhipModel::CAR_XMISSION:
    case eWhipModel::CAR_YMISSION:
      return g_xmissionPolsCount;
    case eWhipModel::CAR_XZIZIN:
    case eWhipModel::CAR_YZIZIN:
      return g_xzizinPolsCount;
    case eWhipModel::CAR_XREISE:
    case eWhipModel::CAR_YREISE:
    case eWhipModel::CAR_DEATH:
      return g_xreisePolsCount;
    case eWhipModel::SIGN_TOWER:
      return g_towerPolsCount;
    case eWhipModel::SIGN_TOWER2:
      return g_tower2PolsCount;
    case eWhipModel::SIGN_SIGN01:
      return g_sign01PolsCount;
    case eWhipModel::SIGN_SIGN02:
      return g_sign02PolsCount;
    case eWhipModel::SIGN_BUILD:
      return g_buildPolsCount;
    case eWhipModel::SIGN_BUILD1:
      return g_build1PolsCount;
    case eWhipModel::SIGN_BUILD2:
      return g_build2PolsCount;
    case eWhipModel::SIGN_BUILD3:
      return g_build3PolsCount;
    case eWhipModel::SIGN_HEELBAR:
      return g_heelbarPolsCount;
    case eWhipModel::SIGN_BALLOON:
      return g_balloonPolsCount;
    case eWhipModel::SIGN_BALLOON2:
      return g_balloon2PolsCount;
    case eWhipModel::SIGN_TREE:
      return g_treePolsCount;
    case eWhipModel::SIGN_ADVERT:
      return g_advertPolsCount;
    case eWhipModel::SIGN_ADVERT2:
      return g_advert2PolsCount;
    case eWhipModel::SIGN_ADVERT3:
      return g_advert3PolsCount;
    case eWhipModel::SIGN_QUADBLD:
      return g_quadbldPolsCount;
    case eWhipModel::SIGN_BLD0:
      return g_bld0PolsCount;
    default:
      assert(0);
      return NULL;
  }
}

//-------------------------------------------------------------------------------------------------

tAnimation *CShapeFactory::GetAnms(eWhipModel model)
{
  switch (model) {
    case eWhipModel::CAR_F1WACK:
      return NULL;
    case eWhipModel::CAR_XAUTO:
    case eWhipModel::CAR_YAUTO:
      return g_xautoAnms;
    case eWhipModel::CAR_XDESILVA:
    case eWhipModel::CAR_YDESILVA:
      return g_xdesilvaAnms;
    case eWhipModel::CAR_XPULSE:
    case eWhipModel::CAR_YPULSE:
      return g_xpulseAnms;
    case eWhipModel::CAR_XGLOBAL:
    case eWhipModel::CAR_YGLOBAL:
      return g_xglobalAnms;
    case eWhipModel::CAR_XMILLION:
    case eWhipModel::CAR_YMILLION:
      return g_xmillionAnms;
    case eWhipModel::CAR_XMISSION:
    case eWhipModel::CAR_YMISSION:
      return g_xmissionAnms;
    case eWhipModel::CAR_XZIZIN:
    case eWhipModel::CAR_YZIZIN:
      return g_xzizinAnms;
    case eWhipModel::CAR_XREISE:
    case eWhipModel::CAR_YREISE:
    case eWhipModel::CAR_DEATH:
      return g_xreiseAnms;
    case eWhipModel::SIGN_TOWER:
      return g_towerAnms;
    case eWhipModel::SIGN_TOWER2:
      return NULL;
    case eWhipModel::SIGN_SIGN01:
      return g_sign01Anms;
    case eWhipModel::SIGN_SIGN02:
      return g_sign02Anms;
    case eWhipModel::SIGN_BUILD:
      return NULL;
    case eWhipModel::SIGN_BUILD1:
      return NULL;
    case eWhipModel::SIGN_BUILD2:
      return NULL;
    case eWhipModel::SIGN_BUILD3:
      return g_build3Anms;
    case eWhipModel::SIGN_HEELBAR:
      return NULL;
    case eWhipModel::SIGN_BALLOON:
      return NULL;
    case eWhipModel::SIGN_BALLOON2:
      return g_balloon2Anms;
    case eWhipModel::SIGN_TREE:
      return NULL;
    case eWhipModel::SIGN_ADVERT:
      return g_advertAnms;
    case eWhipModel::SIGN_ADVERT2:
      return g_advert2Anms;
    case eWhipModel::SIGN_ADVERT3:
      return g_advert3Anms;
    case eWhipModel::SIGN_QUADBLD:
      return NULL;
    case eWhipModel::SIGN_BLD0:
      return NULL;
    default:
      assert(0);
      return NULL;
  }
}

//-------------------------------------------------------------------------------------------------

int CShapeFactory::GetAnmsCount(eWhipModel model)
{
  switch (model) {
    case eWhipModel::CAR_F1WACK:
      return 0;
    case eWhipModel::CAR_XAUTO:
    case eWhipModel::CAR_YAUTO:
      return g_xautoAnmsCount;
    case eWhipModel::CAR_XDESILVA:
    case eWhipModel::CAR_YDESILVA:
      return g_xdesilvaAnmsCount;
    case eWhipModel::CAR_XPULSE:
    case eWhipModel::CAR_YPULSE:
      return g_xpulseAnmsCount;
    case eWhipModel::CAR_XGLOBAL:
    case eWhipModel::CAR_YGLOBAL:
      return g_xglobalAnmsCount;
    case eWhipModel::CAR_XMILLION:
    case eWhipModel::CAR_YMILLION:
      return g_xmillionAnmsCount;
    case eWhipModel::CAR_XMISSION:
    case eWhipModel::CAR_YMISSION:
      return g_xmissionAnmsCount;
    case eWhipModel::CAR_XZIZIN:
    case eWhipModel::CAR_YZIZIN:
      return g_xzizinAnmsCount;
    case eWhipModel::CAR_XREISE:
    case eWhipModel::CAR_YREISE:
    case eWhipModel::CAR_DEATH:
      return g_xreiseAnmsCount;
    case eWhipModel::SIGN_TOWER:
      return g_towerAnmsCount;
    case eWhipModel::SIGN_TOWER2:
      return 0;
    case eWhipModel::SIGN_SIGN01:
      return g_sign01AnmsCount;
    case eWhipModel::SIGN_SIGN02:
      return g_sign02AnmsCount;
    case eWhipModel::SIGN_BUILD:
      return 0;
    case eWhipModel::SIGN_BUILD1:
      return 0;
    case eWhipModel::SIGN_BUILD2:
      return 0;
    case eWhipModel::SIGN_BUILD3:
      return g_build3AnmsCount;
    case eWhipModel::SIGN_HEELBAR:
      return 0;
    case eWhipModel::SIGN_BALLOON:
      return 0;
    case eWhipModel::SIGN_BALLOON2:
      return g_balloon2AnmsCount;
    case eWhipModel::SIGN_TREE:
      return 0;
    case eWhipModel::SIGN_ADVERT:
      return g_advertAnmsCount;
    case eWhipModel::SIGN_ADVERT2:
      return g_advert2AnmsCount;
    case eWhipModel::SIGN_ADVERT3:
      return g_advert3AnmsCount;
    case eWhipModel::SIGN_QUADBLD:
      return 0;
    case eWhipModel::SIGN_BLD0:
      return 0;
    default:
      assert(0);
      return NULL;
  }
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::GetBacks(eWhipModel model)
{
  switch (model) {
    case eWhipModel::CAR_F1WACK:
      return g_f1wackBacks;
    case eWhipModel::CAR_XAUTO:
    case eWhipModel::CAR_YAUTO:
      return g_xautoBacks;
    case eWhipModel::CAR_XDESILVA:
    case eWhipModel::CAR_YDESILVA:
      return g_xdesilvaBacks;
    case eWhipModel::CAR_XPULSE:
    case eWhipModel::CAR_YPULSE:
      return g_xpulseBacks;
    case eWhipModel::CAR_XGLOBAL:
    case eWhipModel::CAR_YGLOBAL:
      return g_xglobalBacks;
    case eWhipModel::CAR_XMILLION:
    case eWhipModel::CAR_YMILLION:
      return g_xmillionBacks;
    case eWhipModel::CAR_XMISSION:
    case eWhipModel::CAR_YMISSION:
      return g_xmissionBacks;
    case eWhipModel::CAR_XZIZIN:
    case eWhipModel::CAR_YZIZIN:
      return g_xzizinBacks;
    case eWhipModel::CAR_XREISE:
    case eWhipModel::CAR_YREISE:
    case eWhipModel::CAR_DEATH:
      return g_xreiseBacks;
    case eWhipModel::SIGN_TOWER:
      return g_towerBacks;
    case eWhipModel::SIGN_TOWER2:
      return g_tower2Backs;
    case eWhipModel::SIGN_SIGN01:
      return g_sign01Backs;
    case eWhipModel::SIGN_SIGN02:
      return g_sign02Backs;
    case eWhipModel::SIGN_BUILD:
      return g_buildBacks;
    case eWhipModel::SIGN_BUILD1:
      return g_build1Backs;
    case eWhipModel::SIGN_BUILD2:
      return g_build2Backs;
    case eWhipModel::SIGN_BUILD3:
      return g_build3Backs;
    case eWhipModel::SIGN_HEELBAR:
      return g_heelbarBacks;
    case eWhipModel::SIGN_BALLOON:
      return NULL;
    case eWhipModel::SIGN_BALLOON2:
      return g_balloon2Backs;
    case eWhipModel::SIGN_TREE:
      return g_treeBacks;
    case eWhipModel::SIGN_ADVERT:
      return g_advertBacks;
    case eWhipModel::SIGN_ADVERT2:
      return g_advert2Backs;
    case eWhipModel::SIGN_ADVERT3:
      return g_advert3Backs;
    case eWhipModel::SIGN_QUADBLD:
      return g_quadbldBacks;
    case eWhipModel::SIGN_BLD0:
      return g_bld0Backs;
    default:
      assert(0);
      return NULL;
  }
}

//-------------------------------------------------------------------------------------------------

int CShapeFactory::GetBacksCount(eWhipModel model)
{
  switch (model) {
    case eWhipModel::CAR_F1WACK:
      return g_f1wackBacksCount;
    case eWhipModel::CAR_XAUTO:
    case eWhipModel::CAR_YAUTO:
      return g_xautoBacksCount;
    case eWhipModel::CAR_XDESILVA:
    case eWhipModel::CAR_YDESILVA:
      return g_xdesilvaBacksCount;
    case eWhipModel::CAR_XPULSE:
    case eWhipModel::CAR_YPULSE:
      return g_xpulseBacksCount;
    case eWhipModel::CAR_XGLOBAL:
    case eWhipModel::CAR_YGLOBAL:
      return g_xglobalBacksCount;
    case eWhipModel::CAR_XMILLION:
    case eWhipModel::CAR_YMILLION:
      return g_xmillionBacksCount;
    case eWhipModel::CAR_XMISSION:
    case eWhipModel::CAR_YMISSION:
      return g_xmissionBacksCount;
    case eWhipModel::CAR_XZIZIN:
    case eWhipModel::CAR_YZIZIN:
      return g_xzizinBacksCount;
    case eWhipModel::CAR_XREISE:
    case eWhipModel::CAR_YREISE:
    case eWhipModel::CAR_DEATH:
      return g_xreiseBacksCount;
    case eWhipModel::SIGN_TOWER:
      return g_towerBacksCount;
    case eWhipModel::SIGN_TOWER2:
      return g_tower2BacksCount;
    case eWhipModel::SIGN_SIGN01:
      return g_sign01BacksCount;
    case eWhipModel::SIGN_SIGN02:
      return g_sign02BacksCount;
    case eWhipModel::SIGN_BUILD:
      return g_buildBacksCount;
    case eWhipModel::SIGN_BUILD1:
      return g_build1BacksCount;
    case eWhipModel::SIGN_BUILD2:
      return g_build2BacksCount;
    case eWhipModel::SIGN_BUILD3:
      return g_build3BacksCount;
    case eWhipModel::SIGN_HEELBAR:
      return g_heelbarBacksCount;
    case eWhipModel::SIGN_BALLOON:
      return 0;
    case eWhipModel::SIGN_BALLOON2:
      return g_balloon2BacksCount;
    case eWhipModel::SIGN_TREE:
      return g_treeBacksCount;
    case eWhipModel::SIGN_ADVERT:
      return g_advertBacksCount;
    case eWhipModel::SIGN_ADVERT2:
      return g_advert2BacksCount;
    case eWhipModel::SIGN_ADVERT3:
      return g_advert3BacksCount;
    case eWhipModel::SIGN_QUADBLD:
      return g_quadbldBacksCount;
    case eWhipModel::SIGN_BLD0:
      return g_bld0BacksCount;
    default:
      assert(0);
      return NULL;
  }
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesCenterline(uint32 &numIndices, CTrack *pTrack, bool bAttachLast)
{
  if (pTrack->m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  numIndices = (uint32)pTrack->m_chunkAy.size() * 2 - (bAttachLast ? 0 : 2);
  uint32 *indices = new uint32[numIndices];

  for (uint32 i = 0; i < numIndices; i++) {
    indices[i] = (i + 1) / 2;
  }
  if (bAttachLast)
    indices[numIndices - 1] = 0;

  return indices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesSurface(uint32 &numIndices, CTrack *pTrack, bool bAttachLast)
{
  if (pTrack->m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 5;
  uint32 uiNumIndicesPerChunk = 24;
  numIndices = (uint32)pTrack->m_chunkAy.size() * uiNumIndicesPerChunk;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  uint32 i = 0;
  for (; i < pTrack->m_chunkAy.size() - 1; i++) {
    indices[i * uiNumIndicesPerChunk + 0] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 1] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 2] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 3] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 4] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 5] = (i * uiNumVertsPerChunk) + 8;
    indices[i * uiNumIndicesPerChunk + 6] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 7] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 8] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 9] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 10] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 11] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 12] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 13] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 14] = (i * uiNumVertsPerChunk) + 7;
    indices[i * uiNumIndicesPerChunk + 15] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 16] = (i * uiNumVertsPerChunk) + 7;
    indices[i * uiNumIndicesPerChunk + 17] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 18] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 19] = (i * uiNumVertsPerChunk) + 4;
    indices[i * uiNumIndicesPerChunk + 20] = (i * uiNumVertsPerChunk) + 9;
    indices[i * uiNumIndicesPerChunk + 21] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 22] = (i * uiNumVertsPerChunk) + 9;
    indices[i * uiNumIndicesPerChunk + 23] = (i * uiNumVertsPerChunk) + 7;
  }
  //final chunk must be tied to first
  if (bAttachLast) {
    indices[i * uiNumIndicesPerChunk + 0] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 1] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 2] = 1;
    indices[i * uiNumIndicesPerChunk + 3] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 4] = 1;
    indices[i * uiNumIndicesPerChunk + 5] = 3;
    indices[i * uiNumIndicesPerChunk + 6] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 7] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 8] = 0;
    indices[i * uiNumIndicesPerChunk + 9] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 10] = 0;
    indices[i * uiNumIndicesPerChunk + 11] = 1;
    indices[i * uiNumIndicesPerChunk + 12] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 13] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 14] = 2;
    indices[i * uiNumIndicesPerChunk + 15] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 16] = 2;
    indices[i * uiNumIndicesPerChunk + 17] = 0;
    indices[i * uiNumIndicesPerChunk + 18] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 19] = (i * uiNumVertsPerChunk) + 4;
    indices[i * uiNumIndicesPerChunk + 20] = 4;
    indices[i * uiNumIndicesPerChunk + 21] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 22] = 4;
    indices[i * uiNumIndicesPerChunk + 23] = 2;
  }

  return indices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesSurfaceWireframe(uint32 &numIndices, CTrack *pTrack, bool bAttachLast)
{
  if (pTrack->m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 5;
  uint32 uiNumIndicesPerChunk = 18;
  numIndices = (uint32)pTrack->m_chunkAy.size() * uiNumIndicesPerChunk;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  uint32 i = 0;
  for (; i < pTrack->m_chunkAy.size() - 1; i++) {
    indices[i * uiNumIndicesPerChunk + 0] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 1] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 2] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 3] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 4] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 5] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 6] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 7] = (i * uiNumVertsPerChunk) + 4;

    indices[i * uiNumIndicesPerChunk + 8] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 9] = (i * uiNumVertsPerChunk) + 8;
    indices[i * uiNumIndicesPerChunk + 10] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 11] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 12] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 13] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 14] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 15] = (i * uiNumVertsPerChunk) + 7;
    indices[i * uiNumIndicesPerChunk + 16] = (i * uiNumVertsPerChunk) + 4;
    indices[i * uiNumIndicesPerChunk + 17] = (i * uiNumVertsPerChunk) + 9;
  }
  //final chunk must be tied to first
  if (bAttachLast) {
    indices[i * uiNumIndicesPerChunk + 8] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 9] = 3;
    indices[i * uiNumIndicesPerChunk + 10] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 11] = 1;
    indices[i * uiNumIndicesPerChunk + 12] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 13] = 0;
    indices[i * uiNumIndicesPerChunk + 14] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 15] = 2;
    indices[i * uiNumIndicesPerChunk + 16] = (i * uiNumVertsPerChunk) + 4;
    indices[i * uiNumIndicesPerChunk + 17] = 4;
  }

  return indices;
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::MakeTrackSurface(CShapeData **pShape, CShader *pShader, CTrack *pTrack, eShapeSection section, bool bAttachLast, bool bWireframe)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = NULL;
  vertices = MakeVerts(uiNumVerts, section, pTrack, pTrack->m_pTex);
  if (!vertices)
    return;

  if (bWireframe) {
    for (uint32 i = 0; i < uiNumVerts; ++i) {
      vertices[i].texCoords = pTrack->m_pTex->GetColorCenterCoordinates(0xCF);
    }
  }

  if (!(*pShape)) {
    uint32 uiNumIndices;
    uint32 *indices = NULL;
    GLenum drawType = GL_TRIANGLES;
    if (!bWireframe) {
      if (section == eShapeSection::EXPORT)
        indices = MakeIndicesExport(uiNumIndices, pTrack);
      else
        indices = MakeIndicesSingleSection(uiNumIndices, section, pTrack, bAttachLast);
    } else {
      indices = MakeIndicesSingleSectionWireframe(uiNumIndices, section, pTrack, bAttachLast);
      drawType = GL_LINES;
    }

    CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts, GL_DYNAMIC_DRAW);
    CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
    CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

    (*pShape) = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, pTrack->m_pTex, drawType);
    (*pShape)->m_vertices = vertices;
    (*pShape)->m_uiNumVerts = uiNumVerts;
    (*pShape)->m_indices = indices;
    (*pShape)->m_uiNumIndices = uiNumIndices;

  } else {
    (*pShape)->m_pVertexBuf->Update(vertices, uiNumVerts);
    if ((*pShape)->m_vertices)
      delete[](*pShape)->m_vertices;
    (*pShape)->m_vertices = vertices;
    (*pShape)->m_uiNumVerts = uiNumVerts;
  }
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::MakeEnvirFloor(CShapeData **pShape, CShader *pShader, CTrack *pTrack, int iIndex)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = NULL;
  vertices = MakeVertsEnvirFloor(uiNumVerts, pTrack, pTrack->m_pTex, iIndex);
  if (!vertices)
    return;

  if (!(*pShape)) {
    uint32 uiNumIndices;
    uint32 *indices = NULL;
    GLenum drawType = GL_TRIANGLES;
    indices = MakeIndicesEnvirFloor(uiNumIndices);

    CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts, GL_DYNAMIC_DRAW);
    CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
    CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);
    (*pShape) = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, pTrack->m_pTex, drawType);

    if (indices)
      delete[] indices;
  } else {
    (*pShape)->m_pVertexBuf->Update(vertices, uiNumVerts);
  }

  if (vertices)
    delete[] vertices;
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::MakeAILine(CShapeData **pShape, CShader *pShader, CTrack *pTrack, eShapeSection section, bool bAttachLast)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVerts(uiNumVerts, section, pTrack, pTrack->m_pTex);
  if (!vertices)
    return;

  for (uint32 i = 0; i < uiNumVerts; ++i) {
    switch (section) {
      case eShapeSection::AILINE1:
        vertices[i].texCoords = pTrack->m_pTex->GetColorCenterCoordinates(0xE6);
        break;
      case eShapeSection::AILINE2:
        vertices[i].texCoords = pTrack->m_pTex->GetColorCenterCoordinates(0xFF);
        break;
      case eShapeSection::AILINE3:
        vertices[i].texCoords = pTrack->m_pTex->GetColorCenterCoordinates(0xF3);
        break;
      case eShapeSection::AILINE4:
        vertices[i].texCoords = pTrack->m_pTex->GetColorCenterCoordinates(0xCF);
        break;
    }
  }

  if (!(*pShape)) {
    uint32 uiNumIndices;
    uint32 *indices = MakeIndicesCenterline(uiNumIndices, pTrack, bAttachLast);
    GLenum drawType = GL_LINES;

    CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts, GL_DYNAMIC_DRAW);
    CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
    CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

    (*pShape) = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, pTrack->m_pTex, drawType);

    if (indices)
      delete[] indices;
  } else {
    (*pShape)->m_pVertexBuf->Update(vertices, uiNumVerts);
  }

  if (vertices)
    delete[] vertices;
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::MakeSelectedChunks(CShapeData **pShape, CShader *pShader, CTrack *pTrack, int iStart, int iEnd)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVerts(uiNumVerts, eShapeSection::SELECTED, pTrack, pTrack->m_pTex);
  if (!vertices)
    return;

  for (uint32 i = 0; i < uiNumVerts; ++i) {
    vertices[i].texCoords = pTrack->m_pTex->GetColorCenterCoordinates(0xDA);
  }

  uint32 uiNumIndices;
  uint32 *indices = MakeIndicesSelectedChunks(uiNumIndices, iStart, iEnd, pTrack);
  GLenum drawType = GL_LINES;

  if (!(*pShape)) {
    CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts, GL_DYNAMIC_DRAW);
    CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices, GL_DYNAMIC_DRAW);
    CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

    (*pShape) = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, pTrack->m_pTex, drawType);
  } else {
    (*pShape)->m_pVertexBuf->Update(vertices, uiNumVerts);
    (*pShape)->m_pIndexBuf->Update(indices, uiNumIndices);
  }

  if (vertices)
    delete[] vertices;
  if (indices)
    delete[] indices;
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::MakeSigns(CShader *pShader, CTrack *pTrack, std::vector<CShapeData *> &signAy)
{
  for (int i = 0; i < (int)pTrack->m_chunkAy.size(); ++i) {
    if (pTrack->m_chunkAy[i].iSignType < 0 || pTrack->m_chunkAy[i].iSignType >= g_signAyCount)
      continue; //no signs in this chunk

    //make sign
    CShapeData *pNewSign = NULL;
    MakeModel(&pNewSign, pShader, pTrack->m_pBld, g_signAy[pTrack->m_chunkAy[i].iSignType].modelType, pTrack->m_chunkAy[i].iSignTexture);
    if (!pNewSign)
      continue;

    uint32 uiSignSurfaceType = CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[i].iSignTexture);

    //position sign
    bool bReversedOffsets = pTrack->m_chunkAy[i].dPitch >= 90.0f && pTrack->m_chunkAy[i].dPitch < 270.0f;
    float fLen = (float)pTrack->m_chunkAy[i].iSignHorizOffset * 15.0f * (bReversedOffsets ? 1.0f : -1.0f);
    float fHeight = (float)pTrack->m_chunkAy[i].iSignVertOffset * 15.0f * (bReversedOffsets ? 1.0f : -1.0f);

    glm::vec3 center; //sign origin is midpoint of centerline
    if (i + 1 < pTrack->m_chunkAy.size())
      center = (pTrack->m_chunkAy[i + 1].math.center - pTrack->m_chunkAy[i].math.center) * 0.5f;
    else
      center = (pTrack->m_chunkAy[i].math.center - pTrack->m_chunkAy[0].math.center) * 0.5f;
    glm::mat4 translateMat = glm::translate(pTrack->m_chunkAy[i].math.center + center);

    glm::mat4 scaleMatWidth = glm::scale(glm::vec3(fLen, fLen, fLen));
    glm::mat4 scaleMatHeight = glm::scale(glm::vec3(fHeight, fHeight, fHeight));
    glm::vec3 widthVec = glm::vec3(scaleMatWidth * glm::vec4(pTrack->m_chunkAy[i].math.pitchAxis, 1.0f));
    glm::vec3 normal = glm::normalize(glm::cross(pTrack->m_chunkAy[i].math.nextChunkPitched, pTrack->m_chunkAy[i].math.pitchAxis));
    glm::vec3 heightVec = glm::vec3(scaleMatHeight * glm::vec4(normal, 1.0f));
    glm::vec3 signPos = widthVec + heightVec;
    glm::vec3 signPosTranslated = glm::vec3(translateMat * glm::vec4(signPos, 1.0f));

    bool bBillboarded = g_signAy[pTrack->m_chunkAy[i].iSignType].bBillboarded;

    glm::mat4 signYawMat = glm::rotate(glm::radians((float)pTrack->m_chunkAy[i].dSignYaw * -1.0f), normal);// glm::vec3(0, 1, 0));
    glm::mat4 signPitchMat = glm::rotate(glm::radians((float)pTrack->m_chunkAy[i].dSignPitch * -1.0f), pTrack->m_chunkAy[i].math.pitchAxis); //glm::vec3(1, 0, 0));
    glm::mat4 signRollMat = glm::rotate(glm::radians((float)pTrack->m_chunkAy[i].dSignRoll * -1.0f), glm::normalize(pTrack->m_chunkAy[i].math.nextChunkPitched));// glm::vec3(0, 0, 1));
    pNewSign->m_modelToWorldMatrix = glm::translate(signPosTranslated) * 
      signRollMat * signPitchMat * (bBillboarded ? pTrack->m_chunkAy[i].math.yawMat : signYawMat) *
      glm::rotate(glm::radians(-90.0f), glm::vec3(0, 0, 1)) * //sign starts on its side
      glm::rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0)); //track starts facing z positive, sign starts facing x positive
    
    //add sign to array
    signAy.push_back(pNewSign);
  }
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::MakeAudio(CShader *pShader, CTrack *pTrack, std::vector<CShapeData *> &audioAy)
{
  for (int i = 0; i < (int)pTrack->m_chunkAy.size(); ++i) {
    bool bChunkHasAudio = pTrack->m_chunkAy[i].iAudioTriggerSpeed != 0;
    if (!bChunkHasAudio)
      continue; //no audio in this chunk

    //make marker
    CShapeData *pNewMarker = NULL;
    MakeAudioMarker(&pNewMarker, pShader, pTrack->m_pTex);

    float fHeight = (float)1000.0f * -1.0f;
    glm::mat4 translateMat = glm::translate(pTrack->m_chunkAy[i].math.centerStunt);
    glm::mat4 scaleMatHeight = glm::scale(glm::vec3(fHeight, fHeight, fHeight));
    glm::vec3 normal = glm::normalize(glm::cross(pTrack->m_chunkAy[i].math.nextChunkPitched, pTrack->m_chunkAy[i].math.pitchAxis));
    glm::vec3 heightVec = glm::vec3(scaleMatHeight * pTrack->m_chunkAy[i].math.rollMat * glm::vec4(normal, 1.0f));
    glm::vec3 markerPos = heightVec;
    glm::vec3 markerPosTranslated = glm::vec3(translateMat * glm::vec4(markerPos, 1.0f));

    pNewMarker->m_modelToWorldMatrix = glm::translate(markerPosTranslated) *
      pTrack->m_chunkAy[i].math.rollMat * pTrack->m_chunkAy[i].math.pitchMat * pTrack->m_chunkAy[i].math.yawMat;

    //add sign to array
    audioAy.push_back(pNewMarker);
  }
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::MakeStunts(CShader *pShader, CTrack *pTrack, std::vector<CShapeData *> &stuntAy)
{
  for (CStuntMap::iterator it = pTrack->m_stuntMap.begin(); it != pTrack->m_stuntMap.end(); ++it) {
    if (it->first < 0 || it->first >= pTrack->m_chunkAy.size()) {
      assert(0);
      continue;
    }

    //make marker
    CShapeData *pNewMarker = NULL;
    MakeStuntMarker(&pNewMarker, pShader, pTrack->m_pTex);

    float fHeight = (float)1000.0f * -1.0f;
    glm::mat4 translateMat = glm::translate(pTrack->m_chunkAy[it->first].math.centerStunt);
    glm::mat4 scaleMatHeight = glm::scale(glm::vec3(fHeight, fHeight, fHeight));
    glm::vec3 normal = glm::normalize(glm::cross(pTrack->m_chunkAy[it->first].math.nextChunkPitched, pTrack->m_chunkAy[it->first].math.pitchAxis));
    glm::vec3 heightVec = glm::vec3(scaleMatHeight * pTrack->m_chunkAy[it->first].math.rollMat * glm::vec4(normal, 1.0f));
    glm::vec3 markerPos = heightVec;
    glm::vec3 markerPosTranslated = glm::vec3(translateMat * glm::vec4(markerPos, 1.0f));

    pNewMarker->m_modelToWorldMatrix = glm::translate(markerPosTranslated) *
      pTrack->m_chunkAy[it->first].math.rollMat * pTrack->m_chunkAy[it->first].math.pitchMat * pTrack->m_chunkAy[it->first].math.yawMat;

    //add sign to array
    stuntAy.push_back(pNewMarker);
  }
}

//-------------------------------------------------------------------------------------------------

CShapeData *CShapeFactory::MakeNormalsTest(const CShapeData &data, CShader *pShader)
{
  uint32 uiNumVerts = data.m_uiNumVerts * 2;
  tVertex *vertices = new tVertex[uiNumVerts];
  glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
  for (int i = 0; i < (int)data.m_uiNumVerts; i++) {
    GLuint vertIndex = i * 2;
    tVertex &v1 = vertices[vertIndex];
    tVertex &v2 = vertices[vertIndex + 1];
    const tVertex &sourceVertex = data.m_vertices[i];
    v1.position = sourceVertex.position;
    v2.position = sourceVertex.position + sourceVertex.normal;
    //TODO
    //v1.color = v2.color = white;
  }

  uint32 uiNumIndices = uiNumVerts;
  uint32 *indices = new uint32[uiNumIndices];
  for (int i = 0; i < (int)uiNumIndices; i++)
    indices[i] = i;

  CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts);
  CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
  CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);
  CShapeData *pRet = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, NULL, GL_LINES);
  pRet->m_vertices = vertices;
  pRet->m_uiNumVerts = uiNumVerts;
  pRet->m_indices = indices;
  pRet->m_uiNumIndices = uiNumIndices;
  return pRet;
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::GetCarPos(CTrack *pTrack, int iChunk, eShapeSection aiLineSection, glm::mat4 &modelToWorldMatrix, bool bMillionPlus)
{
  if (pTrack->m_chunkAy.empty() || iChunk > (int)pTrack->m_chunkAy.size() - 1)
    return;

  glm::vec3 carLine;
  switch (aiLineSection) {
    case eShapeSection::AILINE1:
      carLine = pTrack->m_chunkAy[iChunk].math.carLine1;
      break;
    case eShapeSection::AILINE2:
      carLine = pTrack->m_chunkAy[iChunk].math.carLine2;
      break;
    case eShapeSection::AILINE3:
      carLine = pTrack->m_chunkAy[iChunk].math.carLine3;
      break;
    case eShapeSection::AILINE4:
      carLine = pTrack->m_chunkAy[iChunk].math.carLine4;
      break;
    default:
      assert(0);
  }
 
  float fRotate = -90.0f;
  if (bMillionPlus)
    fRotate = 90.0f;
  modelToWorldMatrix = glm::translate(carLine) *
    pTrack->m_chunkAy[iChunk].math.rollMat * pTrack->m_chunkAy[iChunk].math.pitchMat * pTrack->m_chunkAy[iChunk].math.yawMat *
    glm::rotate(glm::radians(fRotate), glm::vec3(0, 0, 1)) * //car starts on its side
    glm::rotate(glm::radians(fRotate), glm::vec3(0, 1, 0)); //track starts facing z positive, car starts facing x positive
}

//-------------------------------------------------------------------------------------------------

tVertex *CShapeFactory::MakeVerts(uint32 &numVertices, eShapeSection section, CTrack *pTrack, CTexture *pTexture)
{
  if (pTrack->m_chunkAy.empty() || !pTexture) {
    numVertices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 4 * 2;
  if (section == eShapeSection::SELECTED) uiNumVertsPerChunk = 8;
  if (section >= eShapeSection::AILINE1 && section <= eShapeSection::AILINE4) uiNumVertsPerChunk = 1;
  if (section == eShapeSection::EXPORT) uiNumVertsPerChunk = 44 * 2;

  numVertices = (uint32)pTrack->m_chunkAy.size() * uiNumVertsPerChunk;
  tVertex *vertices = new tVertex[numVertices];
  for (uint32 i = 0; i < pTrack->m_chunkAy.size(); ++i) {
    int iChunkIndex = (int)pTrack->m_chunkAy.size() - 1;
    if (i > 0)
      iChunkIndex = i - 1;

    int iVertRunner = 0;
    switch (section) {
      case eShapeSection::CENTER:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.lLane,
                                   pTrack->m_chunkAy[i].math.rLane,
                                   pTrack->m_chunkAy[iChunkIndex].math.lLane,
                                   pTrack->m_chunkAy[iChunkIndex].math.rLane);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0],
                    vertices[i * uiNumVertsPerChunk + 1],
                    vertices[i * uiNumVertsPerChunk + 2],
                    vertices[i * uiNumVertsPerChunk + 3]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 1 + 4],
                    vertices[i * uiNumVertsPerChunk + 0 + 4],
                    vertices[i * uiNumVertsPerChunk + 3 + 4],
                    vertices[i * uiNumVertsPerChunk + 2 + 4]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iCenterSurfaceType),
                                            vertices[i * uiNumVertsPerChunk + 0],
                                            vertices[i * uiNumVertsPerChunk + 1],
                                            vertices[i * uiNumVertsPerChunk + 2],
                                            vertices[i * uiNumVertsPerChunk + 3]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iCenterSurfaceType),
                                            vertices[i * uiNumVertsPerChunk + 0 + 4],
                                            vertices[i * uiNumVertsPerChunk + 1 + 4],
                                            vertices[i * uiNumVertsPerChunk + 2 + 4],
                                            vertices[i * uiNumVertsPerChunk + 3 + 4]);
        break;
      case eShapeSection::LSHOULDER:
        ApplyVerticesSingleSection(i, vertices,
                              pTrack->m_chunkAy[i].math.lShoulder,
                              pTrack->m_chunkAy[i].math.lLane,
                              pTrack->m_chunkAy[iChunkIndex].math.lShoulder,
                              pTrack->m_chunkAy[iChunkIndex].math.lLane);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0],
                    vertices[i * uiNumVertsPerChunk + 1],
                    vertices[i * uiNumVertsPerChunk + 2],
                    vertices[i * uiNumVertsPerChunk + 3]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 1 + 4],
                    vertices[i * uiNumVertsPerChunk + 0 + 4],
                    vertices[i * uiNumVertsPerChunk + 3 + 4],
                    vertices[i * uiNumVertsPerChunk + 2 + 4]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftSurfaceType),
                                            vertices[i * uiNumVertsPerChunk + 0],
                                            vertices[i * uiNumVertsPerChunk + 1],
                                            vertices[i * uiNumVertsPerChunk + 2],
                                            vertices[i * uiNumVertsPerChunk + 3]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftSurfaceType),
                                            vertices[i * uiNumVertsPerChunk + 0 + 4],
                                            vertices[i * uiNumVertsPerChunk + 1 + 4],
                                            vertices[i * uiNumVertsPerChunk + 2 + 4],
                                            vertices[i * uiNumVertsPerChunk + 3 + 4]);
        break;
      case eShapeSection::RSHOULDER:
        ApplyVerticesSingleSection(i, vertices,
                                   pTrack->m_chunkAy[i].math.rLane,
                                   pTrack->m_chunkAy[i].math.rShoulder,
                                   pTrack->m_chunkAy[iChunkIndex].math.rLane,
                                   pTrack->m_chunkAy[iChunkIndex].math.rShoulder);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0],
                    vertices[i * uiNumVertsPerChunk + 1],
                    vertices[i * uiNumVertsPerChunk + 2],
                    vertices[i * uiNumVertsPerChunk + 3]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 1 + 4],
                    vertices[i * uiNumVertsPerChunk + 0 + 4],
                    vertices[i * uiNumVertsPerChunk + 3 + 4],
                    vertices[i * uiNumVertsPerChunk + 2 + 4]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightSurfaceType),
                                            vertices[i * uiNumVertsPerChunk + 0],
                                            vertices[i * uiNumVertsPerChunk + 1],
                                            vertices[i * uiNumVertsPerChunk + 2],
                                            vertices[i * uiNumVertsPerChunk + 3]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightSurfaceType),
                                            vertices[i * uiNumVertsPerChunk + 0 + 4],
                                            vertices[i * uiNumVertsPerChunk + 1 + 4],
                                            vertices[i * uiNumVertsPerChunk + 2 + 4],
                                            vertices[i * uiNumVertsPerChunk + 3 + 4]);
        break;
      case eShapeSection::LWALL:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.lWall, 
                                   pTrack->m_chunkAy[i].math.lWallBottomAttach, 
                                   pTrack->m_chunkAy[iChunkIndex].math.lWall,
                                   pTrack->m_chunkAy[iChunkIndex].math.lWallBottomAttach);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 2],
                    vertices[i * uiNumVertsPerChunk + 0],
                    vertices[i * uiNumVertsPerChunk + 3],
                    vertices[i * uiNumVertsPerChunk + 1]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0 + 4],
                    vertices[i * uiNumVertsPerChunk + 2 + 4],
                    vertices[i * uiNumVertsPerChunk + 1 + 4],
                    vertices[i * uiNumVertsPerChunk + 3 + 4]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftWallType),
                            vertices[i * uiNumVertsPerChunk + 2],
                            vertices[i * uiNumVertsPerChunk + 0],
                            vertices[i * uiNumVertsPerChunk + 3],
                            vertices[i * uiNumVertsPerChunk + 1]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftWallType),
                            vertices[i * uiNumVertsPerChunk + 2 + 4],
                            vertices[i * uiNumVertsPerChunk + 0 + 4],
                            vertices[i * uiNumVertsPerChunk + 3 + 4],
                            vertices[i * uiNumVertsPerChunk + 1 + 4]);
        if (CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftWallType) & SURFACE_FLAG_BACK) {
          uint32 uiTexIdx = CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftWallType) & SURFACE_MASK_TEXTURE_INDEX;
          CSignMap::iterator it = pTrack->m_backsMap.find(uiTexIdx);
          if (it != pTrack->m_backsMap.end()) {
            pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(it->second),
                                vertices[i * uiNumVertsPerChunk + 0 + 4],
                                vertices[i * uiNumVertsPerChunk + 2 + 4],
                                vertices[i * uiNumVertsPerChunk + 1 + 4],
                                vertices[i * uiNumVertsPerChunk + 3 + 4]);
          }
        }
        break;
      case eShapeSection::RWALL:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.rWallBottomAttach, 
                                   pTrack->m_chunkAy[i].math.rWall, 
                                   pTrack->m_chunkAy[iChunkIndex].math.rWallBottomAttach,
                                   pTrack->m_chunkAy[iChunkIndex].math.rWall);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 1],
                    vertices[i * uiNumVertsPerChunk + 3],
                    vertices[i * uiNumVertsPerChunk + 0],
                    vertices[i * uiNumVertsPerChunk + 2]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 3 + 4],
                    vertices[i * uiNumVertsPerChunk + 1 + 4],
                    vertices[i * uiNumVertsPerChunk + 2 + 4],
                    vertices[i * uiNumVertsPerChunk + 0 + 4]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightWallType),
                            vertices[i * uiNumVertsPerChunk + 1],
                            vertices[i * uiNumVertsPerChunk + 3],
                            vertices[i * uiNumVertsPerChunk + 0],
                            vertices[i * uiNumVertsPerChunk + 2]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightWallType),
                            vertices[i * uiNumVertsPerChunk + 1 + 4],
                            vertices[i * uiNumVertsPerChunk + 3 + 4],
                            vertices[i * uiNumVertsPerChunk + 0 + 4],
                            vertices[i * uiNumVertsPerChunk + 2 + 4]);
        if (CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightWallType) & SURFACE_FLAG_BACK) {
          uint32 uiTexIdx = CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightWallType) & SURFACE_MASK_TEXTURE_INDEX;
          CSignMap::iterator it = pTrack->m_backsMap.find(uiTexIdx);
          if (it != pTrack->m_backsMap.end()) {
            pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(it->second),
                                vertices[i * uiNumVertsPerChunk + 3 + 4],
                                vertices[i * uiNumVertsPerChunk + 1 + 4],
                                vertices[i * uiNumVertsPerChunk + 2 + 4],
                                vertices[i * uiNumVertsPerChunk + 0 + 4]);
          }
        }
        break;
      case eShapeSection::ROOF:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.rWall, 
                                   pTrack->m_chunkAy[i].math.lWall, 
                                   pTrack->m_chunkAy[iChunkIndex].math.rWall,
                                   pTrack->m_chunkAy[iChunkIndex].math.lWall);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0],
                    vertices[i * uiNumVertsPerChunk + 1],
                    vertices[i * uiNumVertsPerChunk + 2],
                    vertices[i * uiNumVertsPerChunk + 3]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 1 + 4],
                    vertices[i * uiNumVertsPerChunk + 0 + 4],
                    vertices[i * uiNumVertsPerChunk + 3 + 4],
                    vertices[i * uiNumVertsPerChunk + 2 + 4]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRoofType),
                            vertices[i * uiNumVertsPerChunk + 0],
                            vertices[i * uiNumVertsPerChunk + 1],
                            vertices[i * uiNumVertsPerChunk + 2],
                            vertices[i * uiNumVertsPerChunk + 3]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRoofType),
                            vertices[i * uiNumVertsPerChunk + 0 + 4],
                            vertices[i * uiNumVertsPerChunk + 1 + 4],
                            vertices[i * uiNumVertsPerChunk + 2 + 4],
                            vertices[i * uiNumVertsPerChunk + 3 + 4]);
        break;
      case eShapeSection::OWALLFLOOR:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.lFloor, 
                                   pTrack->m_chunkAy[i].math.rFloor, 
                                   pTrack->m_chunkAy[iChunkIndex].math.lFloor,
                                   pTrack->m_chunkAy[iChunkIndex].math.rFloor);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0],
                    vertices[i * uiNumVertsPerChunk + 1],
                    vertices[i * uiNumVertsPerChunk + 2],
                    vertices[i * uiNumVertsPerChunk + 3]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 1 + 4],
                    vertices[i * uiNumVertsPerChunk + 0 + 4],
                    vertices[i * uiNumVertsPerChunk + 3 + 4],
                    vertices[i * uiNumVertsPerChunk + 2 + 4]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iOuterFloorType),
                            vertices[i * uiNumVertsPerChunk + 0],
                            vertices[i * uiNumVertsPerChunk + 1],
                            vertices[i * uiNumVertsPerChunk + 2],
                            vertices[i * uiNumVertsPerChunk + 3]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iOuterFloorType),
                            vertices[i * uiNumVertsPerChunk + 0 + 4],
                            vertices[i * uiNumVertsPerChunk + 1 + 4],
                            vertices[i * uiNumVertsPerChunk + 2 + 4],
                            vertices[i * uiNumVertsPerChunk + 3 + 4]);
        break;
      case eShapeSection::LLOWALL:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.lloWall, 
                                   pTrack->m_chunkAy[i].math.lloWallBottomAttach, 
                                   pTrack->m_chunkAy[iChunkIndex].math.lloWall,
                                   pTrack->m_chunkAy[iChunkIndex].math.lloWallBottomAttach);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 2],
                    vertices[i * uiNumVertsPerChunk + 0],
                    vertices[i * uiNumVertsPerChunk + 3],
                    vertices[i * uiNumVertsPerChunk + 1]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0 + 4],
                    vertices[i * uiNumVertsPerChunk + 2 + 4],
                    vertices[i * uiNumVertsPerChunk + 1 + 4],
                    vertices[i * uiNumVertsPerChunk + 3 + 4]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLLOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2],
                            vertices[i * uiNumVertsPerChunk + 0],
                            vertices[i * uiNumVertsPerChunk + 3],
                            vertices[i * uiNumVertsPerChunk + 1]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLLOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2 + 4],
                            vertices[i * uiNumVertsPerChunk + 0 + 4],
                            vertices[i * uiNumVertsPerChunk + 3 + 4],
                            vertices[i * uiNumVertsPerChunk + 1 + 4]);
        break;
      case eShapeSection::RLOWALL:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.rloWallBottomAttach, 
                                   pTrack->m_chunkAy[i].math.rloWall, 
                                   pTrack->m_chunkAy[iChunkIndex].math.rloWallBottomAttach,
                                   pTrack->m_chunkAy[iChunkIndex].math.rloWall);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 2],
                    vertices[i * uiNumVertsPerChunk + 0],
                    vertices[i * uiNumVertsPerChunk + 3],
                    vertices[i * uiNumVertsPerChunk + 1]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0 + 4],
                    vertices[i * uiNumVertsPerChunk + 2 + 4],
                    vertices[i * uiNumVertsPerChunk + 1 + 4],
                    vertices[i * uiNumVertsPerChunk + 3 + 4]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRLOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2],
                            vertices[i * uiNumVertsPerChunk + 0],
                            vertices[i * uiNumVertsPerChunk + 3],
                            vertices[i * uiNumVertsPerChunk + 1]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRLOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2 + 4],
                            vertices[i * uiNumVertsPerChunk + 0 + 4],
                            vertices[i * uiNumVertsPerChunk + 3 + 4],
                            vertices[i * uiNumVertsPerChunk + 1 + 4]);
        break;
      case eShapeSection::LUOWALL:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.luoWall, 
                                   pTrack->m_chunkAy[i].math.lloWall, 
                                   pTrack->m_chunkAy[iChunkIndex].math.luoWall,
                                   pTrack->m_chunkAy[iChunkIndex].math.lloWall);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 2],
                    vertices[i * uiNumVertsPerChunk + 0],
                    vertices[i * uiNumVertsPerChunk + 3],
                    vertices[i * uiNumVertsPerChunk + 1]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0 + 4],
                    vertices[i * uiNumVertsPerChunk + 2 + 4],
                    vertices[i * uiNumVertsPerChunk + 1 + 4],
                    vertices[i * uiNumVertsPerChunk + 3 + 4]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLUOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2],
                            vertices[i * uiNumVertsPerChunk + 0],
                            vertices[i * uiNumVertsPerChunk + 3],
                            vertices[i * uiNumVertsPerChunk + 1]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLUOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2 + 4],
                            vertices[i * uiNumVertsPerChunk + 0 + 4],
                            vertices[i * uiNumVertsPerChunk + 3 + 4],
                            vertices[i * uiNumVertsPerChunk + 1 + 4]);
        break;
      case eShapeSection::RUOWALL:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.rloWall, 
                                   pTrack->m_chunkAy[i].math.ruoWall, 
                                   pTrack->m_chunkAy[iChunkIndex].math.rloWall,
                                   pTrack->m_chunkAy[iChunkIndex].math.ruoWall);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 2],
                    vertices[i * uiNumVertsPerChunk + 0],
                    vertices[i * uiNumVertsPerChunk + 3],
                    vertices[i * uiNumVertsPerChunk + 1]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0 + 4],
                    vertices[i * uiNumVertsPerChunk + 2 + 4],
                    vertices[i * uiNumVertsPerChunk + 1 + 4],
                    vertices[i * uiNumVertsPerChunk + 3 + 4]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRUOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2],
                            vertices[i * uiNumVertsPerChunk + 0],
                            vertices[i * uiNumVertsPerChunk + 3],
                            vertices[i * uiNumVertsPerChunk + 1]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRUOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2 + 4],
                            vertices[i * uiNumVertsPerChunk + 0 + 4],
                            vertices[i * uiNumVertsPerChunk + 3 + 4],
                            vertices[i * uiNumVertsPerChunk + 1 + 4]);
        break;
      case eShapeSection::SELECTED:
        vertices[i * uiNumVertsPerChunk + 0].position = pTrack->m_chunkAy[i].math.luoWall;
        vertices[i * uiNumVertsPerChunk + 1].position = pTrack->m_chunkAy[i].math.ruoWall;
        vertices[i * uiNumVertsPerChunk + 2].position = pTrack->m_chunkAy[i].math.lloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + 3].position = pTrack->m_chunkAy[i].math.rloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + 4].position = pTrack->m_chunkAy[iChunkIndex].math.luoWall;
        vertices[i * uiNumVertsPerChunk + 5].position = pTrack->m_chunkAy[iChunkIndex].math.ruoWall;
        vertices[i * uiNumVertsPerChunk + 6].position = pTrack->m_chunkAy[iChunkIndex].math.lloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + 7].position = pTrack->m_chunkAy[iChunkIndex].math.rloWallBottomAttach;
        break;
      case eShapeSection::AILINE1:
        vertices[i * uiNumVertsPerChunk + 0].position = pTrack->m_chunkAy[i].math.aiLine1;
        break;
      case eShapeSection::AILINE2:
        vertices[i * uiNumVertsPerChunk + 0].position = pTrack->m_chunkAy[i].math.aiLine2;
        break;
      case eShapeSection::AILINE3:
        vertices[i * uiNumVertsPerChunk + 0].position = pTrack->m_chunkAy[i].math.aiLine3;
        break;
      case eShapeSection::AILINE4:
        vertices[i * uiNumVertsPerChunk + 0].position = pTrack->m_chunkAy[i].math.aiLine4;
        break;
      case eShapeSection::EXPORT:
        //center
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lLane;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rLane;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lLane;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rLane;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lLane;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rLane;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lLane;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rLane;
        //lshoulder
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lShoulder;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lLane;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lShoulder;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lLane;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lShoulder;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lLane;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lShoulder;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lLane;
        //rshoulder
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rLane;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rShoulder;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rLane;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rShoulder;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rLane;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rShoulder;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rLane;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rShoulder;
        //lwall
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lWallBottomAttach;
        //rwall
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rWall;
        //roof
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lWall;
        //owallfloor
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lFloor;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rFloor;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lFloor;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rFloor;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lFloor;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rFloor;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lFloor;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rFloor;
        //llowall
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lloWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lloWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lloWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lloWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lloWallBottomAttach;
        //rlowall
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rloWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rloWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rloWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rloWall;
        //luowall
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.luoWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lloWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.luoWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lloWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.luoWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.lloWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.luoWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.lloWall;
        //ruowall
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rloWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.ruoWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rloWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.ruoWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.rloWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[i].math.ruoWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.rloWall;
        vertices[i * uiNumVertsPerChunk + iVertRunner++].position = pTrack->m_chunkAy[iChunkIndex].math.ruoWall;
        assert(uiNumVertsPerChunk == (uint32)iVertRunner);
        //center
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0],
                    vertices[i * uiNumVertsPerChunk + 1],
                    vertices[i * uiNumVertsPerChunk + 2],
                    vertices[i * uiNumVertsPerChunk + 3]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 1 + 4],
                    vertices[i * uiNumVertsPerChunk + 0 + 4],
                    vertices[i * uiNumVertsPerChunk + 3 + 4],
                    vertices[i * uiNumVertsPerChunk + 2 + 4]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iCenterSurfaceType),
                                            vertices[i * uiNumVertsPerChunk + 0],
                                            vertices[i * uiNumVertsPerChunk + 1],
                                            vertices[i * uiNumVertsPerChunk + 2],
                                            vertices[i * uiNumVertsPerChunk + 3]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iCenterSurfaceType),
                                            vertices[i * uiNumVertsPerChunk + 0 + 4],
                                            vertices[i * uiNumVertsPerChunk + 1 + 4],
                                            vertices[i * uiNumVertsPerChunk + 2 + 4],
                                            vertices[i * uiNumVertsPerChunk + 3 + 4]);
        //lshoulder
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0 + 8],
                    vertices[i * uiNumVertsPerChunk + 1 + 8],
                    vertices[i * uiNumVertsPerChunk + 2 + 8],
                    vertices[i * uiNumVertsPerChunk + 3 + 8]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 1 + 12],
                    vertices[i * uiNumVertsPerChunk + 0 + 12],
                    vertices[i * uiNumVertsPerChunk + 3 + 12],
                    vertices[i * uiNumVertsPerChunk + 2 + 12]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftSurfaceType),
                                            vertices[i * uiNumVertsPerChunk + 0 + 8],
                                            vertices[i * uiNumVertsPerChunk + 1 + 8],
                                            vertices[i * uiNumVertsPerChunk + 2 + 8],
                                            vertices[i * uiNumVertsPerChunk + 3 + 8]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftSurfaceType),
                                            vertices[i * uiNumVertsPerChunk + 0 + 12],
                                            vertices[i * uiNumVertsPerChunk + 1 + 12],
                                            vertices[i * uiNumVertsPerChunk + 2 + 12],
                                            vertices[i * uiNumVertsPerChunk + 3 + 12]);
        //rshoulder
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0 + 16],
                    vertices[i * uiNumVertsPerChunk + 1 + 16],
                    vertices[i * uiNumVertsPerChunk + 2 + 16],
                    vertices[i * uiNumVertsPerChunk + 3 + 16]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 1 + 20],
                    vertices[i * uiNumVertsPerChunk + 0 + 20],
                    vertices[i * uiNumVertsPerChunk + 3 + 20],
                    vertices[i * uiNumVertsPerChunk + 2 + 20]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightSurfaceType),
                                            vertices[i * uiNumVertsPerChunk + 0 + 16],
                                            vertices[i * uiNumVertsPerChunk + 1 + 16],
                                            vertices[i * uiNumVertsPerChunk + 2 + 16],
                                            vertices[i * uiNumVertsPerChunk + 3 + 16]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightSurfaceType),
                                            vertices[i * uiNumVertsPerChunk + 0 + 20],
                                            vertices[i * uiNumVertsPerChunk + 1 + 20],
                                            vertices[i * uiNumVertsPerChunk + 2 + 20],
                                            vertices[i * uiNumVertsPerChunk + 3 + 20]);
        //lwall
        MakeNormals(vertices[i * uiNumVertsPerChunk + 2 + 24],
                    vertices[i * uiNumVertsPerChunk + 0 + 24],
                    vertices[i * uiNumVertsPerChunk + 3 + 24],
                    vertices[i * uiNumVertsPerChunk + 1 + 24]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0 + 28],
                    vertices[i * uiNumVertsPerChunk + 2 + 28],
                    vertices[i * uiNumVertsPerChunk + 1 + 28],
                    vertices[i * uiNumVertsPerChunk + 3 + 28]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftWallType),
                            vertices[i * uiNumVertsPerChunk + 2 + 24],
                            vertices[i * uiNumVertsPerChunk + 0 + 24],
                            vertices[i * uiNumVertsPerChunk + 3 + 24],
                            vertices[i * uiNumVertsPerChunk + 1 + 24]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftWallType),
                            vertices[i * uiNumVertsPerChunk + 2 + 28],
                            vertices[i * uiNumVertsPerChunk + 0 + 28],
                            vertices[i * uiNumVertsPerChunk + 3 + 28],
                            vertices[i * uiNumVertsPerChunk + 1 + 28]);
        if (CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftWallType) & SURFACE_FLAG_BACK) {
          uint32 uiTexIdx = CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftWallType) & SURFACE_MASK_TEXTURE_INDEX;
          CSignMap::iterator it = pTrack->m_backsMap.find(uiTexIdx);
          if (it != pTrack->m_backsMap.end()) {
            pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(it->second),
                                vertices[i * uiNumVertsPerChunk + 0 + 28],
                                vertices[i * uiNumVertsPerChunk + 2 + 28],
                                vertices[i * uiNumVertsPerChunk + 1 + 28],
                                vertices[i * uiNumVertsPerChunk + 3 + 28]);
          }
        }
        //rwall
        MakeNormals(vertices[i * uiNumVertsPerChunk + 1 + 32],
                    vertices[i * uiNumVertsPerChunk + 3 + 32],
                    vertices[i * uiNumVertsPerChunk + 0 + 32],
                    vertices[i * uiNumVertsPerChunk + 2 + 32]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 3 + 36],
                    vertices[i * uiNumVertsPerChunk + 1 + 36],
                    vertices[i * uiNumVertsPerChunk + 2 + 36],
                    vertices[i * uiNumVertsPerChunk + 0 + 36]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightWallType),
                            vertices[i * uiNumVertsPerChunk + 1 + 32],
                            vertices[i * uiNumVertsPerChunk + 3 + 32],
                            vertices[i * uiNumVertsPerChunk + 0 + 32],
                            vertices[i * uiNumVertsPerChunk + 2 + 32]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightWallType),
                            vertices[i * uiNumVertsPerChunk + 1 + 36],
                            vertices[i * uiNumVertsPerChunk + 3 + 36],
                            vertices[i * uiNumVertsPerChunk + 0 + 36],
                            vertices[i * uiNumVertsPerChunk + 2 + 36]);
        if (CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightWallType) & SURFACE_FLAG_BACK) {
          uint32 uiTexIdx = CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightWallType) & SURFACE_MASK_TEXTURE_INDEX;
          CSignMap::iterator it = pTrack->m_backsMap.find(uiTexIdx);
          if (it != pTrack->m_backsMap.end()) {
            pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(it->second),
                                vertices[i * uiNumVertsPerChunk + 3 + 36],
                                vertices[i * uiNumVertsPerChunk + 1 + 36],
                                vertices[i * uiNumVertsPerChunk + 2 + 36],
                                vertices[i * uiNumVertsPerChunk + 0 + 36]);
          }
        }
        //roof
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0 + 40],
                    vertices[i * uiNumVertsPerChunk + 1 + 40],
                    vertices[i * uiNumVertsPerChunk + 2 + 40],
                    vertices[i * uiNumVertsPerChunk + 3 + 40]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 1 + 44],
                    vertices[i * uiNumVertsPerChunk + 0 + 44],
                    vertices[i * uiNumVertsPerChunk + 3 + 44],
                    vertices[i * uiNumVertsPerChunk + 2 + 44]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRoofType),
                            vertices[i * uiNumVertsPerChunk + 0 + 40],
                            vertices[i * uiNumVertsPerChunk + 1 + 40],
                            vertices[i * uiNumVertsPerChunk + 2 + 40],
                            vertices[i * uiNumVertsPerChunk + 3 + 40]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRoofType),
                            vertices[i * uiNumVertsPerChunk + 0 + 44],
                            vertices[i * uiNumVertsPerChunk + 1 + 44],
                            vertices[i * uiNumVertsPerChunk + 2 + 44],
                            vertices[i * uiNumVertsPerChunk + 3 + 44]);
        //owallfloor
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0 + 48],
                    vertices[i * uiNumVertsPerChunk + 1 + 48],
                    vertices[i * uiNumVertsPerChunk + 2 + 48],
                    vertices[i * uiNumVertsPerChunk + 3 + 48]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 1 + 52],
                    vertices[i * uiNumVertsPerChunk + 0 + 52],
                    vertices[i * uiNumVertsPerChunk + 3 + 52],
                    vertices[i * uiNumVertsPerChunk + 2 + 52]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iOuterFloorType),
                            vertices[i * uiNumVertsPerChunk + 0 + 48],
                            vertices[i * uiNumVertsPerChunk + 1 + 48],
                            vertices[i * uiNumVertsPerChunk + 2 + 48],
                            vertices[i * uiNumVertsPerChunk + 3 + 48]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iOuterFloorType),
                            vertices[i * uiNumVertsPerChunk + 0 + 52],
                            vertices[i * uiNumVertsPerChunk + 1 + 52],
                            vertices[i * uiNumVertsPerChunk + 2 + 52],
                            vertices[i * uiNumVertsPerChunk + 3 + 52]);
        //llowall
        MakeNormals(vertices[i * uiNumVertsPerChunk + 2 + 56],
                    vertices[i * uiNumVertsPerChunk + 0 + 56],
                    vertices[i * uiNumVertsPerChunk + 3 + 56],
                    vertices[i * uiNumVertsPerChunk + 1 + 56]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0 + 60],
                    vertices[i * uiNumVertsPerChunk + 2 + 60],
                    vertices[i * uiNumVertsPerChunk + 1 + 60],
                    vertices[i * uiNumVertsPerChunk + 3 + 60]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLLOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2 + 56],
                            vertices[i * uiNumVertsPerChunk + 0 + 56],
                            vertices[i * uiNumVertsPerChunk + 3 + 56],
                            vertices[i * uiNumVertsPerChunk + 1 + 56]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLLOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2 + 60],
                            vertices[i * uiNumVertsPerChunk + 0 + 60],
                            vertices[i * uiNumVertsPerChunk + 3 + 60],
                            vertices[i * uiNumVertsPerChunk + 1 + 60]);
        //rlowall
        MakeNormals(vertices[i * uiNumVertsPerChunk + 2 + 64],
                    vertices[i * uiNumVertsPerChunk + 0 + 64],
                    vertices[i * uiNumVertsPerChunk + 3 + 64],
                    vertices[i * uiNumVertsPerChunk + 1 + 64]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0 + 68],
                    vertices[i * uiNumVertsPerChunk + 2 + 68],
                    vertices[i * uiNumVertsPerChunk + 1 + 68],
                    vertices[i * uiNumVertsPerChunk + 3 + 68]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRLOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2 + 64],
                            vertices[i * uiNumVertsPerChunk + 0 + 64],
                            vertices[i * uiNumVertsPerChunk + 3 + 64],
                            vertices[i * uiNumVertsPerChunk + 1 + 64]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRLOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2 + 68],
                            vertices[i * uiNumVertsPerChunk + 0 + 68],
                            vertices[i * uiNumVertsPerChunk + 3 + 68],
                            vertices[i * uiNumVertsPerChunk + 1 + 68]);
        //luowall
        MakeNormals(vertices[i * uiNumVertsPerChunk + 2 + 72],
                    vertices[i * uiNumVertsPerChunk + 0 + 72],
                    vertices[i * uiNumVertsPerChunk + 3 + 72],
                    vertices[i * uiNumVertsPerChunk + 1 + 72]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0 + 76],
                    vertices[i * uiNumVertsPerChunk + 2 + 76],
                    vertices[i * uiNumVertsPerChunk + 1 + 76],
                    vertices[i * uiNumVertsPerChunk + 3 + 76]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLUOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2 + 72],
                            vertices[i * uiNumVertsPerChunk + 0 + 72],
                            vertices[i * uiNumVertsPerChunk + 3 + 72],
                            vertices[i * uiNumVertsPerChunk + 1 + 72]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLUOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2 + 76],
                            vertices[i * uiNumVertsPerChunk + 0 + 76],
                            vertices[i * uiNumVertsPerChunk + 3 + 76],
                            vertices[i * uiNumVertsPerChunk + 1 + 76]);
        //ruowall
        MakeNormals(vertices[i * uiNumVertsPerChunk + 2 + 80],
                    vertices[i * uiNumVertsPerChunk + 0 + 80],
                    vertices[i * uiNumVertsPerChunk + 3 + 80],
                    vertices[i * uiNumVertsPerChunk + 1 + 80]);
        MakeNormals(vertices[i * uiNumVertsPerChunk + 0 + 84],
                    vertices[i * uiNumVertsPerChunk + 2 + 84],
                    vertices[i * uiNumVertsPerChunk + 1 + 84],
                    vertices[i * uiNumVertsPerChunk + 3 + 84]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRUOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2 + 80],
                            vertices[i * uiNumVertsPerChunk + 0 + 80],
                            vertices[i * uiNumVertsPerChunk + 3 + 80],
                            vertices[i * uiNumVertsPerChunk + 1 + 80]);
        pTexture->GetTextureCoordinates(CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRUOuterWallType),
                            vertices[i * uiNumVertsPerChunk + 2 + 84],
                            vertices[i * uiNumVertsPerChunk + 0 + 84],
                            vertices[i * uiNumVertsPerChunk + 3 + 84],
                            vertices[i * uiNumVertsPerChunk + 1 + 84]);
        break;
      default:
        assert(0); //shape not implemented
        break;
    }
  }
  return vertices;
}

//-------------------------------------------------------------------------------------------------

tVertex *CShapeFactory::MakeVertsEnvirFloor(uint32 &numVertices, CTrack *pTrack, CTexture *pTexture, int iIndex)
{
  if (pTrack->m_chunkAy.empty() || iIndex >= pTrack->m_chunkAy.size()) {
    numVertices = 0;
    return NULL;
  }

  numVertices = 4;
  tVertex *vertices = new tVertex[numVertices];
  float fEnvirFloorDepth = (float)pTrack->m_header.iFloorDepth * -1.0f;
  float fPadding = 1000000.0f;
  vertices[0].position = glm::vec3(-fPadding, fEnvirFloorDepth, -fPadding);
  vertices[1].position = glm::vec3(-fPadding, fEnvirFloorDepth, +fPadding);
  vertices[2].position = glm::vec3(+fPadding, fEnvirFloorDepth, -fPadding);
  vertices[3].position = glm::vec3(+fPadding, fEnvirFloorDepth, +fPadding);
  pTexture->GetTextureCoordinates(
    CTrack::GetSignedBitValueFromInt(pTrack->m_chunkAy[iIndex].iEnvironmentFloorType) & SURFACE_MASK_TEXTURE_INDEX,
    vertices[0], vertices[1], vertices[2], vertices[3]);

  return vertices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesEnvirFloor(uint32 &numIndices)
{
  uint32 uiNumVertsPerChunk = 4;
  uint32 uiNumIndicesPerChunk = 6;
  numIndices = 6;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));
  indices[0] = 2;
  indices[1] = 1;
  indices[2] = 3;
  indices[3] = 2;
  indices[4] = 0;
  indices[5] = 1;
  return indices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesSingleSection(uint32 &numIndices, eShapeSection section, CTrack *pTrack, bool bAttachLast)
{
  if (pTrack->m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 8;
  uint32 uiNumIndicesPerChunk = 12;
  numIndices = (uint32)pTrack->m_chunkAy.size() * uiNumIndicesPerChunk;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  uint32 i = 0;
  for (; i < pTrack->m_chunkAy.size(); i++) {
    int iIndicesRunner = 0;
    if (i > 0 && !pTrack->ShouldShowChunkSection(i - 1, section))
      continue;
    else if (i == 0 && !pTrack->ShouldShowChunkSection(((int)pTrack->m_chunkAy.size() - 1), section))
      continue;
    if (!bAttachLast && i == 0)
      continue;

    MakeIndicesHelper(indices, i, uiNumIndicesPerChunk, iIndicesRunner, uiNumVertsPerChunk, 0);
  }

  return indices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesExport(uint32 &numIndices, CTrack *pTrack)
{
  if (pTrack->m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 88;
  uint32 uiNumIndicesPerChunk = 132;
  numIndices = (uint32)pTrack->m_chunkAy.size() * uiNumIndicesPerChunk;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  uint32 i = 0;
  for (; i < pTrack->m_chunkAy.size(); i++) {
    int iIndicesRunner = 0;
    if (!(i > 0 && !pTrack->ShouldShowChunkSection(i - 1, eShapeSection::CENTER))
      && !(i == 0 && !pTrack->ShouldShowChunkSection(((int)pTrack->m_chunkAy.size() - 1), eShapeSection::CENTER))) {
      MakeIndicesHelper(indices, i, uiNumIndicesPerChunk, iIndicesRunner, uiNumVertsPerChunk, 0);
    }
    if (!(i > 0 && !pTrack->ShouldShowChunkSection(i - 1, eShapeSection::LSHOULDER))
      && !(i == 0 && !pTrack->ShouldShowChunkSection(((int)pTrack->m_chunkAy.size() - 1), eShapeSection::LSHOULDER))) {
      MakeIndicesHelper(indices, i, uiNumIndicesPerChunk, iIndicesRunner, uiNumVertsPerChunk, 8);
    }
    if (!(i > 0 && !pTrack->ShouldShowChunkSection(i - 1, eShapeSection::RSHOULDER))
      && !(i == 0 && !pTrack->ShouldShowChunkSection(((int)pTrack->m_chunkAy.size() - 1), eShapeSection::RSHOULDER))) {
      MakeIndicesHelper(indices, i, uiNumIndicesPerChunk, iIndicesRunner, uiNumVertsPerChunk, 16);
    }
    if (!(i > 0 && !pTrack->ShouldShowChunkSection(i - 1, eShapeSection::LWALL))
      && !(i == 0 && !pTrack->ShouldShowChunkSection(((int)pTrack->m_chunkAy.size() - 1), eShapeSection::LWALL))) {
      MakeIndicesHelper(indices, i, uiNumIndicesPerChunk, iIndicesRunner, uiNumVertsPerChunk, 24);
    }
    if (!(i > 0 && !pTrack->ShouldShowChunkSection(i - 1, eShapeSection::RWALL))
      && !(i == 0 && !pTrack->ShouldShowChunkSection(((int)pTrack->m_chunkAy.size() - 1), eShapeSection::RWALL))) {
      MakeIndicesHelper(indices, i, uiNumIndicesPerChunk, iIndicesRunner, uiNumVertsPerChunk, 32);
    }
    if (!(i > 0 && !pTrack->ShouldShowChunkSection(i - 1, eShapeSection::ROOF))
      && !(i == 0 && !pTrack->ShouldShowChunkSection(((int)pTrack->m_chunkAy.size() - 1), eShapeSection::ROOF))) {
      MakeIndicesHelper(indices, i, uiNumIndicesPerChunk, iIndicesRunner, uiNumVertsPerChunk, 40);
    }
    if (!(i > 0 && !pTrack->ShouldShowChunkSection(i - 1, eShapeSection::OWALLFLOOR))
      && !(i == 0 && !pTrack->ShouldShowChunkSection(((int)pTrack->m_chunkAy.size() - 1), eShapeSection::OWALLFLOOR))) {
      MakeIndicesHelper(indices, i, uiNumIndicesPerChunk, iIndicesRunner, uiNumVertsPerChunk, 48);
    }
    if (!(i > 0 && !pTrack->ShouldShowChunkSection(i - 1, eShapeSection::LLOWALL))
      && !(i == 0 && !pTrack->ShouldShowChunkSection(((int)pTrack->m_chunkAy.size() - 1), eShapeSection::LLOWALL))) {
      MakeIndicesHelper(indices, i, uiNumIndicesPerChunk, iIndicesRunner, uiNumVertsPerChunk, 56);
    }
    if (!(i > 0 && !pTrack->ShouldShowChunkSection(i - 1, eShapeSection::RLOWALL))
      && !(i == 0 && !pTrack->ShouldShowChunkSection(((int)pTrack->m_chunkAy.size() - 1), eShapeSection::RLOWALL))) {
      MakeIndicesHelper(indices, i, uiNumIndicesPerChunk, iIndicesRunner, uiNumVertsPerChunk, 64);
    }
    if (!(i > 0 && !pTrack->ShouldShowChunkSection(i - 1, eShapeSection::LUOWALL))
      && !(i == 0 && !pTrack->ShouldShowChunkSection(((int)pTrack->m_chunkAy.size() - 1), eShapeSection::LUOWALL))) {
      MakeIndicesHelper(indices, i, uiNumIndicesPerChunk, iIndicesRunner, uiNumVertsPerChunk, 72);
    }
    if (!(i > 0 && !pTrack->ShouldShowChunkSection(i - 1, eShapeSection::RUOWALL))
      && !(i == 0 && !pTrack->ShouldShowChunkSection(((int)pTrack->m_chunkAy.size() - 1), eShapeSection::RUOWALL))) {
      MakeIndicesHelper(indices, i, uiNumIndicesPerChunk, iIndicesRunner, uiNumVertsPerChunk, 80);
    }
  }

  return indices;

}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::MakeIndicesHelper(uint32 *indices, uint32 i, uint32 uiNumIndicesPerChunk, int &iIndicesRunner, uint32 uiNumVertsPerChunk, int iPos)
{
  indices[i * uiNumIndicesPerChunk + iIndicesRunner++] = (i * uiNumVertsPerChunk) + 2 + iPos + 4;
  indices[i * uiNumIndicesPerChunk + iIndicesRunner++] = (i * uiNumVertsPerChunk) + 1 + iPos + 4;
  indices[i * uiNumIndicesPerChunk + iIndicesRunner++] = (i * uiNumVertsPerChunk) + 3 + iPos + 4;
  indices[i * uiNumIndicesPerChunk + iIndicesRunner++] = (i * uiNumVertsPerChunk) + 2 + iPos + 4;
  indices[i * uiNumIndicesPerChunk + iIndicesRunner++] = (i * uiNumVertsPerChunk) + 0 + iPos + 4;
  indices[i * uiNumIndicesPerChunk + iIndicesRunner++] = (i * uiNumVertsPerChunk) + 1 + iPos + 4;

  indices[i * uiNumIndicesPerChunk + iIndicesRunner++] = (i * uiNumVertsPerChunk) + 2 + iPos;
  indices[i * uiNumIndicesPerChunk + iIndicesRunner++] = (i * uiNumVertsPerChunk) + 3 + iPos;
  indices[i * uiNumIndicesPerChunk + iIndicesRunner++] = (i * uiNumVertsPerChunk) + 1 + iPos;
  indices[i * uiNumIndicesPerChunk + iIndicesRunner++] = (i * uiNumVertsPerChunk) + 2 + iPos;
  indices[i * uiNumIndicesPerChunk + iIndicesRunner++] = (i * uiNumVertsPerChunk) + 1 + iPos;
  indices[i * uiNumIndicesPerChunk + iIndicesRunner++] = (i * uiNumVertsPerChunk) + 0 + iPos;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesSelectedChunks(uint32 &numIndices, int iStart, int iEnd, CTrack *pTrack)
{
  if (pTrack->m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 8;
  uint32 uiNumIndicesPerChunk = 24;
  numIndices = (uint32)pTrack->m_chunkAy.size() * uiNumIndicesPerChunk;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  bool bDrawChunk0 = (iEnd == pTrack->m_chunkAy.size() - 1);
  iStart++;
  iEnd++;
  if (iEnd > pTrack->m_chunkAy.size() - 1)
    iEnd = (int)pTrack->m_chunkAy.size() - 1;

  int i = iStart;
  for (; i <= iEnd; i++) {
    indices[i * uiNumIndicesPerChunk + 0] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 1] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 2] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 3] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 4] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 5] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 6] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 7] = (i * uiNumVertsPerChunk) + 0;

    indices[i * uiNumIndicesPerChunk + 8] = (i * uiNumVertsPerChunk) + 4;
    indices[i * uiNumIndicesPerChunk + 9] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 10] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 11] = (i * uiNumVertsPerChunk) + 7;
    indices[i * uiNumIndicesPerChunk + 12] = (i * uiNumVertsPerChunk) + 7;
    indices[i * uiNumIndicesPerChunk + 13] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 14] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 15] = (i * uiNumVertsPerChunk) + 4;

    indices[i * uiNumIndicesPerChunk + 16] = (i * uiNumVertsPerChunk) + 4;
    indices[i * uiNumIndicesPerChunk + 17] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 18] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 19] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 20] = (i * uiNumVertsPerChunk) + 7;
    indices[i * uiNumIndicesPerChunk + 21] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 22] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 23] = (i * uiNumVertsPerChunk) + 1;
  }
  if (bDrawChunk0) {
    int i = 0;
    indices[i * uiNumIndicesPerChunk + 0] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 1] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 2] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 3] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 4] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 5] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 6] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 7] = (i * uiNumVertsPerChunk) + 0;

    indices[i * uiNumIndicesPerChunk + 8] = (i * uiNumVertsPerChunk) + 4;
    indices[i * uiNumIndicesPerChunk + 9] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 10] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 11] = (i * uiNumVertsPerChunk) + 7;
    indices[i * uiNumIndicesPerChunk + 12] = (i * uiNumVertsPerChunk) + 7;
    indices[i * uiNumIndicesPerChunk + 13] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 14] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 15] = (i * uiNumVertsPerChunk) + 4;

    indices[i * uiNumIndicesPerChunk + 16] = (i * uiNumVertsPerChunk) + 4;
    indices[i * uiNumIndicesPerChunk + 17] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 18] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 19] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 20] = (i * uiNumVertsPerChunk) + 7;
    indices[i * uiNumIndicesPerChunk + 21] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 22] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 23] = (i * uiNumVertsPerChunk) + 1;
  }

  return indices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesSingleSectionWireframe(uint32 &numIndices, eShapeSection section, CTrack *pTrack, bool bAttachLast)
{
  if (pTrack->m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 8;
  uint32 uiNumIndicesPerChunk = 16;
  numIndices = (uint32)pTrack->m_chunkAy.size() * uiNumIndicesPerChunk;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  uint32 i = 0;
  for (; i < pTrack->m_chunkAy.size(); i++) {
    if (!bAttachLast && i == 0)
      continue;

    indices[i * uiNumIndicesPerChunk + 0] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 1] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 2] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 3] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 4] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 5] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 6] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 7] = (i * uiNumVertsPerChunk) + 1;

    if (section == eShapeSection::LSHOULDER || section == eShapeSection::RSHOULDER) {
      indices[i * uiNumIndicesPerChunk + 8] = (i * uiNumVertsPerChunk) + 2 + 4;
      indices[i * uiNumIndicesPerChunk + 9] = (i * uiNumVertsPerChunk) + 3 + 4;
      indices[i * uiNumIndicesPerChunk + 10] = (i * uiNumVertsPerChunk) + 3 + 4;
      indices[i * uiNumIndicesPerChunk + 11] = (i * uiNumVertsPerChunk) + 1 + 4;
      indices[i * uiNumIndicesPerChunk + 12] = (i * uiNumVertsPerChunk) + 2 + 4;
      indices[i * uiNumIndicesPerChunk + 13] = (i * uiNumVertsPerChunk) + 0 + 4;
      indices[i * uiNumIndicesPerChunk + 14] = (i * uiNumVertsPerChunk) + 0 + 4;
      indices[i * uiNumIndicesPerChunk + 15] = (i * uiNumVertsPerChunk) + 1 + 4;
    }
  }

  return indices;
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::ApplyVerticesSingleSection(int i, tVertex *vertices,
                                               const glm::vec3 &v0,
                                               const glm::vec3 &v1,
                                               const glm::vec3 &v2,
                                               const glm::vec3 &v3)
{
  uint32 uiNumVertsPerChunk = 8;
  vertices[i * uiNumVertsPerChunk + 0].position = v0;
  vertices[i * uiNumVertsPerChunk + 1].position = v1;
  vertices[i * uiNumVertsPerChunk + 2].position = v2;
  vertices[i * uiNumVertsPerChunk + 3].position = v3;
  vertices[i * uiNumVertsPerChunk + 4].position = v0;
  vertices[i * uiNumVertsPerChunk + 5].position = v1;
  vertices[i * uiNumVertsPerChunk + 6].position = v2;
  vertices[i * uiNumVertsPerChunk + 7].position = v3;
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::MakeNormals(tVertex &topLeft, tVertex &topRight, tVertex &bottomLeft, tVertex &bottomRight)
{
  glm::vec3 tl1 = bottomLeft.position - topLeft.position;
  glm::vec3 tl2 = topRight.position - topLeft.position;
  topLeft.normal = glm::normalize(glm::cross(tl1, tl2));

  glm::vec3 tr1 = topLeft.position - topRight.position;
  glm::vec3 tr2 = bottomRight.position - topRight.position;
  topRight.normal = glm::normalize(glm::cross(tr1, tr2));

  glm::vec3 bl1 = bottomRight.position - bottomLeft.position;
  glm::vec3 bl2 = topLeft.position - bottomLeft.position;
  bottomLeft.normal = glm::normalize(glm::cross(bl1, bl2));

  glm::vec3 br1 = topRight.position - bottomRight.position;
  glm::vec3 br2 = bottomLeft.position - bottomRight.position;
  bottomRight.normal = glm::normalize(glm::cross(br1, br2));
}

//-------------------------------------------------------------------------------------------------