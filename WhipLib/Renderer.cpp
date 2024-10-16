#include "Renderer.h"
#include "ShapeData.h"
#include "Shader.h"
#include "Palette.h"
#include "Texture.h"
#include "ShapeFactory.h"
#include "Camera.h"
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

bool CRenderer::Init()
{
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
  CCarShapeAy::iterator it = m_carShapeAy.begin();
  for (; it != m_carShapeAy.end(); ++it) {
    delete *it;
  }
  m_carShapeAy.clear();
  return true;
}

//-------------------------------------------------------------------------------------------------

void CRenderer::Draw(int iWindowWidth, int iWindowHeight, Camera *pCamera)
{
  glm::mat4 viewToProjectionMatrix = glm::perspective(glm::radians(60.0f), ((float)iWindowWidth) / iWindowHeight, 100.0f, 500000.0f);
  glm::mat4 worldToViewMatrix = pCamera->GetWorldToViewMatrix();
  glm::mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

  CCarShapeAy::iterator it = m_carShapeAy.begin();
  for (; it != m_carShapeAy.end(); ++it) {
    (*it)->pShapeData->Draw(worldToProjectionMatrix, pCamera->m_position);
  }
}

//-------------------------------------------------------------------------------------------------

CShapeData *CRenderer::MakeCarShape(eWhipModel model)
{
  tCarShape *pNewCarShape = new tCarShape;
  pNewCarShape->pPal = new CPalette();
  pNewCarShape->pPal->LoadPalette("C:\\WHIP\\WHIPLASH\\FATDATA\\PALETTE.PAL");
  pNewCarShape->pTex = new CTexture();
  pNewCarShape->pTex->LoadTexture("C:\\WHIP\\WHIPLASH\\FATDATA\\YZIZIN.BM", pNewCarShape->pPal);
  pNewCarShape->pShapeData = CShapeFactory::GetShapeFactory().MakeModel(m_pShader, pNewCarShape->pTex, model);
  m_carShapeAy.emplace_back(pNewCarShape);
  return pNewCarShape->pShapeData;
}

//-------------------------------------------------------------------------------------------------