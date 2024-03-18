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
  int iHeaderUnk1;
  int iHeaderUnk2;
  int iHeaderUnk3;
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
  int iLeftShoulderHeight;
  int iRightShoulderHeight;
  int iLength;
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
  int iUnk04; //todo
  int iUnk05; //todo
  int iUnk06; //todo
  int iUnk07; //todo
  int iUnk08; //todo
  //line 2
  int iLeftSurfaceType;
  int iCenterSurfaceType;
  int iRightSurfaceType;
  int iLeftWallType;
  int iRightWallType;
  int iRoofType;
  int iUnk12; //todo
  int iLeftOuterExtraWallType;
  int iUnk14; //todo
  int iRightOuterExtraWallType;
  int iUnk16; //todo
  int iEnvironmentFloorType;
  int iSignType;
  int iUnk19; //todo
  int iUnk20; //todo
  float fUnk1; //todo
  float fUnk2; //todo
  float fUnk3; //todo
  //line 3
  int iLeftOuterUpperExtraWallAngle;
  int iLeftOuterLowerExtraWallAngle;
  int iUnk23; //todo
  int iUnk24; //todo
  int iRightOuterLowerExtraWallAngle;
  int iRightOuterUpperExtraWallAngle;
  int iLeftOuterUpperExtraWallHeight;
  int iLeftOuterLowerExtraWallHeight;
  int iUnk29; //todo
  int iUnk30; //todo
  int iRightOuterLowerExtraWallHeight;
  int iRightOuterUpperExtraWallHeight;
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
    , QString &sLShoulderHeight, QString &sRShoulderHeight, QString &sLength
    , QString &sYaw, QString &sPitch, QString &sRoll
    , QString &sAILine1, QString &sAILine2, QString &sAILine3, QString &sAILine4
    , QString &sTrackGrip, QString &sLeftShoulderGrip, QString &sRightShoulderGrip
    , QString &sUnk04, QString &sUnk05, QString &sUnk06, QString &sUnk07, QString &sUnk08
    , QString &sLeftSurfaceType, QString &sCenterSurfaceType, QString &sRightSurfaceType
    , QString &sLWallType, QString &sRWallType, QString &sRoofType, QString &sUnk12, QString &sLOuterExtraWallType, QString &sUnk14
    , QString &sROuterExtraWallType, QString &sUnk16, QString &sEnvironmentFloorType, QString &sSignType, QString &sUnk19, QString &sUnk20
    , QString &sfUnk1, QString &sfUnk2, QString &sfUnk3
    , QString &sLOuterUpperExtraWallAngle, QString &sLOuterLowerExtraWallAngle, QString &sUnk23, QString &sUnk24, QString &sROuterLowerExtraWallAngle, QString &sROuterUpperExtraWallAngle
    , QString &sLOuterUpperExtraWallHeight, QString &sLOuterLowerExtraWallHeight, QString &sUnk29, QString &sUnk30, QString &sROuterLowerExtraWallHeight, QString &sROuterUpperExtraWallHeight
    , QString &sUnk33, QString &sUnk34, QString &sUnk35, QString &sUnk36, QString &sUnk37, QString &sUnk38
    , QString &sUnk39, QString &sUnk40, QString &sUnk41, QString &sUnk42, QString &sUnk43, QString &sUnk44
    , QString &sUnk45, QString &sUnk46, QString &sUnk47, QString &sUnk48, QString &sUnk49, QString &sUnk50);
  void ApplyGeometrySettings(int iStartIndex, int iEndIndex
    , const QString &sLeftShoulderWidth, const QString &sLeftLaneWidth, const QString &sRightLaneWidth, const QString &sRightShoulderWidth
    , const QString &sLShoulderHeight, const QString &sRShoulderHeight, const QString &sLength
    , const QString &sYaw, const QString &sPitch, const QString &sRoll
    , const QString &sAILine1, const QString &sAILine2, const QString &sAILine3, const QString &sAILine4
    , const QString &sTrackGrip, const QString &sLeftShoulderGrip, const QString &sRightShoulderGrip
    , const QString &sUnk04, const QString &sUnk05, const QString &sUnk06, const QString &sUnk07, const QString &sUnk08
    , const QString &sLeftSurfaceType, const QString &sCenterSurfaceType, const QString &sRightSurfaceType
    , const QString &sLWallType, const QString &sRWallType, const QString &sRoofType, const QString &sUnk12, const QString &sLOuterExtraWallType, const QString &sUnk14
    , const QString &sROuterExtraWallType, const QString &sUnk16, const QString &sEnvironmentFloorType, const QString &sSignType, const QString &sUnk19, const QString &sUnk20
    , const QString &sfUnk1, const QString &sfUnk2, const QString &sfUnk3
    , const QString &sLOuterUpperExtraWallAngle, const QString &sLOuterLowerExtraWallAngle, const QString &sUnk23, const QString &sUnk24, const QString &sROuterLowerExtraWallAngle, const QString &sROuterUpperExtraWallAngle
    , const QString &sLOuterUpperExtraWallHeight, const QString &sLOuterLowerExtraWallHeight, const QString &sUnk29, const QString &sUnk30, const QString &sROuterLowerExtraWallHeight, const QString &sROuterUpperExtraWallHeight
    , const QString &sUnk33, const QString &sUnk34, const QString &sUnk35, const QString &sUnk36, const QString &sUnk37, const QString &sUnk38
    , const QString &sUnk39, const QString &sUnk40, const QString &sUnk41, const QString &sUnk42, const QString &sUnk43, const QString &sUnk44
    , const QString &sUnk45, const QString &sUnk46, const QString &sUnk47, const QString &sUnk48, const QString &sUnk49, const QString &sUnk50);
  void InsertGeometryChunk(int iIndex, int iCount
    , const QString &sLeftShoulderWidth, const QString &sLeftLaneWidth, const QString &sRightLaneWidth, const QString &sRightShoulderWidth
    , const QString &sLShoulderHeight, const QString &sRShoulderHeight, const QString &sLength
    , const QString &sYaw, const QString &sPitch, const QString &sRoll
    , const QString &sAILine1, const QString &sAILine2, const QString &sAILine3, const QString &sAILine4
    , const QString &sTrackGrip, const QString &sLeftShoulderGrip, const QString &sRightShoulderGrip
    , const QString &sUnk04, const QString &sUnk05, const QString &sUnk06, const QString &sUnk07, const QString &sUnk08
    , const QString &sLeftSurfaceType, const QString &sCenterSurfaceType, const QString &sRightSurfaceType
    , const QString &sLWallType, const QString &sRWallType, const QString &sRoofType, const QString &sUnk12, const QString &sLOuterExtraWallType, const QString &sUnk14
    , const QString &sROuterExtraWallType, const QString &sUnk16, const QString &sEnvironmentFloorType, const QString &sSignType, const QString &sUnk19, const QString &sUnk20
    , const QString &sfUnk1, const QString &sfUnk2, const QString &sfUnk3
    , const QString &sLOuterUpperExtraWallAngle, const QString &sLOuterLowerExtraWallAngle, const QString &sUnk23, const QString &sUnk24, const QString &sROuterLowerExtraWallAngle, const QString &sROuterUpperExtraWallAngle
    , const QString &sLOuterUpperExtraWallHeight, const QString &sLOuterLowerExtraWallHeight, const QString &sUnk29, const QString &sUnk30, const QString &sROuterLowerExtraWallHeight, const QString &sROuterUpperExtraWallHeight
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
