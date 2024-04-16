#ifndef _UTILS_TRACKDATA_H
#define _UTILS_TRACKDATA_H
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
  int iUnk04; //todo
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
enum eShapeSection
{
  DRIVING_SURFACE = 0,
  LLANE,
  RLANE,
  LSHOULDER,
  RSHOULDER
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

protected:
  tVertex *MakeVertsCenterline(uint32 &numVertices);
  uint32 *MakeIndicesCenterline(uint32 &numIndices);
  tVertex *MakeVertsSurface(uint32 &numVertices);
  uint32 *MakeIndicesSurface(uint32 &numIndices);
  uint32 *MakeIndicesSurfaceWireframe(uint32 &numIndices);

  tVertex *MakeVertsLLane(uint32 &numVertices);
  tVertex *MakeVertsRLane(uint32 &numVertices);
  tVertex *MakeVertsLShoulder(uint32 &numVertices);
  tVertex *MakeVertsRShoulder(uint32 &numVertices);

  uint32 *MakeIndicesSingleSection(uint32 &numIndices);
  uint32 *MakeIndicesSingleSectionWireframe(uint32 &numIndices);

  void GetCenter(int i, glm::vec3 prevCenter, float fScale, 
                 glm::vec3 &center, glm::vec3 &pitchAxis, glm::vec3 &nextChunkPitched, glm::mat4 &rollMat);
  void GetLLane(int i, glm::vec3 center, float fScale, glm::vec3 pitchAxis, glm::mat4 rollMat,
                glm::vec3 &lLane);
  void GetRLane(int i, glm::vec3 center, float fScale, glm::vec3 pitchAxis, glm::mat4 rollMat,
                glm::vec3 &rLane);
  void GetLShoulder(int i, glm::vec3 lLane, float fScale, glm::vec3 pitchAxis, glm::mat4 rollMat, glm::vec3 nextChunkPitched, 
                    glm::vec3 &lShoulder);
  void GetRShoulder(int i, glm::vec3 rLane, float fScale, glm::vec3 pitchAxis, glm::mat4 rollMat, glm::vec3 nextChunkPitched,
                    glm::vec3 &rShoulder);

  bool IsNumber(const std::string &str);
  bool ProcessTrackData(const uint8_t *pData, size_t length);
  void ProcessSign(const std::vector<std::string> &lineAy, eFileSection &section);
  void WriteToVector(std::vector<uint8_t> &data, const char *szText);
};

//-------------------------------------------------------------------------------------------------
#endif
