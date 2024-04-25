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
  : m_fScale(10000.0f)
  , m_iAILineHeight(100)
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

CShapeData *CShapeFactory::MakeModel(CShader *pShader, CTexture *pTexture, eWhipModel model)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeModelVerts(uiNumVerts, pTexture, model);
  uint32 uiNumIndices;
  uint32 *indices = MakeModelIndices(uiNumIndices, model);

  CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts);
  CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
  CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

  CShapeData *pRet = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, pTexture);

  if (vertices)
    delete[] vertices;
  if (indices)
    delete[] indices;

  return pRet;
}

//-------------------------------------------------------------------------------------------------

tVertex *CShapeFactory::MakeModelVerts(uint32 &numVertices, CTexture *pTexture, eWhipModel model)
{
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

CShapeData *CShapeFactory::MakeTrackCenterline(CShader *pShader, CTrackData *pTrack)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVerts(uiNumVerts, eShapeSection::CENTER, pTrack, NULL);
  uint32 uiNumIndices;
  uint32 *indices = MakeIndicesCenterline(uiNumIndices, pTrack);

  CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts);
  CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
  CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

  CShapeData *pRet = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, NULL, GL_LINES);

  if (vertices)
    delete[] vertices;
  if (indices)
    delete[] indices;

  return pRet;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesCenterline(uint32 &numIndices, CTrackData *pTrack)
{
  if (pTrack->m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  numIndices = (uint32)pTrack->m_chunkAy.size() * 2;
  uint32 *indices = new uint32[numIndices];

  for (uint32 i = 1; i < numIndices + 1; i++) {
    indices[i - 1] = i / 2;
  }
  indices[numIndices - 1] = 0;

  return indices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesSurface(uint32 &numIndices, CTrackData *pTrack)
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

  return indices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesSurfaceWireframe(uint32 &numIndices, CTrackData *pTrack)
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

  return indices;
}

//-------------------------------------------------------------------------------------------------

CShapeData *CShapeFactory::MakeTrackSurface(CShader *pShader, CTexture *pTexture, CTrackData *pTrack, eShapeSection section, bool bWireframe)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = NULL;
  if (section != eShapeSection::ENVIRFLOOR)
    vertices = MakeVerts(uiNumVerts, section, pTrack, pTexture);
  else
    vertices = MakeVertsEnvirFloor(uiNumVerts, pTrack);
  uint32 uiNumIndices;
  uint32 *indices = NULL;
  GLenum drawType = GL_TRIANGLES;
  if (!bWireframe) {
    if (section == DRIVING_SURFACE)
      indices = MakeIndicesSurface(uiNumIndices, pTrack);
    else
      indices = MakeIndicesSingleSection(uiNumIndices, section, pTrack);
  } else {
    indices = MakeIndicesSingleSectionWireframe(uiNumIndices, pTrack);
    drawType = GL_LINES;

    for (uint32 i = 0; i < uiNumVerts; ++i) {
      vertices[i].flags.x = 1.0f; //use color rather than tex
      vertices[i].color = CTexture::RandomColor();
    }
  }

  CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts);
  CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
  CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

  CShapeData *pRet = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, pTexture, drawType);

  if (vertices)
    delete[] vertices;
  if (indices)
    delete[] indices;

  return pRet;
}

//-------------------------------------------------------------------------------------------------

CShapeData *CShapeFactory::MakeAILine(CShader *pShader, CTrackData *pTrack, eShapeSection section)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVerts(uiNumVerts, section, pTrack, NULL);
  uint32 uiNumIndices;
  uint32 *indices = MakeIndicesCenterline(uiNumIndices, pTrack);
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

