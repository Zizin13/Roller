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
  GLFWwindow *m_pWindow;
};

//-------------------------------------------------------------------------------------------------
#endif