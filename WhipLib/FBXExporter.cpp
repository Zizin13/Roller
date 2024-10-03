#include "FBXExporter.h"
#include "Shape.h"
#include "Logging.h"
#include "Texture.h"
#include <fbxsdk.h>
#include <vector>
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

using namespace FBXSDK_NAMESPACE;

//-------------------------------------------------------------------------------------------------

FbxManager *g_pFbxManager = NULL;

//-------------------------------------------------------------------------------------------------

CFBXExporter &CFBXExporter::GetFBXExporter()
{
  static CFBXExporter s_fbxExporter;

  return s_fbxExporter;
}

//-------------------------------------------------------------------------------------------------

CFBXExporter::CFBXExporter()
{
  g_pFbxManager = FbxManager::Create();
  if (!g_pFbxManager)
    Logging::LogMessage("Error: Could not create FBX manager");
  
  FbxIOSettings *pIos = FbxIOSettings::Create(g_pFbxManager, IOSROOT);
  g_pFbxManager->SetIOSettings(pIos);
}

//-------------------------------------------------------------------------------------------------

CFBXExporter::~CFBXExporter()
{
  if (g_pFbxManager) {
    g_pFbxManager->Destroy();
    g_pFbxManager = NULL;
  }
}

//-------------------------------------------------------------------------------------------------

