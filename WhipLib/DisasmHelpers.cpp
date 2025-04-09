#include "DisasmHelpers.h"
#include <string.h>
#include <algorithm>
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

std::vector<uint8> DisasmHelpers::HexToBytes(const std::string &sBytes)
{
  std::vector<uint8> byteAy;

  for (uint32 i = 0; i < sBytes.length(); i += 2) {
    std::string sByte = sBytes.substr(i, 2);
    char byte = (char)strtol(sByte.c_str(), NULL, 16);
    byteAy.push_back(byte);
  }

  return byteAy;
}

//-------------------------------------------------------------------------------------------------

std::vector<float> DisasmHelpers::BytesToFloats(const std::vector<uint8> bytes)
{
  std::vector<float> floatAy;

  int j = 0;
  uint32 uiData = 0;
  for (uint32 i = 0; i < bytes.size(); ++i) {
    uint32 uiTemp = 0;
    uint8 c = (uint8)bytes[i];
    uiTemp |= c;
    uiTemp = uiTemp << 8 * j;
    uiData |= uiTemp;
    ++j;

    if (j == 4) {
      float *pfData = reinterpret_cast<float *>(&uiData);
      floatAy.push_back(*pfData);
      uiData = 0;
      j = 0;
    }
  }

  return floatAy;
}

//-------------------------------------------------------------------------------------------------

std::vector<uint32> DisasmHelpers::BytesToUInts(const std::vector<uint8> bytes)
{
  std::vector<uint32> intAy;

  int j = 0;
  uint32 uiData = 0;
  for (uint32 i = 0; i < bytes.size(); ++i) {
    uint32 uiTemp = 0;
    uint8 c = (uint8)bytes[i];
    uiTemp |= c;
    uiTemp = uiTemp << 8 * j;
    uiData |= uiTemp;
    ++j;

    if (j == 4) {
      intAy.push_back(uiData);
      uiData = 0;
      j = 0;
    }
  }

  return intAy;
}

//-------------------------------------------------------------------------------------------------

std::vector<tPolygon> DisasmHelpers::BytesToPols(const std::vector<uint8> bytes)
{
  std::vector<tPolygon> polAy;

  int j = 0;
  tPolygon data;
  memset(&data, 0, sizeof(data));
  for (uint32 i = 0; i < bytes.size(); ++i) {
    uint8 c = bytes[i];
    uint32 uiTemp = 0;

    switch (j) {
      case 0:
        data.byVert1 = c;
        break;
      case 1:
        data.byVert2 = c;
        break;
      case 2:
        data.byVert3 = c;
        break;
      case 3:
        data.byVert4 = c;
        break;
      case 4:
      case 5:
      case 6:
      case 7:
        uiTemp |= c;
        uiTemp = uiTemp << 8 * (j - 4);
        data.uiTex |= uiTemp;
        break;
      case 8:
        data.byUnknown1 = c;
        break;
      case 9:
        data.byUnknown2 = c;
        break;
      case 10:
        data.byUnknown3 = c;
        break;
      case 11:
        data.byUnknown4 = c;
        break;
    }

    ++j;

    if (j == 12) {
      polAy.push_back(data);
      memset(&data, 0, sizeof(data));
      j = 0;
    }
  }

  return polAy;
}

//-------------------------------------------------------------------------------------------------

std::vector<tAnimation> DisasmHelpers::UIntsToAnms(const std::vector<uint32> uints)
{
  std::vector<tAnimation> anmAy;

  int j = 0;
  tAnimation data;
  memset(&data, 0, sizeof(data));
  for (uint32 i = 0; i < uints.size(); ++i) {
    if (j == 0)
      data.uiCount = uints[i];
    else
      data.framesAy[j - 1] = uints[i];

    ++j;

    if (j == 17) {
      anmAy.push_back(data);
      memset(&data, 0, sizeof(data));
      j = 0;
    }
  }

  return anmAy;
}

//-------------------------------------------------------------------------------------------------

std::string DisasmHelpers::GetStringBytes(const std::string &sLine)
{
  std::string sRet;
  size_t colPos = sLine.find(":\t");
  size_t hexPos = sLine.find("0x");

  if (colPos != std::string::npos && hexPos != std::string::npos) {
    sRet = sLine.substr(colPos + 2, hexPos - colPos);
    size_t dbPos = sRet.find("\tdb");

    sRet = sRet.substr(0, dbPos - 1);
  }

  sRet.erase(remove_if(sRet.begin(), sRet.end(), isspace), sRet.end());

  return sRet;
}

//-------------------------------------------------------------------------------------------------
