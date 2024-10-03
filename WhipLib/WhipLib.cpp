#include "WhipLib.h"
#include "Palette.h"
#include "Texture.h"
#include "ShapeData.h"
#include "ShapeFactory.h"
#include "Track.h"
#include <vector>
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

static CTrack s_track;
static CShapeData *s_pTrackShape = NULL;
static std::vector<CShapeData *> s_signAy;

//-------------------------------------------------------------------------------------------------

WLFUNC void wlSetLoggingCallback(Logging::WhipLibLoggingCallback pfnLogMsg)
{
  Logging::SetWhipLibLoggingCallback(pfnLogMsg);
}

//-------------------------------------------------------------------------------------------------

WLFUNC int wlLoadTexture(uint8 *pDataBuf, int iBufSize, const char *szTex, const char *szPal)
{
  int iRet = -1;
  if (!pDataBuf || !szTex || !szPal)
    return iRet;

  //load palette
  CPalette pal;
  if (!pal.LoadPalette(szPal))
    return iRet;

  //load texture
  CTexture tex;
  if (!tex.LoadTexture(szTex, &pal))
    return iRet;

  //generate bmp
  int iBmpSize;
  uint8 *pBmpData = tex.GenerateBitmapData(iBmpSize);

  //fill buffer
  if (iBmpSize <= iBufSize) {
    iRet = iBmpSize;
    for (int i = 0; i < iBmpSize; ++i) {
      pDataBuf[i] = pBmpData[i];
    }
  }

  //cleanup
  delete[] pBmpData;

  return iRet;
}

//-------------------------------------------------------------------------------------------------

WLFUNC bool wlGetModel(uint8 *pBmpBuf,
                       int iBmpBufSize,
                       tWhiplashVertex *pVertexBuf,
                       int iVertexBufSize,
                       uint32 *pIndexBuf,
                       int iIndexBufSize,
                       eWhipModel model,
                       const char *szTex,
                       const char *szPal,
                       int &iBmpSize,
                       int &iNumVertices,
                       int &iNumIndices)
{
  bool bSuccess = false;
  if (!pBmpBuf || !pVertexBuf || !pIndexBuf || !szTex || !szPal)
    return bSuccess;

  //load palette
  CPalette pal;
  if (!pal.LoadPalette(szPal))
    return bSuccess;

  //load texture
  CTexture tex;
  if (!tex.LoadTexture(szTex, &pal))
    return bSuccess;

  //generate bmp
  uint8 *pBmpData = tex.GenerateBitmapData(iBmpSize);

  //generate model
  CShapeData *pShape = CShapeFactory::GetShapeFactory().MakeModel(NULL, &tex, model);
  if (!pShape)
    return bSuccess;

  //fill buffers
  if (pShape->m_uiNumVerts <= (uint32)(iVertexBufSize / sizeof(tWhiplashVertex))
      && pShape->m_uiNumIndices <= (uint32)iIndexBufSize / sizeof(uint32)
      && iBmpSize <= iBmpBufSize)
  {
    bSuccess = true;
    iNumVertices = (int)pShape->m_uiNumVerts;
    iNumIndices = (int)pShape->m_uiNumIndices;
    for (int i = 0; i < iBmpSize; ++i) {
      pBmpBuf[i] = pBmpData[i];
    }
    for (int i = 0; i < (int)pShape->m_uiNumVerts; ++i) {
      pVertexBuf[i].fX = pShape->m_vertices[i].position.x;
      pVertexBuf[i].fY = pShape->m_vertices[i].position.y;
      pVertexBuf[i].fZ = pShape->m_vertices[i].position.z;
      pVertexBuf[i].fNormalX = pShape->m_vertices[i].normal.x;
      pVertexBuf[i].fNormalY = pShape->m_vertices[i].normal.y;
      pVertexBuf[i].fNormalZ = pShape->m_vertices[i].normal.z;
      pVertexBuf[i].fTexX = pShape->m_vertices[i].texCoords.x;
      pVertexBuf[i].fTexY = pShape->m_vertices[i].texCoords.y;
    }
    for (int i = 0; i < (int)pShape->m_uiNumIndices; ++i) {
      pIndexBuf[i] = pShape->m_indices[i];
    }
  }

  //cleanup
  delete pShape;
  delete[] pBmpData;

  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

WLFUNC bool wlLoadTrack(const char *szTrack)
{
  wlShutdownModule();

  bool bSuccess = s_track.LoadTrack(szTrack);
  bSuccess |= s_track.LoadTextures();
  s_pTrackShape = CShapeFactory::GetShapeFactory().MakeTrackSurface(s_pTrackShape, NULL, &s_track, eShapeSection::EXPORT, true, false);
  CShapeFactory::GetShapeFactory().MakeSigns(NULL, &s_track, s_signAy);
  
  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

WLFUNC void wlShutdownModule()
{
  s_track.ClearData();
  if (s_pTrackShape) {
    delete s_pTrackShape;
    s_pTrackShape = NULL;
  }
  for (std::vector<CShapeData *>::iterator it = s_signAy.begin(); it != s_signAy.end(); ++it) {
    delete *it;
  }
  s_signAy.clear();
}

//-------------------------------------------------------------------------------------------------

WLFUNC int wlGetTrackTex(uint8 *pDataBuf, int iBufSize)
{
  int iBmpSize = -1;
  if (!s_track.m_pTex)
    return iBmpSize;

  //generate bmp
  uint8 *pBmpData = s_track.m_pTex->GenerateBitmapData(iBmpSize);

  //fill buffer
  if (iBmpSize <= iBufSize) {
    for (int i = 0; i < iBmpSize; ++i) {
      pDataBuf[i] = pBmpData[i];
    }
  }

  //cleanup
  delete[] pBmpData;
  return iBmpSize;
}

//-------------------------------------------------------------------------------------------------

WLFUNC int wlGetTrackBld(uint8 *pDataBuf, int iBufSize)
{
  int iBmpSize = -1;
  if (!s_track.m_pBld)
    return iBmpSize;

  //generate bmp
  uint8 *pBmpData = s_track.m_pBld->GenerateBitmapData(iBmpSize);

  //fill buffer
  if (iBmpSize <= iBufSize) {
    for (int i = 0; i < iBmpSize; ++i) {
      pDataBuf[i] = pBmpData[i];
    }
  }

  //cleanup
  delete[] pBmpData;
  return iBmpSize;
}

//-------------------------------------------------------------------------------------------------

WLFUNC int wlGetNumSigns()
{
  return (int)s_signAy.size();
}

//-------------------------------------------------------------------------------------------------