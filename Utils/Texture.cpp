#include "Texture.h"
#include "Palette.h"
#include <fstream>
#include <sstream>
#include "Unmangler.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CTexture::CTexture()
{

}

//-------------------------------------------------------------------------------------------------

CTexture::~CTexture()
{

}

//-------------------------------------------------------------------------------------------------

void CTexture::ClearData()
{
  m_tileAy.clear();
}

//-------------------------------------------------------------------------------------------------

bool CTexture::LoadTexture(const std::string &sFilename, const CPalette &palette, bool bMangled)
{
  ClearData();

  //TODO LOGGING
  //if (sFilename.isEmpty()) {
  //  g_pMainWindow->LogMessage("Texture filename empty: " + sFilename);
  //  return false;
  //}
  //
  //QFile file(sFilename);
  //if (!file.open(QIODevice::ReadOnly)) {
  //  g_pMainWindow->LogMessage("Failed to open texture: " + sFilename);
  //  return false;
  //}

  //open file
  std::ifstream file(sFilename.c_str(), std::ios::binary);
  file.seekg(0, file.end);
  size_t length = file.tellg();
  file.seekg(0, file.beg);
  if (length <= 0) {
    //todo logging
    return false;
  }

  //read file
  char *szBuf = new char[length];
  memset(szBuf, 0, length);
  file.read(szBuf, length);

  bool bSuccess = false;
  //unmangle
  if (bMangled) {
    int iUnmangledLength = GetUnmangledLength((uint8 *)szBuf, (int)length);
    uint8 *szUnmangledData = new uint8[iUnmangledLength];
    UnmangleFile((uint8 *)szBuf, (int)length, szUnmangledData, iUnmangledLength);
    bSuccess = ProcessTextureData(szUnmangledData, (size_t)iUnmangledLength, palette);
    delete[] szUnmangledData;
  } else {
    bSuccess = ProcessTextureData((uint8 *)szBuf, length, palette);
  }

  delete[] szBuf;
  file.close();

  //TODO g_pMainWindow->LogMessage("Loaded texture: " + sFilename);

  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

bool CTexture::ProcessTextureData(const uint8 *pData, size_t length, const CPalette &palette)
{
  int iPixelsPerTile = TILE_WIDTH * TILE_HEIGHT;
  int iNumTiles = (int)length / iPixelsPerTile;
  for (int i = 0; i < iNumTiles; ++i) {
    tTile tile;
    for (int j = 0; j < iPixelsPerTile; ++j) {
      unsigned char byPaletteIndex = pData[i * iPixelsPerTile + j];
      if (palette.m_paletteAy.size() > byPaletteIndex) {
        tile.data[j % TILE_WIDTH][j / TILE_WIDTH] = glm::vec<3, uint8>(palette.m_paletteAy[byPaletteIndex].r,
                                                                       palette.m_paletteAy[byPaletteIndex].g,
                                                                       palette.m_paletteAy[byPaletteIndex].b);
      } else {
        assert(0);
        //TODO g_pMainWindow->LogMessage("Error loading texture " + sFilename + ": palette index out of bounds");
        return false;
      }
    }
    m_tileAy.push_back(tile);
  }
}

//-------------------------------------------------------------------------------------------------