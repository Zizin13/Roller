#ifndef _WHIPLIB_DEBUGLINECOMPONENT_H
#define _WHIPLIB_DEBUGLINECOMPONENT_H
//-------------------------------------------------------------------------------------------------
#include "Component.h"
#include "Types.h"
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------
class CShapeData;
class CRenderer;
class CTexture;
//-------------------------------------------------------------------------------------------------

class CDebugLineComponent : public IComponent
{
public:
  CDebugLineComponent();

  void Update() override;

  CRenderer *m_pRenderer;
  CTexture *m_pTex;
  glm::vec3 p0;
  glm::vec3 p1;

private:
  CShapeData *m_pShapeData;
};

//-------------------------------------------------------------------------------------------------
#endif