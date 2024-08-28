#ifndef _WHIPLIB_CARHELPERS_H
#define _WHIPLIB_CARHELPERS_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include <string>
//-------------------------------------------------------------------------------------------------

namespace CarHelpers
{
  extern std::string GetCarNameFromModel(eWhipModel model);
  extern std::string GetCarTextureFromModel(eWhipModel model);
  extern void RemapColor(eWhipModel model, uint32 &uiTex);
}

//-------------------------------------------------------------------------------------------------
#endif