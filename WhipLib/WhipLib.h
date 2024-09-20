#ifndef _WHIPLIB_WHIPLIB_H
#define _WHIPLIB_WHIPLIB_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
//-------------------------------------------------------------------------------------------------

#if defined(WIN32) || defined(_WIN32)
  #if defined(WHIPLIB_LIB)
    #define WLFUNC extern "C"
  #elif defined(WHIPLIB_DLL)
    #define WLFUNC extern "C" __declspec(dllexport)
  #else
    #define WLFUNC extern "C" __declspec(dllimport)
  #endif
  #define WLCDECL __cdecl
#else
  #define WLFUNC extern "C"
  #define WLCDECL
#endif

//-------------------------------------------------------------------------------------------------

struct tWhiplashVertex
{
  tWhiplashVertex()
    : fX(0.0f)
    , fY(0.0f)
    , fZ(0.0f)
    , fNormalX(0.0f)
    , fNormalY(0.0f)
    , fNormalZ(0.0f)
    , fTexX(0.0f)
    , fTexY(0.0f)
  {};

  float fX;
  float fY;
  float fZ;
  float fNormalX;
  float fNormalY;
  float fNormalZ;
  float fTexX;
  float fTexY;
};

//-------------------------------------------------------------------------------------------------

// Loads a Whiplash texture file and fills pDataBuf with a bitmap conversion including file header
// Returns the size of the bitmap or -1 if there is an error
WLFUNC int wlLoadTexture(uint8 *pDataBuf, int iBufSize, const char *szTex, const char *szPal);
typedef int (WLCDECL *wlLoadTextureFunc)(uint8 *pDataBuf, int iBufSize, const char *szTex, const char *szPal);


// Fills pBmpBuf with a bitmap conversion of the texture and palette files including file header,
//   pVertexBuf with vertices, and pIndexBuf with indices for the chosen model.
// iBmpSize will be set to the size of the bitmap file
// iNumVertices and iNumIndices will be set to the number of vertices and indices
// Returns true if successful
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
                       int &iNumIndices);
typedef bool (WLCDECL *wlGetModelFunc)(uint8 *pBmpBuf,
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
                                       int &iNumIndices);

//-------------------------------------------------------------------------------------------------
#endif