#ifndef _TRACKEDITOR_TRACK_H
#define _TRACKEDITOR_TRACK_H
//-------------------------------------------------------------------------------------------------
#include <vector>
#include <map>
#include "qstring.h"
//-------------------------------------------------------------------------------------------------
#define MIXED_DATA "<mixed>"
#define SIGN_LINE_LENGTH 13
#define STUNT_LINE_LENGTH 69
//-------------------------------------------------------------------------------------------------
class CChunkEditValues;
//-------------------------------------------------------------------------------------------------
struct tTrackHeader
{
  int iNumChunks;
  int iHeaderUnk1;
  int iHeaderUnk2;
  int iHeaderUnk3;
};
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
  double dYaw;
  double dPitch;
  double dRoll;
  int iAILine1;
  int iAILine2;
  int iAILine3;
  int iAILine4;
  int iTrackGrip;
  int iLeftShoulderGrip;
  int iRightShoulderGrip;
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
  int iLUOuterWallType;
  int iLLOuterWallType;
  int iOuterFloorType;
  int iRLOuterWallType;
  int iRUOuterWallType;
  int iEnvironmentFloorType;
  int iSignType;
  int iSignHorizOffset;
  int iSignVertOffset;
  double dSignYaw;
  double dSignPitch;
  double dSignRoll;
  //line 3
  int iLUOuterWallAngle;
  int iLLOuterWallAngle;
  int iUnk23; //todo
  int iUnk24; //todo
  int iRLOuterWallAngle;
  int iRUOuterWallAngle;
  int iLUOuterWallHeight;
  int iLLOuterWallHeight;
  int iUnk29; //todo
  int iUnk30; //todo
  int iRLOuterWallHeight;
  int iRUOuterWallHeight;
  int iRoofHeight;
  int iDrawOrder1;
  int iDrawOrder2;
  int iDrawOrder3;
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

  //additional data
  int iSignTexture;
  int iBackTexture;
  
  //stunt
  tStunt stunt;
};
typedef std::vector<tGeometryChunk> CChunkAy;
//-------------------------------------------------------------------------------------------------
typedef std::map<int, int> CSignMap;
typedef std::map<int, tStunt *> CStuntMap;
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
  double dTrackMapSize;
  int iTrackMapFidelity;
  double dUnknown; //todo
};
//-------------------------------------------------------------------------------------------------
enum eFileSection
{
  HEADER = 0,
  GEOMETRY,
  SIGNS,
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
  bool LoadTrack(const QString &sFilename, bool bIsMangled);
  bool SaveTrack(const QString &sFilename, bool bIsMangled);
  void GetGeometryCursorPos(int iStartIndex, int iEndIndex, int &iStartCursorPos, int &iEndCursorPos);
  void GetGeometryValuesFromSelection(int iStartIndex, int iEndIndex, CChunkEditValues &editVals);
  void ApplyGeometrySettings(int iStartIndex, int iEndIndex, const CChunkEditValues &editVals);
  void InsertGeometryChunk(int iIndex, int iCount, const CChunkEditValues &editVals);
  void UpdateChunkStrings();

  tTrackHeader m_header;
  CChunkAy m_chunkAy;
  QString m_sTextureFile;
  QString m_sBuildingFile;
  tRaceInfo m_raceInfo;
  bool m_bIsMangled;

private:
  bool ProcessTrackData(const QByteArray &baData);
  void GenerateChunkString(tGeometryChunk &chunk);
  void ProcessSign(const QStringList &slLine, eFileSection &section);
  void GetTrackData(std::vector<uint8_t> &data);
  void WriteToVector(std::vector<uint8_t> &data, const char *szText);
};

//-------------------------------------------------------------------------------------------------
#endif
