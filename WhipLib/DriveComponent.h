#ifndef _WHIPLIB_DRIVECOMPONENT_H
#define _WHIPLIB_DRIVECOMPONENT_H
//-------------------------------------------------------------------------------------------------
#include "Component.h"
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
  void TurnLeft();
  void TurnRight();
};

//-------------------------------------------------------------------------------------------------
#endif