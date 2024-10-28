#ifndef _ROLLER_DEBUGGUI_H
#define _ROLLER_DEBUGGUI_H
//-------------------------------------------------------------------------------------------------
#include <vector>
#include <string>
//-------------------------------------------------------------------------------------------------
struct GLFWwindow;
//-------------------------------------------------------------------------------------------------

class CDebugGui
{
public:
  CDebugGui();
  ~CDebugGui();
  void Init(GLFWwindow *pWindow);
  void Update();
  void Shutdown();
  void SetFatdataDir(const char *szFatdataDir);

private:
  float GetDpi();

  GLFWwindow *m_pWindow;
  float m_fTimer;
  int m_iNumFrames;
  int m_iFramerate;
  bool m_bEditFatdata;
  char m_szFatdataDir[1024];
  std::vector<std::string> m_trackAy;
  int m_iSelectedTrack;
  int m_iSelectedCar;
};

//-------------------------------------------------------------------------------------------------
#endif