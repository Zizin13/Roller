#ifndef _WHIPLIB_DRIVECOMPONENT_H
#define _WHIPLIB_DRIVECOMPONENT_H
//-------------------------------------------------------------------------------------------------
#include "Component.h"
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------
class CTrack;
//-------------------------------------------------------------------------------------------------

class CDriveComponent : public IComponent
{
public:
  CDriveComponent();

  void Update() override;

  float m_fMovementSpeed;
  float m_fRotateSpeed;
  CTrack *m_pTrack;

private:
  void MoveForward(glm::vec3 &newPos);
  void MoveBackward(glm::vec3 &newPos);
  void TurnLeft();
  void TurnRight();
};

//-------------------------------------------------------------------------------------------------
#endif