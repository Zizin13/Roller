#include "ShapeComponent.h"
#include "ShapeData.h"
#include "Palette.h"
#include "Texture.h"
#include "ShapeFactory.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CShapeComponent::CShapeComponent(CShader *pShader, const eWhipModel &model, const std::string &sTex, const std::string &sPal)
  : m_pShapeData(NULL)
  , m_model(model)
  , m_sTex(sTex)
  , m_sPal(sPal)
  , m_pShader(pShader)
{

}

//-------------------------------------------------------------------------------------------------

CShapeComponent::~CShapeComponent()
{
  Shutdown();
}

//-------------------------------------------------------------------------------------------------

bool CShapeComponent::Init()
{
  bool bSuccess = false;

  if (!m_pPal) {
    m_pPal = new CPalette();
    bSuccess |= m_pPal->LoadPalette(m_sPal);
  }
  if (!m_pTex) {
    m_pTex = new CTexture();
    bSuccess |= m_pTex->LoadTexture(m_sTex, m_pPal);
  }
  if (!m_pShapeData) {
    m_pShapeData = CShapeFactory::GetShapeFactory().MakeModel(m_pShader, m_pTex, m_model);
  }
  if (!m_pShapeData)
    bSuccess = false;

  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

bool CShapeComponent::Shutdown()
{
  if (m_pShapeData) {
    delete m_pShapeData;
    m_pShapeData = NULL;
  }
  return true;
}

//-------------------------------------------------------------------------------------------------

void CShapeComponent::Update()
{

}

//-------------------------------------------------------------------------------------------------