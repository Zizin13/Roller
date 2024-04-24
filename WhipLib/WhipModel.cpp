#include <glew.h>
#include "WhipModel.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "ShapeData.h"
#include "Texture.h"
#include "Palette.h"
#include "TrackData.h"
#include "AutoPlans.h"
#include "DesilvaPlans.h"
#include "F1WackPlans.h"
#include "GlobalPlans.h"
#include "MillionPlans.h"
#include "MissionPlans.h"
#include "PulsePlans.h"
#include "ReisePlans.h"
#include "ZizinPlans.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

class CWhipModelPrivate
{
public:
  CWhipModelPrivate() {};
  ~CWhipModelPrivate() {};

  CPalette m_pal;
  CTexture m_tex;
  eWhipModel m_model;
};

//-------------------------------------------------------------------------------------------------

CWhipModel::CWhipModel()
  : m_fScale(10000.0f)
{
  p = new CWhipModelPrivate;
}

//-------------------------------------------------------------------------------------------------

CWhipModel::~CWhipModel()
{
  if (p) {
    delete p;
    p = NULL;
  }
}

//-------------------------------------------------------------------------------------------------

void CWhipModel::LoadTexture(const std::string &sPal, const std::string &sTex, bool bMangled)
{
  p->m_pal.LoadPalette(sPal);
  p->m_tex.LoadTexture(sTex, &p->m_pal, bMangled);
}

//-------------------------------------------------------------------------------------------------

