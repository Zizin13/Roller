#ifndef _WHIPLIB_KEYMAPPER_H
#define _WHIPLIB_KEYMAPPER_H
//-------------------------------------------------------------------------------------------------
#include "glm.hpp"
#include "Types.h"
//-------------------------------------------------------------------------------------------------

class IKeyMapper
{
public:
  virtual ~IKeyMapper() {};
  virtual void Update(uint64 &ullActionsPressed, glm::vec2 &mousePos) = 0;
};

//-------------------------------------------------------------------------------------------------
#endif