//-------------------------------------------------------------------------------------------------
#define GLFW_INCLUDE_NONE
#include <windows.h>
#include <glfw3.h>
#include <glew.h>
#include "Logging.h"
#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

static void LogMessageCbStatic(const char *szMsg, int iLen)
{
  (void)(iLen);
  OutputDebugString(szMsg);
  OutputDebugString("\n");
}

//-------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
#if defined(_DEBUG) && defined(IS_WINDOWS)
  _set_error_mode(_OUT_TO_MSGBOX);
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
  GLFWwindow *window;

  if (!glfwInit())
    return -1;

  //Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(640, 480, "Roller", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  Logging::SetWhipLibLoggingCallback(LogMessageCbStatic);

  //Make the window's context current
  glfwMakeContextCurrent(window);

  CCamera camera;
  camera.m_position.z = 500.0f;
  camera.m_position.y = 1300.0f;
  camera.m_viewDirection.z = 0.05f;
  camera.m_viewDirection.y = -1.0f;

  CRenderer renderer;
  if (!renderer.Init("C:\\WHIP\\WHIPLASH\\FATDATA"))
    return -1;
  renderer.MakeCarShape(eWhipModel::CAR_YZIZIN);

  //Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    int iWidth, iHeight = 0;
    glfwGetWindowSize(window, &iWidth, &iHeight);

    renderer.Draw(iWidth, iHeight, &camera);

    //Swap front and back buffers
    glfwSwapBuffers(window);
    //Poll for and process events
    glfwPollEvents();
  }

  renderer.Shutdown();
  glfwTerminate();

  return 0;
};

//-------------------------------------------------------------------------------------------------