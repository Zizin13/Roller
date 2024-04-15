#ifndef _UTILS_TEXTURE_H
#define _UTILS_TEXTURE_H
//-------------------------------------------------------------------------------------------------
#include <vector>
#include <string>
#include "glm.hpp"
#include "Types.h"
//-------------------------------------------------------------------------------------------------
class CPalette;
//-------------------------------------------------------------------------------------------------
#define TILE_WIDTH 64
#define TILE_HEIGHT TILE_WIDTH
//-------------------------------------------------------------------------------------------------
struct tTile
{
  glm::vec<3, uint8> data[TILE_WIDTH][TILE_HEIGHT];
};
//-------------------------------------------------------------------------------------------------

class CTexture
{
public:
  CTexture();
  ~CTexture();

  void ClearData();
  bool LoadTexture(const std::string &sFilename, const CPalette &palette, bool bMangled);

  std::vector<tTile> m_tileAy;

private:
  bool ProcessTextureData(const uint8 *pData, size_t length, const CPalette &palette);
};

//-------------------------------------------------------------------------------------------------
#endif