void CShapeFactory::MakeSigns(CShader *pShader, CTexture *pBld, CTrackData *pTrack, std::vector<CShapeData *> &signAy)
{
  glm::vec3 prevCenter = glm::vec3(0, 0, 1);
  for (int i = 0; i < (int)pTrack->m_chunkAy.size(); ++i) {
    glm::vec3 nextChunkPitched = glm::vec3(0, 0, 1);
    glm::mat4 yawMat;
    glm::mat4 pitchMat;
    glm::mat4 rollMat;
    glm::vec3 center;
    glm::vec3 pitchAxis;;
    GetCenter(pTrack, i, prevCenter, center, pitchAxis, nextChunkPitched, yawMat, pitchMat, rollMat);
    prevCenter = center;

    if (pTrack->m_chunkAy[i].iSignType < 0 || pTrack->m_chunkAy[i].iSignType >= m_signAyCount)
      continue; //no signs in this chunk

    //make sign
    CShapeData *pNewSign = MakeModel(pShader, pBld, m_signAy[pTrack->m_chunkAy[i].iSignType]);

    //position sign
    float fLen = (float)pTrack->m_chunkAy[i].iSignHorizOffset / m_fScale * -1.0f;
    float fHeight = (float)pTrack->m_chunkAy[i].iSignVertOffset / m_fScale * -1.0f;
    glm::mat4 translateMat = glm::translate(center);
    glm::mat4 scaleMatWidth = glm::scale(glm::vec3(fLen, fLen, fLen));
    glm::mat4 scaleMatHeight = glm::scale(glm::vec3(fHeight, fHeight, fHeight));
    glm::vec3 widthVec = glm::vec3(scaleMatWidth * rollMat * glm::vec4(pitchAxis, 1.0f));
    glm::vec3 normal = glm::normalize(glm::cross(nextChunkPitched, pitchAxis));
    glm::vec3 heightVec = glm::vec3(scaleMatHeight * rollMat * glm::vec4(normal, 1.0f));
    glm::vec3 signPos = widthVec + heightVec;
    glm::vec3 signPosTranslated = glm::vec3(translateMat * glm::vec4(signPos, 1.0f));
    pNewSign->m_modelToWorldMatrix = glm::translate(signPosTranslated);
    
    //add sign to array
    signAy.push_back(pNewSign);
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
  if (pTrack->m_chunkAy.empty() || iChunk > (int)pTrack->m_chunkAy.size())
    return;

  glm::vec3 prevCenter = glm::vec3(0, 0, 1);
  glm::vec3 aiLine = glm::vec3(0, 0, 1);
  glm::vec3 nextChunkPitched = glm::vec3(0, 0, 1);
  glm::mat4 yawMat;
  glm::mat4 pitchMat;
  glm::mat4 rollMat;
  for (int i = 0; i <= iChunk; ++i) {
    glm::vec3 center;
    glm::vec3 pitchAxis;;
    GetCenter(pTrack, i, prevCenter, center, pitchAxis, nextChunkPitched, yawMat, pitchMat, rollMat);
    GetAILine(pTrack, i, center, pitchAxis, rollMat, nextChunkPitched, aiLine, aiLineSection, 0);

    prevCenter = center;
  }

  float fRotate = -90.0f;
  if (bMillionPlus)
    fRotate = 90.0f;
  modelToWorldMatrix = glm::translate(aiLine) * rollMat * pitchMat * yawMat *
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
  glm::vec3 prevCenter = glm::vec3(0, 0, 1);
  glm::vec3 prevLLane = glm::vec3(0, 0, 1);
  glm::vec3 prevRLane = glm::vec3(0, 0, 1);
  glm::vec3 prevLShoulder = glm::vec3(0, 0, 1);
  glm::vec3 prevRShoulder = glm::vec3(0, 0, 1);
  glm::vec3 prevLWall = glm::vec3(0, 0, 1);
  glm::vec3 prevRWall = glm::vec3(0, 0, 1);
  glm::vec3 prevLWallBottomAttach = glm::vec3(0, 0, 1);
  glm::vec3 prevRWallBottomAttach = glm::vec3(0, 0, 1);
  bool bPrevLWallAttachToLane = false;
  bool bPrevRWallAttachToLane = false;
  glm::vec3 prevLFloor = glm::vec3(0, 0, 1);
  glm::vec3 prevRFloor = glm::vec3(0, 0, 1);
  glm::vec3 prevLLOWall = glm::vec3(0, 0, 1);
  glm::vec3 prevRLOWall = glm::vec3(0, 0, 1);
  glm::vec3 prevLLOWallBottomAttach = glm::vec3(0, 0, 1);
  glm::vec3 prevRLOWallBottomAttach = glm::vec3(0, 0, 1);
  glm::vec3 prevLUOWall = glm::vec3(0, 0, 1);
  glm::vec3 prevRUOWall = glm::vec3(0, 0, 1);
  glm::vec3 prevLUOWallBottomAttach = glm::vec3(0, 0, 1);
  glm::vec3 prevRUOWallBottomAttach = glm::vec3(0, 0, 1);
  for (uint32 i = 0; i < pTrack->m_chunkAy.size(); ++i) {
    glm::vec3 center;
    glm::vec3 pitchAxis;
    glm::vec3 nextChunkPitched;
    glm::mat4 yawMat;
    glm::mat4 pitchMat;
    glm::mat4 rollMat;
    glm::mat4 rollMatNoRoll = glm::mat4(1);
    GetCenter(pTrack, i, prevCenter, center, pitchAxis, nextChunkPitched, yawMat, pitchMat, rollMat);

    int iChunkIndex = (int)pTrack->m_chunkAy.size() - 1;
    if (i > 0)
      iChunkIndex = i - 1;

    //left lane
    glm::vec3 lLane;
    GetLane(pTrack, i, center, pitchAxis, rollMat, lLane, true);
    //right lane
    glm::vec3 rLane;
    GetLane(pTrack, i, center, pitchAxis, rollMat, rLane, false);
    //left shoulder
    glm::vec3 lShoulder;
    GetShoulder(pTrack, i, lLane, pitchAxis, rollMat, nextChunkPitched, lShoulder, true);
    //right shoulder
    glm::vec3 rShoulder;
    GetShoulder(pTrack, i, rLane, pitchAxis, rollMat, nextChunkPitched, rShoulder, false);
    //left wall
    bool bLWallAttachToLane = CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[i].iLeftWallType) & SURFACE_FLAG_WALL_31;
    if (pTrack->m_chunkAy[i].iLeftWallType == -1)
      bLWallAttachToLane = bPrevLWallAttachToLane;
    bPrevLWallAttachToLane = bLWallAttachToLane;
    glm::vec3 lWallBottomAttach = bLWallAttachToLane ? lLane : lShoulder;
    glm::vec3 lWall;
    GetWall(pTrack, i, lWallBottomAttach, pitchAxis, rollMat, nextChunkPitched, lWall, eShapeSection::LWALL);
    //right wall
    bool bRWallAttachToLane = CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[i].iRightWallType) & SURFACE_FLAG_WALL_31;
    if (pTrack->m_chunkAy[i].iRightWallType == -1)
      bRWallAttachToLane = bPrevRWallAttachToLane;
    bPrevRWallAttachToLane = bRWallAttachToLane;
    glm::vec3 rWallBottomAttach = bRWallAttachToLane ? rLane : rShoulder;
    glm::vec3 rWall;
    GetWall(pTrack, i, rWallBottomAttach, pitchAxis, rollMat, nextChunkPitched, rWall, eShapeSection::RWALL);
    //outer floor
    glm::vec3 lLaneNoRoll;
    GetLane(pTrack, i, center, pitchAxis, rollMatNoRoll, lLaneNoRoll, true);
    glm::vec3 rLaneNoRoll;
    GetLane(pTrack, i, center, pitchAxis, rollMatNoRoll, rLaneNoRoll, false);
    glm::vec3 lFloor;
    glm::vec3 rFloor;
    GetOWallFloor(pTrack, i, lLaneNoRoll, rLaneNoRoll, pitchAxis, nextChunkPitched, lFloor, rFloor);
    //outer wall roll mat
    glm::mat4 oWallRollMat = pTrack->m_chunkAy[i].iOuterFloorType < 0 ? rollMat : rollMatNoRoll;
    //llowall
    bool bLLOWallNonSolid = CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[i].iLLOuterWallType) & SURFACE_FLAG_NON_SOLID;
    glm::vec3 lloWallBottomAttach = lFloor;
    if (pTrack->m_chunkAy[i].iOuterFloorType < 0) {
      if (bLLOWallNonSolid) {
        glm::vec3 lShoulderNoHeight;
        GetShoulder(pTrack, i, lLane, pitchAxis, rollMat, nextChunkPitched, lShoulderNoHeight, true, true);
        lloWallBottomAttach = lShoulderNoHeight;
      } else {
        lloWallBottomAttach = lShoulder;
      }
    }
    glm::vec3 lloWall;
    GetWall(pTrack, i, lloWallBottomAttach, pitchAxis, oWallRollMat, nextChunkPitched, lloWall, eShapeSection::LLOWALL);
    //rlowall
    bool bRLOWallNonSolid = CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[i].iRLOuterWallType) & SURFACE_FLAG_NON_SOLID;
    glm::vec3 rloWallBottomAttach = rFloor;
    if (pTrack->m_chunkAy[i].iOuterFloorType < 0) {
      if (bRLOWallNonSolid) {
        glm::vec3 rShoulderNoHeight;
        GetShoulder(pTrack, i, rLane, pitchAxis, rollMat, nextChunkPitched, rShoulderNoHeight, false, true);
        rloWallBottomAttach = rShoulderNoHeight;
      } else {
        rloWallBottomAttach = rShoulder;
      }
    }
    glm::vec3 rloWall;
    GetWall(pTrack, i, rloWallBottomAttach, pitchAxis, oWallRollMat, nextChunkPitched, rloWall, eShapeSection::RLOWALL);
    //luowall
    glm::vec3 luoWall;
    glm::vec3 luoWallBottomAttach = lloWall;
    GetWall(pTrack, i, luoWallBottomAttach, pitchAxis, oWallRollMat, nextChunkPitched, luoWall, eShapeSection::LUOWALL);
    //ruowall
    glm::vec3 ruoWall;
    glm::vec3 ruoWallBottomAttach = rloWall;
    GetWall(pTrack, i, ruoWallBottomAttach, pitchAxis, oWallRollMat, nextChunkPitched, ruoWall, eShapeSection::RUOWALL);
    //ailines
    glm::vec3 aiLine1;
    GetAILine(pTrack, i, center, pitchAxis, rollMat, nextChunkPitched, aiLine1, eShapeSection::AILINE1, m_iAILineHeight);
    glm::vec3 aiLine2;
    GetAILine(pTrack, i, center, pitchAxis, rollMat, nextChunkPitched, aiLine2, eShapeSection::AILINE2, m_iAILineHeight);
    glm::vec3 aiLine3;
    GetAILine(pTrack, i, center, pitchAxis, rollMat, nextChunkPitched, aiLine3, eShapeSection::AILINE3, m_iAILineHeight);
    glm::vec3 aiLine4;
    GetAILine(pTrack, i, center, pitchAxis, rollMat, nextChunkPitched, aiLine4, eShapeSection::AILINE4, m_iAILineHeight);

    switch (section) {
      case LLANE:
        ApplyVerticesSingleSection(i, vertices, lLane, center, prevLLane, prevCenter);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iCenterSurfaceType),
                                vertices[i * uiNumVertsPerChunk + 0],
                                vertices[i * uiNumVertsPerChunk + 1],
                                vertices[i * uiNumVertsPerChunk + 2],
                                vertices[i * uiNumVertsPerChunk + 3], true, false);
        break;
      case RLANE:
        ApplyVerticesSingleSection(i, vertices, center, rLane, prevCenter, prevRLane);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iCenterSurfaceType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3], false, true);
        break;
      case LSHOULDER:
        ApplyVerticesSingleSection(i, vertices, lShoulder, lLane, prevLShoulder, prevLLane);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftSurfaceType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3]);
        break;
      case RSHOULDER:
        ApplyVerticesSingleSection(i, vertices, rLane, rShoulder, prevRLane, prevRShoulder);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightSurfaceType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3]);
        break;
      case LWALL:
        ApplyVerticesSingleSection(i, vertices, lWall, lWallBottomAttach, prevLWall, prevLWallBottomAttach);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLeftWallType),
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 1]);
        break;
      case RWALL:
        ApplyVerticesSingleSection(i, vertices, rWallBottomAttach, rWall, prevRWallBottomAttach, prevRWall);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRightWallType),
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 2]);
        break;
      case ROOF:
        ApplyVerticesSingleSection(i, vertices, rWall, lWall, prevRWall, prevLWall);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRoofType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3]);
        break;
      case OWALLFLOOR:
        ApplyVerticesSingleSection(i, vertices, lFloor, rFloor, prevLFloor, prevRFloor);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iOuterFloorType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3]);
        break;
      case LLOWALL:
        ApplyVerticesSingleSection(i, vertices, lloWall, lloWallBottomAttach, prevLLOWall, prevLLOWallBottomAttach);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLLOuterWallType),
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 1]);
        break;
      case RLOWALL:
        ApplyVerticesSingleSection(i, vertices, rloWallBottomAttach, rloWall, prevRLOWallBottomAttach, prevRLOWall);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRLOuterWallType),
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 1]);
        break;
      case LUOWALL:
        ApplyVerticesSingleSection(i, vertices, luoWall, luoWallBottomAttach, prevLUOWall, prevLUOWallBottomAttach);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iLUOuterWallType),
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 1]);
        break;
      case RUOWALL:
        ApplyVerticesSingleSection(i, vertices, ruoWallBottomAttach, ruoWall, prevRUOWallBottomAttach, prevRUOWall);
        if (pTexture)
          pTexture->GetTextureCoordinates(CTrackData::GetSignedBitValueFromInt(pTrack->m_chunkAy[iChunkIndex].iRUOuterWallType),
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 1]);
        break;
      case SELECTED:
        vertices[i * uiNumVertsPerChunk + 0].position = luoWall;
        vertices[i * uiNumVertsPerChunk + 1].position = ruoWall;
        vertices[i * uiNumVertsPerChunk + 2].position = lloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + 3].position = rloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + 4].position = prevLUOWall;
        vertices[i * uiNumVertsPerChunk + 5].position = prevRUOWall;
        vertices[i * uiNumVertsPerChunk + 6].position = prevLLOWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + 7].position = prevRLOWallBottomAttach;
        break;
      case AILINE1:
        vertices[i * uiNumVertsPerChunk + 0].position = aiLine1;
        break;
      case AILINE2:
        vertices[i * uiNumVertsPerChunk + 0].position = aiLine2;
        break;
      case AILINE3:
        vertices[i * uiNumVertsPerChunk + 0].position = aiLine3;
        break;
      case AILINE4:
        vertices[i * uiNumVertsPerChunk + 0].position = aiLine4;
        break;
      default:
        assert(0); //shape not implemented
        break;
    }

    prevCenter = center;
    prevLLane = lLane;
    prevRLane = rLane;
    prevLShoulder = lShoulder;
    prevRShoulder = rShoulder;
    prevLWall = lWall;
    prevRWall = rWall;
    prevLWallBottomAttach = lWallBottomAttach;
    prevRWallBottomAttach = rWallBottomAttach;
    prevLFloor = lFloor;
    prevRFloor = rFloor;
    prevLLOWall = lloWall;
    prevRLOWall = rloWall;
    prevLLOWallBottomAttach = lloWallBottomAttach;
    prevRLOWallBottomAttach = rloWallBottomAttach;
    prevLUOWall = luoWall;
    prevRUOWall = ruoWall;
    prevLUOWallBottomAttach = luoWallBottomAttach;
    prevRUOWallBottomAttach = ruoWallBottomAttach;
  }

  //attach final chunk
  switch (section) {
    case LLANE:
      vertices[3].position = prevCenter;
      vertices[2].position = prevLLane;
      break;
    case RLANE:
      vertices[2].position = prevCenter;
      vertices[3].position = prevRLane;
      break;
    case LSHOULDER:
      vertices[3].position = prevLLane;
      vertices[2].position = prevLShoulder;
      break;
    case RSHOULDER:
      vertices[2].position = prevRLane;
      vertices[3].position = prevRShoulder;
      break;
    case LWALL:
      vertices[3].position = prevLWallBottomAttach;
      vertices[2].position = prevLWall;
      break;
    case RWALL:
      vertices[2].position = prevRWallBottomAttach;
      vertices[3].position = prevRWall;
      break;
    case ROOF:
      vertices[3].position = prevLWall;
      vertices[2].position = prevRWall;
      break;
    case OWALLFLOOR:
      vertices[2].position = prevLFloor;
      vertices[3].position = prevRFloor;
      break;
    case LLOWALL:
      vertices[3].position = prevLLOWallBottomAttach;
      vertices[2].position = prevLLOWall;
      break;
    case RLOWALL:
      vertices[2].position = prevRLOWallBottomAttach;
      vertices[3].position = prevRLOWall;
      break;
    case LUOWALL:
      vertices[3].position = prevLLOWall;
      vertices[2].position = prevLUOWall;
      break;
    case RUOWALL:
      vertices[2].position = prevRLOWall;
      vertices[3].position = prevRUOWall;
      break;
    case SELECTED:
      vertices[4].position = prevLUOWall;
      vertices[5].position = prevRUOWall;
      vertices[6].position = prevLLOWallBottomAttach;
      vertices[7].position = prevRLOWallBottomAttach;
      break;
  }

  return vertices;
}

