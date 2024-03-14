#ifndef _TRACKEDITOR_TRACK_H
#define _TRACKEDITOR_TRACK_H
//-------------------------------------------------------------------------------------------------
#include <vector>
#include <map>
#include "qstring.h"
//-------------------------------------------------------------------------------------------------
#define MIXED_DATA "<mixed>"
#define TUPLE_LINE_LENGTH 13
#define STUNT_LINE_LENGTH 69
//-------------------------------------------------------------------------------------------------
struct tTrackHeader
{
  int iNumChunks;
  int iUnk1;
  int iUnk2;
  int iUnk3;
};
//-------------------------------------------------------------------------------------------------
struct tGeometryChunk
{
  std::string sString;
  //line 1
  int iLeftShoulderWidth;
  int iLeftLaneWidth;
  int iRightLaneWidth;
  int iRightShoulderWidth;
  int iUnk1; //todo
  int iUnk2; //todo
  int iUnk3; //todo
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
  int iUnk4; //todo
  int iUnk5; //todo
  int iUnk6; //todo
  int iUnk7; //todo
  int iUnk8; //todo
  //line 2
  int iLeftSurfaceType;
  int iCenterSurfaceType;
  int iRightSurfaceType;
  int iUnk9; //todo
  int iUnk10; //todo
  int iUnk11; //todo
  int iUnk12; //todo
  int iUnk13; //todo
  int iUnk14; //todo
  int iUnk15; //todo
  int iUnk16; //todo
  int iUnk17; //todo
  int iUnk18; //todo
  int iUnk19; //todo
  int iUnk20; //todo
  float fUnk1; //todo
  float fUnk2; //todo
  float fUnk3; //todo
  //line 3
  int iUnk21; //todo
  int iUnk22; //todo
  int iUnk23; //todo
  int iUnk24; //todo
  int iUnk25; //todo
  int iUnk26; //todo
  int iUnk27; //todo
  int iUnk28; //todo
  int iUnk29; //todo
  int iUnk30; //todo
  int iUnk31; //todo
  int iUnk32; //todo
  int iUnk33; //todo
  int iUnk34; //todo
  int iUnk35; //todo
  int iUnk36; //todo
  int iUnk37; //todo
  int iUnk38; //todo
  int iUnk39; //todo
  int iUnk40; //todo
  int iUnk41; //todo
  int iUnk42; //todo
  int iUnk43; //todo
  int iUnk44; //todo
  int iUnk45; //todo
  int iUnk46; //todo
  int iUnk47; //todo
  int iUnk48; //todo
  int iUnk49; //todo
  int iUnk50; //todo
};
typedef std::vector<tGeometryChunk> CChunkAy;
//-------------------------------------------------------------------------------------------------
typedef std::map<int, int> CTupleMap;
//-------------------------------------------------------------------------------------------------
struct tStunt
{
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
typedef std::map<int, tStunt> CStuntMap;
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
  void GetTextureCursorPos(int iKey, int &iStartCursorPos, int &iEndCursorPos);
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
  void InsertGeometryChunk(int iIndex, int iCount
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
  CStuntMap m_stuntMap;
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
