#include "DebugGui.h"
#include "glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GameClock.h"
#include "SceneManager.h"
#ifdef IS_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <ShellScalingApi.h>
#endif
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

static int FatdataDirCbStatic(ImGuiInputTextCallbackData *pData)
{
  CSceneManager::GetSceneManager().SetFatDataDir(pData->Buf);
  return 0;
}

//-------------------------------------------------------------------------------------------------

CDebugGui::CDebugGui()
  : m_pWindow(NULL)
  , m_fTimer(0.0f)
  , m_iNumFrames(0)
  , m_iFramerate(0)
{

}

//-------------------------------------------------------------------------------------------------

CDebugGui::~CDebugGui()
{

}

//-------------------------------------------------------------------------------------------------

void CDebugGui::Init(GLFWwindow *pWindow)
{
  m_pWindow = pWindow;

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
  float fScale = GetDpi();
  style.ScaleAllSizes(fScale);
  io.Fonts->Clear();
#ifdef IS_WINDOWS
  ImFont *font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 16.0f * fScale);
#endif
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }
  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
  const char *glsl_version = "#version 130";
  ImGui_ImplOpenGL3_Init(glsl_version);
}

//-------------------------------------------------------------------------------------------------

void CDebugGui::Update()
{
  if (!m_pWindow) {
    assert(0);
    return;
  }

  //calculate framerate
  m_fTimer += CGameClock::GetGameClock().DeltaTimeLastFrame();
  ++m_iNumFrames;
  if (m_fTimer >= 1.0f) {
    m_iFramerate = m_iNumFrames;
    m_fTimer = 0.0f;
    m_iNumFrames = 0;
  }

  //get data for gui
  char szFatdataDir[1024];
  snprintf(szFatdataDir, sizeof(szFatdataDir), CSceneManager::GetSceneManager().GetFatDataDir().c_str());

  //imgui
  //if (glfwGetWindowAttrib(m_pWindow, GLFW_ICONIFIED) != 0) {
  //  ImGui_ImplGlfw_Sleep(10);
  //  continue;
  //}
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::Begin("Debug Menu");
  ImGui::Text("%d fps", m_iFramerate);
  ImGui::InputText("FATDATA directory", szFatdataDir, sizeof(szFatdataDir),
                   ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_EnterReturnsTrue,
                   FatdataDirCbStatic);
  ImGui::End();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  ImGuiIO &io = ImGui::GetIO(); (void)io;
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

//-------------------------------------------------------------------------------------------------

void CDebugGui::Shutdown()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

//-------------------------------------------------------------------------------------------------

float CDebugGui::GetDpi()
{
  float fScale = 1.0f;
#ifdef IS_WINDOWS
  HWND hwnd = glfwGetWin32Window(m_pWindow);
  if (hwnd == nullptr)
    return fScale;

  HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
  unsigned int uiDpiX;
  unsigned int uiDpiY;
  GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &uiDpiX, &uiDpiY);
  fScale = uiDpiX / 96.0f;
#endif
  return fScale;
}

//-------------------------------------------------------------------------------------------------