#ifndef _ROLLER_DEBUGGUI_H
#define _ROLLER_DEBUGGUI_H
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
};

//-------------------------------------------------------------------------------------------------
#endif