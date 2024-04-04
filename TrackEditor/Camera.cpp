#include "Camera.h"
#include "gtx\transform.hpp"

Camera::Camera()
  : viewDirection(0.0f, 0.0f, -1.0f)
  , UP(0.0f, 1.0f, 0.0f)
{

}

void Camera::mouseUpdate(const glm::vec2 &newMousePos)
{
  glm::vec2 mouseDelta = newMousePos - oldMousePos;
  if (glm::length(mouseDelta) > 50.0f) {
    oldMousePos = newMousePos;
    return;
  }
  const float ROTATIONAL_SPEED = 0.5f;
  glm::vec3 toRotateAround = glm::cross(viewDirection, UP);
  glm::mat4 rotator = glm::rotate(glm::radians(mouseDelta.x * ROTATIONAL_SPEED), UP) *
    glm::rotate(glm::radians(mouseDelta.y * ROTATIONAL_SPEED), toRotateAround);
  viewDirection = glm::mat3(rotator) * viewDirection;

  oldMousePos = newMousePos;
}

glm::mat4 Camera::getWorldToViewMatrix() const
{
  return glm::lookAt(position, position + viewDirection, UP);
}