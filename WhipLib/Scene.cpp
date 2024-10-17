#include "Scene.h"
#include "Camera.h"
#include "Renderer.h"
#include "Entity.h"
#include "NoclipComponent.h"
#include "Track.h"
#include "TrackComponent.h"
#include "ShapeComponent.h"
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
    m_trackEntity.Shutdown();
    m_defaultEntity.Shutdown();
    m_renderer.Shutdown();
  };

  std::string m_sFatDataDir;
  CCamera m_camera;
  CRenderer m_renderer;
  CNoclipComponent m_noClip;
  CEntity m_defaultEntity;

  CTrack m_track;
  CEntity m_trackEntity;
  CTrackComponent m_trackComponent;
  CShapeComponent m_trackShapeComponent;
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
  p->m_trackEntity.Update();
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

void CScene::LoadTrack(const std::string &sTrackFile)
{
  if (!p)
    return;

  //load track
  p->m_track.LoadTrack(p->m_sFatDataDir + "/" + sTrackFile);
  p->m_track.LoadTextures();

  //make shapes
  CShapeData *pTrackShape = p->m_renderer.MakeTrackShape(&p->m_track);
  p->m_renderer.MakeSigns(&p->m_track);
  p->m_renderer.MakeEnvirFloor(&p->m_track);

  //setup components
  p->m_trackShapeComponent.m_pShapeData = pTrackShape;
  p->m_trackComponent.SetData(&p->m_track, &p->m_renderer);
  p->m_trackShapeComponent.Init();
  p->m_trackComponent.Init();

  //setup track entity
  p->m_trackEntity.AddComponent(&p->m_trackShapeComponent);
  p->m_trackEntity.AddComponent(&p->m_trackComponent);
  p->m_trackEntity.Init();
}

//-------------------------------------------------------------------------------------------------