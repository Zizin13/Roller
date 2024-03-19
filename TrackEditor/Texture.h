#ifndef _TRACKEDITOR_TEXTURE_H
#define _TRACKEDITOR_TEXTURE_H
//-------------------------------------------------------------------------------------------------
#include <vector>
#include "qimage.h"
//-------------------------------------------------------------------------------------------------
class CPalette;
//-------------------------------------------------------------------------------------------------
#define TILE_WIDTH 64
#define TILE_HEIGHT TILE_WIDTH
//-------------------------------------------------------------------------------------------------

class CTexture
{
public:
  CTexture();
  ~CTexture();

  void ClearData();
  bool LoadTexture(const QString &sFilename, const CPalette &palette);

  std::vector<QImage> m_tileAy;
};

//-------------------------------------------------------------------------------------------------
#endif