//-------------------------------------------------------------------------------------------------

tVertex *CShapeFactory::MakeVertsEnvirFloor(uint32 &numVertices, CTrackData *pTrack)
{
  if (pTrack->m_chunkAy.empty()) {
    numVertices = 0;
    return NULL;
  }

  numVertices = 4;
  tVertex *vertices = new tVertex[numVertices];
  glm::vec3 prevCenter = glm::vec3(0, 0, 1);
  float fMinX = 0.0f;
  float fMaxX = 0.0f;
  float fMinZ = 0.0f;
  float fMaxZ = 0.0f;
  for (uint32 i = 0; i < pTrack->m_chunkAy.size(); ++i) {
    glm::vec3 center;
    glm::vec3 pitchAxis;
    glm::vec3 nextChunkPitched;
    glm::mat4 yawMat;
    glm::mat4 pitchMat;
    glm::mat4 rollMat;
    GetCenter(pTrack, i, prevCenter, center, pitchAxis, nextChunkPitched, yawMat, pitchMat, rollMat);
    if (center.x > fMaxX)
      fMaxX = center.x;
    if (center.x < fMinX)
      fMinX = center.x;
    if (center.z > fMaxZ)
      fMaxZ = center.z;
    if (center.z < fMinZ)
      fMinZ = center.z;

    prevCenter = center;
  }

  float fEnvirFloorDepth = (float)pTrack->m_header.iFloorDepth / m_fScale * -1.0f;
  float fPadding = 10.0f;
  vertices[0].position = glm::vec3(fMinX - fPadding, fEnvirFloorDepth, fMinZ - fPadding);
  vertices[1].position = glm::vec3(fMinX - fPadding, fEnvirFloorDepth, fMaxZ + fPadding);
  vertices[2].position = glm::vec3(fMaxX + fPadding, fEnvirFloorDepth, fMinZ - fPadding);
  vertices[3].position = glm::vec3(fMaxX + fPadding, fEnvirFloorDepth, fMaxZ + fPadding);
  vertices[0].color = glm::vec3(0.235f, 0.317f, 0.157f);
  vertices[1].color = glm::vec3(0.235f, 0.317f, 0.157f);
  vertices[2].color = glm::vec3(0.235f, 0.317f, 0.157f);
  vertices[3].color = glm::vec3(0.235f, 0.317f, 0.157f);
  vertices[0].flags.x = 1.0f;
  vertices[1].flags.x = 1.0f;
  vertices[2].flags.x = 1.0f;
  vertices[3].flags.x = 1.0f;

  return vertices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesSingleSection(uint32 &numIndices, eShapeSection section, CTrackData *pTrack)
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
    if (i > 0 && !ShouldMakeIndicesForChunk(i - 1, section, pTrack))
      continue;
    else if (i == 0 && !ShouldMakeIndicesForChunk(((int)pTrack->m_chunkAy.size() - 1), section, pTrack))
      continue;
    //else if (i == 0 && !ShouldMakeIndicesForChunk(((int)pTrack->m_chunkAy.size() - (int)i + 1), section))
    //  continue;
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

  if (iStart >= pTrack->m_chunkAy.size() - 1)
    iStart = 0;
  else
    iStart++;
  if (iEnd >= pTrack->m_chunkAy.size() - 1)
    iEnd = 0;
  else
    iEnd++;

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

  return indices;
}

