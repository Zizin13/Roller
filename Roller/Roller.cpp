//-------------------------------------------------------------------------------------------------
#define GLFW_INCLUDE_NONE
#include <windows.h>
#include <glfw3.h>
#include <glew.h>
#include "OpenGLDebug.h"
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

  if (glewInit() != GLEW_OK)
    return -1;
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(GLErrorCb, 0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.24f);
  glLineWidth(3.0f);

  Camera camera;
  camera.m_position.z = 500.0f;
  camera.m_position.y = 1300.0f;
  camera.m_viewDirection.z = 0.05f;
  camera.m_viewDirection.y = -1.0f;

  CRenderer renderer;
  renderer.Init();
  renderer.MakeCarShape(eWhipModel::CAR_YZIZIN);

  //Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.03125f, 0.296875f, 0.984375f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    int iWidth, iHeight = 0;
    glfwGetWindowSize(window, &iWidth, &iHeight);
    glViewport(0, 0, iWidth, iHeight);

    renderer.Draw(iWidth, iHeight, &camera);

    //Swap front and back buffers
    glfwSwapBuffers(window);
    //Poll for and process events
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
};

//-------------------------------------------------------------------------------------------------