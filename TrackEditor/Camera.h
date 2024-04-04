#pragma once
#include "glm.hpp"

class Camera
{
  glm::vec3 position;
  glm::vec3 viewDirection;
  const glm::vec3 UP;
  glm::vec2 oldMousePos;
public:
  Camera();
  void mouseUpdate(const glm::vec2 &newMousePos);
  glm::mat4 getWorldToViewMatrix() const;
  void moveForward();
  void moveBackward();
  void strafeLeft();
  void strafeRight();
  void moveUp();
  void moveDown();
};