#ifndef _WHIPLIB_WHIPLIB_H
#define _WHIPLIB_WHIPLIB_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include "Logging.h"
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

// Sets logging callback
WLFUNC void wlSetLoggingCallback(Logging::WhipLibLoggingCallback pfnLogMsg);
typedef void (WLCDECL *wlSetLoggingCallbackFunc)(Logging::WhipLibLoggingCallback pfnLogMsg);

// Loads a Whiplash texture and palette file and fills pDataBuf with an 8-bit rgba bitmap
// Returns the size of the bitmap or -1 if there is an error
WLFUNC int wlLoadTexture(uint8 *pDataBuf, int iBufSize, const char *szTex, const char *szPal);
typedef int (WLCDECL *wlLoadTextureFunc)(uint8 *pDataBuf, int iBufSize, const char *szTex, const char *szPal);

// Fills pBmpBuf with an 8-bit rgba bitmap of the texture and palette files,
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

// Loads a Whiplash track into memory, must be called to get track model
//   and sign models.
// Returns an id to access the loaded track if successful, returns -1 if unsuccessful
WLFUNC int wlLoadTrack(const char *szTrack);
typedef int (WLCDECL *wlLoadTrackFunc)(const char *szTrack);

// Cleans up loaded track, returns true if successful
WLFUNC bool wlUnloadTrack(int iTrackId);
typedef bool (WLCDECL *wlUnloadTrackFunc)(int iTrackId);

// Must be called to clean up memory. Deletes all currently loaded tracks.
WLFUNC void wlShutdownModule();
typedef void (WLCDECL *wlShutdownModuleFunc)();

// Fills pDataBuf with an 8-bit rgba bitmap of the loaded track's TEX file
WLFUNC int wlGetTrackTex(int iTrackId, uint8 *pDataBuf, int iBufSize);
typedef int (WLCDECL *wlGetTrackTexFunc)(uint8 *pDataBuf, int iBufSize);

// Fills pDataBuf with an 8-bit rgba bitmap of the loaded track's BLD file
WLFUNC int wlGetTrackBld(int iTrackId, uint8 *pDataBuf, int iBufSize);
typedef int (WLCDECL *wlGetTrackBldFunc)(uint8 *pDataBuf, int iBufSize);

// Returns the number of signs in the loaded track
WLFUNC int wlGetNumSigns(int iTrackId);
typedef int (WLCDECL *wlGetNumSignsFunc)();

// Fills pVertexBuf with the vertices and pIndexBuf with the indices of
//   the loaded track.
// iNumVertices and iNumIndices will be set to the number of vertices and indices
// Returns true if successful
WLFUNC bool wlGetTrackModel(int iTrackId,
                            tWhiplashVertex *pVertexBuf,
                            int iVertexBufSize,
                            uint32 *pIndexBuf,
                            int iIndexBufSize,
                            int &iNumVertices,
                            int &iNumIndices);
typedef bool (WLCDECL *wlGetTrackModelFunc)(int iTrackId,
                                            tWhiplashVertex *pVertexBuf,
                                            int iVertexBufSize,
                                            uint32 *pIndexBuf,
                                            int iIndexBufSize,
                                            int &iNumVertices,
                                            int &iNumIndices);

// Fills pVertexBuf with the vertices and pIndexBuf with the indices of
//   the sign at the chosen index in the loaded track.
// iNumVertices and iNumIndices will be set to the number of vertices and indices
// Returns true if successful
WLFUNC bool wlGetSignModel(int iTrackId,
                           int iSignIndex,
                           tWhiplashVertex *pVertexBuf,
                           int iVertexBufSize,
                           uint32 *pIndexBuf,
                           int iIndexBufSize,
                           int &iNumVertices,
                           int &iNumIndices);
typedef bool (WLCDECL *wlGetSignModelFunc)(int iTrackId,
                                           int iSignIndex,
                                           tWhiplashVertex *pVertexBuf,
                                           int iVertexBufSize,
                                           uint32 *pIndexBuf,
                                           int iIndexBufSize,
                                           int &iNumVertices,
                                           int &iNumIndices);

//-------------------------------------------------------------------------------------------------
#endif