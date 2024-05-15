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
#include "TrackData.h"
#include "SignType.h"
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

CShapeFactory &CShapeFactory::GetShapeFactory()
{
  static CShapeFactory s_shapeFactory;

  return s_shapeFactory;
}

//-------------------------------------------------------------------------------------------------

CShapeFactory::CShapeFactory()
  : m_fScale(1000.0f)
{

}

//-------------------------------------------------------------------------------------------------

CShapeFactory::~CShapeFactory()
{

}

//-------------------------------------------------------------------------------------------------

CShapeData *CShapeFactory::MakeAxes(CShader *pShader)
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

tVertex *CShapeFactory::MakeVertsAxes(uint32 &uiNumVerts)
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

uint32 *CShapeFactory::MakeIndicesAxes(uint32 &uiNumIndices)
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

CShapeData *CShapeFactory::MakeModel(CShader *pShader, CTexture *pTexture, eWhipModel model, int iSignSurfaceType)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeModelVerts(uiNumVerts, pTexture, model, iSignSurfaceType);
  uint32 uiNumIndices;
  uint32 *indices = MakeModelIndices(uiNumIndices, model);

  CShapeData *pRet = NULL;

  if (vertices && indices) {
    CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts);
    CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
    CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

    pRet = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, pTexture);

    if (vertices)
      delete[] vertices;
    if (indices)
      delete[] indices;
  }

  return pRet;
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
    coordAy[i / 3].position[iVertIndex] = GetCoords(model)[i] / m_fScale;
    iVertIndex++;
    if (iVertIndex == 3)
      iVertIndex = 0;
  }

  //then get vertices for polygons
  numVertices = GetPolsCount(model) * 4;
  tVertex *vertices = new tVertex[numVertices];
  for (int i = 0; i < GetPolsCount(model); ++i) {
    vertices[i * 4 + 0] = coordAy[GetPols(model)[i].byVert1];
    vertices[i * 4 + 1] = coordAy[GetPols(model)[i].byVert2];
    vertices[i * 4 + 2] = coordAy[GetPols(model)[i].byVert3];
    vertices[i * 4 + 3] = coordAy[GetPols(model)[i].byVert4];

    uint32 uiUseTex = GetPols(model)[i].uiTex;
    if (uiUseTex & SURFACE_FLAG_ANMS_LOOKUP && GetAnms(model)) {
      uiUseTex = GetAnms(model)[uiUseTex & SURFACE_TEXTURE_INDEX].framesAy[0];
    }
    if (IsSign(model)) {
      if (GetSignTypeFromModel(model).bCanHaveTexture) {
        if (GetPols(model)[i].uiTex &SURFACE_FLAG_ANMS_LOOKUP) {
          uint32 uiSignSurfaceType = CTrackData::GetSignedBitValueFromInt(iSignSurfaceType);
          uiUseTex = uiSignSurfaceType;
        } else {
          uiUseTex &= ~SURFACE_FLAG_APPLY_TEXTURE;
        }
      }
    }
    pTexture->GetTextureCoordinates(uiUseTex,
                                    vertices[i * 4 + 1],
                                    vertices[i * 4 + 0],
                                    vertices[i * 4 + 2],
                                    vertices[i * 4 + 3]);
  }

  delete[] coordAy;
  return vertices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeModelIndices(uint32 &numIndices, eWhipModel model)
{
  int iNumIndicesPerPol = 6;
  int iNumVertsPerPol = 4;
  numIndices = GetPolsCount(model) * iNumIndicesPerPol;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  int i = 0;
  for (; i < GetPolsCount(model); i++) {
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

CShapeData *CShapeFactory::MakeAudioMarker(CShader *pShader)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVertsAudioMarker(uiNumVerts);
  uint32 uiNumIndices;
  uint32 *indices = MakeIndicesAudioMarker(uiNumIndices);
  GLenum drawType = GL_TRIANGLES;

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

tVertex *CShapeFactory::MakeVertsAudioMarker(uint32 &uiNumVerts)
{
  uiNumVerts = 6;
  tVertex *vertices = new tVertex[uiNumVerts];

  glm::vec3 color = glm::vec3(1, 1, 1);
  vertices[0].position = glm::vec3(  +0.0f / m_fScale,   +0.0f / m_fScale, +0.0f);
  vertices[1].position = glm::vec3(+500.0f / m_fScale,   +0.0f / m_fScale, +0.0f);
  vertices[2].position = glm::vec3(+500.0f / m_fScale, +500.0f / m_fScale, +0.0f);
  vertices[3].position = glm::vec3(  +0.0f / m_fScale, +500.0f / m_fScale, +0.0f);
  vertices[4].position = glm::vec3(-500.0f / m_fScale, +800.0f / m_fScale, +0.0f);
  vertices[5].position = glm::vec3(-500.0f / m_fScale, -300.0f / m_fScale, +0.0f);
  vertices[0].color = color;
  vertices[1].color = color;
  vertices[2].color = color;
  vertices[3].color = color;
  vertices[4].color = color;
  vertices[5].color = color;
  vertices[0].flags.x = 1.0f;
  vertices[1].flags.x = 1.0f;
  vertices[2].flags.x = 1.0f;
  vertices[3].flags.x = 1.0f;
  vertices[4].flags.x = 1.0f;
  vertices[5].flags.x = 1.0f;

  return vertices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesAudioMarker(uint32 &uiNumIndices)
{
  uiNumIndices = 12;
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

  return indices;
}

//-------------------------------------------------------------------------------------------------

CShapeData *CShapeFactory::MakeStuntMarker(CShader *pShader)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVertsStuntMarker(uiNumVerts);
  uint32 uiNumIndices;
  uint32 *indices = MakeIndicesStuntMarker(uiNumIndices);
  GLenum drawType = GL_TRIANGLES;

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

tVertex *CShapeFactory::MakeVertsStuntMarker(uint32 &uiNumVerts)
{
  uiNumVerts = 8;
  tVertex *vertices = new tVertex[uiNumVerts];

  glm::vec3 color = glm::vec3(0, 1, 0);
  vertices[0].position = glm::vec3(+0.0f / m_fScale, +100.0f / m_fScale, +0.0f);
  vertices[1].position = glm::vec3(+100.0f / m_fScale, +0.0f / m_fScale, +0.0f);
  vertices[2].position = glm::vec3(+400.0f / m_fScale, -300.0f / m_fScale, +0.0f);
  vertices[3].position = glm::vec3(+300.0f / m_fScale, -400.0f / m_fScale, +0.0f);
  vertices[4].position = glm::vec3(+0.0f / m_fScale, -100.0f / m_fScale, +0.0f);
  vertices[5].position = glm::vec3(-300.0f / m_fScale, -400.0f / m_fScale, +0.0f);
  vertices[6].position = glm::vec3(-400.0f / m_fScale, -300.0f / m_fScale, +0.0f);

  vertices[0].color = color;
  vertices[1].color = color;
  vertices[2].color = color;
  vertices[3].color = color;
  vertices[4].color = color;
  vertices[5].color = color;
  vertices[6].color = color;

  vertices[0].flags.x = 1.0f;
  vertices[1].flags.x = 1.0f;
  vertices[2].flags.x = 1.0f;
  vertices[3].flags.x = 1.0f;
  vertices[4].flags.x = 1.0f;
  vertices[5].flags.x = 1.0f;
  vertices[6].flags.x = 1.0f;

  return vertices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesStuntMarker(uint32 &uiNumIndices)
{
  uiNumIndices = 12;
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

  return indices;
}

//-------------------------------------------------------------------------------------------------

float *CShapeFactory::GetCoords(eWhipModel model)
{
  switch (model) {
    case CAR_F1WACK:
      return g_f1wackCoords;
    case CAR_AUTO:
      return g_xautoCoords;
    case CAR_DESILVA:
      return g_xdesilvaCoords;
    case CAR_PULSE:
      return g_xpulseCoords;
    case CAR_GLOBAL:
      return g_xglobalCoords;
    case CAR_MILLION:
      return g_xmillionCoords;
    case CAR_MISSION:
      return g_xmissionCoords;
    case CAR_ZIZIN:
      return g_xzizinCoords;
    case CAR_REISE:
      return g_xreiseCoords;
    case SIGN_TOWER:
      return g_towerCoords;
    case SIGN_TOWER2:
      return g_tower2Coords;
    case SIGN_SIGN01:
      return g_sign01Coords;
    case SIGN_SIGN02:
      return g_sign02Coords;
    case SIGN_BUILD:
      return g_buildCoords;
    case SIGN_BUILD1:
      return g_build1Coords;
    case SIGN_BUILD2:
      return g_build2Coords;
    case SIGN_BUILD3:
      return g_build3Coords;
    case SIGN_HEELBAR:
      return g_heelbarCoords;
    case SIGN_BALLOON:
      return g_balloonCoords;
    case SIGN_BALLOON2:
      return g_balloon2Coords;
    case SIGN_TREE:
      return g_treeCoords;
    case SIGN_ADVERT:
      return g_advertCoords;
    case SIGN_ADVERT2:
      return g_advert2Coords;
    case SIGN_ADVERT3:
      return g_advert3Coords;
    case SIGN_QUADBLD:
      return g_quadbldCoords;
    case SIGN_BLD0:
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
    case CAR_F1WACK:
      return g_f1wackCoordsCount;
    case CAR_AUTO:
      return g_xautoCoordsCount;
    case CAR_DESILVA:
      return g_xdesilvaCoordsCount;
    case CAR_PULSE:
      return g_xpulseCoordsCount;
    case CAR_GLOBAL:
      return g_xglobalCoordsCount;
    case CAR_MILLION:
      return g_xmillionCoordsCount;
    case CAR_MISSION:
      return g_xmissionCoordsCount;
    case CAR_ZIZIN:
      return g_xzizinCoordsCount;
    case CAR_REISE:
      return g_xreiseCoordsCount;
    case SIGN_TOWER:
      return g_towerCoordsCount;
    case SIGN_TOWER2:
      return g_tower2CoordsCount;
    case SIGN_SIGN01:
      return g_sign01CoordsCount;
    case SIGN_SIGN02:
      return g_sign02CoordsCount;
    case SIGN_BUILD:
      return g_buildCoordsCount;
    case SIGN_BUILD1:
      return g_build1CoordsCount;
    case SIGN_BUILD2:
      return g_build2CoordsCount;
    case SIGN_BUILD3:
      return g_build3CoordsCount;
    case SIGN_HEELBAR:
      return g_heelbarCoordsCount;
    case SIGN_BALLOON:
      return g_balloonCoordsCount;
    case SIGN_BALLOON2:
      return g_balloon2CoordsCount;
    case SIGN_TREE:
      return g_treeCoordsCount;
    case SIGN_ADVERT:
      return g_advertCoordsCount;
    case SIGN_ADVERT2:
      return g_advert2CoordsCount;
    case SIGN_ADVERT3:
      return g_advert3CoordsCount;
    case SIGN_QUADBLD:
      return g_quadbldCoordsCount;
    case SIGN_BLD0:
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
    case CAR_F1WACK:
      return g_f1wackPols;
    case CAR_AUTO:
      return g_xautoPols;
    case CAR_DESILVA:
      return g_xdesilvaPols;
    case CAR_PULSE:
      return g_xpulsePols;
    case CAR_GLOBAL:
      return g_xglobalPols;
    case CAR_MILLION:
      return g_xmillionPols;
    case CAR_MISSION:
      return g_xmissionPols;
    case CAR_ZIZIN:
      return g_xzizinPols;
    case CAR_REISE:
      return g_xreisePols;
    case SIGN_TOWER:
      return g_towerPols;
    case SIGN_TOWER2:
      return g_tower2Pols;
    case SIGN_SIGN01:
      return g_sign01Pols;
    case SIGN_SIGN02:
      return g_sign02Pols;
    case SIGN_BUILD:
      return g_buildPols;
    case SIGN_BUILD1:
      return g_build1Pols;
    case SIGN_BUILD2:
      return g_build2Pols;
    case SIGN_BUILD3:
      return g_build3Pols;
    case SIGN_HEELBAR:
      return g_heelbarPols;
    case SIGN_BALLOON:
      return g_balloonPols;
    case SIGN_BALLOON2:
      return g_balloon2Pols;
    case SIGN_TREE:
      return g_treePols;
    case SIGN_ADVERT:
      return g_advertPols;
    case SIGN_ADVERT2:
      return g_advert2Pols;
    case SIGN_ADVERT3:
      return g_advert3Pols;
    case SIGN_QUADBLD:
      return g_quadbldPols;
    case SIGN_BLD0:
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
    case CAR_F1WACK:
      return g_f1wackPolsCount;
    case CAR_AUTO:
      return g_xautoPolsCount;
    case CAR_DESILVA:
      return g_xdesilvaPolsCount;
    case CAR_PULSE:
      return g_xpulsePolsCount;
    case CAR_GLOBAL:
      return g_xglobalPolsCount;
    case CAR_MILLION:
      return g_xmillionPolsCount;
    case CAR_MISSION:
      return g_xmissionPolsCount;
    case CAR_ZIZIN:
      return g_xzizinPolsCount;
    case CAR_REISE:
      return g_xreisePolsCount;
    case SIGN_TOWER:
      return g_towerPolsCount;
    case SIGN_TOWER2:
      return g_tower2PolsCount;
    case SIGN_SIGN01:
      return g_sign01PolsCount;
    case SIGN_SIGN02:
      return g_sign02PolsCount;
    case SIGN_BUILD:
      return g_buildPolsCount;
    case SIGN_BUILD1:
      return g_build1PolsCount;
    case SIGN_BUILD2:
      return g_build2PolsCount;
    case SIGN_BUILD3:
      return g_build3PolsCount;
    case SIGN_HEELBAR:
      return g_heelbarPolsCount;
    case SIGN_BALLOON:
      return g_balloonPolsCount;
    case SIGN_BALLOON2:
      return g_balloon2PolsCount;
    case SIGN_TREE:
      return g_treePolsCount;
    case SIGN_ADVERT:
      return g_advertPolsCount;
    case SIGN_ADVERT2:
      return g_advert2PolsCount;
    case SIGN_ADVERT3:
      return g_advert3PolsCount;
    case SIGN_QUADBLD:
      return g_quadbldPolsCount;
    case SIGN_BLD0:
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
    case CAR_F1WACK:
      return NULL;
    case CAR_AUTO:
      return g_xautoAnms;
    case CAR_DESILVA:
      return g_xdesilvaAnms;
    case CAR_PULSE:
      return g_xpulseAnms;
    case CAR_GLOBAL:
      return g_xglobalAnms;
    case CAR_MILLION:
      return g_xmillionAnms;
    case CAR_MISSION:
      return g_xmissionAnms;
    case CAR_ZIZIN:
      return g_xzizinAnms;
    case CAR_REISE:
      return g_xreiseAnms;
    case SIGN_TOWER:
      return g_towerAnms;
    case SIGN_TOWER2:
      return NULL;
    case SIGN_SIGN01:
      return g_sign01Anms;
    case SIGN_SIGN02:
      return g_sign02Anms;
    case SIGN_BUILD:
      return NULL;
    case SIGN_BUILD1:
      return NULL;
    case SIGN_BUILD2:
      return NULL;
    case SIGN_BUILD3:
      return g_build3Anms;
    case SIGN_HEELBAR:
      return NULL;
    case SIGN_BALLOON:
      return NULL;
    case SIGN_BALLOON2:
      return g_balloon2Anms;
    case SIGN_TREE:
      return NULL;
    case SIGN_ADVERT:
      return g_advertAnms;
    case SIGN_ADVERT2:
      return g_advert2Anms;
    case SIGN_ADVERT3:
      return g_advert3Anms;
    case SIGN_QUADBLD:
      return NULL;
    case SIGN_BLD0:
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
    case CAR_F1WACK:
      return 0;
    case CAR_AUTO:
      return g_xautoAnmsCount;
    case CAR_DESILVA:
      return g_xdesilvaAnmsCount;
    case CAR_PULSE:
      return g_xpulseAnmsCount;
    case CAR_GLOBAL:
      return g_xglobalAnmsCount;
    case CAR_MILLION:
      return g_xmillionAnmsCount;
    case CAR_MISSION:
      return g_xmissionAnmsCount;
    case CAR_ZIZIN:
      return g_xzizinAnmsCount;
    case CAR_REISE:
      return g_xreiseAnmsCount;
    case SIGN_TOWER:
      return g_towerAnmsCount;
    case SIGN_TOWER2:
      return 0;
    case SIGN_SIGN01:
      return g_sign01AnmsCount;
    case SIGN_SIGN02:
      return g_sign02AnmsCount;
    case SIGN_BUILD:
      return 0;
    case SIGN_BUILD1:
      return 0;
    case SIGN_BUILD2:
      return 0;
    case SIGN_BUILD3:
      return g_build3AnmsCount;
    case SIGN_HEELBAR:
      return 0;
    case SIGN_BALLOON:
      return 0;
    case SIGN_BALLOON2:
      return g_balloon2AnmsCount;
    case SIGN_TREE:
      return 0;
    case SIGN_ADVERT:
      return g_advertAnmsCount;
    case SIGN_ADVERT2:
      return g_advert2AnmsCount;
    case SIGN_ADVERT3:
      return g_advert3AnmsCount;
    case SIGN_QUADBLD:
      return 0;
    case SIGN_BLD0:
      return 0;
    default:
      assert(0);
      return NULL;
  }
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesCenterline(uint32 &numIndices, CTrackData *pTrack, bool bAttachLast)
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

uint32 *CShapeFactory::MakeIndicesSurface(uint32 &numIndices, CTrackData *pTrack, bool bAttachLast)
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

uint32 *CShapeFactory::MakeIndicesSurfaceWireframe(uint32 &numIndices, CTrackData *pTrack, bool bAttachLast)
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

CShapeData *CShapeFactory::MakeTrackSurface(CShader *pShader, CTrackData *pTrack, eShapeSection section, bool bAttachLast, bool bWireframe)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = NULL;
  vertices = MakeVerts(uiNumVerts, section, pTrack, pTrack->m_pTex);
  uint32 uiNumIndices;
  uint32 *indices = NULL;
  GLenum drawType = GL_TRIANGLES;
  if (!bWireframe) {
    indices = MakeIndicesSingleSection(uiNumIndices, section, pTrack, bAttachLast);
  } else {
    indices = MakeIndicesSingleSectionWireframe(uiNumIndices, pTrack, bAttachLast);
    drawType = GL_LINES;

    for (uint32 i = 0; i < uiNumVerts; ++i) {
      vertices[i].flags.x = 1.0f; //use color rather than tex
      vertices[i].color = CTexture::RandomColor();
    }
  }

  CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts);
  CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
  CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

  CShapeData *pRet = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, pTrack->m_pTex, drawType);

  if (vertices)
    delete[] vertices;
  if (indices)
    delete[] indices;

  return pRet;
}

//-------------------------------------------------------------------------------------------------

CShapeData *CShapeFactory::MakeEnvirFloor(CShader *pShader, CTrackData *pTrack, int iIndex)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = NULL;
  vertices = MakeVertsEnvirFloor(uiNumVerts, pTrack, pTrack->m_pTex, iIndex);
  if (!vertices)
    return NULL;

  uint32 uiNumIndices;
  uint32 *indices = NULL;
  GLenum drawType = GL_TRIANGLES;
  indices = MakeIndicesEnvirFloor(uiNumIndices);

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

CShapeData *CShapeFactory::MakeAILine(CShader *pShader, CTrackData *pTrack, eShapeSection section, bool bAttachLast)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVerts(uiNumVerts, section, pTrack, NULL);
  uint32 uiNumIndices;
  uint32 *indices = MakeIndicesCenterline(uiNumIndices, pTrack, bAttachLast);
  GLenum drawType = GL_LINES;

  for (uint32 i = 0; i < uiNumVerts; ++i) {
    vertices[i].flags.x = 1.0f; //use color rather than tex
    switch (section) {
      case eShapeSection::AILINE1:
        vertices[i].color = glm::vec3(1.0f, 0.0f, 0.0f);
        break;
      case eShapeSection::AILINE2:
        vertices[i].color = glm::vec3(0.0f, 1.0f, 0.0f);
        break;
      case eShapeSection::AILINE3:
        vertices[i].color = glm::vec3(0.0f, 0.0f, 1.0f);
        break;
      case eShapeSection::AILINE4:
        vertices[i].color = glm::vec3(1.0f, 1.0f, 0.0f);
        break;
    }
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

CShapeData *CShapeFactory::MakeSelectedChunks(CShader *pShader, CTrackData *pTrack, int iStart, int iEnd)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVerts(uiNumVerts, eShapeSection::SELECTED, pTrack, NULL);
  uint32 uiNumIndices;
  uint32 *indices = MakeIndicesSelectedChunks(uiNumIndices, iStart, iEnd, pTrack);
  GLenum drawType = GL_LINES;

  for (uint32 i = 0; i < uiNumVerts; ++i) {
    vertices[i].flags.x = 1.0f; //use color rather than tex
    vertices[i].color = CTexture::RandomColor();
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

void CShapeFactory::MakeSigns(CShader *pShader, CTrackData *pTrack, std::vector<CShapeData *> &signAy)
{
  for (int i = 0; i < (int)pTrack->m_chunkAy.size(); ++i) {
    if (pTrack->m_chunkAy[i].iSignType < 0 || pTrack->m_chunkAy[i].iSignType >= g_signAyCount)
      continue; //no signs in this chunk

    //make sign
    CShapeData *pNewSign = MakeModel(pShader, pTrack->m_pBld, g_signAy[pTrack->m_chunkAy[i].iSignType].modelType, pTrack->m_chunkAy[i].iSignTexture);
    if (!pNewSign)
      continue;

    uint32 uiSignSurfaceType = CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[i].iSignTexture);

    //position sign
    bool bReversedOffsets = pTrack->m_chunkAy[i].dPitch >= 90.0f && pTrack->m_chunkAy[i].dPitch < 270.0f;
    float fLen = (float)pTrack->m_chunkAy[i].iSignHorizOffset * 15.0f / m_fScale * (bReversedOffsets ? 1.0f : -1.0f);
    float fHeight = (float)pTrack->m_chunkAy[i].iSignVertOffset * 15.0f / m_fScale * (bReversedOffsets ? 1.0f : -1.0f);

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

void CShapeFactory::MakeAudio(CShader *pShader, CTrackData *pTrack, std::vector<CShapeData *> &audioAy)
{
  for (int i = 0; i < (int)pTrack->m_chunkAy.size(); ++i) {
    bool bChunkHasAudio = pTrack->m_chunkAy[i].iAudioTriggerSpeed != 0;
    if (!bChunkHasAudio)
      continue; //no audio in this chunk

    //make marker
    CShapeData *pNewMarker = MakeAudioMarker(pShader);

    float fHeight = (float)1000.0f / m_fScale * -1.0f;
    glm::mat4 translateMat = glm::translate(pTrack->m_chunkAy[i].math.center);
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

void CShapeFactory::MakeStunts(CShader *pShader, CTrackData *pTrack, std::vector<CShapeData *> &stuntAy)
{
  for (int i = 0; i < (int)pTrack->m_chunkAy.size(); ++i) {
    bool bChunkHasStunt = pTrack->m_chunkAy[i].stunt.iChunkCount != 0
      || pTrack->m_chunkAy[i].stunt.iNumTicks != 0
      || pTrack->m_chunkAy[i].stunt.iTickStartIdx != 0
      || pTrack->m_chunkAy[i].stunt.iTimingGroup != 0
      || pTrack->m_chunkAy[i].stunt.iHeight != 0
      || pTrack->m_chunkAy[i].stunt.iTimeBulging != 0
      || pTrack->m_chunkAy[i].stunt.iTimeFlat != 0
      || pTrack->m_chunkAy[i].stunt.iRampSideLength != 0
      || pTrack->m_chunkAy[i].stunt.iFlags != 0;
    if (!bChunkHasStunt)
      continue; //no stunt in this chunk

    //make marker
    CShapeData *pNewMarker = MakeStuntMarker(pShader);

    float fHeight = (float)1000.0f / m_fScale * -1.0f;
    glm::mat4 translateMat = glm::translate(pTrack->m_chunkAy[i].math.center);
    glm::mat4 scaleMatHeight = glm::scale(glm::vec3(fHeight, fHeight, fHeight));
    glm::vec3 normal = glm::normalize(glm::cross(pTrack->m_chunkAy[i].math.nextChunkPitched, pTrack->m_chunkAy[i].math.pitchAxis));
    glm::vec3 heightVec = glm::vec3(scaleMatHeight * pTrack->m_chunkAy[i].math.rollMat * glm::vec4(normal, 1.0f));
    glm::vec3 markerPos = heightVec;
    glm::vec3 markerPosTranslated = glm::vec3(translateMat * glm::vec4(markerPos, 1.0f));

    pNewMarker->m_modelToWorldMatrix = glm::translate(markerPosTranslated) *
      pTrack->m_chunkAy[i].math.rollMat * pTrack->m_chunkAy[i].math.pitchMat * pTrack->m_chunkAy[i].math.yawMat;

    //add sign to array
    stuntAy.push_back(pNewMarker);
  }
}

//-------------------------------------------------------------------------------------------------

//CShapeData ShapeGenerator::GenerateNormals(const CShapeData &data)
//{
//  CShapeData ret;
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
//}*/

//-------------------------------------------------------------------------------------------------

void CShapeFactory::GetCarPos(CTrackData *pTrack, int iChunk, eShapeSection aiLineSection, glm::mat4 &modelToWorldMatrix, bool bMillionPlus)
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

tVertex *CShapeFactory::MakeVerts(uint32 &numVertices, eShapeSection section, CTrackData *pTrack, CTexture *pTexture)
{
  if (pTrack->m_chunkAy.empty()) {
    numVertices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 4;
  if (section == eShapeSection::SELECTED) uiNumVertsPerChunk = 8;
  if (section >= eShapeSection::AILINE1 && section <= eShapeSection::AILINE4) uiNumVertsPerChunk = 1;

  numVertices = (uint32)pTrack->m_chunkAy.size() * uiNumVertsPerChunk;
  tVertex *vertices = new tVertex[numVertices];
  for (uint32 i = 0; i < pTrack->m_chunkAy.size(); ++i) {
    int iChunkIndex = (int)pTrack->m_chunkAy.size() - 1;
    if (i > 0)
      iChunkIndex = i - 1;

    switch (section) {
      case LLANE:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.lLane, 
                                   pTrack->m_chunkAy[i].math.center, 
                                   pTrack->m_chunkAy[iChunkIndex].math.lLane,
                                   pTrack->m_chunkAy[iChunkIndex].math.center);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iCenterSurfaceType),
                                vertices[i * uiNumVertsPerChunk + 0],
                                vertices[i * uiNumVertsPerChunk + 1],
                                vertices[i * uiNumVertsPerChunk + 2],
                                vertices[i * uiNumVertsPerChunk + 3], true, false);
        break;
      case RLANE:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.center, 
                                   pTrack->m_chunkAy[i].math.rLane, 
                                   pTrack->m_chunkAy[iChunkIndex].math.center,
                                   pTrack->m_chunkAy[iChunkIndex].math.rLane);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iCenterSurfaceType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3], false, true);
        break;
      case LSHOULDER:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.lShoulder, 
                                   pTrack->m_chunkAy[i].math.lLane, 
                                   pTrack->m_chunkAy[iChunkIndex].math.lShoulder,
                                   pTrack->m_chunkAy[iChunkIndex].math.lLane);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftSurfaceType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3]);
        break;
      case RSHOULDER:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.rLane, 
                                   pTrack->m_chunkAy[i].math.rShoulder, 
                                   pTrack->m_chunkAy[iChunkIndex].math.rLane,
                                   pTrack->m_chunkAy[iChunkIndex].math.rShoulder);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightSurfaceType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3]);
        break;
      case LWALL:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.lWall, 
                                   pTrack->m_chunkAy[i].math.lWallBottomAttach, 
                                   pTrack->m_chunkAy[iChunkIndex].math.lWall,
                                   pTrack->m_chunkAy[iChunkIndex].math.lWallBottomAttach);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftWallType),
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 1]);
        break;
      case RWALL:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.rWallBottomAttach, 
                                   pTrack->m_chunkAy[i].math.rWall, 
                                   pTrack->m_chunkAy[iChunkIndex].math.rWallBottomAttach,
                                   pTrack->m_chunkAy[iChunkIndex].math.rWall);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightWallType),
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 2]);
        break;
      case ROOF:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.rWall, 
                                   pTrack->m_chunkAy[i].math.lWall, 
                                   pTrack->m_chunkAy[iChunkIndex].math.rWall,
                                   pTrack->m_chunkAy[iChunkIndex].math.lWall);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRoofType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3]);
        break;
      case OWALLFLOOR:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.lFloor, 
                                   pTrack->m_chunkAy[i].math.rFloor, 
                                   pTrack->m_chunkAy[iChunkIndex].math.lFloor,
                                   pTrack->m_chunkAy[iChunkIndex].math.rFloor);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iOuterFloorType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3]);
        break;
      case LLOWALL:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.lloWall, 
                                   pTrack->m_chunkAy[i].math.lloWallBottomAttach, 
                                   pTrack->m_chunkAy[iChunkIndex].math.lloWall,
                                   pTrack->m_chunkAy[iChunkIndex].math.lloWallBottomAttach);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLLOuterWallType),
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 1]);
        break;
      case RLOWALL:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.rloWallBottomAttach, 
                                   pTrack->m_chunkAy[i].math.rloWall, 
                                   pTrack->m_chunkAy[iChunkIndex].math.rloWallBottomAttach,
                                   pTrack->m_chunkAy[iChunkIndex].math.rloWall);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRLOuterWallType),
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 1]);
        break;
      case LUOWALL:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.luoWall, 
                                   pTrack->m_chunkAy[i].math.lloWall, 
                                   pTrack->m_chunkAy[iChunkIndex].math.luoWall,
                                   pTrack->m_chunkAy[iChunkIndex].math.lloWall);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLUOuterWallType),
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 1]);
        break;
      case RUOWALL:
        ApplyVerticesSingleSection(i, vertices, 
                                   pTrack->m_chunkAy[i].math.rloWall, 
                                   pTrack->m_chunkAy[i].math.ruoWall, 
                                   pTrack->m_chunkAy[iChunkIndex].math.rloWall,
                                   pTrack->m_chunkAy[iChunkIndex].math.ruoWall);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRUOuterWallType),
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 1]);
        break;
      case SELECTED:
        vertices[i * uiNumVertsPerChunk + 0].position = pTrack->m_chunkAy[i].math.luoWall;
        vertices[i * uiNumVertsPerChunk + 1].position = pTrack->m_chunkAy[i].math.ruoWall;
        vertices[i * uiNumVertsPerChunk + 2].position = pTrack->m_chunkAy[i].math.lloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + 3].position = pTrack->m_chunkAy[i].math.rloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + 4].position = pTrack->m_chunkAy[iChunkIndex].math.luoWall;
        vertices[i * uiNumVertsPerChunk + 5].position = pTrack->m_chunkAy[iChunkIndex].math.ruoWall;
        vertices[i * uiNumVertsPerChunk + 6].position = pTrack->m_chunkAy[iChunkIndex].math.lloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + 7].position = pTrack->m_chunkAy[iChunkIndex].math.rloWallBottomAttach;
        break;
      case AILINE1:
        vertices[i * uiNumVertsPerChunk + 0].position = pTrack->m_chunkAy[i].math.aiLine1;
        break;
      case AILINE2:
        vertices[i * uiNumVertsPerChunk + 0].position = pTrack->m_chunkAy[i].math.aiLine2;
        break;
      case AILINE3:
        vertices[i * uiNumVertsPerChunk + 0].position = pTrack->m_chunkAy[i].math.aiLine3;
        break;
      case AILINE4:
        vertices[i * uiNumVertsPerChunk + 0].position = pTrack->m_chunkAy[i].math.aiLine4;
        break;
      default:
        assert(0); //shape not implemented
        break;
    }
  }
  return vertices;
}

