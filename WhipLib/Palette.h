#ifndef _WHIPLIB_PALETTE_H
#define _WHIPLIB_PALETTE_H
//-------------------------------------------------------------------------------------------------
#include <vector>
#include <string>
#include "glm.hpp"
#include "Types.h"
//-------------------------------------------------------------------------------------------------
#define PALETTE_SIZE 256
//-------------------------------------------------------------------------------------------------

class CPalette
{
public:
  CPalette();
  ~CPalette();

  void ClearData();
  bool LoadPalette(const std::string &sFilename);
  bool IsLoaded() { return m_bLoaded; };

  glm::vec<3, uint8> m_paletteAy[PALETTE_SIZE];

private:
  bool m_bLoaded;
};

//-------------------------------------------------------------------------------------------------
#endif