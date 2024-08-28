#ifndef _WHIPLIB_DISASMHELPERS_H
#define _WHIPLIB_DISASMHELPERS_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include "Polygon.h"
#include "Animation.h"
#include <string>
#include <vector>
//-------------------------------------------------------------------------------------------------

namespace DisasmHelpers
{
  extern std::vector<uint8> HexToBytes(const std::string &sBytes);
  extern std::vector<float> BytesToFloats(const std::vector<uint8> bytes);
  extern std::vector<uint32> BytesToUInts(const std::vector<uint8> bytes);
  extern std::vector<tPolygon> BytesToPols(const std::vector<uint8> bytes);
  extern std::vector<tAnimation> UIntsToAnms(const std::vector<uint32> uints);
  extern std::string GetStringBytes(const std::string &sLine);
}

//-------------------------------------------------------------------------------------------------
#endif