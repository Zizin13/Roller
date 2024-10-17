#ifdef IS_WINDOWS
//-------------------------------------------------------------------------------------------------
#include "GlfwKeyMapper.h"
#include "GameInput.h"
#include <glfw3.h>
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

static void KeyCallbackCbStatic(GLFWwindow *pWindow, int iKey, int iScancode, int iAction, int iMods)
{
  CGlfwKeyMapper::GetGlfwKeyMapper().KeyCallback(pWindow, iKey, iScancode, iAction, iMods);
}

//-------------------------------------------------------------------------------------------------

CGlfwKeyMapper &CGlfwKeyMapper::GetGlfwKeyMapper()
{
  static CGlfwKeyMapper s_glfwKeyMapper;

  return s_glfwKeyMapper;
}

//-------------------------------------------------------------------------------------------------

void CGlfwKeyMapper::Update(uint64 &ullActionsPressed, glm::vec2 &mousePos)
{
  ullActionsPressed = m_ullActionsPressed;

  double dX, dY = 0.0;
  glfwGetCursorPos(m_pWindow, &dX, &dY);
  mousePos.x = (float)dX;
  mousePos.y = (float)dY;
}

//-------------------------------------------------------------------------------------------------

void CGlfwKeyMapper::Init(GLFWwindow *pWindow)
{
  m_pWindow = pWindow;
  m_ullActionsPressed = 0;
  glfwSetKeyCallback(pWindow, KeyCallbackCbStatic);
  if (glfwRawMouseMotionSupported())
    glfwSetInputMode(m_pWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  SetMouseCaptured(true);
}

//-------------------------------------------------------------------------------------------------

void CGlfwKeyMapper::KeyCallback(GLFWwindow *pWindow, int iKey, int iScancode, int iAction, int iMods)
{
  (void)(pWindow); (void)(iScancode); (void)(iMods);
  if (iKey == GLFW_KEY_W) {
    if (iAction == GLFW_PRESS)
      m_ullActionsPressed |= ACTION_FORWARD;
    else if (iAction == GLFW_RELEASE)
      m_ullActionsPressed &= ~ACTION_FORWARD;
  }
  if (iKey == GLFW_KEY_A) {
    if (iAction == GLFW_PRESS)
      m_ullActionsPressed |= ACTION_LEFT;
    else if (iAction == GLFW_RELEASE)
      m_ullActionsPressed &= ~ACTION_LEFT;
  }
  if (iKey == GLFW_KEY_S) {
    if (iAction == GLFW_PRESS)
      m_ullActionsPressed |= ACTION_BACKWARD;
    else if (iAction == GLFW_RELEASE)
      m_ullActionsPressed &= ~ACTION_BACKWARD;
  }
  if (iKey == GLFW_KEY_D) {
    if (iAction == GLFW_PRESS)
      m_ullActionsPressed |= ACTION_RIGHT;
    else if (iAction == GLFW_RELEASE)
      m_ullActionsPressed &= ~ACTION_RIGHT;
  }
  if (iKey == GLFW_KEY_E || iKey == GLFW_KEY_R) {
    if (iAction == GLFW_PRESS)
      m_ullActionsPressed |= ACTION_UP;
    else if (iAction == GLFW_RELEASE)
      m_ullActionsPressed &= ~ACTION_UP;
  }
  if (iKey == GLFW_KEY_Q || iKey == GLFW_KEY_F) {
    if (iAction == GLFW_PRESS)
      m_ullActionsPressed |= ACTION_DOWN;
    else if (iAction == GLFW_RELEASE)
      m_ullActionsPressed &= ~ACTION_DOWN;
  }
  if (iKey == GLFW_KEY_C && iAction == GLFW_PRESS) {
    SetMouseCaptured(!m_bMouseCaptured);
  }
}

//-------------------------------------------------------------------------------------------------

void CGlfwKeyMapper::SetMouseCaptured(bool bCaptured)
{
  m_bMouseCaptured = bCaptured;
  glfwSetInputMode(m_pWindow, GLFW_CURSOR, m_bMouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

//-------------------------------------------------------------------------------------------------
#endif