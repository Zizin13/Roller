#ifndef _WHIPLIB_UNMANGLER_H
#define _WHIPLIB_UNMANGLER_H
#include <stdint.h>
#include <vector>
//-------------------------------------------------------------------------------------------------

int GetUnmangledLength(const uint8_t *pSource, int iSourceLen);
bool UnmangleFile(const uint8_t *pSource, int iSourceLen, uint8_t *pDestination, int iLength);
void MangleFile(const std::vector<uint8_t> &source, std::vector<uint8_t> &destination);

//-------------------------------------------------------------------------------------------------
#endif