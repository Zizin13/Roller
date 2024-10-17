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

  glm::vec3 m_position;
  glm::vec3 m_viewDirection;
  static const glm::vec3 s_UP;
};

//-------------------------------------------------------------------------------------------------
#endif