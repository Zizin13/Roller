#ifndef _TRACKEDITOR_TRACK_H
#define _TRACKEDITOR_TRACK_H
//-------------------------------------------------------------------------------------------------
#include <vector>
#include <map>
#include "qstring.h"
//-------------------------------------------------------------------------------------------------
#define MIXED_DATA "<mixed>"
//-------------------------------------------------------------------------------------------------
struct tTrackHeader
{
  int iNumChunks;
  long long llUnk1;
  long long llUnk2;
  long long llUnk3;
};
//-------------------------------------------------------------------------------------------------
struct tGeometryChunk
{
  std::string sString;
  //line 1
  unsigned short unLeftShoulderWidth;
  unsigned short unLeftLaneWidth;
  unsigned short unRightLaneWidth;
  unsigned short unRightShoulderWidth;
  long long llUnk1; //todo
  long long llUnk2; //todo
  long long llUnk3; //todo
  float fYaw;
  float fPitch;
  float fRoll;
  int iAILine1;
  int iAILine2;
  int iAILine3;
  int iAILine4;
  unsigned short unTrackGrip;
  unsigned short unLeftShoulderGrip;
  unsigned short unRightShoulderGrip;
  long long llUnk4; //todo
  long long llUnk5; //todo
  long long llUnk6; //todo
  long long llUnk7; //todo
  long long llUnk8; //todo
  //line 2
  unsigned long ulLeftSurfaceType;
  unsigned long ulCenterSurfaceType;
  unsigned long ulRightSurfaceType;
  long long llUnk9; //todo
  long long llUnk10; //todo
  long long llUnk11; //todo
  long long llUnk12; //todo
  long long llUnk13; //todo
  long long llUnk14; //todo
  long long llUnk15; //todo
  long long llUnk16; //todo
  long long llUnk17; //todo
  long long llUnk18; //todo
  long long llUnk19; //todo
  long long llUnk20; //todo
  float fUnk1; //todo
  float fUnk2; //todo
  float fUnk3; //todo
  //line 3
  long long llUnk21; //todo
  long long llUnk22; //todo
  long long llUnk23; //todo
  long long llUnk24; //todo
  long long llUnk25; //todo
  long long llUnk26; //todo
  long long llUnk27; //todo
  long long llUnk28; //todo
  long long llUnk29; //todo
  long long llUnk30; //todo
  long long llUnk31; //todo
  long long llUnk32; //todo
  long long llUnk33; //todo
  long long llUnk34; //todo
  long long llUnk35; //todo
  long long llUnk36; //todo
  long long llUnk37; //todo
  long long llUnk38; //todo
  long long llUnk39; //todo
  long long llUnk40; //todo
  long long llUnk41; //todo
  long long llUnk42; //todo
  long long llUnk43; //todo
  long long llUnk44; //todo
  long long llUnk45; //todo
  long long llUnk46; //todo
  long long llUnk47; //todo
  long long llUnk48; //todo
  long long llUnk49; //todo
  long long llUnk50; //todo
};
typedef std::vector<tGeometryChunk> CChunkAy;
//-------------------------------------------------------------------------------------------------
typedef std::map<int, int> CTupleMap;
//-------------------------------------------------------------------------------------------------
struct tStunt
{
  int iGeometryIndex;
  int iScaleFactor;
  int iAngle;
  int iUnknown; //todo
  int iTimingGroup;
  int iHeight;
  int iTimeBulging;
  int iTimeFlat;
  int iSmallerExpandsLargerContracts;
  int iBulge;
};
typedef std::vector<tStunt> CStuntAy;
//-------------------------------------------------------------------------------------------------
struct tRaceInfo
{
  int iTrackNumber;
  int iImpossibleLaps;
  int iHardLaps;
  int iTrickyLaps;
  int iMediumLaps;
  int iEasyLaps;
  int iGirlieLaps;
  float fTrackMapSize;
  int iTrackMapFidelity;
  float fUnknown; //todo
};
//-------------------------------------------------------------------------------------------------
enum eFileSection
{
  HEADER = 0,
  GEOMETRY,
  TUPLES,
  STUNTS,
  TEXTURE,
  TRACK_NUM,
  LAPS,
  MAP
};
//-------------------------------------------------------------------------------------------------

class CTrack
{
public:
  CTrack();
  ~CTrack();

