#ifndef _WHIPLIB_FBXEXPORTER_H
#define _WHIPLIB_FBXEXPORTER_H
//-------------------------------------------------------------------------------------------------
#include <map>
#include <string>
#include <vector>
#include "Types.h"
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------
class CShape;
class CTexture;
struct tVertex;
namespace fbxsdk
{
  class FbxNode;
  class FbxScene;
  class FbxSurfacePhong;
  class FbxMesh;
  class FbxFileTexture;
};
//-------------------------------------------------------------------------------------------------

class CFBXExporter
{
public:
  static CFBXExporter &GetFBXExporter();
  ~CFBXExporter();
  CFBXExporter(CFBXExporter const &) = delete;
  void operator=(CFBXExporter const &) = delete;

  bool ExportShape(CShape *pShapeData, const char *szName, const char *szFile, const char *szTextureFile);
  bool ExportTrack(CShape *pTrackShape,
                   std::vector<CShape *> signAy,
                   const char *szName,
                   const char *szFile,
                   const char *szTextureFile,
                   const char *szSignTextureFile);

private:
  CFBXExporter();
  fbxsdk::FbxNode *CreateShapeMesh(CShape *pShapeData, const char *szName, const char *szTextureFile, fbxsdk::FbxScene *pScene);
  fbxsdk::FbxSurfacePhong *CreateTextureMaterial(const char *szTextureFile, fbxsdk::FbxScene *pScene);
  fbxsdk::FbxFileTexture *CreateFileTexture(const char *szTextureFile, fbxsdk::FbxScene *pScene);
  void FlipTexCoordsForExport(tVertex *vertices, uint32 uiNumVerts);
};

//-------------------------------------------------------------------------------------------------
#endif