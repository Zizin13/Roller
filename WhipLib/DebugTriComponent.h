#ifndef _WHIPLIB_DEBUGTRICOMPONENT_H
#define _WHIPLIB_DEBUGTRICOMPONENT_H
//-------------------------------------------------------------------------------------------------
#include "Component.h"
#include "Types.h"
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------
class CShapeData;
class CRenderer;
class CTexture;
//-------------------------------------------------------------------------------------------------

class CDebugTriComponent : public IComponent
{
public:
  CDebugTriComponent();

  void Update() override;

  CRenderer *m_pRenderer;
  CTexture *m_pTex;
  glm::vec3 p0;
  glm::vec3 p1;
  glm::vec3 p2;

private:
  CShapeData *m_pShapeData;
};

//-------------------------------------------------------------------------------------------------
#endif