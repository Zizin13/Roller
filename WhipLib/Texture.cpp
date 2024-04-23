#include "Texture.h"
#include "Palette.h"
#include <fstream>
#include <sstream>
#include "Unmangler.h"
#include "glew.h"
#include "OpenGLDebug.h"
#include "Vertex.h"
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
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
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
  if (!file.is_open()) {
    //todo logging
    return false;
  }

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

void CTexture::GetTextureCoordinates(uint32 uiSurfaceType,
                                     tVertex &topLeft, tVertex &topRight, tVertex &bottomLeft, tVertex &bottomRight,
                                     bool bLeftLane, bool bRightLane)
{
  bool bPair = uiSurfaceType & SURFACE_FLAG_TEXTURE_PAIR && uiSurfaceType & SURFACE_FLAG_PAIR_NEXT_TEX; //TODO: having pair but not pair next should double current texture
  bool bFlipVert = uiSurfaceType & SURFACE_FLAG_FLIP_VERT;
  bool bFlipHoriz = uiSurfaceType & SURFACE_FLAG_FLIP_HORIZ;
  bool bTransparent = uiSurfaceType & SURFACE_FLAG_TRANSPARENT;
  bool bPartialTrans = uiSurfaceType & SURFACE_FLAG_PARTIAL_TRANS;
  uint32 uiTexIndex = uiSurfaceType & SURFACE_TEXTURE_INDEX;

  //right lane takes the second texture on center surface
  //both center lanes only draw one texture each when paired
  uint32 uiTexIncVal = (bPair && !(bLeftLane || bRightLane)) ? 2 : 1;
  if (bRightLane && uiSurfaceType & SURFACE_FLAG_TEXTURE_PAIR)
    uiTexIndex++;

  if (!bFlipHoriz && !bFlipVert)
    topLeft.texCoords = glm::vec2(1.0f, (float)uiTexIndex / (float)m_iNumTiles);
  else if (bFlipHoriz && !bFlipVert)
    topLeft.texCoords = glm::vec2(1.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);
  else if (!bFlipHoriz && bFlipVert)
    topLeft.texCoords = glm::vec2(0.0f, (float)uiTexIndex / (float)m_iNumTiles);
  else if (bFlipHoriz && bFlipVert)
    topLeft.texCoords = glm::vec2(0.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);

  if (!bFlipHoriz && !bFlipVert)
    topRight.texCoords = glm::vec2(1.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);
  else if (bFlipHoriz && !bFlipVert)
    topRight.texCoords = glm::vec2(1.0f, (float)uiTexIndex / (float)m_iNumTiles);
  else if (!bFlipHoriz && bFlipVert)
    topRight.texCoords = glm::vec2(0.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);
  else if (bFlipHoriz && bFlipVert)
    topRight.texCoords = glm::vec2(0.0f, (float)uiTexIndex / (float)m_iNumTiles);

  if (!bFlipHoriz && !bFlipVert)
    bottomLeft.texCoords = glm::vec2(0.0f, (float)uiTexIndex / (float)m_iNumTiles);
  else if (bFlipHoriz && !bFlipVert)
    bottomLeft.texCoords = glm::vec2(0.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);
  else if (!bFlipHoriz && bFlipVert)
    bottomLeft.texCoords = glm::vec2(1.0f, (float)uiTexIndex / (float)m_iNumTiles);
  else if (bFlipHoriz && bFlipVert)
    bottomLeft.texCoords = glm::vec2(1.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);

  if (!bFlipHoriz && !bFlipVert)
    bottomRight.texCoords = glm::vec2(0.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);
  else if (bFlipHoriz && !bFlipVert)
    bottomRight.texCoords = glm::vec2(0.0f, (float)uiTexIndex / (float)m_iNumTiles);
  else if (!bFlipHoriz && bFlipVert)
    bottomRight.texCoords = glm::vec2(1.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);
  else if (bFlipHoriz && bFlipVert)
    bottomRight.texCoords = glm::vec2(1.0f, (float)uiTexIndex / (float)m_iNumTiles);

  if (bTransparent) {
    //use color
    topLeft.flags.x = 1.0f;
    topRight.flags.x = 1.0f;
    bottomLeft.flags.x = 1.0f;
    bottomRight.flags.x = 1.0f;
    //alpha
    float fAlphaVal = 0.8f;
    topLeft.flags.y = fAlphaVal;
    topRight.flags.y = fAlphaVal;
    bottomLeft.flags.y = fAlphaVal;
    bottomRight.flags.y = fAlphaVal;
    //color
    topLeft.color = glm::vec3(0);
    topRight.color = glm::vec3(0);
    bottomLeft.color = glm::vec3(0);
    bottomRight.color = glm::vec3(0);
  }
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
        pTile->data[j % TILE_WIDTH][j / TILE_WIDTH] = glm::vec<4, uint8>(palette.m_paletteAy[byPaletteIndex].r,
                                                                         palette.m_paletteAy[byPaletteIndex].g,
                                                                         palette.m_paletteAy[byPaletteIndex].b,
                                                                         byPaletteIndex ? 255 : 0);
      } else {
        assert(0);
        //TODO g_pMainWindow->LogMessage("Error loading texture " + sFilename + ": palette index out of bounds");
        return false;
      }
    }
  }

  tTile *pTilesFlipped = new tTile[m_iNumTiles];
  FlipTileLines(m_pTileAy, pTilesFlipped, m_iNumTiles);
  int iLength = TILE_WIDTH;
  int iHeight = TILE_HEIGHT * m_iNumTiles;
  GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 
                      iLength, iHeight, 0,
                      GL_RGBA, GL_UNSIGNED_BYTE, pTilesFlipped));
  GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
  delete[] pTilesFlipped;
  return true;
}

//-------------------------------------------------------------------------------------------------

void CTexture::FlipTileLines(tTile *pSource, tTile *pDest, int iNumTiles)
{
  for (int i = 0; i < iNumTiles; ++i) {
    for (int x = 0; x < TILE_WIDTH; ++x) {
      for (int y = 0; y < TILE_HEIGHT; ++y) {
        pDest[i].data[x][y] = pSource[i].data[x][TILE_HEIGHT - (y + 1)];
      }
    }
  }
}

//-------------------------------------------------------------------------------------------------