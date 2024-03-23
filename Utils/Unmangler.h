#ifndef _UTILS_UNMANGLER_H
#define _UTILS_UNMANGLER_H
#include <stdint.h>
#include <vector>
//-------------------------------------------------------------------------------------------------

int GetUnmangledLength(const uint8_t *pSource, int iSourceLen);
bool UnmangleFile(const uint8_t *pSource, int iSourceLen, uint8_t *pDestination, int iLength);
//std::vector<uint8_t> MangleFile(const std::vector<uint8_t> &source, std::vector<uint8_t> &destination);

//-------------------------------------------------------------------------------------------------
#endif