//-------------------------------------------------------------------------------------------------

bool CShapeFactory::ShouldMakeIndicesForChunk(int i, eShapeSection section, CTrackData *pTrack)
{
  if ((section == eShapeSection::LLANE || section == eShapeSection::RLANE)
      && !ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iCenterSurfaceType))
    return false;
  if (section == eShapeSection::LSHOULDER
      && !ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iLeftSurfaceType))
    return false;
  if (section == eShapeSection::RSHOULDER
      && !ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iRightSurfaceType))
    return false;
  if (section == eShapeSection::LWALL
      && !ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iLeftWallType))
    return false;
  if (section == eShapeSection::RWALL
      && !ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iRightWallType))
    return false;
  if (section == eShapeSection::ROOF
      && (!ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iRoofType)
          || pTrack->m_chunkAy[i].iLeftWallType == -1
          || pTrack->m_chunkAy[i].iRightWallType == -1
          || (!ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iLeftWallType) && !ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iRightWallType))))
    return false;
  if (section == eShapeSection::ENVIRFLOOR
      && !ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iEnvironmentFloorType))
    return false;
  if (section == eShapeSection::OWALLFLOOR
      && (pTrack->m_chunkAy[i].iOuterFloorType == -2
          || !ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iOuterFloorType)))
    return false;
  if (section == eShapeSection::LLOWALL
      && (!ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iLLOuterWallType)
          || pTrack->m_chunkAy[i].iOuterFloorType == -1
          || (pTrack->m_chunkAy[i].iOuterFloorType == -2 && !ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iCenterSurfaceType))))
    return false;
  if (section == eShapeSection::RLOWALL
      && (!ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iRLOuterWallType)
          || pTrack->m_chunkAy[i].iOuterFloorType == -1
          || (pTrack->m_chunkAy[i].iOuterFloorType == -2 && !ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iCenterSurfaceType))))
    return false;
  if (section == eShapeSection::LUOWALL
      && (!ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iLUOuterWallType)
          || pTrack->m_chunkAy[i].iOuterFloorType == -1
          || pTrack->m_chunkAy[i].iLLOuterWallType == -1))
    return false;
  if (section == eShapeSection::RUOWALL
      && (!ShouldDrawSurfaceType(pTrack->m_chunkAy[i].iRUOuterWallType)
          || pTrack->m_chunkAy[i].iOuterFloorType == -1
          || pTrack->m_chunkAy[i].iRLOuterWallType == -1))
    return false;
  return true;
}

