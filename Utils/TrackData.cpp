#include "TrackData.h"
#include "Unmangler.h"
#include <assert.h>
#include <fstream>
#include <sstream>
#include "ShapeGenerator.h"
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
#define HEADER_ELEMENT_COUNT 4
#define CHUNK_LINE_0_COUNT 22
#define CHUNK_LINE_1_COUNT 18
#define CHUNK_LINE_2_COUNT 30
#define SIGNS_COUNT 2
#define STUNTS_COUNT 10
#define BACKS_COUNT 2
#define LAPS_COUNT 6
#define MAP_COUNT 3
//-------------------------------------------------------------------------------------------------

void tGeometryChunk::Clear()
{
  sString = "";
  //line 1
  iLeftShoulderWidth = 0;
  iLeftLaneWidth = 0;
  iRightLaneWidth = 0;
  iRightShoulderWidth = 0;
  iLeftShoulderHeight = 0;
  iRightShoulderHeight = 0;
  iLength = 0;
  dYaw = 0.0;
  dPitch = 0.0;
  dRoll = 0.0;
  iAILine1 = 0;
  iAILine2 = 0;
  iAILine3 = 0;
  iAILine4 = 0;
  iTrackGrip = 0;
  iLeftShoulderGrip = 0;
  iRightShoulderGrip = 0;
  iUnk04 = 0;
  iUnk05 = 0;
  iAudioAboveTrigger = 0;
  iAudioTriggerSpeed = 0;
  iAudioBelowTrigger = 0;
  //line 2
  iLeftSurfaceType = 0;
  iCenterSurfaceType = 0;
  iRightSurfaceType = 0;
  iLeftWallType = 0;
  iRightWallType = 0;
  iRoofType = 0;
  iLUOuterWallType = 0;
  iLLOuterWallType = 0;
  iOuterFloorType = 0;
  iRLOuterWallType = 0;
  iRUOuterWallType = 0;
  iEnvironmentFloorType = 0;
  iSignType = 0;
  iSignHorizOffset = 0;
  iSignVertOffset = 0;
  dSignYaw = 0.0;
  dSignPitch = 0.0;
  dSignRoll = 0.0;
  //line 3
  iLUOuterWallAngle = 0;
  iLLOuterWallAngle = 0;
  iUnk23 = 0;
  iUnk24 = 0;
  iRLOuterWallAngle = 0;
  iRUOuterWallAngle = 0;
  iLUOuterWallHeight = 0;
  iLLOuterWallHeight = 0;
  iUnk29 = 0;
  iUnk30 = 0;
  iRLOuterWallHeight = 0;
  iRUOuterWallHeight = 0;
  iRoofHeight = 0;
  iDrawOrder1 = 0;
  iDrawOrder2 = 0;
  iDrawOrder3 = 0;
  iUnk37 = 0;
  iUnk38 = 0;
  iUnk39 = 0;
  iUnk40 = 0;
  iUnk41 = 0;
  iUnk42 = 0;
  iUnk43 = 0;
  iUnk44 = 0;
  iUnk45 = 0;
  iUnk46 = 0;
  iUnk47 = 0;
  iUnk48 = 0;
  iUnk49 = 0;
  iUnk50 = 0;

  //additional data
  iSignTexture = 0;
  iBackTexture = 0;

  //stunt
  memset(&stunt, 0, sizeof(stunt));
}

//-------------------------------------------------------------------------------------------------

CTrackData::CTrackData()
{
  ClearData();
}

//-------------------------------------------------------------------------------------------------

CTrackData::~CTrackData()
{

}

//-------------------------------------------------------------------------------------------------

void CTrackData::ClearData()
{
  memset(&m_header, 0, sizeof(m_header));
  m_header.iHeaderUnk3 = 2048;
  m_chunkAy.clear();
  m_sTextureFile = "";
  m_sBuildingFile = "";
  m_bIsMangled = false;
  memset(&m_raceInfo, 0, sizeof(m_raceInfo));
}

//-------------------------------------------------------------------------------------------------

