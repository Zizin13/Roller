#ifndef _TRACKEDITOR_TRACK_H
#define _TRACKEDITOR_TRACK_H
//-------------------------------------------------------------------------------------------------
#include <vector>
#include <map>
#include "qstring.h"
#include "TrackData.h"
//-------------------------------------------------------------------------------------------------
#define MIXED_DATA "<mixed>"
#define SIGN_LINE_LENGTH 13
#define STUNT_LINE_LENGTH 69
//-------------------------------------------------------------------------------------------------
class CChunkEditValues;
//-------------------------------------------------------------------------------------------------

class CTrack : public CTrackData
{
public:
  CTrack();
  ~CTrack();
  
  bool SaveTrack(const QString &sFilename);
};

//-------------------------------------------------------------------------------------------------
#endif
