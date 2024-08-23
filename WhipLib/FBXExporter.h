#ifndef _WHIPLIB_FBXEXPORTER_H
#define _WHIPLIB_FBXEXPORTER_H
//-------------------------------------------------------------------------------------------------
#include <map>
#include <string>
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------
class CShapeData;
class CTexture;
namespace fbxsdk
{
  class FbxNode;
  class FbxScene;
  class FbxSurfacePhong;
  class FbxMesh;
  class FbxProceduralTexture;
};
//-------------------------------------------------------------------------------------------------

class CFBXExporter
{
public:
  static CFBXExporter &GetFBXExporter();
  ~CFBXExporter();
  CFBXExporter(CFBXExporter const &) = delete;
  void operator=(CFBXExporter const &) = delete;

  bool ExportShape(CShapeData *pShapeData, const char *szName, const char *szFile);

private:
  CFBXExporter();
  fbxsdk::FbxNode *CreateShapeMesh(CShapeData *pShapeData, const char *szName, fbxsdk::FbxScene *pScene);
  fbxsdk::FbxSurfacePhong *CreateColorMaterial(const glm::vec3 &color, fbxsdk::FbxScene *pScene);
  fbxsdk::FbxSurfacePhong *CreateTextureMaterial(CTexture *pTexture, fbxsdk::FbxScene *pScene);
  fbxsdk::FbxProceduralTexture *CreateProceduralTexture(CTexture *pTexture, fbxsdk::FbxScene *pScene);
  std::string GetColorString(const glm::vec3 &color);
};

//-------------------------------------------------------------------------------------------------
#endif