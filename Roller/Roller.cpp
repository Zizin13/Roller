//-------------------------------------------------------------------------------------------------
#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#include <windows.h>
#include <glfw3.h>
#include <glew.h>
#include "Logging.h"
#include "GameClock.h"
#include "GameInput.h"
#include "GlfwKeyMapper.h"
#include "DebugGui.h"
#include "SceneManager.h"
#include "stb_image.h"
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
  //memory leak detection
#if defined(_DEBUG) && defined(IS_WINDOWS)
  _set_error_mode(_OUT_TO_MSGBOX);
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  std::string sAppPath = argv[0];
  size_t pos = sAppPath.find_last_of("/");
  if (pos == std::string::npos)
    pos = sAppPath.find_last_of("\\");
  sAppPath = sAppPath.substr(0, pos + 1);

  //init glfw
  if (!glfwInit())
    return -1;

  //Create a windowed mode window and its OpenGL context
  GLFWwindow *pWindow;
  pWindow = glfwCreateWindow(640, 480, "Roller", NULL, NULL);
  if (!pWindow) {
    glfwTerminate();
    return -1;
  }

  //load window icon
  GLFWimage image;
  image.pixels = stbi_load("./images/fatal.png", &image.width, &image.height, 0, 4);
  glfwSetWindowIcon(pWindow, 1, &image);
  stbi_image_free(image.pixels);

  //Make the window's context current
  glfwMakeContextCurrent(pWindow);
  //don't limit framerate, must be done after glfwMakeContextCurrent
  glfwSwapInterval(0);
  
  //init game
  Logging::SetWhipLibLoggingCallback(LogMessageCbStatic);
  CGameClock::GetGameClock().Init();
  CGlfwKeyMapper::GetGlfwKeyMapper().Init(pWindow);
  CGameInput::GetGameInput().Init(&CGlfwKeyMapper::GetGlfwKeyMapper());
  CSceneManager::GetSceneManager().Init(sAppPath);

  //init imgui
  CDebugGui debugGui;
  debugGui.Init(pWindow, sAppPath);

  //Loop until the user closes the window
  while (!glfwWindowShouldClose(pWindow)) {
    //get window size
    int iWidth, iHeight = 0;
    glfwGetWindowSize(pWindow, &iWidth, &iHeight);

    //update game
    CGameClock::GetGameClock().NewFrame();
    CGameInput::GetGameInput().Update();
    CSceneManager::GetSceneManager().Update(iWidth, iHeight);

    //update imgui
    debugGui.Update();

    //Swap front and back buffers
    glfwSwapBuffers(pWindow);
    //Poll for and process events
    glfwPollEvents();
  }

  //shutdown
  debugGui.Shutdown();
  CSceneManager::GetSceneManager().Shutdown();
  glfwTerminate();

  return 0;
};

//-------------------------------------------------------------------------------------------------