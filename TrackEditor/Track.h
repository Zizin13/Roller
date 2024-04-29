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

  bool LoadTrack(const QString &sFilename);
  bool SaveTrack(const QString &sFilename);
  void GetGeometryCursorPos(int iStartIndex, int iEndIndex, int &iStartCursorPos, int &iEndCursorPos);
  void GetGeometryValuesFromSelection(int iStartIndex, int iEndIndex, CChunkEditValues &editVals);
  void ApplyGeometrySettings(int iStartIndex, int iEndIndex, const CChunkEditValues &editVals);
  void InsertGeometryChunk(int iIndex, int iCount, const CChunkEditValues &editVals);
  void UpdateChunkStrings();

private:
  void GenerateChunkString(tGeometryChunk &chunk);
};

//-------------------------------------------------------------------------------------------------
#endif