bool CFBXExporter::ExportShape(CShape *pShapeData, const char *szName, const char *szFile, const char *szTextureFile)
{
  if (!pShapeData || !szFile || !szTextureFile)
    return false;

  FbxScene *pScene = FbxScene::Create(g_pFbxManager, "Export Scene");
  if (!pScene) {
    Logging::LogMessage("Error: Could not create FBX scene");
    return false;
  }

  FbxExporter *pExporter = FbxExporter::Create(g_pFbxManager, "Exporter");
  if (!pExporter) {
    Logging::LogMessage("Error: Could not create FBX exporter");
    pScene->Destroy();
    return false;
  }

  if (pExporter->Initialize(szFile, g_pFbxManager->GetIOPluginRegistry()->GetNativeWriterFormat(), g_pFbxManager->GetIOSettings()) == false) {
    Logging::LogMessage("FbxExporter::Initialize() failed. %s\n", pExporter->GetStatus().GetErrorString());
    pScene->Destroy();
    pExporter->Destroy();
    return false;
  }

  //OpenGL expects texture data in reverse order
  FlipTexCoordsForExport(pShapeData->m_shapeData.m_vertices, pShapeData->m_shapeData.m_uiNumVerts);

  FbxNode *pShapeMesh = CreateShapeMesh(pShapeData, szName, szTextureFile, pScene);
  pShapeMesh->SetGeometricRotation(FbxNode::eSourcePivot, FbxVector4(-90.0f, 0.0f, 0.0f));
  pScene->GetRootNode()->AddChild(pShapeMesh);

  //g_pFbxManager->GetIOSettings()->SetBoolProp(EXP_FBX_EMBEDDED, true);

  bool bSuccess = pExporter->Export(pScene);

  pScene->Destroy();
  pExporter->Destroy();
  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

bool CFBXExporter::ExportTrack(CShape *pTrackShape,
                               std::vector<CShape *> signAy,
                               const char *szName,
                               const char *szFile,
                               const char *szTextureFile,
                               const char *szSignTextureFile)
{
  if (!pTrackShape || !szFile || !szTextureFile || !szSignTextureFile)
    return false;

  FbxScene *pScene = FbxScene::Create(g_pFbxManager, "Export Scene");
  if (!pScene) {
    Logging::LogMessage("Error: Could not create FBX scene");
    return false;
  }

  FbxExporter *pExporter = FbxExporter::Create(g_pFbxManager, "Exporter");
  if (!pExporter) {
    Logging::LogMessage("Error: Could not create FBX exporter");
    pScene->Destroy();
    return false;
  }

  if (pExporter->Initialize(szFile, g_pFbxManager->GetIOPluginRegistry()->GetNativeWriterFormat(), g_pFbxManager->GetIOSettings()) == false) {
    Logging::LogMessage("FbxExporter::Initialize() failed. %s\n", pExporter->GetStatus().GetErrorString());
    pScene->Destroy();
    pExporter->Destroy();
    return false;
  }

  FlipTexCoordsForExport(pTrackShape->m_shapeData.m_vertices, pTrackShape->m_shapeData.m_uiNumVerts); //OpenGL expects texture data in reverse order

  FbxNode *pTrackMesh = CreateShapeMesh(pTrackShape, szName, szTextureFile, pScene);
  pScene->GetRootNode()->AddChild(pTrackMesh);
  for (int i = 0; i < (int)signAy.size(); ++i) {
    std::string sSignName = "Sign " + std::to_string(i);
    FlipTexCoordsForExport(signAy[i]->m_shapeData.m_vertices, signAy[i]->m_shapeData.m_uiNumVerts); //OpenGL expects texture data in reverse order
    FbxNode *pSignMesh = CreateShapeMesh(signAy[i], sSignName.c_str(), szSignTextureFile, pScene);
    pScene->GetRootNode()->AddChild(pSignMesh);
  }

  //g_pFbxManager->GetIOSettings()->SetBoolProp(EXP_FBX_EMBEDDED, true);

  bool bSuccess = pExporter->Export(pScene);

  pScene->Destroy();
  pExporter->Destroy();
  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

FbxNode *CFBXExporter::CreateShapeMesh(CShape *pShapeData, const char *szName, const char *szTextureFile, FbxScene *pScene)
{
  int iNumPols = (int)pShapeData->m_shapeData.m_uiNumIndices / 3;

  //create mesh object
  FbxMesh *pMesh = FbxMesh::Create(pScene, szName);

  //create vertices
  pMesh->InitControlPoints((int)pShapeData->m_shapeData.m_uiNumVerts);
  FbxVector4 *controlPointsAy = pMesh->GetControlPoints();
  for (int i = 0; i < (int)pShapeData->m_shapeData.m_uiNumVerts; ++i) {
    FbxVector4 controlPoint(pShapeData->m_shapeData.m_vertices[i].position.x,
                            pShapeData->m_shapeData.m_vertices[i].position.y,
                            pShapeData->m_shapeData.m_vertices[i].position.z);
    controlPointsAy[i] = controlPoint;
  }

  //create normals
  FbxGeometryElementNormal *pGeometryElementNormal = pMesh->CreateElementNormal();
  pGeometryElementNormal->SetMappingMode(FbxGeometryElement::eByControlPoint);
  pGeometryElementNormal->SetReferenceMode(FbxGeometryElement::eDirect);
  for (int i = 0; i < (int)pShapeData->m_shapeData.m_uiNumVerts; ++i) {
    FbxVector4 normal(pShapeData->m_shapeData.m_vertices[i].normal.x,
                      pShapeData->m_shapeData.m_vertices[i].normal.y,
                      pShapeData->m_shapeData.m_vertices[i].normal.z);
    pGeometryElementNormal->GetDirectArray().Add(normal);
  }

  //set material mapping
  FbxGeometryElementMaterial *pMaterialElement = pMesh->CreateElementMaterial();
  pMaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
  pMaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
  pMaterialElement->GetIndexArray().SetCount(iNumPols);

  //create polygons and materials
  FbxSurfacePhong *pTextureMaterial = CreateTextureMaterial(szTextureFile, pScene);
  for (int i = 0; i < iNumPols; ++i) {
    pMesh->BeginPolygon();
    //create polygon from indices
    for (int j = 0; j < 3; ++j) {
      pMesh->AddPolygon(pShapeData->m_shapeData.m_indices[i * 3 + j]);
    }
    pMesh->EndPolygon();
  }

  //create UV for diffuse channel.
  FbxGeometryElementUV *lUVDiffuseElement = pMesh->CreateElementUV("DiffuseUV");
  FBX_ASSERT(lUVDiffuseElement != NULL);
  lUVDiffuseElement->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
  lUVDiffuseElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
  lUVDiffuseElement->GetIndexArray().SetCount((int)pShapeData->m_shapeData.m_uiNumIndices);
  //generate UV mapping
  for (int i = 0; i < (int)pShapeData->m_shapeData.m_uiNumIndices; ++i) {
    FbxVector2 uv(pShapeData->m_shapeData.m_vertices[pShapeData->m_shapeData.m_indices[i]].texCoords.x,
                  pShapeData->m_shapeData.m_vertices[pShapeData->m_shapeData.m_indices[i]].texCoords.y);
    lUVDiffuseElement->GetDirectArray().Add(uv);
    lUVDiffuseElement->GetIndexArray().SetAt(i, i);
  }

  // create a FbxNode
  FbxNode *pNode = FbxNode::Create(pScene, szName);
  pNode->SetNodeAttribute(pMesh);
  pNode->SetShadingMode(FbxNode::eTextureShading);

  //add materials to mesh
  pNode->AddMaterial(pTextureMaterial);

  //assign materials to polygons
  for (int i = 0; i < iNumPols; ++i) {
    //texture is always the first material added
    pMaterialElement->GetIndexArray().SetAt(i, 0);
  }

  return pNode;
}

//-------------------------------------------------------------------------------------------------

fbxsdk::FbxSurfacePhong *CFBXExporter::CreateTextureMaterial(const char *szTextureFile, fbxsdk::FbxScene *pScene)
{
  FbxSurfacePhong *pMaterial;

  std::string sName = "Texture";
  FbxDouble3 black(0.0, 0.0, 0.0);
  FbxDouble3 diffuseColor(0.75, 0.75, 0.0);
  pMaterial = FbxSurfacePhong::Create(pScene, sName.c_str());

  // Generate primary and secondary colors.
  pMaterial->Emissive.Set(black);
  pMaterial->Ambient.Set(diffuseColor);
  pMaterial->Diffuse.Set(diffuseColor);
  pMaterial->TransparencyFactor.Set(40.5);
  pMaterial->ShadingModel.Set("Phong");
  pMaterial->Shininess.Set(0.5);

  FbxFileTexture *pFbxTex = CreateFileTexture(szTextureFile, pScene);

  // the texture need to be connected to the material on the corresponding property
  if (pFbxTex)
    pMaterial->Diffuse.ConnectSrcObject(pFbxTex);

  return pMaterial;
}

//-------------------------------------------------------------------------------------------------

//FbxProceduralTexture *CFBXExporter::CreateProceduralTexture(CTexture *pTexture, FbxScene *pScene)
//{
//  FbxProceduralTexture *pProceduralTexture = FbxProceduralTexture::Create(pScene, "texture");
//
//  int iBmpSize;
//  uint8 *pBmpData = pTexture->GenerateBitmapData(iBmpSize);
//  FbxBlob binaryBlob(pBmpData, iBmpSize);
//  pProceduralTexture->SetBlob(binaryBlob);
//  pProceduralTexture->SetTextureUse(FbxTexture::eStandard);
//  pProceduralTexture->SetMappingType(FbxTexture::eUV);
//  //pProceduralTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
//  pProceduralTexture->SetSwapUV(false);
//  pProceduralTexture->SetTranslation(0.0, 0.0);
//  pProceduralTexture->SetScale(1.0, 1.0);
//  pProceduralTexture->SetRotation(0.0, 0.0);
//
//  return pProceduralTexture;
//}

//-------------------------------------------------------------------------------------------------

FbxFileTexture *CFBXExporter::CreateFileTexture(const char *szTextureFile, FbxScene *pScene)
{
  std::string sTexName = "Texture: " + std::string(szTextureFile);
  FbxFileTexture *pFileTexture = FbxFileTexture::Create(pScene, sTexName.c_str());

  pFileTexture->SetFileName(szTextureFile);
  pFileTexture->SetTextureUse(FbxTexture::eStandard);
  pFileTexture->SetMappingType(FbxTexture::eUV);
  pFileTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
  pFileTexture->SetSwapUV(false);
  pFileTexture->SetTranslation(0.0, 0.0);
  pFileTexture->SetScale(1.0, 1.0);
  pFileTexture->SetRotation(0.0, 0.0);

  return pFileTexture;
}

//-------------------------------------------------------------------------------------------------

void CFBXExporter::FlipTexCoordsForExport(tVertex *vertices, uint32 uiNumVerts)
{
  for (uint32 i = 0; i < uiNumVerts; ++i) {
    vertices[i].texCoords.y = 1.0f - vertices[i].texCoords.y;
  }
}

//-------------------------------------------------------------------------------------------------