#ifndef _TRACKEDITOR_PALETTE_H
#define _TRACKEDITOR_PALETTE_H
//-------------------------------------------------------------------------------------------------
#include <vector>
#include <string>
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------

class CPalette
{
public:
  CPalette();
  ~CPalette();

  void ClearData();
  bool LoadPalette(const std::string &sFilename);

  std::vector<glm::vec3> m_paletteAy;
};

//-------------------------------------------------------------------------------------------------
#endif