  void ClearData();
  bool LoadTrack(const QString &sFilename);
  bool SaveTrack(const QString &sFilename);
  void GetGeometryCursorPos(int iStartIndex, int iEndIndex, int &iStartCursorPos, int &iEndCursorPos);
  void GetGeometryValuesFromSelection(int iStartIndex, int iEndIndex
    , QString &sLeftShoulderWidth, QString &sLeftLaneWidth, QString &sRightLaneWidth, QString &sRightShoulderWidth
    , QString &sUnk1, QString &sUnk2, QString &sUnk3
    , QString &sYaw, QString &sPitch, QString &sRoll
    , QString &sAILine1, QString &sAILine2, QString &sAILine3, QString &sAILine4
    , QString &sTrackGrip, QString &sLeftShoulderGrip, QString &sRightShoulderGrip
    , QString &sUnk4, QString &sUnk5, QString &sUnk6, QString &sUnk7, QString &sUnk8
    , QString &sLeftSurfaceType, QString &sCenterSurfaceType, QString &sRightSurfaceType
    , QString &sUnk9, QString &sUnk10, QString &sUnk11, QString &sUnk12, QString &sUnk13, QString &sUnk14
    , QString &sUnk15, QString &sUnk16, QString &sUnk17, QString &sUnk18, QString &sUnk19, QString &sUnk20
    , QString &sfUnk1, QString &sfUnk2, QString &sfUnk3
    , QString &sUnk21, QString &sUnk22, QString &sUnk23, QString &sUnk24, QString &sUnk25, QString &sUnk26
    , QString &sUnk27, QString &sUnk28, QString &sUnk29, QString &sUnk30, QString &sUnk31, QString &sUnk32
    , QString &sUnk33, QString &sUnk34, QString &sUnk35, QString &sUnk36, QString &sUnk37, QString &sUnk38
    , QString &sUnk39, QString &sUnk40, QString &sUnk41, QString &sUnk42, QString &sUnk43, QString &sUnk44
    , QString &sUnk45, QString &sUnk46, QString &sUnk47, QString &sUnk48, QString &sUnk49, QString &sUnk50);
  void ApplyGeometrySettings(int iStartIndex, int iEndIndex
    , const QString &sLeftShoulderWidth, const QString &sLeftLaneWidth, const QString &sRightLaneWidth, const QString &sRightShoulderWidth
    , const QString &sUnk1, const QString &sUnk2, const QString &sUnk3
    , const QString &sYaw, const QString &sPitch, const QString &sRoll
    , const QString &sAILine1, const QString &sAILine2, const QString &sAILine3, const QString &sAILine4
    , const QString &sTrackGrip, const QString &sLeftShoulderGrip, const QString &sRightShoulderGrip
    , const QString &sUnk4, const QString &sUnk5, const QString &sUnk6, const QString &sUnk7, const QString &sUnk8
    , const QString &sLeftSurfaceType, const QString &sCenterSurfaceType, const QString &sRightSurfaceType
    , const QString &sUnk9, const QString &sUnk10, const QString &sUnk11, const QString &sUnk12, const QString &sUnk13, const QString &sUnk14
    , const QString &sUnk15, const QString &sUnk16, const QString &sUnk17, const QString &sUnk18, const QString &sUnk19, const QString &sUnk20
    , const QString &sfUnk1, const QString &sfUnk2, const QString &sfUnk3
    , const QString &sUnk21, const QString &sUnk22, const QString &sUnk23, const QString &sUnk24, const QString &sUnk25, const QString &sUnk26
    , const QString &sUnk27, const QString &sUnk28, const QString &sUnk29, const QString &sUnk30, const QString &sUnk31, const QString &sUnk32
    , const QString &sUnk33, const QString &sUnk34, const QString &sUnk35, const QString &sUnk36, const QString &sUnk37, const QString &sUnk38
    , const QString &sUnk39, const QString &sUnk40, const QString &sUnk41, const QString &sUnk42, const QString &sUnk43, const QString &sUnk44
    , const QString &sUnk45, const QString &sUnk46, const QString &sUnk47, const QString &sUnk48, const QString &sUnk49, const QString &sUnk50);
  void UpdateChunkStrings();

  tTrackHeader m_header;
  CChunkAy m_chunkAy;
  CTupleMap m_tupleMap;
  CStuntAy m_stuntAy;
  QString m_sTextureFile;
  QString m_sBuildingFile;
  CTupleMap m_backsMap;
  tRaceInfo m_raceInfo;

private:
  void GenerateChunkString(tGeometryChunk &chunk);
  void ProcessTuple(const QStringList &slLine, eFileSection &section);
};

//-------------------------------------------------------------------------------------------------
#endif
