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

// Create a cube mesh. 
FbxNode *CreateCubeMesh(FbxScene *pScene)
{
  int i, j;
  FbxMesh *lMesh = FbxMesh::Create(pScene, "le cube");

  FbxVector4 lControlPoint0(-50, 0, 50);
  FbxVector4 lControlPoint1(50, 0, 50);
  FbxVector4 lControlPoint2(50, 100, 50);
  FbxVector4 lControlPoint3(-50, 100, 50);
  FbxVector4 lControlPoint4(-50, 0, -50);
  FbxVector4 lControlPoint5(50, 0, -50);
  FbxVector4 lControlPoint6(50, 100, -50);
  FbxVector4 lControlPoint7(-50, 100, -50);

  FbxVector4 lNormalXPos(1, 0, 0);
  FbxVector4 lNormalXNeg(-1, 0, 0);
  FbxVector4 lNormalYPos(0, 1, 0);
  FbxVector4 lNormalYNeg(0, -1, 0);
  FbxVector4 lNormalZPos(0, 0, 1);
  FbxVector4 lNormalZNeg(0, 0, -1);

  // Create control points.
  lMesh->InitControlPoints(24);
  FbxVector4 *lControlPoints = lMesh->GetControlPoints();

  lControlPoints[0] = lControlPoint0;
  lControlPoints[1] = lControlPoint1;
  lControlPoints[2] = lControlPoint2;
  lControlPoints[3] = lControlPoint3;
  lControlPoints[4] = lControlPoint1;
  lControlPoints[5] = lControlPoint5;
  lControlPoints[6] = lControlPoint6;
  lControlPoints[7] = lControlPoint2;
  lControlPoints[8] = lControlPoint5;
  lControlPoints[9] = lControlPoint4;
  lControlPoints[10] = lControlPoint7;
  lControlPoints[11] = lControlPoint6;
  lControlPoints[12] = lControlPoint4;
  lControlPoints[13] = lControlPoint0;
  lControlPoints[14] = lControlPoint3;
  lControlPoints[15] = lControlPoint7;
  lControlPoints[16] = lControlPoint3;
  lControlPoints[17] = lControlPoint2;
  lControlPoints[18] = lControlPoint6;
  lControlPoints[19] = lControlPoint7;
  lControlPoints[20] = lControlPoint1;
  lControlPoints[21] = lControlPoint0;
  lControlPoints[22] = lControlPoint4;
  lControlPoints[23] = lControlPoint5;

  // We want to have one normal for each vertex (or control point),
  // so we set the mapping mode to eByControlPoint.
  FbxGeometryElementNormal *lGeometryElementNormal = lMesh->CreateElementNormal();

  lGeometryElementNormal->SetMappingMode(FbxGeometryElement::eByControlPoint);

  // Set the normal values for every control point.
  lGeometryElementNormal->SetReferenceMode(FbxGeometryElement::eDirect);

  lGeometryElementNormal->GetDirectArray().Add(lNormalZPos);
  lGeometryElementNormal->GetDirectArray().Add(lNormalZPos);
  lGeometryElementNormal->GetDirectArray().Add(lNormalZPos);
  lGeometryElementNormal->GetDirectArray().Add(lNormalZPos);
  lGeometryElementNormal->GetDirectArray().Add(lNormalXPos);
  lGeometryElementNormal->GetDirectArray().Add(lNormalXPos);
  lGeometryElementNormal->GetDirectArray().Add(lNormalXPos);
  lGeometryElementNormal->GetDirectArray().Add(lNormalXPos);
  lGeometryElementNormal->GetDirectArray().Add(lNormalZNeg);
  lGeometryElementNormal->GetDirectArray().Add(lNormalZNeg);
  lGeometryElementNormal->GetDirectArray().Add(lNormalZNeg);
  lGeometryElementNormal->GetDirectArray().Add(lNormalZNeg);
  lGeometryElementNormal->GetDirectArray().Add(lNormalXNeg);
  lGeometryElementNormal->GetDirectArray().Add(lNormalXNeg);
  lGeometryElementNormal->GetDirectArray().Add(lNormalXNeg);
  lGeometryElementNormal->GetDirectArray().Add(lNormalXNeg);
  lGeometryElementNormal->GetDirectArray().Add(lNormalYPos);
  lGeometryElementNormal->GetDirectArray().Add(lNormalYPos);
  lGeometryElementNormal->GetDirectArray().Add(lNormalYPos);
  lGeometryElementNormal->GetDirectArray().Add(lNormalYPos);
  lGeometryElementNormal->GetDirectArray().Add(lNormalYNeg);
  lGeometryElementNormal->GetDirectArray().Add(lNormalYNeg);
  lGeometryElementNormal->GetDirectArray().Add(lNormalYNeg);
  lGeometryElementNormal->GetDirectArray().Add(lNormalYNeg);


  // Array of polygon vertices.
  int lPolygonVertices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,12, 13,
      14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };


  // Create UV for Diffuse channel.
  FbxGeometryElementUV *lUVDiffuseElement = lMesh->CreateElementUV("DiffuseUV");
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
  lUVDiffuseElement->GetIndexArray().SetCount(24);

  // Create polygons. Assign texture and texture UV indices.
  for (i = 0; i < 6; i++) {
      // all faces of the cube have the same texture
    lMesh->BeginPolygon(-1, -1, -1, false);

    for (j = 0; j < 4; j++) {
        // Control point index
      lMesh->AddPolygon(lPolygonVertices[i * 4 + j]);

      // update the index array of the UVs that map the texture to the face
      lUVDiffuseElement->GetIndexArray().SetAt(i * 4 + j, j);
    }

    lMesh->EndPolygon();
  }

  // create a FbxNode
  FbxNode *lNode = FbxNode::Create(pScene, "le cube");

  // set the node attribute
  lNode->SetNodeAttribute(lMesh);

  // set the shading mode to view texture
  lNode->SetShadingMode(FbxNode::eTextureShading);

  // rescale the cube
  lNode->LclScaling.Set(FbxVector4(0.3, 0.3, 0.3));

  // return the FbxNode
  return lNode;
}

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

  bool bSuccess = pExporter->Export(pScene);

  pScene->Destroy();
  pExporter->Destroy();
  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

FbxNode *CFBXExporter::CreateShapeMesh(CShapeData *pShapeData, FbxScene *pScene)
{
  FbxMesh *pMesh = FbxMesh::Create(pScene, "shape mesh");
  pMesh->InitControlPoints((int)pShapeData->m_uiNumVerts);
  FbxVector4 *controlPointsAy = pMesh->GetControlPoints();

  for (int i = 0; i < (int)pShapeData->m_uiNumVerts; ++i) {
    FbxVector4 controlPoint(pShapeData->m_vertices[i].position.x, pShapeData->m_vertices[i].position.y, pShapeData->m_vertices[i].position.z);
    controlPointsAy[i] = controlPoint;
  }

  for (int i = 0; i < (int)pShapeData->m_uiNumIndices / 3; ++i) {
    pMesh->BeginPolygon();
    for (int j = 0; j < 3; ++j) {
      pMesh->AddPolygon(pShapeData->m_indices[i * 3 + j]);
    }
    pMesh->EndPolygon();
  }

  // create a FbxNode
  FbxNode *pNode = FbxNode::Create(pScene, "shape");
  // set the node attribute
  pNode->SetNodeAttribute(pMesh);

  return pNode;
}

//-------------------------------------------------------------------------------------------------