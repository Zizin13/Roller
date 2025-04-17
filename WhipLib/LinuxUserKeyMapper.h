#ifndef _WHIPLIB_LINUXUSERKEYMAPPER_H
#define _WHIPLIB_LINUXUSERKEYMAPPER_H
//-------------------------------------------------------------------------------------------------
#include "KeyMapper.h"
//-------------------------------------------------------------------------------------------------

#ifndef IS_WINDOWS
class CLinuxUserKeyMapper : public IKeyMapper
{
public:
  void Update(uint64 &ullActionsPressed, glm::vec2 &mousePos) override;
};
#endif

//-------------------------------------------------------------------------------------------------
#endif
