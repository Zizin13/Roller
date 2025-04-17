#ifndef _WHIPLIB_SCENEMANAGER_H
#define _WHIPLIB_SCENEMANAGER_H
//-------------------------------------------------------------------------------------------------
#include <string>
//-------------------------------------------------------------------------------------------------
class CScene;
//-------------------------------------------------------------------------------------------------

class CSceneManager
{
public:
  static CSceneManager &GetSceneManager();
  ~CSceneManager();
  CSceneManager(CSceneManager const &) = delete;
  void operator=(CSceneManager const &) = delete;
  
  void Init(const std::string &sAppPath);
  void Update(int iWindowWidth, int iWindowHeight);
  void Shutdown();
  void SetFatDataDir(const std::string &sFatDataDir);
  const std::string &GetFatDataDir() { return m_sFatDataDir; };
  CScene *GetCurrentScene();

private:
  CSceneManager();

  CScene *m_pScene; //only one scene for now
  std::string m_sFatDataDir;
  std::string m_sAppPath;
};

//-------------------------------------------------------------------------------------------------
#endif