//-------------------------------------------------------------------------------------------------

bool CShapeFactory::ShouldDrawSurfaceType(int iSurfaceType)
{
  if (iSurfaceType == -1)
    return false;
  uint32 uiSurfaceType = CTrackData::GetSignedBitValueFromInt(iSurfaceType);
  if (uiSurfaceType & SURFACE_FLAG_NON_SOLID)
    return false;
  if (!(uiSurfaceType & SURFACE_FLAG_TRANSPARENT)
      && !(uiSurfaceType & SURFACE_FLAG_APPLY_TEXTURE))
    return false;
  return true;
}

//-------------------------------------------------------------------------------------------------

uint32 *CShapeFactory::MakeIndicesSingleSectionWireframe(uint32 &numIndices, CTrackData *pTrack)
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

void CShapeFactory::GetCenter(CTrackData *pTrack, int i, glm::vec3 prevCenter,
                           glm::vec3 &center, glm::vec3 &pitchAxis, glm::vec3 &nextChunkPitched,
                           glm::mat4 &yawMat, glm::mat4 &pitchMat, glm::mat4 &rollMat)
{
  glm::vec3 nextChunkBase = glm::vec3(0, 0, 1);

  yawMat = glm::rotate(glm::radians((float)pTrack->m_chunkAy[i].dYaw), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 nextChunkYawed = glm::vec3(yawMat * glm::vec4(nextChunkBase, 1.0f));
  pitchAxis = glm::normalize(glm::cross(nextChunkYawed, glm::vec3(0.0f, 1.0f, 0.0f)));

  pitchMat = glm::rotate(glm::radians((float)pTrack->m_chunkAy[i].dPitch), pitchAxis);
  nextChunkPitched = glm::vec3(pitchMat * glm::vec4(nextChunkYawed, 1.0f));

  glm::mat4 translateMat = glm::mat4(1);
  if (i > 0)
    translateMat = glm::translate(prevCenter);
  //center
  float fLen = (float)pTrack->m_chunkAy[i].iLength / m_fScale;
  glm::mat4 scaleMat = glm::scale(glm::vec3(fLen, fLen, fLen));
  center = glm::vec3(translateMat * scaleMat * glm::vec4(nextChunkPitched, 1.0f));
  rollMat = glm::rotate(glm::radians((float)pTrack->m_chunkAy[i].dRoll * -1.0f), glm::normalize(nextChunkPitched));
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::GetLane(CTrackData *pTrack, int i, glm::vec3 center, glm::vec3 pitchAxis, glm::mat4 rollMat,
                          glm::vec3 &lane, bool bLeft)
{
  glm::mat4 translateMat = glm::translate(center); //translate to centerline
  float fLen;
  if (bLeft)
    fLen = (float)(pTrack->m_chunkAy[i].iLeftLaneWidth) / m_fScale * -1.0f;
  else
    fLen = (float)(pTrack->m_chunkAy[i].iRightLaneWidth) / m_fScale;
  glm::mat4 scaleMat = glm::scale(glm::vec3(fLen, fLen, fLen));
  lane = glm::vec3(translateMat * scaleMat * rollMat * glm::vec4(pitchAxis, 1.0f));
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::GetShoulder(CTrackData *pTrack, int i, glm::vec3 lLane, glm::vec3 pitchAxis, glm::mat4 rollMat, glm::vec3 nextChunkPitched,
                              glm::vec3 &shoulder, bool bLeft, bool bIgnoreHeight)
{
  glm::mat4 translateMat = glm::translate(lLane); //translate to end of left lane
  float fLen = 0.0f;
  float fHeight = 0.0f;
  if (bLeft) {
    fLen = (float)pTrack->m_chunkAy[i].iLeftShoulderWidth / m_fScale * -1.0f;
    if (!bIgnoreHeight)
      fHeight = (float)pTrack->m_chunkAy[i].iLeftShoulderHeight / m_fScale * -1.0f;
  } else {
    fLen = (float)pTrack->m_chunkAy[i].iRightShoulderWidth / m_fScale;
    if (!bIgnoreHeight)
      fHeight = (float)pTrack->m_chunkAy[i].iRightShoulderHeight / m_fScale * -1.0f;
  }
  glm::mat4 scaleMatWidth = glm::scale(glm::vec3(fLen, fLen, fLen));
  glm::mat4 scaleMatHeight = glm::scale(glm::vec3(fHeight, fHeight, fHeight));
  glm::vec3 widthVec = glm::vec3(scaleMatWidth * rollMat * glm::vec4(pitchAxis, 1.0f));
  glm::vec3 normal = glm::normalize(glm::cross(nextChunkPitched, pitchAxis));
  glm::vec3 heightVec = glm::vec3(scaleMatHeight * rollMat * glm::vec4(normal, 1.0f));
  glm::vec3 shoulderVec = widthVec + heightVec;
  shoulder = glm::vec3(translateMat * glm::vec4(shoulderVec, 1.0f));
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::GetEnvirFloor(CTrackData *pTrack, int i, glm::vec3 lShoulder, glm::vec3 rShoulder,
                               glm::vec3 &lEnvirFloor, glm::vec3 &rEnvirFloor)
{
  float fEnvirFloorDepth = (float)pTrack->m_header.iFloorDepth / m_fScale * -1.0f;
  lEnvirFloor = lShoulder;
  rEnvirFloor = rShoulder;
  lEnvirFloor.y = fEnvirFloorDepth;
  rEnvirFloor.y = fEnvirFloorDepth;
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::GetOWallFloor(CTrackData *pTrack, int i, glm::vec3 lLane, glm::vec3 rLane, glm::vec3 pitchAxis, glm::vec3 nextChunkPitched,
                               glm::vec3 &lFloor, glm::vec3 &rFloor)
{
  glm::mat4 translateMatL = glm::translate(lLane);
  glm::mat4 translateMatR = glm::translate(rLane);
  float fEnvirFloorDepth = (float)pTrack->m_header.iFloorDepth / m_fScale * -1.0f;
  float fLOFloorHeight = (float)pTrack->m_chunkAy[i].iLOuterFloorHeight / m_fScale * 1.0f;
  float fROFloorHeight = (float)pTrack->m_chunkAy[i].iROuterFloorHeight / m_fScale * 1.0f;
  float fROFloorOffset = (float)pTrack->m_chunkAy[i].iROuterFloorHOffset / m_fScale * 1.0f;
  float fLOFloorOffset = (float)pTrack->m_chunkAy[i].iLOuterFloorHOffset / m_fScale * -1.0f;

  glm::mat4 scaleMatRWidth = glm::scale(glm::vec3(fROFloorOffset, fROFloorOffset, fROFloorOffset));
  glm::vec3 rWidthVec = glm::vec3(scaleMatRWidth * glm::vec4(pitchAxis, 1.0f));
  glm::mat4 scaleMatLWidth = glm::scale(glm::vec3(fLOFloorOffset, fLOFloorOffset, fLOFloorOffset));
  glm::vec3 lWidthVec = glm::vec3(scaleMatLWidth * glm::vec4(pitchAxis, 1.0f));

  lFloor = glm::vec3(translateMatL * glm::vec4(lWidthVec, 1.0f));;
  rFloor = glm::vec3(translateMatR * glm::vec4(rWidthVec, 1.0f));;
  lFloor.y = fEnvirFloorDepth + fLOFloorHeight;
  rFloor.y = fEnvirFloorDepth + fROFloorHeight;
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::GetWall(CTrackData *pTrack, int i, glm::vec3 bottomAttach, glm::vec3 pitchAxis, glm::mat4 rollMat, glm::vec3 nextChunkPitched,
                            glm::vec3 &lloWall, eShapeSection wallSection)
{
  glm::mat4 translateMat = glm::translate(bottomAttach);
  float fHOffset = 0.0f;
  float fHeight = 0.0f;
  switch (wallSection) {
    case LWALL:
      if (pTrack->m_chunkAy[i].iLeftWallType != -1)
        fHeight = (float)pTrack->m_chunkAy[i].iRoofHeight / m_fScale * -1.0f;
    case RWALL:
      if (pTrack->m_chunkAy[i].iRightWallType != -1)
        fHeight = (float)pTrack->m_chunkAy[i].iRoofHeight / m_fScale * -1.0f;
      break;
    case LLOWALL:
      fHOffset = (float)pTrack->m_chunkAy[i].iLLOuterWallHOffset / m_fScale * -1.0f;
      fHeight = (float)pTrack->m_chunkAy[i].iLLOuterWallHeight / m_fScale * -1.0f;
      break;
    case RLOWALL:
      fHOffset = (float)pTrack->m_chunkAy[i].iRLOuterWallHOffset / m_fScale;
      fHeight = (float)pTrack->m_chunkAy[i].iRLOuterWallHeight / m_fScale * -1.0f;
      break;
    case LUOWALL:
      fHOffset = (float)pTrack->m_chunkAy[i].iLUOuterWallHOffset / m_fScale * -1.0f;
      fHeight = (float)pTrack->m_chunkAy[i].iLUOuterWallHeight / m_fScale * -1.0f;
      break;
    case RUOWALL:
      fHOffset = (float)pTrack->m_chunkAy[i].iRUOuterWallHOffset / m_fScale;
      fHeight = (float)pTrack->m_chunkAy[i].iRUOuterWallHeight / m_fScale * -1.0f;
      break;
    default:
      assert(0); //only wall sections should use this function
  }
  glm::mat4 scaleMatWidth = glm::scale(glm::vec3(fHOffset, fHOffset, fHOffset));
  glm::mat4 scaleMatHeight = glm::scale(glm::vec3(fHeight, fHeight, fHeight));
  glm::vec3 widthVec = glm::vec3(scaleMatWidth * rollMat * glm::vec4(pitchAxis, 1.0f));
  glm::vec3 normal = glm::normalize(glm::cross(nextChunkPitched, pitchAxis));
  glm::vec3 heightVec = glm::vec3(scaleMatHeight * rollMat * glm::vec4(normal, 1.0f));
  glm::vec3 wallVec = widthVec + heightVec;
  lloWall = glm::vec3(translateMat * glm::vec4(wallVec, 1.0f));
}

//-------------------------------------------------------------------------------------------------

void CShapeFactory::GetAILine(CTrackData *pTrack, int i, glm::vec3 center, glm::vec3 pitchAxis, glm::mat4 rollMat, glm::vec3 nextChunkPitched,
                           glm::vec3 &aiLine, eShapeSection lineSection, int iHeight)
{
  glm::mat4 translateMat = glm::translate(center);
  float fLen = 0.0f;
  int iUseAILine;
  switch (lineSection) {
    case eShapeSection::AILINE1:
      iUseAILine = pTrack->m_chunkAy[i].iAILine1;
      break;
    case eShapeSection::AILINE2:
      iUseAILine = pTrack->m_chunkAy[i].iAILine2;
      break;
    case eShapeSection::AILINE3:
      iUseAILine = pTrack->m_chunkAy[i].iAILine3;
      break;
    case eShapeSection::AILINE4:
      iUseAILine = pTrack->m_chunkAy[i].iAILine4;
      break;
    default:
      assert(0);
  }
  int iShoulderHeight = 0;
  if (iUseAILine > 0 && iUseAILine > pTrack->m_chunkAy[i].iLeftLaneWidth) {
    //ai line must be on left shoulder
    float fTheta = atan((float)pTrack->m_chunkAy[i].iLeftShoulderHeight / (float)pTrack->m_chunkAy[i].iLeftShoulderWidth);
    int iLengthIntoShoulder = abs(iUseAILine) - pTrack->m_chunkAy[i].iLeftLaneWidth;
    iShoulderHeight = (int)(tan(fTheta) * (float)iLengthIntoShoulder);
  }
  if (iUseAILine < 0 && abs(iUseAILine) > pTrack->m_chunkAy[i].iRightLaneWidth) {
    //ai line must be on right shoulder
    float fTheta = atan((float)pTrack->m_chunkAy[i].iRightShoulderHeight / (float)pTrack->m_chunkAy[i].iRightShoulderWidth);
    int iLengthIntoShoulder = abs(iUseAILine) - pTrack->m_chunkAy[i].iRightLaneWidth;
    iShoulderHeight = (int)(tan(fTheta) * (float)iLengthIntoShoulder);
  }

  fLen = (float)iUseAILine / m_fScale * -1.0f;
  float fHeight = (float)(iHeight + iShoulderHeight) / m_fScale * -1.0f;

  glm::mat4 scaleMatWidth = glm::scale(glm::vec3(fLen, fLen, fLen));
  glm::mat4 scaleMatHeight = glm::scale(glm::vec3(fHeight, fHeight, fHeight));
  glm::vec3 widthVec = glm::vec3(scaleMatWidth * rollMat * glm::vec4(pitchAxis, 1.0f));
  glm::vec3 normal = glm::normalize(glm::cross(nextChunkPitched, pitchAxis));
  glm::vec3 heightVec = glm::vec3(scaleMatHeight * rollMat * glm::vec4(normal, 1.0f));
  glm::vec3 lineVec = widthVec + heightVec;
  aiLine = glm::vec3(translateMat * glm::vec4(lineVec, 1.0f));
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