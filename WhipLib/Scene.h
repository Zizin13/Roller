#ifndef _WHIPLIB_SCENE_H
#define _WHIPLIB_SCENE_H
//-------------------------------------------------------------------------------------------------
#include <string>
#include "Types.h"
//-------------------------------------------------------------------------------------------------
class CScenePrivate;
class CEntity;
//-------------------------------------------------------------------------------------------------

class CScene
{
public:
  CScene();
  ~CScene();

  bool Init(const std::string &sAppPath);
  bool Shutdown();
  void Update(int iWindowWidth, int iWindowHeight);
  void SpawnCar(eWhipModel model);
  void UpdateCarModel(eWhipModel model);
  void LoadTrack(const std::string &sTrackFile);
  bool SetPlayer(CEntity *pEntity);
  void TogglePlayer();
  const std::string &GetCurrentTrack();

private:
  CScenePrivate *p;
};

//-------------------------------------------------------------------------------------------------
#endif