//-------------------------------------------------------------------------------------------------

tVertex *CShapeFactory::MakeVertsEnvirFloor(uint32 &numVertices, CTrackData *pTrack, CTexture *pTexture, int iIndex)
{
  if (pTrack->m_chunkAy.empty() || iIndex >= pTrack->m_chunkAy.size()) {
    numVertices = 0;
    return NULL;
  }

  numVertices = 4;
  tVertex *vertices = new tVertex[numVertices];
  float fEnvirFloorDepth = (float)pTrack->m_header.iFloorDepth / m_fScale * -1.0f;
  float fPadding = 1000.0f;
  vertices[0].position = glm::vec3(-fPadding, fEnvirFloorDepth, -fPadding);
  vertices[1].position = glm::vec3(-fPadding, fEnvirFloorDepth, +fPadding);
  vertices[2].position = glm::vec3(+fPadding, fEnvirFloorDepth, -fPadding);
  vertices[3].position = glm::vec3(+fPadding, fEnvirFloorDepth, +fPadding);
  pTexture->GetTextureCoordinates(
    CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iIndex].iEnvironmentFloorType) & SURFACE_TEXTURE_INDEX,
    vertices[0], vertices[1], vertices[2], vertices[3]);
  vertices[0].flags.x = 1.0f;
  vertices[1].flags.x = 1.0f;
  vertices[2].flags.x = 1.0f;
  vertices[3].flags.x = 1.0f;

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
  indices[1] = 3;
  indices[2] = 1;
  indices[3] = 2;
  indices[4] = 1;
  indices[5] = 0;
  return indices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesSingleSection(uint32 &numIndices, eShapeSection section, CTrackData *pTrack, bool bAttachLast)
{
  if (pTrack->m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 4;
  uint32 uiNumIndicesPerChunk = 6;
  numIndices = (uint32)pTrack->m_chunkAy.size() * uiNumIndicesPerChunk;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  uint32 i = 0;
  for (; i < pTrack->m_chunkAy.size(); i++) {
    //if (section == eShapeSection::LWALL || section == eShapeSection::RWALL) {
    //  //walls don't draw if forward section isn't drawn
    //  if (!ShouldMakeIndicesForChunk(i, section))
    //    continue;
    //}
    if (i > 0 && !pTrack->ShouldShowChunkSection(i - 1, section))
      continue;
    else if (i == 0 && !pTrack->ShouldShowChunkSection(((int)pTrack->m_chunkAy.size() - 1), section))
      continue;
    //else if (i == 0 && !ShouldMakeIndicesForChunk(((int)pTrack->m_chunkAy.size() - (int)i + 1), section))
    //  continue;
    if (!bAttachLast && i == 0)
      continue;

    indices[i * uiNumIndicesPerChunk + 0] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 1] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 2] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 3] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 4] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 5] = (i * uiNumVertsPerChunk) + 0;
  }

  return indices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesSelectedChunks(uint32 &numIndices, int iStart, int iEnd, CTrackData *pTrack)
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

uint32 *CShapeFactory::MakeIndicesSingleSectionWireframe(uint32 &numIndices, CTrackData *pTrack, bool bAttachLast)
{
  if (pTrack->m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 4;
  uint32 uiNumIndicesPerChunk = 8;
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
  }

  return indices;
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::ApplyVerticesSingleSection(int i, tVertex *vertices, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3)
{
  uint32 uiNumVertsPerChunk = 4;
  vertices[i * uiNumVertsPerChunk + 0].position = v0;
  vertices[i * uiNumVertsPerChunk + 1].position = v1;
  vertices[i * uiNumVertsPerChunk + 2].position = v2;
  vertices[i * uiNumVertsPerChunk + 3].position = v3;
}

//-------------------------------------------------------------------------------------------------