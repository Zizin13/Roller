#ifndef _WHIPLIB_UNMANGLER_H
#define _WHIPLIB_UNMANGLER_H
//-------------------------------------------------------------------------------------------------
#include <stdint.h>
#include <vector>
#include "Types.h"
//-------------------------------------------------------------------------------------------------
#define MAX_MANGLED_LENGTH 67108864 // arbitrary 64 MB max, realistic maximum is much smaller
//-------------------------------------------------------------------------------------------------

namespace Unmangler
{
  extern int GetUnmangledLength(const uint8 *pSource, int iSourceLen);
  extern bool UnmangleFile(const uint8 *pSource, int iSourceLen, uint8 *pDestination, int iDestLength);
  extern void MangleFile(const std::vector<uint8> &source, std::vector<uint8> &destination);
}

//-------------------------------------------------------------------------------------------------
#endif