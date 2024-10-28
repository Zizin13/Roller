#include "DebugGui.h"
#include "glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GameClock.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CarHelpers.h"
#include <filesystem>
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
  CDebugGui *pDebugGui = (CDebugGui *)pData->UserData;
  pDebugGui->SetFatdataDir(pData->Buf);
  return 0;
}

//-------------------------------------------------------------------------------------------------

static eWhipModel s_carAy[] = {
  eWhipModel::CAR_F1WACK,
  eWhipModel::CAR_XAUTO,
  eWhipModel::CAR_XDESILVA,
  eWhipModel::CAR_XPULSE,
  eWhipModel::CAR_XGLOBAL,
  eWhipModel::CAR_XMILLION,
  eWhipModel::CAR_XMISSION,
  eWhipModel::CAR_XZIZIN,
  eWhipModel::CAR_XREISE,
  eWhipModel::CAR_YAUTO,
  eWhipModel::CAR_YDESILVA,
  eWhipModel::CAR_YPULSE,
  eWhipModel::CAR_YGLOBAL,
  eWhipModel::CAR_YMILLION,
  eWhipModel::CAR_YMISSION,
  eWhipModel::CAR_YZIZIN,
  eWhipModel::CAR_YREISE,
  eWhipModel::CAR_DEATH
};
static int s_carAyCount = sizeof(s_carAy) / sizeof(eWhipModel);

//-------------------------------------------------------------------------------------------------

CDebugGui::CDebugGui()
  : m_pWindow(NULL)
  , m_fTimer(0.0f)
  , m_iNumFrames(0)
  , m_iFramerate(0)
  , m_bEditFatdata(false)
  , m_iSelectedTrack(0)
  , m_iSelectedCar(15) //YZIZIN
{
  *m_szFatdataDir = '\0';
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

  SetFatdataDir(CSceneManager::GetSceneManager().GetFatDataDir().c_str());
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

  //init frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::Begin("Debug Menu");

  //framerate
  ImGui::Text("%d fps", m_iFramerate);

  //fatdata directory
  if (m_bEditFatdata) {
    ImGui::InputText("FATDATA directory", m_szFatdataDir, sizeof(m_szFatdataDir), ImGuiInputTextFlags_CallbackEdit, FatdataDirCbStatic, this);
    if (ImGui::Button("Apply FATDATA directory")) {
      m_bEditFatdata = false;
      CSceneManager::GetSceneManager().SetFatDataDir(m_szFatdataDir);
    }
  } else {
    ImGui::Text("FATDATA directory: %s", m_szFatdataDir);
    m_bEditFatdata = ImGui::Button("Edit FATDATA directory");
  }

  //load track
  if (m_iSelectedTrack < (int)m_trackAy.size()) {
    if (ImGui::BeginCombo("Track", m_trackAy[m_iSelectedTrack].c_str())) {
      for (int i = 0; i < (int)m_trackAy.size(); i++) {
        const bool bIsSelected = (m_iSelectedTrack == i);
        if (ImGui::Selectable(m_trackAy[i].c_str(), bIsSelected)) {
          m_iSelectedTrack = i;
          CScene *pScene = CSceneManager::GetSceneManager().GetCurrentScene();
          pScene->Shutdown();
          pScene->Init();
          pScene->LoadTrack(CSceneManager::GetSceneManager().GetFatDataDir() + "/" + m_trackAy[m_iSelectedTrack]);
          pScene->SpawnCar(s_carAy[m_iSelectedCar]);
        }
        if (bIsSelected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
  } else {
    assert(0);
  }

  //select car
  if (ImGui::BeginCombo("Car", CarHelpers::GetCarNameFromModel(s_carAy[m_iSelectedCar]).c_str())) {
    for (int i = 0; i < s_carAyCount; ++i) {
      const bool bIsSelected = (m_iSelectedCar == i);
      if (ImGui::Selectable(CarHelpers::GetCarNameFromModel(s_carAy[i]).c_str(), bIsSelected)) {
        m_iSelectedCar = i;
        CScene *pScene = CSceneManager::GetSceneManager().GetCurrentScene();
        pScene->UpdateCarModel(s_carAy[m_iSelectedCar]);
      }
      if (bIsSelected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }

  //toggle player
  if (ImGui::Button("Toggle Player")) {
    CScene *pScene = CSceneManager::GetSceneManager().GetCurrentScene();
    pScene->TogglePlayer();
  }

  //render imgui
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

void CDebugGui::SetFatdataDir(const char *szFatdataDir)
{
  strncpy_s(m_szFatdataDir, szFatdataDir, sizeof(m_szFatdataDir));

  //get current track
  std::string sCurrentTrack = "";
  CScene *pScene = CSceneManager::GetSceneManager().GetCurrentScene();
  if (pScene) {
    sCurrentTrack = pScene->GetCurrentTrack();
    size_t posWin = sCurrentTrack.find_last_of("\\");
    size_t posUnix = sCurrentTrack.find_last_of("/");
    size_t pos = posWin;
    if (pos == std::string::npos || posUnix > posWin)
      pos = posUnix;

    if (pos != std::string::npos) {
      sCurrentTrack = sCurrentTrack.substr(pos + 1, sCurrentTrack.size() + 1 - pos);
    }
  }

  //load all tracks in dir
  m_trackAy.clear();
  m_iSelectedTrack = 0;
  for (const auto &entry : std::filesystem::directory_iterator(m_szFatdataDir)) {
    std::string sEntry = entry.path().string();
    size_t pos = sEntry.find_last_of('.');
    if (pos != std::string::npos) {
      //see if file is TRK file
      std::string sExtension = sEntry.substr(pos, sEntry.size() - pos);
      if (sExtension.compare(".TRK") == 0) {
        //select current track
        if (sCurrentTrack.compare(entry.path().filename().string()) == 0) {
          m_iSelectedTrack = (int)m_trackAy.size();
        }

        m_trackAy.push_back(entry.path().filename().string());
      }
    }
  }
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