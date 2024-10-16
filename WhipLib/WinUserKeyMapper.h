#ifndef _WHIPLIB_WINUSERKEYMAPPER_H
#define _WHIPLIB_WINUSERKEYMAPPER_H
//-------------------------------------------------------------------------------------------------
#include "KeyMapper.h"
//-------------------------------------------------------------------------------------------------

#ifdef IS_WINDOWS
class CWinUserKeyMapper : public IKeyMapper
{
public:
  void Update(uint64 &ullActionsPressed, glm::vec2 &mousePos) override;
};
#endif

//-------------------------------------------------------------------------------------------------
#endif