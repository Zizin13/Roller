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
  viewDirection = glm::mat3(glm::rotate((mouseDelta.x * -0.01f), UP)) * viewDirection;
  oldMousePos = newMousePos;
}

glm::mat4 Camera::getWorldToViewMatrix() const
{
  return glm::lookAt(position, position + viewDirection, UP);
}