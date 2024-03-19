#ifndef _TRACKEDITOR_PALETTE_H
#define _TRACKEDITOR_PALETTE_H
//-------------------------------------------------------------------------------------------------
#include <vector>
#include "qcolor.h"
//-------------------------------------------------------------------------------------------------

class CPalette
{
public:
  CPalette();
  ~CPalette();

  void ClearData();
  bool LoadPalette(const QString &sFilename);

  std::vector<QColor> m_paletteAy;
};

//-------------------------------------------------------------------------------------------------
#endif