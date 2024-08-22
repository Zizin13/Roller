#ifndef _WHIPLIB_FBXEXPORTER_H
#define _WHIPLIB_FBXEXPORTER_H
//-------------------------------------------------------------------------------------------------
class CShapeData;
namespace fbxsdk
{
  class FbxNode;
  class FbxScene;
  class FbxSurfacePhong;
  class FbxMesh;
};
//-------------------------------------------------------------------------------------------------

class CFBXExporter
{
public:
  static CFBXExporter &GetFBXExporter();
  ~CFBXExporter();
  CFBXExporter(CFBXExporter const &) = delete;
  void operator=(CFBXExporter const &) = delete;

  bool ExportShape(CShapeData *pShapeData, const char *szFile);

private:
  CFBXExporter();
  fbxsdk::FbxNode *CreateShapeMesh(CShapeData *pShapeData, fbxsdk::FbxScene *pScene);
  fbxsdk::FbxSurfacePhong *CreateMaterial(fbxsdk::FbxScene *pScene);
  void AddMaterials(fbxsdk::FbxSurfacePhong *pMaterial, fbxsdk::FbxMesh *pMesh, CShapeData *pShapeData);
};

//-------------------------------------------------------------------------------------------------
#endif