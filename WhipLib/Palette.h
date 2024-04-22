#ifndef _WHIPLIB_PALETTE_H
#define _WHIPLIB_PALETTE_H
//-------------------------------------------------------------------------------------------------
#include <vector>
#include <string>
#include "glm.hpp"
#include "Types.h"
//-------------------------------------------------------------------------------------------------

class CPalette
{
public:
  CPalette();
  ~CPalette();

  void ClearData();
  bool LoadPalette(const std::string &sFilename);

  std::vector<glm::vec<3, uint8>> m_paletteAy;
};

//-------------------------------------------------------------------------------------------------
#endif