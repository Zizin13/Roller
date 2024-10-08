#include "Palette.h"
#include <fstream>
#include <sstream>
#include "Unmangler.h"
#include "Logging.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
CPalette::CPalette()
{

}

//-------------------------------------------------------------------------------------------------

CPalette::~CPalette()
{

}

//-------------------------------------------------------------------------------------------------

void CPalette::ClearData()
{
  m_paletteAy.clear();
}

//-------------------------------------------------------------------------------------------------

bool CPalette::LoadPalette(const std::string &sFilename)
{
  ClearData();

  if (sFilename.empty()) {
    Logging::LogMessage("Palette filename empty");
    return false;
  }

  //open file
  std::ifstream file(sFilename.c_str(), std::ios::binary);
  if (!file.is_open()) {
    Logging::LogMessage("Failed to open palette: %s", sFilename.c_str());
    return false;
  }

  file.seekg(0, file.end);
  size_t length = file.tellg();
  file.seekg(0, file.beg);
  if (length <= 0) {
    Logging::LogMessage("Palette file %s is empty", sFilename.c_str());
    return false;
  }

  //read file
  char *szBuf = new char[length];
  memset(szBuf, 0, length);
  file.read(szBuf, length);

  int iLength = (int)length / 3;
  for (int i = 0; i < iLength; ++i) {
    uint8 byR = szBuf[i * 3] << 2;
    uint8 byG = szBuf[i * 3 + 1] << 2;
    uint8 byB = szBuf[i * 3 + 2] << 2;
    m_paletteAy.push_back(glm::vec<3, uint8>(byR, byG, byB));
  }

  delete[] szBuf;
  file.close();

  Logging::LogMessage("Loaded palette: %s", sFilename.c_str());
  return true;
}

//-------------------------------------------------------------------------------------------------