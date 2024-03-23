#include "Palette.h"
#include "qfile.h"
#include "MainWindow.h"
#include "Unmangler.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
CPalette::CPalette()
{

}

//-------------------------------------------------------------------------------------------------

CPalette::~CPalette()
{

}

//-------------------------------------------------------------------------------------------------

void CPalette::ClearData()
{
  m_paletteAy.clear();
}

//-------------------------------------------------------------------------------------------------

bool CPalette::LoadPalette(const QString &sFilename)
{
  ClearData();

  if (sFilename.isEmpty()) {
    g_pMainWindow->LogMessage("Palette filename empty: " + sFilename);
    return false;
  }

  QFile file(sFilename);
  if (!file.open(QIODevice::ReadOnly)) {
    g_pMainWindow->LogMessage("Failed to open palette: " + sFilename);
    return false;
  }

  QByteArray baData = file.readAll();

  int iLength = baData.size() / 3;
  for (int i = 0; i < iLength; ++i) {
    int iR = baData[i * 3] << 2;
    int iG = baData[i * 3 + 1] << 2;
    int iB = baData[i * 3 + 2] << 2;
    m_paletteAy.push_back(QColor(iR, iG, iB));
  }

  file.close();

  g_pMainWindow->LogMessage("Loaded palette: " + sFilename);
  return true;
}

//-------------------------------------------------------------------------------------------------