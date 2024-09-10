#ifndef _WHIPLIB_FBXEXPORTER_H
#define _WHIPLIB_FBXEXPORTER_H
//-------------------------------------------------------------------------------------------------
#include <map>
#include <string>
#include <vector>
#include "Types.h"
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------
class CShapeData;
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

  bool ExportShape(CShapeData *pShapeData, const char *szName, const char *szFile, const char *szTextureFile);
  bool ExportTrack(CShapeData *pTrackShape,
                   std::vector<CShapeData *> signAy,
                   const char *szName,
                   const char *szFile,
                   const char *szTextureFile,
                   const char *szSignTextureFile);

private:
  CFBXExporter();
  fbxsdk::FbxNode *CreateShapeMesh(CShapeData *pShapeData, const char *szName, const char *szTextureFile, fbxsdk::FbxScene *pScene);
  fbxsdk::FbxSurfacePhong *CreateColorMaterial(const glm::vec4 &color, fbxsdk::FbxScene *pScene);
  fbxsdk::FbxSurfacePhong *CreateTextureMaterial(const char *szTextureFile, fbxsdk::FbxScene *pScene);
  fbxsdk::FbxFileTexture *CreateFileTexture(const char *szTextureFile, fbxsdk::FbxScene *pScene);
  std::string GetColorString(const glm::vec4 &color);
  void FlipTexCoordsForExport(tVertex *vertices, uint32 uiNumVerts);
};

//-------------------------------------------------------------------------------------------------
#endif