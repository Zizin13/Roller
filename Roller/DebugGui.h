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

private:
  float GetDpi();

  GLFWwindow *m_pWindow;
  float m_fTimer;
  int m_iNumFrames;
  int m_iFramerate;
};

//-------------------------------------------------------------------------------------------------
#endif