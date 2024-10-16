#ifndef _WHIPLIB_GAMEINPUT_H
#define _WHIPLIB_GAMEINPUT_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------
#define ACTION_FORWARD  0x00000001
#define ACTION_BACKWARD 0x00000002
#define ACTION_LEFT     0x00000004
#define ACTION_RIGHT    0x00000008
#define ACTION_UP       0x00000010
#define ACTION_DOWN     0x00000020
#define ACTION_CLICK    0x00000040
//-------------------------------------------------------------------------------------------------
class IKeyMapper;
//-------------------------------------------------------------------------------------------------

class CGameInput
{
public:
  static CGameInput &GetGameInput();
  CGameInput(const CGameInput &) = delete;
  CGameInput &operator=(const CGameInput &) = delete;

  bool Init(IKeyMapper *pKeyMapper);
  void Update();
  const uint64 &GetActionsPressed() { return m_ullActionsPressed; };
  const glm::vec2 &GetMousePos() { return m_mousePos; };

private:
  CGameInput() {}

  IKeyMapper *m_pKeyMapper;
  uint64 m_ullActionsPressed;
  glm::vec2 m_mousePos;
};
//-------------------------------------------------------------------------------------------------
#endif