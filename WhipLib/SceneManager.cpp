#include "SceneManager.h"
#include "Scene.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CSceneManager &CSceneManager::GetSceneManager()
{
  static CSceneManager s_sceneMgr;

  return s_sceneMgr;
}

//-------------------------------------------------------------------------------------------------

CSceneManager::CSceneManager()
  : m_pScene(NULL)
  , m_sFatDataDir("C:\\WHIP\\WHIPLASH\\FATDATA")
  , m_sAppPath("")
{

}

//-------------------------------------------------------------------------------------------------

CSceneManager::~CSceneManager()
{
  Shutdown();
}

//-------------------------------------------------------------------------------------------------

void CSceneManager::Init(const std::string &sAppPath)
{
  if (m_pScene)
    return;

  m_sAppPath = sAppPath;
  m_pScene = new CScene();
  m_pScene->Init(sAppPath);
  std::string sTrackFile = "/TRACK8.TRK";
  m_pScene->LoadTrack(m_sFatDataDir + sTrackFile);
  m_pScene->SpawnCar(eWhipModel::CAR_YZIZIN);
}

//-------------------------------------------------------------------------------------------------

void CSceneManager::Update(int iWindowWidth, int iWindowHeight)
{
  if (m_pScene)
    m_pScene->Update(iWindowWidth, iWindowHeight);
}

//-------------------------------------------------------------------------------------------------

void CSceneManager::Shutdown()
{
  if (m_pScene) {
    m_pScene->Shutdown();
    delete m_pScene;
  }
  m_pScene = NULL;
}

//-------------------------------------------------------------------------------------------------

void CSceneManager::SetFatDataDir(const std::string &sFatDataDir)
{
  m_sFatDataDir = sFatDataDir;
  Shutdown();
  Init(m_sAppPath);
}

//-------------------------------------------------------------------------------------------------

CScene *CSceneManager::GetCurrentScene()
{
  return m_pScene;
}

//-------------------------------------------------------------------------------------------------