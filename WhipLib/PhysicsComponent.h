#ifndef _WHIPLIB_PHYSICSCOMPONENT_H
#define _WHIPLIB_PHYSICSCOMPONENT_H
//-------------------------------------------------------------------------------------------------
#include "Component.h"
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------
class CTrack;
class CRenderer;
class CTexture;
class CShapeData;
//-------------------------------------------------------------------------------------------------

class CPhysicsComponent : public IComponent
{
public:
  CPhysicsComponent();

  void Update() override;

  float m_fLinearAccel;
  glm::vec3 m_linearVelocity;
  CTrack *m_pTrack;

  //for debugging
  CRenderer *m_pRenderer;
  CTexture *m_pTex;

private:
  //for debugging
  CShapeData *m_pDebugTri;
  CShapeData *m_pDebugLine;
};

//-------------------------------------------------------------------------------------------------
#endif