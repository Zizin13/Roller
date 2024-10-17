#ifndef _ROLLER_GLFWKEYMAPPER_H
#define _ROLLER_GLFWKEYMAPPER_H
//-------------------------------------------------------------------------------------------------
#include "KeyMapper.h"
//-------------------------------------------------------------------------------------------------
struct GLFWwindow;
//-------------------------------------------------------------------------------------------------

class CGlfwKeyMapper : public IKeyMapper
{
public:
  static CGlfwKeyMapper &GetGlfwKeyMapper();
  CGlfwKeyMapper(const CGlfwKeyMapper &) = delete;
  CGlfwKeyMapper &operator=(const CGlfwKeyMapper &) = delete;

  void Update(uint64 &ullActionsPressed, glm::vec2 &mousePos) override;

  void Init(GLFWwindow *pWindow);
  void KeyCallback(GLFWwindow *pWindow, int iKey, int iScancode, int iAction, int iMods);
  void SetMouseCaptured(bool bCaptured);

private:
  CGlfwKeyMapper() { m_pWindow = NULL; };

  GLFWwindow *m_pWindow;
  uint64 m_ullActionsPressed;
  bool m_bMouseCaptured;
};

//-------------------------------------------------------------------------------------------------
#endif