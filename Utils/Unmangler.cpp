#define _CRT_SECURE_NO_DEPRECATE
#include "Unmangler.h"
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <sstream>
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

int GetUnmangledLength(const uint8_t *pSource, int iSourceLen)
{
  if (iSourceLen < 4)
    return -1;

  return *(int *)pSource;
}

//-------------------------------------------------------------------------------------------------

bool UnmangleFile(const uint8_t *pSource, int iSourceLen, uint8_t *pDestination, int iLength)
{
  // start positions
  int iInputPos = 4;
  int iOutputPos = 0;
  
  while ((iInputPos < iSourceLen) && (iOutputPos < iLength)) {
    int iValue = (int)pSource[iInputPos];
  
    if (iValue <= 0x3F) // 0x00 to 0x3F: read bytes from input
    {
      uint8_t* pTempArray = new uint8_t[iValue];
      memcpy(pTempArray, &pSource[iInputPos + 1], iValue);
      memcpy(&pDestination[iOutputPos], pTempArray, iValue);
      iInputPos += iValue + 1;
      iOutputPos += iValue;
    } else if (iValue <= 0x4F) // 0x40 to 0x4F: generate ascending bytes based on last 2 bytes
    {
      int iDelta = pDestination[iOutputPos - 1] - pDestination[iOutputPos - 2];
      for (int i = 0; i < ((iValue & 0x0F) + 3); i++) {
        pDestination[iOutputPos] = ((pDestination[iOutputPos - 1] + iDelta) & 0xFF);
        iOutputPos++;
      }
      iInputPos++;
    } else if (iValue <= 0x5F) // 0x50 to 0x5F: generate ascending words based on last 2 words
    {
      short sDelta = *(short*)(pDestination + iOutputPos - 2) - *(short*)(pDestination + iOutputPos - 4);
      for (int i = 0; i < ((iValue & 0x0F) + 2); i++) {
        short sNewShort = (*(short *)(pDestination + iOutputPos - 2)) + sDelta;
        pDestination[iOutputPos] = (uint8_t)(sNewShort & 0xFF);
        pDestination[iOutputPos + 1] = (uint8_t)((sNewShort >> 8) & 0xFF);
        iOutputPos += 2;
      }
      iInputPos++;
    } else if (iValue <= 0x6F) // 0x60 to 0x6F: clone last byte in output
    {
      for (int i = 0; i < ((iValue & 0x0F) + 3); i++) {
        pDestination[iOutputPos] = pDestination[iOutputPos - 1];
        iOutputPos++;
      }
      iInputPos++;
    } else if (iValue <= 0x7F) // 0x70 to 0x7F: clone last word in output
    {
      for (int i = 0; i < ((iValue & 0x0F) + 2); i++) {
        pDestination[iOutputPos] = pDestination[iOutputPos - 2];
        pDestination[iOutputPos + 1] = pDestination[iOutputPos - 1];
        iOutputPos += 2;
      }
      iInputPos++;
    } else if (iValue <= 0xBF) // 0x80 to 0xBF: clone 3 bytes using offset
    {
      int iOffset = (iValue & 0x3F);
      pDestination[iOutputPos] = pDestination[iOutputPos - iOffset - 3];
      pDestination[iOutputPos + 1] = pDestination[iOutputPos - iOffset - 2];
      pDestination[iOutputPos + 2] = pDestination[iOutputPos - iOffset - 1];
      iOutputPos += 3;
      iInputPos++;
    } else if (iValue <= 0xDF) // 0xC0 to 0xDF: clone using offset and length from next byte
    {
      int iOffset = ((iValue & 0x03) << 8) + (int)(pSource[iInputPos + 1]) + 3;
      int iLength = ((iValue >> 2) & 0x07) + 4;
      for (int i = 0; i < iLength; i++) {
        pDestination[iOutputPos] = pDestination[iOutputPos - iOffset];
        iOutputPos++;
      }
      iInputPos += 2;
    } else // 0xE0 to 0xFF: clone using offset and length from next 2 bytes
    {
      int iOffset = ((iValue & 0x1F) << 8) + (int)(pSource[iInputPos + 1]) + 3;
      int iLength = (int)(pSource[iInputPos + 2]) + 5;
      for (int i = 0; i < iLength; i++) {
        pDestination[iOutputPos] = pDestination[iOutputPos - iOffset];
        iOutputPos++;
      }
      iInputPos += 3;
    }
  }
  return true;
}

//-------------------------------------------------------------------------------------------------