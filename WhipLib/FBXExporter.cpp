#include "FBXExporter.h"
#include "ShapeData.h"
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

typedef std::map<std::string, int> CColorMaterialMap; //map of color to material index

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

bool CFBXExporter::ExportShape(CShapeData *pShapeData, const char *szName, const char *szFile)
{
  if (!pShapeData || !szFile)
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

  FbxNode *pShapeMesh = CreateShapeMesh(pShapeData, szName, pScene);
  pScene->GetRootNode()->AddChild(pShapeMesh);

  g_pFbxManager->GetIOSettings()->SetBoolProp(EXP_FBX_EMBEDDED, true);

  bool bSuccess = pExporter->Export(pScene);

  pScene->Destroy();
  pExporter->Destroy();
  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

FbxNode *CFBXExporter::CreateShapeMesh(CShapeData *pShapeData, const char *szName, FbxScene *pScene)
{
  int iNumPols = (int)pShapeData->m_uiNumIndices / 3;

  //create mesh object
  FbxMesh *pMesh = FbxMesh::Create(pScene, szName);

  //create vertices
  pMesh->InitControlPoints((int)pShapeData->m_uiNumVerts);
  FbxVector4 *controlPointsAy = pMesh->GetControlPoints();
  for (int i = 0; i < (int)pShapeData->m_uiNumVerts; ++i) {
    FbxVector4 controlPoint(pShapeData->m_vertices[i].position.x, pShapeData->m_vertices[i].position.y, pShapeData->m_vertices[i].position.z);
    controlPointsAy[i] = controlPoint;
  }

  //create normals
  FbxGeometryElementNormal *pGeometryElementNormal = pMesh->CreateElementNormal();
  pGeometryElementNormal->SetMappingMode(FbxGeometryElement::eByControlPoint);
  pGeometryElementNormal->SetReferenceMode(FbxGeometryElement::eDirect);
  for (int i = 0; i < (int)pShapeData->m_uiNumVerts; ++i) {
    FbxVector4 normal(pShapeData->m_vertices[i].normal.x, pShapeData->m_vertices[i].normal.y, pShapeData->m_vertices[i].normal.z);
    pGeometryElementNormal->GetDirectArray().Add(normal);
  }

  // Create UV for Diffuse channel.
  FbxGeometryElementUV *lUVDiffuseElement = pMesh->CreateElementUV("DiffuseUV");
  FBX_ASSERT(lUVDiffuseElement != NULL);
  lUVDiffuseElement->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
  lUVDiffuseElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
  FbxVector2 lVectors0(0, 0);
  FbxVector2 lVectors1(1, 0);
  FbxVector2 lVectors2(1, 1);
  FbxVector2 lVectors3(0, 1);
  lUVDiffuseElement->GetDirectArray().Add(lVectors0);
  lUVDiffuseElement->GetDirectArray().Add(lVectors1);
  lUVDiffuseElement->GetDirectArray().Add(lVectors2);
  lUVDiffuseElement->GetDirectArray().Add(lVectors3);
  //Now we have set the UVs as eIndexToDirect reference and in eByPolygonVertex  mapping mode
  //we must update the size of the index array.
  lUVDiffuseElement->GetIndexArray().SetCount((int)pShapeData->m_uiNumIndices);

  //set material mapping
  FbxGeometryElementMaterial *pMaterialElement = pMesh->CreateElementMaterial();
  pMaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
  pMaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
  pMaterialElement->GetIndexArray().SetCount(iNumPols);
  std::vector<FbxSurfacePhong *> addedMaterials;
  CColorMaterialMap colorMaterialMap;

  //create polygons and materials
  FbxSurfacePhong *pTextureMaterial = CreateTextureMaterial(pShapeData->m_pTexture, pScene);
  addedMaterials.push_back(pTextureMaterial);
  for (int i = 0; i < iNumPols; ++i) {
    pMesh->BeginPolygon();

    if (pShapeData->m_vertices[pShapeData->m_indices[i * 3]].flags.x == 1.0f) {
      //polygon is solid color
      glm::vec3 color = pShapeData->m_vertices[pShapeData->m_indices[i * 3]].color;
      CColorMaterialMap::iterator it = colorMaterialMap.find(GetColorString(color));
      if (it == colorMaterialMap.end()) { //only add new material if it's a new color
        FbxSurfacePhong *pNewMaterial = CreateColorMaterial(color, pScene);
        colorMaterialMap[GetColorString(color)] = (int)addedMaterials.size();
        addedMaterials.push_back(pNewMaterial);
      }
    } else {
      //texture, already created this material so do nothing
    }

    for (int j = 0; j < 3; ++j) {
      pMesh->AddPolygon(pShapeData->m_indices[i * 3 + j]);
      // update the index array of the UVs that map the texture to the face
      lUVDiffuseElement->GetIndexArray().SetAt(i * 3 + j, j);
    }
    
    pMesh->EndPolygon();
  }

  // create a FbxNode
  FbxNode *pNode = FbxNode::Create(pScene, szName);
  pNode->SetNodeAttribute(pMesh);
  pNode->SetShadingMode(FbxNode::eTextureShading);

  //add materials to mesh
  for (int i = 0; i < (int)addedMaterials.size(); ++i) {
    pNode->AddMaterial(addedMaterials[i]);
  }

  //assign materials to polygons
  for (int i = 0; i < iNumPols; ++i) {
    if (pShapeData->m_vertices[pShapeData->m_indices[i * 3]].flags.x == 1.0f) {
      //polygon is solid color
      glm::vec3 color = pShapeData->m_vertices[pShapeData->m_indices[i * 3]].color;
      CColorMaterialMap::iterator it = colorMaterialMap.find(GetColorString(color));
      if (it != colorMaterialMap.end()) {
        pMaterialElement->GetIndexArray().SetAt(i, it->second);
      } else {
        assert(0); //color should have been added
      }
    } else {
      //texture is always the first material added
      pMaterialElement->GetIndexArray().SetAt(i, 0);
    }
  }

  return pNode;
}

//-------------------------------------------------------------------------------------------------

FbxSurfacePhong *CFBXExporter::CreateColorMaterial(const glm::vec3 &color, FbxScene *pScene)
{
  FbxSurfacePhong *pMaterial;

  std::string sName = "Color: " + GetColorString(color);
  FbxDouble3 diffuseColor(color.r, color.g, color.b);
  pMaterial = FbxSurfacePhong::Create(pScene, sName.c_str());
  pMaterial->Diffuse.Set(diffuseColor);

  return pMaterial;
}

//-------------------------------------------------------------------------------------------------

fbxsdk::FbxSurfacePhong *CFBXExporter::CreateTextureMaterial(CTexture *pTexture, fbxsdk::FbxScene *pScene)
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

  FbxProceduralTexture *pFbxTex = CreateProceduralTexture(pTexture, pScene);

  // the texture need to be connected to the material on the corresponding property
  if (pFbxTex)
    pMaterial->Diffuse.ConnectSrcObject(pFbxTex);

  return pMaterial;
}

//-------------------------------------------------------------------------------------------------

FbxProceduralTexture *CFBXExporter::CreateProceduralTexture(CTexture *pTexture, FbxScene *pScene)
{
  FbxProceduralTexture *pProceduralTexture = FbxProceduralTexture::Create(pScene, "texture");

  int iBmpSize;
  uint8 *pBmpData = pTexture->GenerateBitmapData(iBmpSize);
  FbxBlob binaryBlob(pBmpData, iBmpSize);
  pProceduralTexture->SetBlob(binaryBlob);

  return pProceduralTexture;
}

//-------------------------------------------------------------------------------------------------

std::string CFBXExporter::GetColorString(const glm::vec3 &color)
{
  return "(" + std::to_string(color.r) +
    "," + std::to_string(color.g) +
    "," + std::to_string(color.b) + ")";
}

//-------------------------------------------------------------------------------------------------