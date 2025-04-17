#ifndef IS_WINDOWS
//-------------------------------------------------------------------------------------------------
#include "LinuxUserKeyMapper.h"
#include "GameInput.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

void CLinuxUserKeyMapper::Update(uint64 &ullActionsPressed, glm::vec2 &mousePos)
{
/*
  ullActionsPressed = 0;
  if (GetAsyncKeyState(0x57)) //W
    ullActionsPressed |= ACTION_FORWARD;
  if (GetAsyncKeyState(0x41)) //A
    ullActionsPressed |= ACTION_LEFT;
  if (GetAsyncKeyState(0x53)) //S
    ullActionsPressed |= ACTION_BACKWARD;
  if (GetAsyncKeyState(0x44)) //D
    ullActionsPressed |= ACTION_RIGHT;
  if (GetAsyncKeyState(0x52) //R
      || GetAsyncKeyState(0x45)) //E
    ullActionsPressed |= ACTION_UP;
  if (GetAsyncKeyState(0x46) //F
      || GetAsyncKeyState(0x51)) //Q
    ullActionsPressed |= ACTION_DOWN;
  if (GetAsyncKeyState(VK_LBUTTON)
      || GetAsyncKeyState(VK_RBUTTON))
    ullActionsPressed |= ACTION_CLICK;

  POINT mousePoint;
  GetCursorPos(&mousePoint);
  mousePos.x = (float)mousePoint.x;
  mousePos.y = (float)mousePoint.y;
*/
}

//-------------------------------------------------------------------------------------------------
#endif
