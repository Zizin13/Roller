#include "Renderer.h"
#include "ShapeData.h"
#include "Shader.h"
#include "Palette.h"
#include "Texture.h"
#include "ShapeFactory.h"
#include "Camera.h"
#include "CarHelpers.h"
#include "OpenGLDebug.h"
#include "SceneManager.h"
#include <glm.hpp>
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

struct tCarShape
{
  tCarShape()
    : pShapeData(NULL)
    , pTex(NULL)
    , pPal(NULL)
  {}
  ~tCarShape()
  {
    if (pShapeData) {
      delete pShapeData;
      pShapeData = NULL;
    }
    if (pTex) {
      delete pTex;
      pTex = NULL;
    }
    if (pPal) {
      delete pPal;
      pPal = NULL;
    }
  }
  CShapeData *pShapeData;
  CTexture *pTex;
  CPalette *pPal;
};

//-------------------------------------------------------------------------------------------------

CRenderer::CRenderer()
  : m_pShader(NULL)
{
  m_carShapeAy.reserve(16);
}

//-------------------------------------------------------------------------------------------------

CRenderer::~CRenderer()
{
  Shutdown();
}

//-------------------------------------------------------------------------------------------------

bool CRenderer::Init(const std::string &sFatDataDir)
{
  if (glewInit() != GLEW_OK)
    return false;
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(GLErrorCb, 0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.24f);
  glLineWidth(3.0f);

  if (!m_pShader) {
    m_pShader = new CShader("Shaders/WhiplashVertexShader.glsl", "Shaders/WhiplashFragmentShader.glsl");
    return true;
  }
  return false;
}

//-------------------------------------------------------------------------------------------------

bool CRenderer::Shutdown()
{
  if (m_pShader) {
    delete m_pShader;
    m_pShader = NULL;
  }
  for (CCarShapeAy::iterator it = m_carShapeAy.begin(); it != m_carShapeAy.end(); ++it) {
    delete *it;
  }
  m_carShapeAy.clear();
  for (CShapeAy::iterator it = m_shapeAy.begin(); it != m_shapeAy.end(); ++it) {
    delete *it;
  }
  m_shapeAy.clear();
  return true;
}

//-------------------------------------------------------------------------------------------------

void CRenderer::Draw(int iWindowWidth, int iWindowHeight, CCamera *pCamera)
{
  if (!m_pShader || !iWindowWidth || !iWindowHeight || !pCamera)
    return;

  glClearColor(0.03125f, 0.296875f, 0.984375f, 1.0f);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, iWindowWidth, iWindowHeight);

  glm::mat4 viewToProjectionMatrix = glm::perspective(glm::radians(60.0f), ((float)iWindowWidth) / iWindowHeight, 100.0f, 500000.0f);
  glm::mat4 worldToViewMatrix = pCamera->GetWorldToViewMatrix();
  glm::mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

  for (CCarShapeAy::iterator it = m_carShapeAy.begin(); it != m_carShapeAy.end(); ++it) {
    (*it)->pShapeData->Draw(worldToProjectionMatrix, pCamera->GetPosition());
  }
  for (CShapeAy::iterator it = m_shapeAy.begin(); it != m_shapeAy.end(); ++it) {
    (*it)->Draw(worldToProjectionMatrix, pCamera->GetPosition());
  }
}

//-------------------------------------------------------------------------------------------------

CShapeData *CRenderer::MakeCarShape(eWhipModel model)
{
  if (!m_pShader)
    return NULL;

  std::string sTexName = CarHelpers::GetCarTextureFromModel(model);

  tCarShape *pNewCarShape = new tCarShape;
  pNewCarShape->pPal = new CPalette();
  pNewCarShape->pPal->LoadPalette(CSceneManager::GetSceneManager().GetFatDataDir() + "/PALETTE.PAL");
  pNewCarShape->pTex = new CTexture();
  pNewCarShape->pTex->LoadTexture(CSceneManager::GetSceneManager().GetFatDataDir() + "/" + sTexName, pNewCarShape->pPal);
  pNewCarShape->pShapeData = NULL;
  CShapeFactory::GetShapeFactory().MakeModel(&pNewCarShape->pShapeData, m_pShader, pNewCarShape->pTex, model);

  if (pNewCarShape->pShapeData) {
    m_carShapeAy.emplace_back(pNewCarShape);
    return pNewCarShape->pShapeData;
  } else {
    delete pNewCarShape;
    return NULL;
  }
}

//-------------------------------------------------------------------------------------------------

void CRenderer::MakeTrackShape(CShapeData **pShape, CTrack *pTrack)
{
  if (!m_pShader)
    return;

  bool bNew = !(*pShape);
  CShapeFactory::GetShapeFactory().MakeTrackSurface(pShape, m_pShader, pTrack, eShapeSection::EXPORT, true);
  if (bNew && *pShape)
    m_shapeAy.emplace_back(*pShape);
}

//-------------------------------------------------------------------------------------------------

void CRenderer::MakeSigns(CTrack *pTrack)
{
  if (!m_pShader)
    return;

  std::vector<CShapeData *> signAy;

  CShapeFactory::GetShapeFactory().MakeSigns(m_pShader, pTrack, signAy);
  for (std::vector<CShapeData *>::iterator it = signAy.begin(); it != signAy.end(); ++it) {
    m_shapeAy.emplace_back(*it);
  }
}

//-------------------------------------------------------------------------------------------------

void CRenderer::MakeEnvirFloor(CTrack *pTrack)
{
  if (!m_pShader)
    return;

  CShapeData *pNewShape = NULL;
  CShapeFactory::GetShapeFactory().MakeEnvirFloor(&pNewShape, m_pShader, pTrack, 0);
  if (pNewShape && pNewShape)
    m_shapeAy.emplace_back(pNewShape);
}

//-------------------------------------------------------------------------------------------------

void CRenderer::MakeDebugTri(CShapeData **pShape, CTexture *pTexture,
                             const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2)
{
  if (!m_pShader || !pTexture)
    return;

  bool bNew = !(*pShape);
  CShapeFactory::GetShapeFactory().MakeDebugTri(pShape, m_pShader, pTexture, p0, p1, p2);
  if (bNew && *pShape)
    m_shapeAy.emplace_back((*pShape));
}

//-------------------------------------------------------------------------------------------------

void CRenderer::MakeDebugLine(CShapeData **pShape, CTexture *pTexture,
                             const glm::vec3 &p0, const glm::vec3 &p1)
{
  if (!m_pShader || !pTexture)
    return;

  bool bNew = !(*pShape);
  CShapeFactory::GetShapeFactory().MakeDebugLine(pShape, m_pShader, pTexture, p0, p1);
  if (bNew && *pShape)
    m_shapeAy.emplace_back((*pShape));
}

//-------------------------------------------------------------------------------------------------