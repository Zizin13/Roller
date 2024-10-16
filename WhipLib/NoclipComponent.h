#ifndef _WHIPLIB_NOCLIPCOMPONENT_H
#define _WHIPLIB_NOCLIPCOMPONENT_H
//-------------------------------------------------------------------------------------------------
#include "glm.hpp"
#include "Component.h"
//-------------------------------------------------------------------------------------------------
class CNoclipComponent : public IComponent
{
public:
  CNoclipComponent(bool bRequiresClick = false);

  void Update() override;

  static float s_fMovementSpeed;

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
  bool m_bRequiresClick;
};
//-------------------------------------------------------------------------------------------------
#endif