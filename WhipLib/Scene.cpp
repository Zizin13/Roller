#include "Scene.h"
#include "Camera.h"
#include "Renderer.h"
#include "Entity.h"
#include "NoclipComponent.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

class CScenePrivate
{
public:
  CScenePrivate() {};
  ~CScenePrivate()
  {
    m_defaultEntity.Shutdown();
    m_renderer.Shutdown();
  };

  std::string m_sFatDataDir;
  CCamera m_camera;
  CRenderer m_renderer;
  CNoclipComponent m_noClip;
  CEntity m_defaultEntity;
};

//-------------------------------------------------------------------------------------------------

CScene::CScene()
  : p(NULL)
{
}

//-------------------------------------------------------------------------------------------------

CScene::~CScene()
{
  Shutdown();
}

//-------------------------------------------------------------------------------------------------

bool CScene::Init(const std::string &sFatDataDir)
{
  bool bSuccess = false;
  Shutdown();
  p = new CScenePrivate;

  p->m_sFatDataDir = sFatDataDir;
  bSuccess |= p->m_renderer.Init(p->m_sFatDataDir);
  bSuccess |= p->m_noClip.Init();
  bSuccess |= p->m_defaultEntity.Init();
  p->m_defaultEntity.AddComponent(&p->m_noClip);

  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

bool CScene::Shutdown()
{
  if (p) {
    delete p;
    p = NULL;
  }
  return true;
}

//-------------------------------------------------------------------------------------------------

void CScene::Update(int iWindowWidth, int iWindowHeight)
{
  if (!p)
    return;

  p->m_defaultEntity.Update();
  p->m_camera.m_position = p->m_defaultEntity.m_position;
  p->m_camera.m_viewDirection = p->m_defaultEntity.m_orientation;
  p->m_renderer.Draw(iWindowWidth, iWindowHeight, &p->m_camera);
}

//-------------------------------------------------------------------------------------------------

void CScene::SpawnCar(eWhipModel model)
{
  if (!p)
    return;

  p->m_renderer.MakeCarShape(model);
}

//-------------------------------------------------------------------------------------------------