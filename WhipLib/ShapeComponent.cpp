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

CShapeComponent::CShapeComponent()
  : m_pShapeData(NULL)
{

}

//-------------------------------------------------------------------------------------------------

CShapeComponent::~CShapeComponent()
{
}

//-------------------------------------------------------------------------------------------------

bool CShapeComponent::Init()
{
  return true;
}

//-------------------------------------------------------------------------------------------------

bool CShapeComponent::Shutdown()
{
  return true;
}

//-------------------------------------------------------------------------------------------------

void CShapeComponent::Update()
{

}

//-------------------------------------------------------------------------------------------------

void CShapeComponent::SetData(CShapeData *pShapeData)
{
  m_pShapeData = pShapeData;
}

//-------------------------------------------------------------------------------------------------