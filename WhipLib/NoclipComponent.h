#ifndef _WHIPLIB_NOCLIPCOMPONENT_H
#define _WHIPLIB_NOCLIPCOMPONENT_H
//-------------------------------------------------------------------------------------------------
#include "glm.hpp"
#include "Component.h"
//-------------------------------------------------------------------------------------------------
class CNoclipComponent : public IComponent
{
public:
  CNoclipComponent();

  void Update() override;

private:
  void MouseUpdate(const glm::vec2 &newMousePos);
  void MoveForward();
  void MoveBackward();
  void StrafeLeft();
  void StrafeRight();
  void MoveUp();
  void MoveDown();

  glm::vec3 m_strafeDirection;
  glm::vec2 m_oldMousePos;
  const glm::vec3 m_UP;
  static float s_fMovementSpeed;
};
//-------------------------------------------------------------------------------------------------
#endif