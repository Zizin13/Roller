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
  , m_pPalette(NULL)
{
}

//-------------------------------------------------------------------------------------------------

CTexture::~CTexture()
{
  ClearData();
}

//-------------------------------------------------------------------------------------------------

void CTexture::ClearData()
{
  GLCALL(glDeleteTextures(1, &m_uiId));
  m_uiId = 0;
  m_iNumTiles = 0;
  if (m_pTileAy) {
    delete[] m_pTileAy;
    m_pTileAy = NULL;
  }
  m_pPalette = NULL;
}

//-------------------------------------------------------------------------------------------------

bool CTexture::LoadTexture(const std::string &sFilename, CPalette *pPalette)
{
  ClearData();
  m_pPalette = pPalette;
  if (m_uiId == 0) {
    GLCALL(glGenTextures(1, &m_uiId));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_uiId));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  }

  if (sFilename.empty()) {
    Logging::LogMessage("Texture filename is empty");
    return false;
  }

  //open file
  std::ifstream file(sFilename.c_str(), std::ios::binary);
  if (!file.is_open()) {
    Logging::LogMessage("Failed to open texture: %s", sFilename.c_str());
    return false;
  }

  file.seekg(0, file.end);
  size_t length = file.tellg();
  file.seekg(0, file.beg);
  if (length <= 0) {
    Logging::LogMessage("Texture file %s is empty", sFilename.c_str());
    return false;
  }

  //read file
  char *szBuf = new char[length];
  memset(szBuf, 0, length);
  file.read(szBuf, length);

  bool bSuccess = false;
  //unmangle
  int iUnmangledLength = GetUnmangledLength((uint8 *)szBuf, (int)length);
  if (iUnmangledLength > 0 && iUnmangledLength < 67108864) { // arbitrary 64 MB max, realistic maximum is much smaller
    Logging::LogMessage("Texture file %s is mangled", sFilename.c_str());
    uint8 *szUnmangledData = new uint8[iUnmangledLength];
    bSuccess = UnmangleFile((uint8 *)szBuf, (int)length, szUnmangledData, iUnmangledLength);
    Logging::LogMessage("%s texture file %s", bSuccess ? "Unmangled" : "Failed to unmangle", sFilename.c_str());

    if (bSuccess)
      bSuccess = ProcessTextureData(szUnmangledData, (size_t)iUnmangledLength);

    delete[] szUnmangledData;
  } else {
    bSuccess = ProcessTextureData((uint8 *)szBuf, length);
  }

  delete[] szBuf;
  file.close();

  Logging::LogMessage("%s texture: %s", bSuccess ? "Loaded" : "Failed to load", sFilename.c_str());

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
  if (!m_pPalette) {
    return;
  }

  bool bPair = uiSurfaceType & SURFACE_FLAG_TEXTURE_PAIR && uiSurfaceType & SURFACE_FLAG_PAIR_NEXT_TEX; //TODO: having pair but not pair next should double current texture
  bool bFlipVert = uiSurfaceType & SURFACE_FLAG_FLIP_VERT;
  bool bFlipHoriz = uiSurfaceType & SURFACE_FLAG_FLIP_HORIZ;
  bool bTransparent = uiSurfaceType & SURFACE_FLAG_TRANSPARENT;
  bool bPartialTrans = uiSurfaceType & SURFACE_FLAG_PARTIAL_TRANS;
  bool bApplyTexture = uiSurfaceType & SURFACE_FLAG_APPLY_TEXTURE;
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

  if (!bApplyTexture) {
    //use color
    topLeft.flags.x = 1.0f;
    topRight.flags.x = 1.0f;
    bottomLeft.flags.x = 1.0f;
    bottomRight.flags.x = 1.0f;

    //color
    if (uiTexIndex < m_pPalette->m_paletteAy.size()) {
      topLeft.color = ColorBytesToFloat(m_pPalette->m_paletteAy[uiTexIndex]);
      topRight.color = ColorBytesToFloat(m_pPalette->m_paletteAy[uiTexIndex]);
      bottomLeft.color = ColorBytesToFloat(m_pPalette->m_paletteAy[uiTexIndex]);
      bottomRight.color = ColorBytesToFloat(m_pPalette->m_paletteAy[uiTexIndex]);
    }
  }
}

//-------------------------------------------------------------------------------------------------

glm::vec3 CTexture::RandomColor()
{
  glm::vec3 ret;
  ret.x = rand() / (float)RAND_MAX;
  ret.y = rand() / (float)RAND_MAX;
  ret.z = rand() / (float)RAND_MAX;
  return ret;
}

//-------------------------------------------------------------------------------------------------

glm::vec3 CTexture::ColorBytesToFloat(const glm::vec3 &color)
{
  glm::vec3 ret;
  ret.r = (float)color.r / 255.0f;
  ret.g = (float)color.g / 255.0f;
  ret.b = (float)color.b / 255.0f;
  return ret;
}

//-------------------------------------------------------------------------------------------------

bool CTexture::ProcessTextureData(const uint8 *pData, size_t length)
{
  if (!m_pPalette) {
    assert(0);
    return false;
  }

  int iPixelsPerTile = TILE_WIDTH * TILE_HEIGHT;
  m_iNumTiles = (int)length / iPixelsPerTile;
  m_pTileAy = new tTile[m_iNumTiles];
  for (int i = 0; i < m_iNumTiles; ++i) {
    tTile *pTile = &m_pTileAy[i];
    for (int j = 0; j < iPixelsPerTile; ++j) {
      unsigned char byPaletteIndex = pData[i * iPixelsPerTile + j];
      if (m_pPalette->m_paletteAy.size() > byPaletteIndex) {
        pTile->data[j % TILE_WIDTH][j / TILE_WIDTH] = glm::vec<4, uint8>(m_pPalette->m_paletteAy[byPaletteIndex].r,
                                                                         m_pPalette->m_paletteAy[byPaletteIndex].g,
                                                                         m_pPalette->m_paletteAy[byPaletteIndex].b,
                                                                         byPaletteIndex ? 255 : 0);
      } else {
        assert(0);
        Logging::LogMessage("Error loading texture: palette index out of bounds");
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