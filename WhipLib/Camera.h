#ifndef _WHIPLIB_CAMERA_H
#define _WHIPLIB_CAMERA_H
//-------------------------------------------------------------------------------------------------
#include "glm.hpp"
#include "Component.h"
//-------------------------------------------------------------------------------------------------

class CCamera : public IComponent
{
public:
  CCamera();

  void Update() override;
  glm::mat4 GetWorldToViewMatrix() const;
  glm::vec3 GetPosition() const;
  glm::vec3 GetViewDirection() const;

  glm::vec3 m_offset;
  static const glm::vec3 s_UP;

private:
  float m_fTimer;
  glm::vec3 m_position;
  glm::vec3 m_viewDirection;
};

//-------------------------------------------------------------------------------------------------
#endif