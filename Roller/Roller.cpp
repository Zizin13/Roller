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
#include "Scene.h"
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

  //init test scene
  CScene testScene;
  testScene.Init("C:\\WHIP\\WHIPLASH\\FATDATA");
  testScene.SpawnCar(eWhipModel::CAR_YZIZIN);
  testScene.LoadTrack("TRACK3.TRK");

#if defined(_DEBUG)
  //init framerate test
  float fTimer = 0.0f;
  int iNumFrames = 0;
#endif

  //Loop until the user closes the window
  while (!glfwWindowShouldClose(pWindow)) {
#if defined(_DEBUG)
    //calculate framerate
    fTimer += CGameClock::GetGameClock().DeltaTimeLastFrame();
    ++iNumFrames;
    if (fTimer >= 1.0f) {
      char szFramerate[100];
      snprintf(szFramerate, sizeof(szFramerate), "%d fps\n", iNumFrames);
      //OutputDebugString(szFramerate);
      fTimer = 0.0f;
      iNumFrames = 0;
    }
#endif

    //get window size
    int iWidth, iHeight = 0;
    glfwGetWindowSize(pWindow, &iWidth, &iHeight);

    //update game
    CGameClock::GetGameClock().NewFrame();
    CGameInput::GetGameInput().Update();
    testScene.Update(iWidth, iHeight);

    //Swap front and back buffers
    glfwSwapBuffers(pWindow);
    //Poll for and process events
    glfwPollEvents();
  }

  //shutdown
  testScene.Shutdown();
  glfwTerminate();

  return 0;
};

//-------------------------------------------------------------------------------------------------