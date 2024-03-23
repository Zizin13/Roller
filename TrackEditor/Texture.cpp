#include "Texture.h"
#include "qfile.h"
#include "MainWindow.h"
#include "Palette.h"
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

bool CTexture::LoadTexture(const QString &sFilename, const CPalette &palette, bool bMangled)
{
  ClearData();

  if (sFilename.isEmpty()) {
    g_pMainWindow->LogMessage("Texture filename empty: " + sFilename);
    return false;
  }

  QFile file(sFilename);
  if (!file.open(QIODevice::ReadOnly)) {
    g_pMainWindow->LogMessage("Failed to open texture: " + sFilename);
    return false;
  }

  QByteArray baData = file.readAll();

  if (bMangled) {
    int iLength = GetUnmangledLength((uint8_t *)baData.constData(), baData.size());
    if (iLength > 0) {
      uint8_t *szData = new uint8_t[iLength];
      UnmangleFile((uint8_t *)baData.constData(), baData.size(), szData, iLength);
      baData = QByteArray((char *)szData, iLength);
      delete [] szData;
    } else {
      g_pMainWindow->LogMessage("Error loading texture " + sFilename + " unable to unmangle");
      return false;
    }
  }

  int iPixelsPerTile = TILE_WIDTH * TILE_HEIGHT;
  int iNumTiles = baData.size() / iPixelsPerTile;
  for (int i = 0; i < iNumTiles; ++i) {
    QImage tile(TILE_WIDTH, TILE_HEIGHT, QImage::Format_RGB32);
    for (int j = 0; j < iPixelsPerTile; ++j) {
      unsigned char byPaletteIndex = baData.constData()[i * iPixelsPerTile + j];
      if (palette.m_paletteAy.size() > byPaletteIndex) {
        tile.setPixelColor(j % TILE_WIDTH, j / TILE_WIDTH, QColor(palette.m_paletteAy[byPaletteIndex].red(),
                                                                  palette.m_paletteAy[byPaletteIndex].green(),
                                                                  palette.m_paletteAy[byPaletteIndex].blue()));
      } else {
        assert(0);
        g_pMainWindow->LogMessage("Error loading texture " + sFilename + ": palette index out of bounds");
        return false;
      }
    }
    m_tileAy.push_back(tile);
  }

  file.close();

  g_pMainWindow->LogMessage("Loaded texture: " + sFilename);

  return true;
}

//-------------------------------------------------------------------------------------------------