bool CTrackData::LoadTrack(const std::string &sFilename, bool bIsMangled)
{
  ClearData();
  m_bIsMangled = bIsMangled;

  if (sFilename.empty()) {
    //TODO Logging g_pMainWindow->LogMessage("Track filename empty: " + sFilename);
    return false;
  }

  //open file
  std::ifstream file(sFilename.c_str(), std::ios::binary);
  file.seekg(0, file.end);
  size_t length = file.tellg();
  file.seekg(0, file.beg);
  if (length <= 0) {
    //todo logging
    return false;
  }

  //read file
  char *szBuf = new char[length];
  memset(szBuf, 0, length);
  file.read(szBuf, length);

  bool bSuccess = false;
  //unmangle
  if (bIsMangled) {
    int iUnmangledLength = GetUnmangledLength((uint8_t *)szBuf, (int)length);
    uint8_t *szUnmangledData = new uint8_t[iUnmangledLength];
    UnmangleFile((uint8_t *)szBuf, (int)length, szUnmangledData, iUnmangledLength);
    bSuccess = ProcessTrackData(szUnmangledData, (size_t)iUnmangledLength);
    delete[] szUnmangledData;
  } else {
    bSuccess = ProcessTrackData((uint8_t *)szBuf, length);
  }

  //cleanup
  delete[] szBuf;
  file.close();

  //TODO logging
  //QString sSuccess = (bSuccess ? "Successfully loaded" : "Failed to load");
  //QString sLogMsg = sSuccess + " file " + sFilename + "\n"
  //  + "  geometry chunks: " + QString::number(m_chunkAy.size()) + "\n"
  //  + "  texture file: " + m_sTextureFile + "\n"
  //  + "  building file: " + m_sBuildingFile;
  //g_pMainWindow->LogMessage(sLogMsg);

  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

bool CTrackData::IsNumber(const std::string &str)
{
  char *ptr;
  strtol(str.c_str(), &ptr, 10);
  return *ptr == '\0';
}

//-------------------------------------------------------------------------------------------------

bool CTrackData::ProcessTrackData(const uint8_t *pData, size_t length)
{
  bool bSuccess = true;
  int iChunkLine = 0;
  struct tGeometryChunk currChunk;
  eFileSection section = HEADER;

  std::stringstream ssFile((char*)pData);
  while (ssFile.good()) {
    std::string sLine;
    getline(ssFile, sLine, '\n');

    //get value array
    std::vector<std::string> lineAy;
    std::stringstream ssLine(sLine);
    while (ssLine.good()) {
      std::string sSubStr;
      getline(ssLine, sSubStr, ' ');

      //strip \r off
      size_t pos = sSubStr.find('\r');
      if (pos != std::string::npos)
        sSubStr = sSubStr.substr(0, pos);

      if (!sSubStr.empty() && sSubStr.find("ýýýý") == std::string::npos)
        lineAy.push_back(sSubStr);
    }

    switch (section) {
      case HEADER:
        //must be first thing in file
        if (lineAy.size() == HEADER_ELEMENT_COUNT) {
          //found header
          m_header.iNumChunks  = std::stoi(lineAy[0]);
          m_header.iHeaderUnk1 = std::stoi(lineAy[1]);
          m_header.iHeaderUnk2 = std::stoi(lineAy[2]);
          m_header.iHeaderUnk3 = std::stoi(lineAy[3]);
          section = GEOMETRY;
        }
        break;
      case GEOMETRY:
        if (iChunkLine == 0) {
          if (lineAy.size() == SIGNS_COUNT) {
            if (m_chunkAy.size() != m_header.iNumChunks) {
              //TODO logging
              //QString sLogMsg = "Warning loading file: number of chunks loaded ("
              //  + QString::number(m_chunkAy.size())
              //  + ") does not match header ("
              //  + QString::number(m_header.iNumChunks)
              //  + ")";
              //g_pMainWindow->LogMessage(sLogMsg);
            }
            // reached next section
            // there is no defined end to geometry chunks
            // more chunks than the count at the top of the file is allowed
            // so we must detect the beginning of the next section
            section = SIGNS;
            ProcessSign(lineAy, section);
          } else if (lineAy.size() == CHUNK_LINE_0_COUNT) {
            //start new chunk
            currChunk.Clear();
            currChunk.iBackTexture = -1;
            currChunk.iSignTexture = -1;
            //process line 1
            currChunk.iLeftShoulderWidth    = std::stoi(lineAy[0]);
            currChunk.iLeftLaneWidth        = std::stoi(lineAy[1]);
            currChunk.iRightLaneWidth       = std::stoi(lineAy[2]);
            currChunk.iRightShoulderWidth   = std::stoi(lineAy[3]);
            currChunk.iLeftShoulderHeight   = std::stoi(lineAy[4]);
            currChunk.iRightShoulderHeight  = std::stoi(lineAy[5]);
            currChunk.iLength               = std::stoi(lineAy[6]);
            currChunk.dYaw                  = std::stod(lineAy[7]);
            currChunk.dPitch                = std::stod(lineAy[8]);
            currChunk.dRoll                 = std::stod(lineAy[9]);
            currChunk.iAILine1              = std::stoi(lineAy[10]);
            currChunk.iAILine2              = std::stoi(lineAy[11]);
            currChunk.iAILine3              = std::stoi(lineAy[12]);
            currChunk.iAILine4              = std::stoi(lineAy[13]);
            currChunk.iTrackGrip            = std::stoi(lineAy[14]);
            currChunk.iLeftShoulderGrip     = std::stoi(lineAy[15]);
            currChunk.iRightShoulderGrip    = std::stoi(lineAy[16]);
            currChunk.iUnk04                = std::stoi(lineAy[17]);
            currChunk.iUnk05                = std::stoi(lineAy[18]);
            currChunk.iAudioAboveTrigger    = std::stoi(lineAy[19]);
            currChunk.iAudioTriggerSpeed    = std::stoi(lineAy[20]);
            currChunk.iAudioBelowTrigger    = std::stoi(lineAy[21]);
            //inc chunk index
            ++iChunkLine;
          }
          //TODO: what about invalid number of chunks?
        } else if (iChunkLine == 1) {
          if (lineAy.size() != CHUNK_LINE_1_COUNT) {
            assert(0);
            //TODO logging
            //g_pMainWindow->LogMessage("Error loading file: invalid line before chunk completion");
            bSuccess = false;
          }
          //process line 2
          currChunk.iLeftSurfaceType      = std::stoi(lineAy[0]);
          currChunk.iCenterSurfaceType    = std::stoi(lineAy[1]);
          currChunk.iRightSurfaceType     = std::stoi(lineAy[2]);
          currChunk.iLeftWallType         = std::stoi(lineAy[3]);
          currChunk.iRightWallType        = std::stoi(lineAy[4]);
          currChunk.iRoofType             = std::stoi(lineAy[5]);
          currChunk.iLUOuterWallType      = std::stoi(lineAy[6]);
          currChunk.iLLOuterWallType      = std::stoi(lineAy[7]);
          currChunk.iOuterFloorType       = std::stoi(lineAy[8]);
          currChunk.iRLOuterWallType      = std::stoi(lineAy[9]);
          currChunk.iRUOuterWallType      = std::stoi(lineAy[10]);
          currChunk.iEnvironmentFloorType = std::stoi(lineAy[11]);
          currChunk.iSignType             = std::stoi(lineAy[12]);
          currChunk.iSignHorizOffset      = std::stoi(lineAy[13]);
          currChunk.iSignVertOffset       = std::stoi(lineAy[14]);
          currChunk.dSignYaw              = std::stod(lineAy[15]);
          currChunk.dSignPitch            = std::stod(lineAy[16]);
          currChunk.dSignRoll             = std::stod(lineAy[17]);
          //inc chunk index
          ++iChunkLine;
        } else if (iChunkLine == 2) {
          if (lineAy.size() != CHUNK_LINE_2_COUNT) {
            assert(0);
            //TODO: logging
            //g_pMainWindow->LogMessage("Error loading file: invalid line before chunk completion");
            bSuccess = false;
          }
          //process line 3
          currChunk.iLUOuterWallAngle   = std::stoi(lineAy[0]);
          currChunk.iLLOuterWallAngle   = std::stoi(lineAy[1]);
          currChunk.iUnk23              = std::stoi(lineAy[2]);
          currChunk.iUnk24              = std::stoi(lineAy[3]);
          currChunk.iRLOuterWallAngle   = std::stoi(lineAy[4]);
          currChunk.iRUOuterWallAngle   = std::stoi(lineAy[5]);
          currChunk.iLUOuterWallHeight  = std::stoi(lineAy[6]);
          currChunk.iLLOuterWallHeight  = std::stoi(lineAy[7]);
          currChunk.iUnk29              = std::stoi(lineAy[8]);
          currChunk.iUnk30              = std::stoi(lineAy[9]);
          currChunk.iRLOuterWallHeight  = std::stoi(lineAy[10]);
          currChunk.iRUOuterWallHeight  = std::stoi(lineAy[11]);
          currChunk.iRoofHeight         = std::stoi(lineAy[12]);
          currChunk.iDrawOrder1         = std::stoi(lineAy[13]);
          currChunk.iDrawOrder2         = std::stoi(lineAy[14]);
          currChunk.iDrawOrder3         = std::stoi(lineAy[15]);
          currChunk.iUnk37              = std::stoi(lineAy[16]);
          currChunk.iUnk38              = std::stoi(lineAy[17]);
          currChunk.iUnk39              = std::stoi(lineAy[18]);
          currChunk.iUnk40              = std::stoi(lineAy[19]);
          currChunk.iUnk41              = std::stoi(lineAy[20]);
          currChunk.iUnk42              = std::stoi(lineAy[21]);
          currChunk.iUnk43              = std::stoi(lineAy[22]);
          currChunk.iUnk44              = std::stoi(lineAy[23]);
          currChunk.iUnk45              = std::stoi(lineAy[24]);
          currChunk.iUnk46              = std::stoi(lineAy[25]);
          currChunk.iUnk47              = std::stoi(lineAy[26]);
          currChunk.iUnk48              = std::stoi(lineAy[27]);
          currChunk.iUnk49              = std::stoi(lineAy[28]);
          currChunk.iUnk50              = std::stoi(lineAy[29]);
          //chunk is complete, add to array and reset index
          m_chunkAy.push_back(currChunk);
          iChunkLine = 0;
        }
        break;
      case SIGNS:
        if (lineAy.size() == SIGNS_COUNT) {
          ProcessSign(lineAy, section);
        } else {
          assert(0);
          //TODO: logging
          //g_pMainWindow->LogMessage("Error loading file: signs section ended before anticipated");
          bSuccess = false;
        }
        break;
      case STUNTS:
        if (lineAy.size() == 0) {
            //do nothing
        } else if (lineAy.size() == 1) {
          if (lineAy[0].compare("-1") == 0) {
            //stunts section always ends with a single -1 value
            section = TEXTURE;
          }
        } else if (lineAy.size() == STUNTS_COUNT) {
          //process stunt
          tStunt stunt;
          memset(&stunt, 0, sizeof(stunt));
          int iGeometryIndex                    = std::stoi(lineAy[0]);
          stunt.iScaleFactor                    = std::stoi(lineAy[1]);
          stunt.iAngle                          = std::stoi(lineAy[2]);
          stunt.iUnknown                        = std::stoi(lineAy[3]);
          stunt.iTimingGroup                    = std::stoi(lineAy[4]);
          stunt.iHeight                         = std::stoi(lineAy[5]);
          stunt.iTimeBulging                    = std::stoi(lineAy[6]);
          stunt.iTimeFlat                       = std::stoi(lineAy[7]);
          stunt.iSmallerExpandsLargerContracts  = std::stoi(lineAy[8]);
          stunt.iBulge                          = std::stoi(lineAy[9]);
          if (iGeometryIndex < m_chunkAy.size()) {
            memcpy(&m_chunkAy[iGeometryIndex].stunt, &stunt, sizeof(stunt));
          }
        } else {
          assert(0);
          //TODO: logging
          //g_pMainWindow->LogMessage("Error loading file: stunts section ended before anticipated");
          bSuccess = false;
        }
        break;
      case TEXTURE:
        if (lineAy.size() == 0) {
            //do nothing
        } else if (lineAy.size() == 1) {
          if (lineAy[0].compare("-1") == 0) {
            //texture section always ends with a single -1 value
            section = TRACK_NUM;
          } else {
            //texture and building files
            size_t pos = lineAy[0].find(':');
            if (pos != std::string::npos) {
              std::string sTextureType = lineAy[0].substr(0, pos);
              std::string sTextureFile = lineAy[0].substr(pos + 1, lineAy[0].size() - 1);
              if (sTextureType.compare("TEX") == 0) {
                m_sTextureFile = sTextureFile;
              } else if (sTextureType.compare("BLD") == 0) {
                m_sBuildingFile = sTextureFile;
              }
            }
          }
        } else if (lineAy.size() == BACKS_COUNT) {
          //process backs
          int iGeometryIndex = std::stoi(lineAy[0]);
          if (iGeometryIndex < m_chunkAy.size()) {
            m_chunkAy[iGeometryIndex].iBackTexture = std::stoi(lineAy[1]);
          }
        } else {
          assert(0);
          //TODO: logging
          //g_pMainWindow->LogMessage("Error loading file: texture section ended before anticipated");
          bSuccess = false;
        }
        break;
      case TRACK_NUM:
        if (lineAy.size() == 1 && IsNumber(lineAy[0])) {
          m_raceInfo.iTrackNumber = std::stoi(lineAy[0]);
          section = LAPS;
        }
        break;
      case LAPS:
        if (lineAy.size() == LAPS_COUNT) {
          m_raceInfo.iImpossibleLaps  = std::stoi(lineAy[0]);
          m_raceInfo.iHardLaps        = std::stoi(lineAy[1]);
          m_raceInfo.iTrickyLaps      = std::stoi(lineAy[2]);
          m_raceInfo.iMediumLaps      = std::stoi(lineAy[3]);
          m_raceInfo.iEasyLaps        = std::stoi(lineAy[4]);
          m_raceInfo.iGirlieLaps      = std::stoi(lineAy[5]);
          section = MAP;
        }
        break;
      case MAP:
        if (lineAy.size() == MAP_COUNT) {
          m_raceInfo.dTrackMapSize      = std::stod(lineAy[0]);
          m_raceInfo.iTrackMapFidelity  = std::stoi(lineAy[1]);
          m_raceInfo.dUnknown           = std::stod(lineAy[2]);
        }
        break;
    }
  }

  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

unsigned int CTrackData::GetSignedBitValueFromInt(int iValue)
{
  bool bNegative = iValue < 0;
  unsigned int uiRetVal = (unsigned int)abs(iValue);
  if (bNegative)
    uiRetVal |= 0x80000000;
  return uiRetVal;
}

//-------------------------------------------------------------------------------------------------

int CTrackData::GetIntValueFromSignedBit(unsigned int uiValue)
{
  bool bNegative = uiValue & 0x80000000;
  uiValue &= ~0x80000000;
  int iRetVal = (int)uiValue;
  if (bNegative)
    iRetVal = iRetVal * -1;
  return iRetVal;
}

//-------------------------------------------------------------------------------------------------

tShapeData CTrackData::MakeTrackCenterline()
{
  tShapeData ret;

  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVertsCenterline(uiNumVerts);
  uint32 uiNumIndices;
  uint32 *indices = MakeIndicesCenterline(uiNumIndices);

  ret.pVertexBuf = new CVertexBuffer(vertices, uiNumVerts);
  ret.pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
  ret.pVertexArray = new CVertexArray(ret.pVertexBuf);

  if (vertices)
    delete[] vertices;
  if (indices)
    delete[] indices;

  return ret;
}

//-------------------------------------------------------------------------------------------------

tVertex *CTrackData::MakeVertsCenterline(uint32 &numVertices)
{
  if (m_chunkAy.empty())
    return NULL;

  numVertices = (uint32)m_chunkAy.size();
  tVertex *vertices = new tVertex[numVertices];
  vertices[0].position = glm::vec3();
  vertices[0].color = ShapeGenerator::RandomColor();
  for (uint32 i = 1; i < m_chunkAy.size(); ++i) {
    float fLen = (float)m_chunkAy[i].iLength / 10000.0f;
    glm::vec3 nextChunkBase = glm::vec3(1, 0, 0);

    glm::mat4 yawMat = glm::rotate(glm::radians((float)m_chunkAy[i].dYaw), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 nextChunkYawed = glm::vec3(yawMat * glm::vec4(nextChunkBase, 1.0f));
    glm::vec3 pitchDirecton = glm::cross(nextChunkYawed, glm::vec3(0.0f, -1.0f, 0.0f));

    glm::mat4 pitchMat = glm::rotate(glm::radians((float)m_chunkAy[i].dPitch), pitchDirecton);
    glm::vec3 nextChunkPitched = glm::vec3(pitchMat * glm::vec4(nextChunkYawed, 1.0f));

    glm::mat4 translateMat = glm::translate(vertices[i - 1].position);
    glm::mat4 scaleMat = glm::scale(glm::vec3(fLen, fLen, fLen));
    glm::vec3 nextChunk = glm::vec3(translateMat * scaleMat * glm::vec4(nextChunkPitched, 1.0f));
    vertices[i].position = nextChunk;
    vertices[i].color = ShapeGenerator::RandomColor();
  }

  return vertices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CTrackData::MakeIndicesCenterline(uint32 &numIndices)
{
  if (m_chunkAy.empty())
    return NULL;

  numIndices = (uint32)m_chunkAy.size() * 2;
  uint32 *indices = new uint32[numIndices];
  
  for (uint32 i = 1; i < numIndices + 1; i++) {
    indices[i - 1] = i / 2;
  }
  
  return indices;
}

//-------------------------------------------------------------------------------------------------

void CTrackData::ProcessSign(const std::vector<std::string> &lineAy, eFileSection &section)
{
  //helper function because this process must be done in two places
  int iVal0 = std::stoi(lineAy[0]);
  int iVal1 = std::stoi(lineAy[1]);
  if (iVal0 == -1 || iVal1 == -1) {
    //sign section always ends in two -1 values
    section = STUNTS;
  } else {
    //process sign
    if (iVal0 < m_chunkAy.size()) {
      m_chunkAy[iVal0].iSignTexture = std::stoi(lineAy[1]);
    }
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackData::GetTrackData(std::vector<uint8_t> &data)
{
  //write header
  char szBuf[1024];
  snprintf(szBuf, sizeof(szBuf), " %4d %6d %6d %6d\r\n\r\n\r\n", (int)m_chunkAy.size(), m_header.iHeaderUnk1, m_header.iHeaderUnk2, m_header.iHeaderUnk3);
  WriteToVector(data, szBuf);

  //write chunks
  CSignMap signMap;
  CSignMap backsMap;
  CStuntMap stuntMap;
  for (int i = 0; i < m_chunkAy.size(); ++i) {
    WriteToVector(data, m_chunkAy[i].sString.c_str());
    WriteToVector(data, "\r\n");
    if (m_chunkAy[i].iSignTexture >= 0) {
      signMap[i] = m_chunkAy[i].iSignTexture;
    }
    if (m_chunkAy[i].iBackTexture >= 0) {
      backsMap[i] = m_chunkAy[i].iBackTexture;
    }
    if (m_chunkAy[i].stunt.iScaleFactor != 0
        || m_chunkAy[i].stunt.iAngle != 0
        || m_chunkAy[i].stunt.iUnknown != 0
        || m_chunkAy[i].stunt.iTimingGroup != 0
        || m_chunkAy[i].stunt.iHeight != 0
        || m_chunkAy[i].stunt.iTimeBulging != 0
        || m_chunkAy[i].stunt.iTimeFlat != 0
        || m_chunkAy[i].stunt.iSmallerExpandsLargerContracts != 0
        || m_chunkAy[i].stunt.iBulge != 0) {
      stuntMap[i] = &m_chunkAy[i].stunt;
    }
  }

  //write signs
  for (CSignMap::iterator it = signMap.begin(); it != signMap.end(); ++it) {
    memset(szBuf, 0, sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), " %4d %6d\r\n", it->first, (int)it->second);
    WriteToVector(data, szBuf);
  }
  memset(szBuf, 0, sizeof(szBuf));
  snprintf(szBuf, sizeof(szBuf), " %4d %6d\r\n", -1, -1);
  WriteToVector(data, szBuf);

  //write stunts
  for (CStuntMap::iterator it = stuntMap.begin(); it != stuntMap.end(); ++it) {
    memset(szBuf, 0, sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), " %4d %6d %6d %6d %6d %6d %6d %6d %6d %6d\r\n",
             it->first, it->second->iScaleFactor, it->second->iAngle, it->second->iUnknown,
             it->second->iTimingGroup, it->second->iHeight, it->second->iTimeBulging,
             it->second->iTimeFlat, it->second->iSmallerExpandsLargerContracts, it->second->iBulge);
    WriteToVector(data, szBuf);
  }
  WriteToVector(data, "\r\n");
  memset(szBuf, 0, sizeof(szBuf));
  snprintf(szBuf, sizeof(szBuf), " %4d\r\n\r\n", -1);
  WriteToVector(data, szBuf);

  //write textures
  WriteToVector(data, "TEX:");
  WriteToVector(data, m_sTextureFile.c_str());
  WriteToVector(data, "\r\n");
  WriteToVector(data, "BLD:");
  WriteToVector(data, m_sBuildingFile.c_str());
  WriteToVector(data, "\r\n");
  WriteToVector(data, "BACKS:");
  WriteToVector(data, "\r\n");
  for (CSignMap::iterator it = backsMap.begin(); it != backsMap.end(); ++it) {
    memset(szBuf, 0, sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), "%d %d\r\n", it->first, (int)it->second);
    WriteToVector(data, szBuf);
  }
  memset(szBuf, 0, sizeof(szBuf));
  snprintf(szBuf, sizeof(szBuf), " %4d\r\n\r\n", -1);
  WriteToVector(data, szBuf);

  //write info
  if (!(m_raceInfo.iTrackNumber == 0
        && m_raceInfo.iImpossibleLaps == 0
        && m_raceInfo.iHardLaps == 0
        && m_raceInfo.iTrickyLaps == 0
        && m_raceInfo.iMediumLaps == 0
        && m_raceInfo.iEasyLaps == 0
        && m_raceInfo.iGirlieLaps == 0
        && m_raceInfo.dTrackMapSize == 0
        && m_raceInfo.iTrackMapFidelity == 0
        && m_raceInfo.dUnknown == 0)) {

    memset(szBuf, 0, sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), "%d\r\n", m_raceInfo.iTrackNumber);
    WriteToVector(data, szBuf);
    memset(szBuf, 0, sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), "%4d %4d %4d %4d %4d %4d\r\n",
             m_raceInfo.iImpossibleLaps, m_raceInfo.iHardLaps, m_raceInfo.iTrickyLaps,
             m_raceInfo.iMediumLaps, m_raceInfo.iEasyLaps, m_raceInfo.iGirlieLaps);
    WriteToVector(data, szBuf);
    memset(szBuf, 0, sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), "%.2lf %4d %.2lf\r\n\r\n",
             m_raceInfo.dTrackMapSize, m_raceInfo.iTrackMapFidelity, m_raceInfo.dUnknown);
    WriteToVector(data, szBuf);
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackData::WriteToVector(std::vector<uint8_t> &data, const char *szText)
{
  int iLength = (int)strlen(szText);
  for (int i = 0; i < iLength; ++i) {
    uint8_t val = (uint8_t)szText[i];
    data.push_back(val);
  }
}

//-------------------------------------------------------------------------------------------------