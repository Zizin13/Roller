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
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
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

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  ImGuiStyle &style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }
  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
  const char *glsl_version = "#version 130";
  ImGui_ImplOpenGL3_Init(glsl_version);
  
  //init game
  Logging::SetWhipLibLoggingCallback(LogMessageCbStatic);
  CGameClock::GetGameClock().Init();
  CGlfwKeyMapper::GetGlfwKeyMapper().Init(pWindow);
  CGameInput::GetGameInput().Init(&CGlfwKeyMapper::GetGlfwKeyMapper());

  //init test scene
  CScene testScene;
  testScene.Init("C:\\WHIP\\WHIPLASH\\FATDATA");
  testScene.LoadTrack("TRACK8.TRK");
  testScene.SpawnCar(eWhipModel::CAR_XDESILVA);

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
      float fTest = CGameClock::GetGameClock().DeltaTimeLastFrame();
      snprintf(szFramerate, sizeof(szFramerate), "%d fps, dt = %.2f\n", iNumFrames, fTest);
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

    //imgui
    if (glfwGetWindowAttrib(pWindow, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow *backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }

    //Swap front and back buffers
    glfwSwapBuffers(pWindow);
    //Poll for and process events
    glfwPollEvents();
  }

  //shutdown
  testScene.Shutdown();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();

  return 0;
};

//-------------------------------------------------------------------------------------------------