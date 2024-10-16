//-------------------------------------------------------------------------------------------------
#define GLFW_INCLUDE_NONE
#include <windows.h>
#include <glfw3.h>
#include <glew.h>
#include "Logging.h"
#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"
#include "Entity.h"
#include "NoclipComponent.h"
#include "GameClock.h"
#include "GameInput.h"
#include "WinUserKeyMapper.h"
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

  CGameClock::GetGameClock().Init();
  CWinUserKeyMapper keyMapper;
  CGameInput::GetGameInput().Init(&keyMapper);

  CCamera camera;

  CRenderer renderer;
  if (!renderer.Init("C:\\WHIP\\WHIPLASH\\FATDATA"))
    return -1;
  renderer.MakeCarShape(eWhipModel::CAR_YZIZIN);

  CNoclipComponent noclip;
  noclip.Init();

  CEntity defaultEntity;
  defaultEntity.Init();
  defaultEntity.AddComponent(&noclip);

  //Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    CGameClock::GetGameClock().NewFrame();
    CGameInput::GetGameInput().Update();

    int iWidth, iHeight = 0;
    glfwGetWindowSize(window, &iWidth, &iHeight);

    defaultEntity.Update();

    camera.m_position = defaultEntity.m_position;
    camera.m_viewDirection = defaultEntity.m_orientation;
    renderer.Draw(iWidth, iHeight, &camera);

    //Swap front and back buffers
    glfwSwapBuffers(window);
    //Poll for and process events
    glfwPollEvents();
  }

  defaultEntity.Shutdown();
  renderer.Shutdown();
  glfwTerminate();

  return 0;
};

//-------------------------------------------------------------------------------------------------