#ifndef _UTILS_UNMANGLER_H
#define _UTILS_UNMANGLER_H
#include <stdint.h>
//-------------------------------------------------------------------------------------------------

int GetUnmangledLength(const uint8_t *pSource, int iSourceLen);
bool UnmangleFile(const uint8_t *pSource, int iSourceLen, uint8_t *pDestination, int iLength);

//-------------------------------------------------------------------------------------------------
#endif