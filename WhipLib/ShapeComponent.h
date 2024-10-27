#ifndef _WHIPLIB_SHAPECOMPONENT_H
#define _WHIPLIB_SHAPECOMPONENT_H
//-------------------------------------------------------------------------------------------------
#include "Component.h"
#include "Types.h"
#include "glm.hpp"
#include <string>
//-------------------------------------------------------------------------------------------------
class CShapeData;
//-------------------------------------------------------------------------------------------------

class CShapeComponent : public IComponent
{
public:
  CShapeComponent();
  ~CShapeComponent();

  void Update() override;

  CShapeData *m_pShapeData;
  int m_iShapeKey;
  glm::mat4 m_rotationOffset;
};

//-------------------------------------------------------------------------------------------------
#endif