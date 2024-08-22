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

  //g_pFbxManager->GetIOSettings()->SetBoolProp(EXP_FBX_EMBEDDED, true);
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