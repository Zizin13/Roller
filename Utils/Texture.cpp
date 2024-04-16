#include "Texture.h"
#include "Palette.h"
#include <fstream>
#include <sstream>
#include "Unmangler.h"
#include "glew.h"
#include "OpenGLDebug.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CTexture::CTexture()
  : m_uiId(0)
  , m_iNumTiles(0)
  , m_pTileAy(NULL)
{
}

//-------------------------------------------------------------------------------------------------

CTexture::~CTexture()
{
  ClearData();
  GLCALL(glDeleteTextures(1, &m_uiId));
}

//-------------------------------------------------------------------------------------------------

void CTexture::ClearData()
{
  m_iNumTiles = 0;
  if (m_pTileAy) {
    delete[] m_pTileAy;
    m_pTileAy = NULL;
  }
}

//-------------------------------------------------------------------------------------------------

bool CTexture::LoadTexture(const std::string &sFilename, const CPalette &palette, bool bMangled)
{
  if (m_uiId == 0) {
    GLCALL(glGenTextures(1, &m_uiId));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_uiId));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
  }
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

void CTexture::Bind(uint32 uiSlot) const
{
  GLCALL(glActiveTexture(GL_TEXTURE0 + uiSlot));
  GLCALL(glBindTexture(GL_TEXTURE_2D, m_uiId));
}

//-------------------------------------------------------------------------------------------------

void CTexture::Unbind() const
{
  GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

//-------------------------------------------------------------------------------------------------

bool CTexture::ProcessTextureData(const uint8 *pData, size_t length, const CPalette &palette)
{
  int iPixelsPerTile = TILE_WIDTH * TILE_HEIGHT;
  m_iNumTiles = (int)length / iPixelsPerTile;
  m_pTileAy = new tTile[m_iNumTiles];
  for (int i = 0; i < m_iNumTiles; ++i) {
    tTile *pTile = &m_pTileAy[i];
    for (int j = 0; j < iPixelsPerTile; ++j) {
      unsigned char byPaletteIndex = pData[i * iPixelsPerTile + j];
      if (palette.m_paletteAy.size() > byPaletteIndex) {
        pTile->data[j % TILE_WIDTH][j / TILE_WIDTH] = glm::vec<3, uint8>(palette.m_paletteAy[byPaletteIndex].r,
                                                                         palette.m_paletteAy[byPaletteIndex].g,
                                                                         palette.m_paletteAy[byPaletteIndex].b);
      } else {
        assert(0);
        //TODO g_pMainWindow->LogMessage("Error loading texture " + sFilename + ": palette index out of bounds");
        return false;
      }
    }
  }

  int iLength = TILE_WIDTH * 3;
  int iHeight = (int)length / (TILE_WIDTH * 3);
  GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 
                      iLength, iHeight, 0,
                      GL_RGB, GL_UNSIGNED_BYTE, m_pTileAy));
  GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
  return true;
}

//-------------------------------------------------------------------------------------------------