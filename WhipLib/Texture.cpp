#include "Texture.h"
#include "Palette.h"
#include <fstream>
#include <sstream>
#include "Unmangler.h"
#include "glew.h"
#include "OpenGLDebug.h"
#include "Vertex.h"
#include "ShapeFactory.h"
#include <Windows.h>
#include <fstream>
#include "Logging.h"
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
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
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
  int iUnmangledLength = Unmangler::GetUnmangledLength((uint8 *)szBuf, (int)length);
  if (iUnmangledLength > 0 && iUnmangledLength < MAX_MANGLED_LENGTH) {
    Logging::LogMessage("Texture file %s is mangled", sFilename.c_str());
    uint8 *szUnmangledData = new uint8[iUnmangledLength];
    bSuccess = Unmangler::UnmangleFile((uint8 *)szBuf, (int)length, szUnmangledData, iUnmangledLength);
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
                                     bool bBack)
{
  if (!m_pPalette) {
    return;
  }

  bool bPair = uiSurfaceType & SURFACE_FLAG_TEXTURE_PAIR;// &&uiSurfaceType &SURFACE_FLAG_PAIR_NEXT_TEX;
  bool bFlipVert = uiSurfaceType & SURFACE_FLAG_FLIP_VERT;
  bool bFlipHoriz = uiSurfaceType & SURFACE_FLAG_FLIP_HORIZ;
  bool bTransparent = uiSurfaceType & SURFACE_FLAG_TRANSPARENT;
  bool bPartialTrans = uiSurfaceType & SURFACE_FLAG_PARTIAL_TRANS;
  bool bApplyTexture = uiSurfaceType & SURFACE_FLAG_APPLY_TEXTURE;
  uint32 uiTexIndex = uiSurfaceType & SURFACE_MASK_TEXTURE_INDEX;
  uint32 uiTexIncVal = bPair ? 2 : 1;

  if (bApplyTexture) {
    ApplyTexCoords(bBack ? topLeft.backTexCoords : topLeft.texCoords,
                   bBack ? topRight.backTexCoords : topRight.texCoords,
                   bBack ? bottomLeft.backTexCoords : bottomLeft.texCoords,
                   bBack ? bottomRight.backTexCoords : bottomRight.texCoords,
                   uiTexIndex, uiTexIncVal, bFlipHoriz, bFlipVert);
  } else if (bTransparent) {
    ApplyTransparency(topLeft, uiTexIndex, bBack);
    ApplyTransparency(topRight, uiTexIndex, bBack);
    ApplyTransparency(bottomLeft, uiTexIndex, bBack);
    ApplyTransparency(bottomRight, uiTexIndex, bBack);
  } else {
    ApplyColor(topLeft, uiTexIndex, bBack);
    ApplyColor(topRight, uiTexIndex, bBack);
    ApplyColor(bottomLeft, uiTexIndex, bBack);
    ApplyColor(bottomRight, uiTexIndex, bBack);
  }
}

//-------------------------------------------------------------------------------------------------

uint8 *CTexture::GenerateBitmapData(int &iSize)
{
  iSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (4 * TILE_WIDTH * TILE_HEIGHT * m_iNumTiles);

  BITMAPFILEHEADER fileHeader = { 0 };
  fileHeader.bfType = 0x4D42; //BM
  fileHeader.bfSize = iSize;
  fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

  BITMAPINFOHEADER infoHeader = { 0 };
  infoHeader.biSize = sizeof(BITMAPINFOHEADER);
  infoHeader.biWidth = TILE_WIDTH;
  infoHeader.biHeight = m_iNumTiles * TILE_HEIGHT * -1;
  infoHeader.biPlanes = 1;
  infoHeader.biBitCount = 32;
  infoHeader.biCompression = 0;
  infoHeader.biSizeImage = 0;
  infoHeader.biXPelsPerMeter = 3780;
  infoHeader.biYPelsPerMeter = 3780;
  infoHeader.biClrUsed = 0;
  infoHeader.biClrImportant = 0;

  uint8 *pData = new uint8[fileHeader.bfSize];
  memcpy(pData, &fileHeader, sizeof(fileHeader));
  memcpy(pData + sizeof(fileHeader), &infoHeader, sizeof(infoHeader));


  tTile *pTilesFlipped = new tTile[m_iNumTiles];
  FlipTileLines(m_pTileAy, pTilesFlipped, m_iNumTiles);

  int iOffset = fileHeader.bfOffBits;
  for (int i = 0; i < m_iNumTiles; ++i) {
    for (int x = 0; x < TILE_WIDTH; ++x) {
      for (int y = 0; y < TILE_HEIGHT; ++y) {
        pData[iOffset++] = pTilesFlipped[i].data[x][y].b;
        pData[iOffset++] = pTilesFlipped[i].data[x][y].g;
        pData[iOffset++] = pTilesFlipped[i].data[x][y].r;
        pData[iOffset++] = pTilesFlipped[i].data[x][y].a;
      }
    }
  }
  delete[] pTilesFlipped;

  return pData;
}