CShapeData *CWhipModel::MakeModel(CShader *pShader, eWhipModel model)
{
  p->m_model = model;
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVerts(uiNumVerts);
  uint32 uiNumIndices;
  uint32 *indices = MakeIndices(uiNumIndices);

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

tVertex *CWhipModel::MakeVerts(uint32 &numVertices)
{
  //first turn float array into vertex array
  uint32 uiNumCoords = GetCoordsCount() / 3;
  tVertex *coordAy = new tVertex[uiNumCoords];
  int iVertIndex = 0;
  for (int i = 0; i < GetCoordsCount(); ++i) {
    coordAy[i / 3].position[iVertIndex] = GetCoords()[i] / m_fScale;
    iVertIndex++;
    if (iVertIndex == 3)
      iVertIndex = 0;
  }

  //then get vertices for polygons
  numVertices = GetPolsCount() * 4;  
  tVertex *vertices = new tVertex[numVertices];
  for (int i = 0; i < GetPolsCount(); ++i) {
    vertices[i * 4 + 0] = coordAy[GetPols()[i].byVert1];
    vertices[i * 4 + 1] = coordAy[GetPols()[i].byVert2];
    vertices[i * 4 + 2] = coordAy[GetPols()[i].byVert3];
    vertices[i * 4 + 3] = coordAy[GetPols()[i].byVert4];

    uint32 uiUseTex = GetPols()[i].uiTex;
    if (uiUseTex & SURFACE_FLAG_ANMS_LOOKUP) {
      uiUseTex = GetAnms()[uiUseTex & SURFACE_TEXTURE_INDEX].framesAy[0];
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

uint32 *CWhipModel::MakeIndices(uint32 &numIndices)
{
  int iNumIndicesPerPol = 6;
  int iNumVertsPerPol = 4;
  numIndices = GetPolsCount() * iNumIndicesPerPol;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  int i = 0;
  for (; i < GetPolsCount(); i++) {
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

float *CWhipModel::GetCoords()
{
  switch (p->m_model) {
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
    case SIGN_TOWER2:
    case SIGN_SIGN01:
    case SIGN_SIGN02:
    case SIGN_BUILD:
    case SIGN_BUILD1:
    case SIGN_BUILD2:
    case SIGN_BUILD3:
    case SIGN_HEELBAR:
    case SIGN_BALLOON:
    case SIGN_BALLOON2:
    case SIGN_TREE:
    case SIGN_ADVERT:
    case SIGN_ADVERT2:
    case SIGN_ADVERT3:
    case SIGN_QUADBLD:
    case SIGN_BLD0:
    default:
      assert(0);
      return NULL;
  }
}

//-------------------------------------------------------------------------------------------------

int CWhipModel::GetCoordsCount()
{
  switch (p->m_model) {
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
    case SIGN_TOWER2:
    case SIGN_SIGN01:
    case SIGN_SIGN02:
    case SIGN_BUILD:
    case SIGN_BUILD1:
    case SIGN_BUILD2:
    case SIGN_BUILD3:
    case SIGN_HEELBAR:
    case SIGN_BALLOON:
    case SIGN_BALLOON2:
    case SIGN_TREE:
    case SIGN_ADVERT:
    case SIGN_ADVERT2:
    case SIGN_ADVERT3:
    case SIGN_QUADBLD:
    case SIGN_BLD0:
    default:
      assert(0);
      return NULL;
  }
}

//-------------------------------------------------------------------------------------------------

tPolygon *CWhipModel::GetPols()
{
  switch (p->m_model) {
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
    case SIGN_TOWER2:
    case SIGN_SIGN01:
    case SIGN_SIGN02:
    case SIGN_BUILD:
    case SIGN_BUILD1:
    case SIGN_BUILD2:
    case SIGN_BUILD3:
    case SIGN_HEELBAR:
    case SIGN_BALLOON:
    case SIGN_BALLOON2:
    case SIGN_TREE:
    case SIGN_ADVERT:
    case SIGN_ADVERT2:
    case SIGN_ADVERT3:
    case SIGN_QUADBLD:
    case SIGN_BLD0:
    default:
      assert(0);
      return NULL;
  }
}

//-------------------------------------------------------------------------------------------------

int CWhipModel::GetPolsCount()
{
  switch (p->m_model) {
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
    case SIGN_TOWER2:
    case SIGN_SIGN01:
    case SIGN_SIGN02:
    case SIGN_BUILD:
    case SIGN_BUILD1:
    case SIGN_BUILD2:
    case SIGN_BUILD3:
    case SIGN_HEELBAR:
    case SIGN_BALLOON:
    case SIGN_BALLOON2:
    case SIGN_TREE:
    case SIGN_ADVERT:
    case SIGN_ADVERT2:
    case SIGN_ADVERT3:
    case SIGN_QUADBLD:
    case SIGN_BLD0:
    default:
      assert(0);
      return NULL;
  }
}

//-------------------------------------------------------------------------------------------------

tAnimation *CWhipModel::GetAnms()
{
  switch (p->m_model) {
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
    case SIGN_TOWER2:
    case SIGN_SIGN01:
    case SIGN_SIGN02:
    case SIGN_BUILD:
    case SIGN_BUILD1:
    case SIGN_BUILD2:
    case SIGN_BUILD3:
    case SIGN_HEELBAR:
    case SIGN_BALLOON:
    case SIGN_BALLOON2:
    case SIGN_TREE:
    case SIGN_ADVERT:
    case SIGN_ADVERT2:
    case SIGN_ADVERT3:
    case SIGN_QUADBLD:
    case SIGN_BLD0:
    default:
      assert(0);
      return NULL;
  }
}

//-------------------------------------------------------------------------------------------------

int CWhipModel::GetAnmsCount()
{
  switch (p->m_model) {
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
    case SIGN_TOWER2:
    case SIGN_SIGN01:
    case SIGN_SIGN02:
    case SIGN_BUILD:
    case SIGN_BUILD1:
    case SIGN_BUILD2:
    case SIGN_BUILD3:
    case SIGN_HEELBAR:
    case SIGN_BALLOON:
    case SIGN_BALLOON2:
    case SIGN_TREE:
    case SIGN_ADVERT:
    case SIGN_ADVERT2:
    case SIGN_ADVERT3:
    case SIGN_QUADBLD:
    case SIGN_BLD0:
    default:
      assert(0);
      return NULL;
  }
}

//-------------------------------------------------------------------------------------------------