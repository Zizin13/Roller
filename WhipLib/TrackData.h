#ifndef _WHIPLIB_TRACKDATA_H
#define _WHIPLIB_TRACKDATA_H
//-------------------------------------------------------------------------------------------------
#include <vector>
#include <map>
#include <string>
#include "Vertex.h"
#include "Texture.h"
//-------------------------------------------------------------------------------------------------
struct tTrackHeader
{
  int iNumChunks;
  int iHeaderUnk1;
  int iHeaderUnk2;
  int iFloorDepth;
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
  void Clear();

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
  int iAIMaxAccel; //todo
  int iUnk05; //todo
  int iAudioAboveTrigger;
  int iAudioTriggerSpeed;
  int iAudioBelowTrigger;
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
  int iLUOuterWallHOffset;
  int iLLOuterWallHOffset;
  int iLOuterFloorHOffset;
  int iROuterFloorHOffset;
  int iRLOuterWallHOffset;
  int iRUOuterWallHOffset;
  int iLUOuterWallHeight;
  int iLLOuterWallHeight;
  int iLOuterFloorHeight;
  int iROuterFloorHeight;
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
  double dPreviewSize;
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
enum eShapeSection
{
  DRIVING_SURFACE = 0,
  CENTER,
  LLANE,
  RLANE,
  LSHOULDER,
  RSHOULDER,
  LWALL,
  RWALL,
  ROOF,
  ENVIRFLOOR,
  OWALLFLOOR,
  LLOWALL,
  RLOWALL,
  LUOWALL,
  RUOWALL,
  SELECTED,
};
//-------------------------------------------------------------------------------------------------
class CShader;
class CShapeData;
//-------------------------------------------------------------------------------------------------

class CTrackData
{
public:
  CTrackData();
  ~CTrackData();

  void ClearData();
  bool LoadTrack(const std::string &sFilename, bool bIsMangled);
  void GetTrackData(std::vector<uint8_t> &data);
  CShapeData *MakeTrackCenterline(CShader *pShader);
  CShapeData *MakeTrackSurface(CShader *pShader, eShapeSection section, bool bWireframe = false);
  CShapeData *MakeSelectedChunks(CShader *pShader, int iStart, int iEnd);

  static unsigned int GetSignedBitValueFromInt(int iValue);
  static int GetIntValueFromSignedBit(unsigned int uiValue);

  tTrackHeader m_header;
  CChunkAy m_chunkAy;
  std::string m_sTextureFile;
  std::string m_sBuildingFile;
  tRaceInfo m_raceInfo;
  bool m_bIsMangled;
  CTexture m_tex;
  CTexture m_bld;
  float m_fScale;

protected:
  tVertex *MakeVerts(uint32 &numVertices, eShapeSection section);
  tVertex *MakeVertsEnvirFloor(uint32 &numVertices);

  uint32 *MakeIndicesCenterline(uint32 &numIndices);
  uint32 *MakeIndicesSurface(uint32 &numIndices);
  uint32 *MakeIndicesSurfaceWireframe(uint32 &numIndices);
  uint32 *MakeIndicesSingleSection(uint32 &numIndices, eShapeSection section);
  uint32 *MakeIndicesSelectedChunks(uint32 &numIndices, int iStart, int iEnd);
  bool ShouldMakeIndicesForChunk(int i, eShapeSection section);
  bool ShouldDrawSurfaceType(int iSurfaceType);
  uint32 *MakeIndicesSingleSectionWireframe(uint32 &numIndices);

  void GetCenter(int i, glm::vec3 prevCenter,
                 glm::vec3 &center, glm::vec3 &pitchAxis, glm::vec3 &nextChunkPitched, glm::mat4 &rollMat);
  void GetLane(int i, glm::vec3 center, glm::vec3 pitchAxis, glm::mat4 rollMat,
               glm::vec3 &lane, bool bLeft);
  void GetShoulder(int i, glm::vec3 attach, glm::vec3 pitchAxis, glm::mat4 rollMat, glm::vec3 nextChunkPitched, 
                   glm::vec3 &shoulder, bool bLeft, bool bIgnoreHeight = false);
  void GetEnvirFloor(int i, glm::vec3 lShoulder, glm::vec3 rShoulder,
                     glm::vec3 &lEnvirFloor, glm::vec3 &rEnvirFloor);
  void GetOWallFloor(int i, glm::vec3 lLane, glm::vec3 rLane, glm::vec3 pitchAxis, glm::vec3 nextChunkPitched,
                     glm::vec3 &lFloor, glm::vec3 &rFloor);
  void GetWall(int i, glm::vec3 bottomAttach, glm::vec3 pitchAxis, glm::mat4 rollMat, glm::vec3 nextChunkPitched,
               glm::vec3 &lloWall, eShapeSection wallSection);
  void ApplyVerticesSingleSection(int i, tVertex *vertices, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);

  bool IsNumber(const std::string &str);
  bool ProcessTrackData(const uint8_t *pData, size_t length);
  void ProcessSign(const std::vector<std::string> &lineAy, eFileSection &section);
  void WriteToVector(std::vector<uint8_t> &data, const char *szText);
};

//-------------------------------------------------------------------------------------------------
#endif