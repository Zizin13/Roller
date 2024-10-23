#include "ShapeComponent.h"
#include "ShapeData.h"
#include "Palette.h"
#include "Texture.h"
#include "ShapeFactory.h"
#include "Entity.h"
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CShapeComponent::CShapeComponent()
  : m_pShapeData(NULL)
  , m_rotationOffset(glm::mat4(1))
{

}

//-------------------------------------------------------------------------------------------------

CShapeComponent::~CShapeComponent()
{
}

//-------------------------------------------------------------------------------------------------

void CShapeComponent::Update()
{
  if (!m_pShapeData)
    return;

  m_pShapeData->m_modelToWorldMatrix = glm::translate(m_pContainingEntity->m_position) *
    //rollMat * pitchMat * yawMat *
    glm::inverse(m_pContainingEntity->m_rotationMat) *
    m_rotationOffset;
}

//-------------------------------------------------------------------------------------------------