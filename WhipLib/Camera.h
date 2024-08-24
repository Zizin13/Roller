#ifndef _WHIPLIB_CAMERA_H
#define _WHIPLIB_CAMERA_H
//-------------------------------------------------------------------------------------------------
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------
class Camera
{
public:
  Camera();

  void MouseUpdate(const glm::vec2 &newMousePos);
  glm::mat4 GetWorldToViewMatrix() const;
  void MoveForward();
  void MoveBackward();
  void StrafeLeft();
  void StrafeRight();
  void MoveUp();
  void MoveDown();
  glm::vec3 GetPosition() { return m_position; };

  static float s_fMovementSpeed;

private:
  glm::vec3 m_position;
  glm::vec3 m_viewDirection;
  glm::vec3 m_strafeDirection;
  const glm::vec3 m_UP;
  glm::vec2 m_oldMousePos;
};
//-------------------------------------------------------------------------------------------------
#endif