//-------------------------------------------------------------------------------------------------

bool CTexture::ExportToBitmapFile(const std::string &sFilename)
{
  int iBmpSize;
  uint8 *pBmpData = GenerateBitmapData(iBmpSize);
  std::ofstream out(sFilename.c_str(), std::ios_base::binary);
  if (!out.is_open()) {
    Logging::LogMessage("Failed to open bmp output file %s\n", sFilename.c_str());
    return false;
  }
  for (int i = 0; i < iBmpSize; ++i) {
    out << pBmpData[i];
  }
  out.close();
  delete[] pBmpData;
  return true;
}

//-------------------------------------------------------------------------------------------------

glm::vec4 CTexture::RandomColor()
{
  glm::vec4 ret;
  ret.r = rand() / (float)RAND_MAX;
  ret.b = rand() / (float)RAND_MAX;
  ret.g = rand() / (float)RAND_MAX;
  ret.a = 1.0f;
  return ret;
}

//-------------------------------------------------------------------------------------------------

glm::vec4 CTexture::ColorBytesToFloat(const glm::vec3 &color)
{
  glm::vec4 ret;
  ret.r = (float)color.r / 255.0f;
  ret.g = (float)color.g / 255.0f;
  ret.b = (float)color.b / 255.0f;
  ret.a = 1.0f;
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
      uint8 byPaletteIndex = pData[i * iPixelsPerTile + j];
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

void CTexture::ApplyTransparency(tVertex &vertex, uint32 uiTexIndex, bool bBack)
{
  glm::vec4 color = glm::vec4(0, 0, 0, 1);
  switch (uiTexIndex) {
    case 0:
      break;
    case 1:
      color.a = 0.25f;
      break;
    case 2:
      color.a = 0.5f;
      break;
    case 3:
      color.a = 0.75f;
      break;
    case 4:
      color = glm::vec4(0, 0, 1, 0.25f);
      break;
  }
  if (!bBack) {
    //use color
    vertex.byUseColor = 1;
    //color
    vertex.color = color;
  } else {
    //use color
    vertex.byBackUseColor = 1;
    //color
    vertex.backColor = color;
  }

}

//-------------------------------------------------------------------------------------------------

void CTexture::ApplyColor(tVertex &vertex, uint32 uiTexIndex, bool bBack)
{
  if (!bBack) {
    //use color
    vertex.byUseColor = 1;
    //color
    if (uiTexIndex < m_pPalette->m_paletteAy.size()) {
      vertex.color = ColorBytesToFloat(m_pPalette->m_paletteAy[uiTexIndex]);
    }
  } else {
    //use color
    vertex.byBackUseColor = 1;
    //color
    if (uiTexIndex < m_pPalette->m_paletteAy.size()) {
      vertex.backColor = ColorBytesToFloat(m_pPalette->m_paletteAy[uiTexIndex]);
    }
  }
}

//-------------------------------------------------------------------------------------------------

void CTexture::ApplyTexCoords(glm::vec2 &topLeft,
                              glm::vec2 &topRight,
                              glm::vec2 &bottomLeft,
                              glm::vec2 &bottomRight,
                              uint32 uiTexIndex, uint32 uiTexIncVal,
                              bool bFlipHoriz, bool bFlipVert)
{
  if (!bFlipHoriz && !bFlipVert) {
    topLeft = glm::vec2(1.0f, (float)uiTexIndex / (float)m_iNumTiles);
    topRight = glm::vec2(1.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);
    bottomLeft = glm::vec2(0.0f, (float)uiTexIndex / (float)m_iNumTiles);
    bottomRight = glm::vec2(0.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);
  } else if (bFlipHoriz && !bFlipVert) {
    topLeft = glm::vec2(1.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);
    topRight = glm::vec2(1.0f, (float)uiTexIndex / (float)m_iNumTiles);
    bottomLeft = glm::vec2(0.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);
    bottomRight = glm::vec2(0.0f, (float)uiTexIndex / (float)m_iNumTiles);
  } else if (!bFlipHoriz && bFlipVert) {
    topLeft = glm::vec2(0.0f, (float)uiTexIndex / (float)m_iNumTiles);
    topRight = glm::vec2(0.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);
    bottomLeft = glm::vec2(1.0f, (float)uiTexIndex / (float)m_iNumTiles);
    bottomRight = glm::vec2(1.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);
  } else if (bFlipHoriz && bFlipVert) {
    topLeft = glm::vec2(0.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);
    topRight = glm::vec2(0.0f, (float)uiTexIndex / (float)m_iNumTiles);
    bottomLeft = glm::vec2(1.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_iNumTiles);
    bottomRight = glm::vec2(1.0f, (float)uiTexIndex / (float)m_iNumTiles);
  }
}

//-------------------------------------------------------------------------------------------------