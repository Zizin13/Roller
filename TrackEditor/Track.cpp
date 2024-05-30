#include "TrackEditor.h"
#include "Track.h"
#include "MainWindow.h"
#include "qfile.h"
#include "qtextstream.h"
#include "Unmangler.h"
#include "TrackPreview.h"
#include <assert.h>
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CTrack::CTrack()
{
  ClearData();
}

//-------------------------------------------------------------------------------------------------

CTrack::~CTrack()
{

}

//-------------------------------------------------------------------------------------------------

bool CTrack::SaveTrack(const QString &sFilename)
{
  if (sFilename.isEmpty())
    return false;

  std::vector<uint8> data;
  std::vector<uint8> mangledData;
  GetTrackData(data);

  std::vector<uint8> *pOutData;
  //if (bIsMangled) {
  //  MangleFile(data, mangledData);
  //  pOutData = &mangledData;
  //} else {
    pOutData = &data;
  //}

  QFile file(sFilename);
  file.resize(0);
  if (file.open(QIODevice::ReadWrite)) {
    QTextStream stream(&file);
    for (int i = 0; i < pOutData->size(); ++i) {
      stream << (char)((*pOutData)[i]);
    }
    file.close();
  }

  return true;
}

//-------------------------------------------------------------------------------------------------