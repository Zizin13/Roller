#include "Scene.h"
#include "Camera.h"
#include "Renderer.h"
#include "Entity.h"
#include "NoclipComponent.h"
#include "Track.h"
#include "TrackComponent.h"
#include "ShapeComponent.h"
#include "DriveComponent.h"
#include "PhysicsComponent.h"
#include "SceneManager.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
static std::string sEmpty = "";
//-------------------------------------------------------------------------------------------------

class CScenePrivate
{
public:
  CScenePrivate()
    : m_pPlayer(NULL)
  {};
  ~CScenePrivate()
  {
    m_car.Shutdown();
    m_trackEntity.Shutdown();
    m_defaultEntity.Shutdown();
    m_renderer.Shutdown();
  };

  CRenderer m_renderer;
  CEntity *m_pPlayer;

  //default entity
  CNoclipComponent m_noClip;
  CEntity m_defaultEntity;
  CCamera m_defaultCamera;

  //track entity
  CTrack m_track;
  CEntity m_trackEntity;
  CTrackComponent m_trackComponent;
  CShapeComponent m_trackShapeComponent;

  //car entity
  CEntity m_car;
  CDriveComponent m_driveComponent;
  CPhysicsComponent m_physicsComponent;
  CShapeComponent m_carShapeComponent;
  CCamera m_carCamera;
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

bool CScene::Init()
{
  bool bSuccess = false;
  Shutdown();
  p = new CScenePrivate;

  bSuccess |= p->m_renderer.Init();
  bSuccess |= p->m_noClip.Init();
  bSuccess |= p->m_defaultCamera.Init();
  bSuccess |= p->m_defaultEntity.Init();
  p->m_defaultEntity.AddComponent(&p->m_noClip);
  p->m_defaultEntity.AddComponent(&p->m_defaultCamera);
  p->m_defaultEntity.m_position = glm::vec3(0.0f, 4000.0f, -5000.0f);
  SetPlayer(&p->m_defaultEntity);

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
  p->m_car.Update();
  CCamera *pCamera = p->m_pPlayer->GetComponent<CCamera>();
  p->m_renderer.Draw(iWindowWidth, iWindowHeight, pCamera);
}

//-------------------------------------------------------------------------------------------------

void CScene::SpawnCar(eWhipModel model)
{
  if (!p)
    return;

  //make shape
  CShapeData *pCarShape = NULL;
  int iShapeKey = p->m_renderer.MakeCarShape(&pCarShape, model);

  //setup components
  p->m_carShapeComponent.m_pShapeData = pCarShape;
  p->m_carShapeComponent.m_iShapeKey = iShapeKey;
  p->m_carShapeComponent.m_rotationOffset = glm::rotate(glm::radians(-90.0f), glm::vec3(0, 0, 1)) * //car starts on its side
    glm::rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0)); //entity starts facing z positive, car starts facing x positive
  p->m_carShapeComponent.Init();
  p->m_physicsComponent.m_pRenderer = &p->m_renderer;
  p->m_physicsComponent.m_pTex = p->m_track.m_pTex;
  p->m_carCamera.m_offset = glm::vec3(0.0f, 800.0f, -3000.0f);
  p->m_carCamera.Init();
  p->m_physicsComponent.m_pTrack = &p->m_track;
  p->m_driveComponent.Init();
  p->m_physicsComponent.Init();

  //setup car entity
  p->m_car.AddComponent(&p->m_driveComponent);
  p->m_car.AddComponent(&p->m_physicsComponent);
  p->m_car.AddComponent(&p->m_carShapeComponent);
  p->m_car.AddComponent(&p->m_carCamera);
  p->m_car.Init();
  SetPlayer(&p->m_car);
}

//-------------------------------------------------------------------------------------------------

void CScene::UpdateCarModel(eWhipModel model)
{
  if (!p)
    return;

  p->m_renderer.DeleteCarShape(p->m_carShapeComponent.m_iShapeKey);

  //make shape
  CShapeData *pCarShape = NULL;
  int iShapeKey = p->m_renderer.MakeCarShape(&pCarShape, model);
  //setup components
  p->m_carShapeComponent.m_pShapeData = pCarShape;
  p->m_carShapeComponent.m_iShapeKey = iShapeKey;
}

//-------------------------------------------------------------------------------------------------

void CScene::LoadTrack(const std::string &sTrackFile)
{
  if (!p)
    return;

  //load track
  p->m_track.LoadTrack(sTrackFile);
  p->m_track.LoadTextures();

  //make shapes
  p->m_renderer.MakeTrackShape(&p->m_trackShapeComponent.m_pShapeData, &p->m_track);
  p->m_renderer.MakeSigns(&p->m_track);
  p->m_renderer.MakeEnvirFloor(&p->m_track);

  //setup components
  p->m_trackComponent.SetData(&p->m_track, &p->m_renderer);
  p->m_trackShapeComponent.Init();
  p->m_trackComponent.Init();

  //setup track entity
  p->m_trackEntity.AddComponent(&p->m_trackShapeComponent);
  p->m_trackEntity.AddComponent(&p->m_trackComponent);
  p->m_trackEntity.Init();
}

//-------------------------------------------------------------------------------------------------

bool CScene::SetPlayer(CEntity *pEntity)
{
  if (!pEntity || !p)
    return false;
  
  //can't make an entity a player without a camera
  CCamera *pCamera = pEntity->GetComponent<CCamera>();
  if (!pCamera)
    return false;

  //old player should no longer accept controls
  if (p->m_pPlayer)
    p->m_pPlayer->m_bAcceptControls = false;

  //set new player
  p->m_pPlayer = pEntity;
  pEntity->m_bAcceptControls = true;
  return true;
}

//-------------------------------------------------------------------------------------------------

void CScene::TogglePlayer()
{
  if (!p)
    return;

  if (p->m_pPlayer == &p->m_defaultEntity)
    SetPlayer(&p->m_car);
  else
    SetPlayer(&p->m_defaultEntity);
}

//-------------------------------------------------------------------------------------------------

const std::string &CScene::GetCurrentTrack()
{
  if (!p)
    return sEmpty;

  return p->m_track.m_sTrackFile;
}

//-------------------------------------------------------------------------------------------------