#ifndef _WHIPLIB_DRIVECOMPONENT_H
#define _WHIPLIB_DRIVECOMPONENT_H
//-------------------------------------------------------------------------------------------------
#include "Component.h"
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------

class CDriveComponent : public IComponent
{
public:
  CDriveComponent();

  void Update() override;

  float m_fMovementSpeed;
  float m_fRotateSpeed;

private:
  void MoveForward();
  void MoveBackward();
  void Coast();
  void TurnLeft();
  void TurnRight();
};

//-------------------------------------------------------------------------------------------------
#endif