#include "FBXExporter.h"
#include "ShapeData.h"
#include "Logging.h"
#include <fbxsdk.h>
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

bool CFBXExporter::ExportShape(CShapeData *pShapeData, const char *szFile)
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

  FbxNode *pShapeMesh = CreateShapeMesh(pShapeData, pScene);
  pScene->GetRootNode()->AddChild(pShapeMesh);

  g_pFbxManager->GetIOSettings()->SetBoolProp(EXP_FBX_EMBEDDED, true);

  bool bSuccess = pExporter->Export(pScene);

  pScene->Destroy();
  pExporter->Destroy();
  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

FbxNode *CFBXExporter::CreateShapeMesh(CShapeData *pShapeData, FbxScene *pScene)
{
  //create mesh object
  FbxMesh *pMesh = FbxMesh::Create(pScene, "shape mesh");

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

  //// Create UV for Diffuse channel.
  //FbxGeometryElementUV *lUVDiffuseElement = pMesh->CreateElementUV("DiffuseUV");
  //FBX_ASSERT(lUVDiffuseElement != NULL);
  //lUVDiffuseElement->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
  //lUVDiffuseElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
  //FbxVector2 lVectors0(0, 0);
  //FbxVector2 lVectors1(1, 0);
  //FbxVector2 lVectors2(1, 1);
  //FbxVector2 lVectors3(0, 1);
  //lUVDiffuseElement->GetDirectArray().Add(lVectors0);
  //lUVDiffuseElement->GetDirectArray().Add(lVectors1);
  //lUVDiffuseElement->GetDirectArray().Add(lVectors2);
  //lUVDiffuseElement->GetDirectArray().Add(lVectors3);
  ////Now we have set the UVs as eIndexToDirect reference and in eByPolygonVertex  mapping mode
  ////we must update the size of the index array.
  //lUVDiffuseElement->GetIndexArray().SetCount((int)pShapeData->m_uiNumIndices);

  //FbxSurfacePhong *pMaterial = CreateMaterial(pScene);
  //create polygons
  for (int i = 0; i < (int)pShapeData->m_uiNumIndices / 3; ++i) {
    pMesh->BeginPolygon();

    for (int j = 0; j < 3; ++j) {
      pMesh->AddPolygon(pShapeData->m_indices[i * 3 + j]);
      //// update the index array of the UVs that map the texture to the face
      //lUVDiffuseElement->GetIndexArray().SetAt(i * 3 + j, j);
    }
    
    pMesh->EndPolygon();
  }

  //AddMaterials(pMaterial, pMesh, pShapeData);

  // create a FbxNode
  FbxNode *pNode = FbxNode::Create(pScene, "shape");
  // set the node attribute
  pNode->SetNodeAttribute(pMesh);
  // set the shading mode to view texture
  //pNode->SetShadingMode(FbxNode::eTextureShading);

  return pNode;
}

//-------------------------------------------------------------------------------------------------

FbxSurfacePhong *CFBXExporter::CreateMaterial(FbxScene *pScene)
{
  FbxSurfacePhong *pMaterial;

  FbxString lMaterialName = "material";
  FbxString lShadingName = "Phong";
  FbxDouble3 lBlack(0.0, 0.0, 0.0);
  FbxDouble3 lRed(1.0, 0.0, 0.0);
  FbxDouble3 lDiffuseColor(0.75, 0.75, 0.0);
  pMaterial = FbxSurfacePhong::Create(pScene, lMaterialName.Buffer());

  // Generate primary and secondary colors.
  pMaterial->Emissive.Set(lBlack);
  pMaterial->Ambient.Set(lRed);
  pMaterial->Diffuse.Set(lDiffuseColor);
  pMaterial->TransparencyFactor.Set(40.5);
  pMaterial->ShadingModel.Set(lShadingName);
  pMaterial->Shininess.Set(0.5);

  //// the texture need to be connected to the material on the corresponding property
  //if (gTexture)
  //  gMaterial->Diffuse.ConnectSrcObject(gTexture);

  return pMaterial;
}

//-------------------------------------------------------------------------------------------------

void CFBXExporter::AddMaterials(FbxSurfacePhong *pMaterial, FbxMesh *pMesh, CShapeData *pShapeData)
{
    // Set material mapping.
  FbxGeometryElementMaterial *lMaterialElement = pMesh->CreateElementMaterial();
  lMaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
  lMaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

  //get the node of mesh, add material for it.
  FbxNode *lNode = pMesh->GetNode();
  if (lNode == NULL)
    return;
  lNode->AddMaterial(pMaterial);

  // We are in eByPolygon, so there's only need for 6 index (a cube has 6 polygons).
  int iNumPols = (int)pShapeData->m_uiNumIndices / 3;
  lMaterialElement->GetIndexArray().SetCount(iNumPols);

  // Set the Index 0 to 6 to the material in position 0 of the direct array.
  for (int i = 0; i < iNumPols; ++i)
    lMaterialElement->GetIndexArray().SetAt(i, 0);
}

//-------------------------------------------------------------------------------------------------