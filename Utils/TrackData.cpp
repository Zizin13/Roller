#include "TrackData.h"
#include "Unmangler.h"
#include <assert.h>
#include <fstream>
#include <sstream>
#include "ShapeGenerator.h"
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
#include "Types.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
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
  iLUOuterWallHOffset = 0;
  iLLOuterWallHOffset = 0;
  iLOuterFloorHOffset = 0;
  iROuterFloorHOffset = 0;
  iRLOuterWallHOffset = 0;
  iRUOuterWallHOffset = 0;
  iLUOuterWallHeight = 0;
  iLLOuterWallHeight = 0;
  iLOuterFloorHeight = 0;
  iROuterFloorHeight = 0;
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
  : m_fScale(10000.0f)
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
  m_header.iFloorDepth = 2048;
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
  if (!file.is_open()) {
    //todo logging
    return false;
  }

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
          m_header.iFloorDepth = std::stoi(lineAy[3]);
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
          currChunk.iLUOuterWallHOffset   = std::stoi(lineAy[0]);
          currChunk.iLLOuterWallHOffset   = std::stoi(lineAy[1]);
          currChunk.iLOuterFloorHOffset              = std::stoi(lineAy[2]);
          currChunk.iROuterFloorHOffset              = std::stoi(lineAy[3]);
          currChunk.iRLOuterWallHOffset   = std::stoi(lineAy[4]);
          currChunk.iRUOuterWallHOffset   = std::stoi(lineAy[5]);
          currChunk.iLUOuterWallHeight  = std::stoi(lineAy[6]);
          currChunk.iLLOuterWallHeight  = std::stoi(lineAy[7]);
          currChunk.iLOuterFloorHeight              = std::stoi(lineAy[8]);
          currChunk.iROuterFloorHeight              = std::stoi(lineAy[9]);
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

CShapeData *CTrackData::MakeTrackCenterline(CShader *pShader)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVerts(uiNumVerts, eShapeSection::CENTER);
  uint32 uiNumIndices;
  uint32 *indices = MakeIndicesCenterline(uiNumIndices);

  CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts);
  CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
  CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

  CShapeData *pRet = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, NULL, GL_LINES);

  if (vertices)
    delete[] vertices;
  if (indices)
    delete[] indices;

  return pRet;
}

//-------------------------------------------------------------------------------------------------

uint32 *CTrackData::MakeIndicesCenterline(uint32 &numIndices)
{
  if (m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  numIndices = (uint32)m_chunkAy.size() * 2;
  uint32 *indices = new uint32[numIndices];
  
  for (uint32 i = 1; i < numIndices + 1; i++) {
    indices[i - 1] = i / 2;
  }
  
  return indices;
}
uint32 *CTrackData::MakeIndicesSurface(uint32 &numIndices)
{
  if (m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 5;
  uint32 uiNumIndicesPerChunk = 24;
  numIndices = (uint32)m_chunkAy.size() * uiNumIndicesPerChunk;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  uint32 i = 0;
  for (; i < m_chunkAy.size() - 1; i++) {
    indices[i * uiNumIndicesPerChunk + 0] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 1] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 2] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 3] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 4] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 5] = (i * uiNumVertsPerChunk) + 8;
    indices[i * uiNumIndicesPerChunk + 6] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 7] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 8] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 9] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 10] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 11] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 12] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 13] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 14] = (i * uiNumVertsPerChunk) + 7;
    indices[i * uiNumIndicesPerChunk + 15] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 16] = (i * uiNumVertsPerChunk) + 7;
    indices[i * uiNumIndicesPerChunk + 17] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 18] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 19] = (i * uiNumVertsPerChunk) + 4;
    indices[i * uiNumIndicesPerChunk + 20] = (i * uiNumVertsPerChunk) + 9;
    indices[i * uiNumIndicesPerChunk + 21] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 22] = (i * uiNumVertsPerChunk) + 9;
    indices[i * uiNumIndicesPerChunk + 23] = (i * uiNumVertsPerChunk) + 7;
  }
  //final chunk must be tied to first
  indices[i * uiNumIndicesPerChunk + 0] = (i * uiNumVertsPerChunk) + 3;
  indices[i * uiNumIndicesPerChunk + 1] = (i * uiNumVertsPerChunk) + 1;
  indices[i * uiNumIndicesPerChunk + 2] = 1;
  indices[i * uiNumIndicesPerChunk + 3] = (i * uiNumVertsPerChunk) + 3;
  indices[i * uiNumIndicesPerChunk + 4] = 1;
  indices[i * uiNumIndicesPerChunk + 5] = 3;
  indices[i * uiNumIndicesPerChunk + 6] = (i * uiNumVertsPerChunk) + 1;
  indices[i * uiNumIndicesPerChunk + 7] = (i * uiNumVertsPerChunk) + 0;
  indices[i * uiNumIndicesPerChunk + 8] = 0;
  indices[i * uiNumIndicesPerChunk + 9] = (i * uiNumVertsPerChunk) + 1;
  indices[i * uiNumIndicesPerChunk + 10] = 0;
  indices[i * uiNumIndicesPerChunk + 11] = 1;
  indices[i * uiNumIndicesPerChunk + 12] = (i * uiNumVertsPerChunk) + 0;
  indices[i * uiNumIndicesPerChunk + 13] = (i * uiNumVertsPerChunk) + 2;
  indices[i * uiNumIndicesPerChunk + 14] = 2;
  indices[i * uiNumIndicesPerChunk + 15] = (i * uiNumVertsPerChunk) + 0;
  indices[i * uiNumIndicesPerChunk + 16] = 2;
  indices[i * uiNumIndicesPerChunk + 17] = 0;
  indices[i * uiNumIndicesPerChunk + 18] = (i * uiNumVertsPerChunk) + 2;
  indices[i * uiNumIndicesPerChunk + 19] = (i * uiNumVertsPerChunk) + 4;
  indices[i * uiNumIndicesPerChunk + 20] = 4;
  indices[i * uiNumIndicesPerChunk + 21] = (i * uiNumVertsPerChunk) + 2;
  indices[i * uiNumIndicesPerChunk + 22] = 4;
  indices[i * uiNumIndicesPerChunk + 23] = 2;

  return indices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CTrackData::MakeIndicesSurfaceWireframe(uint32 &numIndices)
{
  if (m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 5;
  uint32 uiNumIndicesPerChunk = 18;
  numIndices = (uint32)m_chunkAy.size() * uiNumIndicesPerChunk;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  uint32 i = 0;
  for (; i < m_chunkAy.size() - 1; i++) {
    indices[i * uiNumIndicesPerChunk + 0] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 1] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 2] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 3] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 4] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 5] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 6] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 7] = (i * uiNumVertsPerChunk) + 4;

    indices[i * uiNumIndicesPerChunk + 8] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 9] = (i * uiNumVertsPerChunk) + 8;
    indices[i * uiNumIndicesPerChunk + 10] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 11] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 12] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 13] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 14] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 15] = (i * uiNumVertsPerChunk) + 7;
    indices[i * uiNumIndicesPerChunk + 16] = (i * uiNumVertsPerChunk) + 4;
    indices[i * uiNumIndicesPerChunk + 17] = (i * uiNumVertsPerChunk) + 9;
  }
  //final chunk must be tied to first
  indices[i * uiNumIndicesPerChunk + 8] = (i * uiNumVertsPerChunk) + 3;
  indices[i * uiNumIndicesPerChunk + 9] = 3;
  indices[i * uiNumIndicesPerChunk + 10] = (i * uiNumVertsPerChunk) + 1;
  indices[i * uiNumIndicesPerChunk + 11] = 1;
  indices[i * uiNumIndicesPerChunk + 12] = (i * uiNumVertsPerChunk) + 0;
  indices[i * uiNumIndicesPerChunk + 13] = 0;
  indices[i * uiNumIndicesPerChunk + 14] = (i * uiNumVertsPerChunk) + 2;
  indices[i * uiNumIndicesPerChunk + 15] = 2;
  indices[i * uiNumIndicesPerChunk + 16] = (i * uiNumVertsPerChunk) + 4;
  indices[i * uiNumIndicesPerChunk + 17] = 4;

  return indices;
}

//-------------------------------------------------------------------------------------------------

CShapeData *CTrackData::MakeTrackSurface(CShader *pShader, eShapeSection section, bool bWireframe)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = NULL;
  if (section != eShapeSection::ENVIRFLOOR)
    vertices = MakeVerts(uiNumVerts, section);
  else
    vertices = MakeVertsEnvirFloor(uiNumVerts);
  uint32 uiNumIndices;
  uint32 *indices = NULL;
  GLenum drawType = GL_TRIANGLES;
  if (!bWireframe) {
    if (section == DRIVING_SURFACE)     
      indices = MakeIndicesSurface(uiNumIndices);
    else
      indices = MakeIndicesSingleSection(uiNumIndices, section);
  } else {
    indices = MakeIndicesSingleSectionWireframe(uiNumIndices);
    drawType = GL_LINES;

    for (uint32 i = 0; i < uiNumVerts; ++i) {
      vertices[i].flags.x = 1.0f; //use color rather than tex
      vertices[i].color = ShapeGenerator::RandomColor();
    }
  }

  CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts);
  CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
  CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

  CShapeData *pRet = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, &m_tex, drawType);

  if (vertices)
    delete[] vertices;
  if (indices)
    delete[] indices;

  return pRet;
}
//-------------------------------------------------------------------------------------------------

CShapeData *CTrackData::MakeSelectedChunks(CShader *pShader, int iStart, int iEnd)
{
  uint32 uiNumVerts;
  struct tVertex *vertices = MakeVerts(uiNumVerts, eShapeSection::SELECTED);
  uint32 uiNumIndices;
  uint32 *indices = MakeIndicesSelectedChunks(uiNumIndices, iStart, iEnd);
  GLenum drawType = GL_LINES;

  for (uint32 i = 0; i < uiNumVerts; ++i) {
    vertices[i].flags.x = 1.0f; //use color rather than tex
    vertices[i].color = ShapeGenerator::RandomColor();
  }

  CVertexBuffer *pVertexBuf = new CVertexBuffer(vertices, uiNumVerts);
  CIndexBuffer *pIndexBuf = new CIndexBuffer(indices, uiNumIndices);
  CVertexArray *pVertexArray = new CVertexArray(pVertexBuf);

  CShapeData *pRet = new CShapeData(pVertexBuf, pIndexBuf, pVertexArray, pShader, &m_tex, drawType);

  if (vertices)
    delete[] vertices;
  if (indices)
    delete[] indices;

  return pRet;
}

//-------------------------------------------------------------------------------------------------

tVertex *CTrackData::MakeVerts(uint32 &numVertices, eShapeSection section)
{
  if (m_chunkAy.empty()) {
    numVertices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 4;
  if (section == eShapeSection::SELECTED) uiNumVertsPerChunk = 8;

  numVertices = (uint32)m_chunkAy.size() * uiNumVertsPerChunk;
  tVertex *vertices = new tVertex[numVertices];
  glm::vec3 prevCenter = glm::vec3(0, 0, 1);
  glm::vec3 prevLLane = glm::vec3(0, 0, 1);
  glm::vec3 prevRLane = glm::vec3(0, 0, 1);
  glm::vec3 prevLShoulder = glm::vec3(0, 0, 1);
  glm::vec3 prevRShoulder = glm::vec3(0, 0, 1);
  glm::vec3 prevLWall = glm::vec3(0, 0, 1);
  glm::vec3 prevRWall = glm::vec3(0, 0, 1);
  glm::vec3 prevLWallBottomAttach = glm::vec3(0, 0, 1);
  glm::vec3 prevRWallBottomAttach = glm::vec3(0, 0, 1);
  bool bPrevLWallAttachToLane = false;
  bool bPrevRWallAttachToLane = false;
  glm::vec3 prevLFloor = glm::vec3(0, 0, 1);
  glm::vec3 prevRFloor = glm::vec3(0, 0, 1);
  glm::vec3 prevLLOWall = glm::vec3(0, 0, 1);
  glm::vec3 prevRLOWall = glm::vec3(0, 0, 1);
  glm::vec3 prevLLOWallBottomAttach = glm::vec3(0, 0, 1);
  glm::vec3 prevRLOWallBottomAttach = glm::vec3(0, 0, 1);
  glm::vec3 prevLUOWall = glm::vec3(0, 0, 1);
  glm::vec3 prevRUOWall = glm::vec3(0, 0, 1);
  glm::vec3 prevLUOWallBottomAttach = glm::vec3(0, 0, 1);
  glm::vec3 prevRUOWallBottomAttach = glm::vec3(0, 0, 1);
  for (uint32 i = 0; i < m_chunkAy.size(); ++i) {
    glm::vec3 center;
    glm::vec3 pitchAxis;
    glm::vec3 nextChunkPitched;
    glm::mat4 rollMat;
    glm::mat4 rollMatNoRoll = glm::mat4(1);
    GetCenter(i, prevCenter, center, pitchAxis, nextChunkPitched, rollMat);

    int iChunkIndex = (int)m_chunkAy.size() - 1;
    if (i > 0)
      iChunkIndex = i - 1;

    //left lane
    glm::vec3 lLane;
    GetLane(i, center, pitchAxis, rollMat, lLane, true);
    //right lane
    glm::vec3 rLane;
    GetLane(i, center, pitchAxis, rollMat, rLane, false);
    //left shoulder
    glm::vec3 lShoulder;
    GetShoulder(i, lLane, pitchAxis, rollMat, nextChunkPitched, lShoulder, true);
    //right shoulder
    glm::vec3 rShoulder;
    GetShoulder(i, rLane, pitchAxis, rollMat, nextChunkPitched, rShoulder, false);
    //left wall
    bool bLWallAttachToLane = GetSignedBitValueFromInt(m_chunkAy[i].iLeftWallType) & SURFACE_FLAG_WALL_31;
    if (m_chunkAy[i].iLeftWallType == -1)
      bLWallAttachToLane = bPrevLWallAttachToLane;
    bPrevLWallAttachToLane = bLWallAttachToLane;
    glm::vec3 lWallBottomAttach = bLWallAttachToLane ? lLane : lShoulder;
    glm::vec3 lWall;
    GetWall(i, lWallBottomAttach, pitchAxis, rollMat, nextChunkPitched, lWall, eShapeSection::LWALL);
    //right wall
    bool bRWallAttachToLane = GetSignedBitValueFromInt(m_chunkAy[i].iRightWallType) & SURFACE_FLAG_WALL_31;
    if (m_chunkAy[i].iRightWallType == -1)
      bRWallAttachToLane = bPrevRWallAttachToLane;
    bPrevRWallAttachToLane = bRWallAttachToLane;
    glm::vec3 rWallBottomAttach = bRWallAttachToLane ? rLane : rShoulder;
    glm::vec3 rWall;
    GetWall(i, rWallBottomAttach, pitchAxis, rollMat, nextChunkPitched, rWall, eShapeSection::RWALL);
    //outer floor
    glm::vec3 lLaneNoRoll;
    GetLane(i, center, pitchAxis, rollMatNoRoll, lLaneNoRoll, true);
    glm::vec3 rLaneNoRoll;
    GetLane(i, center, pitchAxis, rollMatNoRoll, rLaneNoRoll, false);
    glm::vec3 lFloor;
    glm::vec3 rFloor;
    GetOWallFloor(i, lLaneNoRoll, rLaneNoRoll, pitchAxis, nextChunkPitched, lFloor, rFloor);
    //outer wall roll mat
    glm::mat4 oWallRollMat = m_chunkAy[i].iOuterFloorType < 0 ? rollMat : rollMatNoRoll;
    //llowall
    bool bLLOWallNonSolid = GetSignedBitValueFromInt(m_chunkAy[i].iLLOuterWallType) & SURFACE_FLAG_NON_SOLID;
    glm::vec3 lloWallBottomAttach = lFloor;
    if (m_chunkAy[i].iOuterFloorType < 0) {
      if (bLLOWallNonSolid) {
        glm::vec3 lShoulderNoHeight;
        GetShoulder(i, lLane, pitchAxis, rollMat, nextChunkPitched, lShoulderNoHeight, true, true);
        lloWallBottomAttach = lShoulderNoHeight;
      } else {
        lloWallBottomAttach = lShoulder;
      }
    }
    glm::vec3 lloWall;
    GetWall(i, lloWallBottomAttach, pitchAxis, oWallRollMat, nextChunkPitched, lloWall, eShapeSection::LLOWALL);
    //rlowall
    bool bRLOWallNonSolid = GetSignedBitValueFromInt(m_chunkAy[i].iRLOuterWallType) & SURFACE_FLAG_NON_SOLID;
    glm::vec3 rloWallBottomAttach = rFloor;
    if (m_chunkAy[i].iOuterFloorType < 0) {
      if (bRLOWallNonSolid) {
        glm::vec3 rShoulderNoHeight;
        GetShoulder(i, rLane, pitchAxis, rollMat, nextChunkPitched, rShoulderNoHeight, false, true);
        rloWallBottomAttach = rShoulderNoHeight;
      } else {
        rloWallBottomAttach = rShoulder;
      }
    }
    glm::vec3 rloWall;
    GetWall(i, rloWallBottomAttach, pitchAxis, oWallRollMat, nextChunkPitched, rloWall, eShapeSection::RLOWALL);
    //luowall
    glm::vec3 luoWall;
    glm::vec3 luoWallBottomAttach = lloWall;
    GetWall(i, luoWallBottomAttach, pitchAxis, oWallRollMat, nextChunkPitched, luoWall, eShapeSection::LUOWALL);
    //ruowall
    glm::vec3 ruoWall;
    glm::vec3 ruoWallBottomAttach = rloWall;
    GetWall(i, ruoWallBottomAttach, pitchAxis, oWallRollMat, nextChunkPitched, ruoWall, eShapeSection::RUOWALL);

    switch (section) {
      case LLANE:
        ApplyVerticesSingleSection(i, vertices, lLane, center, prevLLane, prevCenter);
        GetTextureCoordinates(GetSignedBitValueFromInt(m_chunkAy[iChunkIndex].iCenterSurfaceType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3], true, false);
        break;
      case RLANE:
        ApplyVerticesSingleSection(i, vertices, center, rLane, prevCenter, prevRLane);
        GetTextureCoordinates(GetSignedBitValueFromInt(m_chunkAy[iChunkIndex].iCenterSurfaceType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3], false, true);
        break;
      case LSHOULDER:
        ApplyVerticesSingleSection(i, vertices, lShoulder, lLane, prevLShoulder, prevLLane);
        GetTextureCoordinates(GetSignedBitValueFromInt(m_chunkAy[iChunkIndex].iLeftSurfaceType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3]);
        break;
      case RSHOULDER:
        ApplyVerticesSingleSection(i, vertices, rLane, rShoulder, prevRLane, prevRShoulder);
        GetTextureCoordinates(GetSignedBitValueFromInt(m_chunkAy[iChunkIndex].iRightSurfaceType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3]);
        break;
      case LWALL:
        ApplyVerticesSingleSection(i, vertices, lWall, lWallBottomAttach, prevLWall, prevLWallBottomAttach);
        GetTextureCoordinates(GetSignedBitValueFromInt(m_chunkAy[iChunkIndex].iLeftWallType),
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 1]);
        break;
      case RWALL:
        ApplyVerticesSingleSection(i, vertices, rWallBottomAttach, rWall, prevRWallBottomAttach, prevRWall);
        GetTextureCoordinates(GetSignedBitValueFromInt(m_chunkAy[iChunkIndex].iRightWallType),
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 2]);
        break;
      case ROOF:
        ApplyVerticesSingleSection(i, vertices, rWall, lWall, prevRWall, prevLWall);
        GetTextureCoordinates(GetSignedBitValueFromInt(m_chunkAy[iChunkIndex].iRoofType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3]);
        break;
      case OWALLFLOOR:
        ApplyVerticesSingleSection(i, vertices, lFloor, rFloor, prevLFloor, prevRFloor);
        GetTextureCoordinates(GetSignedBitValueFromInt(m_chunkAy[iChunkIndex].iOuterFloorType),
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 1],
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 3]);
        break;
      case LLOWALL:
        ApplyVerticesSingleSection(i, vertices, lloWall, lloWallBottomAttach, prevLLOWall, prevLLOWallBottomAttach);
        GetTextureCoordinates(GetSignedBitValueFromInt(m_chunkAy[iChunkIndex].iLLOuterWallType),
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 1]);
        break;
      case RLOWALL:
        ApplyVerticesSingleSection(i, vertices, rloWallBottomAttach, rloWall, prevRLOWallBottomAttach, prevRLOWall);
        GetTextureCoordinates(GetSignedBitValueFromInt(m_chunkAy[iChunkIndex].iRLOuterWallType),
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 1]);
        break;
      case LUOWALL:
        ApplyVerticesSingleSection(i, vertices, luoWall, luoWallBottomAttach, prevLUOWall, prevLUOWallBottomAttach);
        GetTextureCoordinates(GetSignedBitValueFromInt(m_chunkAy[iChunkIndex].iLUOuterWallType),
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 1]);
        break;
      case RUOWALL:
        ApplyVerticesSingleSection(i, vertices, ruoWallBottomAttach, ruoWall, prevRUOWallBottomAttach, prevRUOWall);
        GetTextureCoordinates(GetSignedBitValueFromInt(m_chunkAy[iChunkIndex].iRUOuterWallType),
                              vertices[i * uiNumVertsPerChunk + 2],
                              vertices[i * uiNumVertsPerChunk + 0],
                              vertices[i * uiNumVertsPerChunk + 3],
                              vertices[i * uiNumVertsPerChunk + 1]);
        break;
      case SELECTED:
        vertices[i * uiNumVertsPerChunk + 0].position = luoWall;
        vertices[i * uiNumVertsPerChunk + 1].position = ruoWall;
        vertices[i * uiNumVertsPerChunk + 2].position = lloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + 3].position = rloWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + 4].position = prevLUOWall;
        vertices[i * uiNumVertsPerChunk + 5].position = prevRUOWall;
        vertices[i * uiNumVertsPerChunk + 6].position = prevLLOWallBottomAttach;
        vertices[i * uiNumVertsPerChunk + 7].position = prevRLOWallBottomAttach;
        break;
      default:
        assert(0); //shape not implemented
        break;
    }

    prevCenter = center;
    prevLLane = lLane;
    prevRLane = rLane;
    prevLShoulder = lShoulder;
    prevRShoulder = rShoulder;
    prevLWall = lWall;
    prevRWall = rWall;
    prevLWallBottomAttach = lWallBottomAttach;
    prevRWallBottomAttach = rWallBottomAttach;
    prevLFloor = lFloor;
    prevRFloor = rFloor;
    prevLLOWall = lloWall;
    prevRLOWall = rloWall;
    prevLLOWallBottomAttach = lloWallBottomAttach;
    prevRLOWallBottomAttach = rloWallBottomAttach;
    prevLUOWall = luoWall;
    prevRUOWall = ruoWall;
    prevLUOWallBottomAttach = luoWallBottomAttach;
    prevRUOWallBottomAttach = ruoWallBottomAttach;
  }

  //attach final chunk
  switch (section) {
    case LLANE:
      vertices[3].position = prevCenter;
      vertices[2].position = prevLLane;
      break;
    case RLANE:
      vertices[2].position = prevCenter;
      vertices[3].position = prevRLane;
      break;
    case LSHOULDER:
      vertices[3].position = prevLLane;
      vertices[2].position = prevLShoulder;
      break;
    case RSHOULDER:
      vertices[2].position = prevRLane;
      vertices[3].position = prevRShoulder;
      break;
    case LWALL:
      vertices[3].position = prevLWallBottomAttach;
      vertices[2].position = prevLWall;
      break;
    case RWALL:
      vertices[2].position = prevRWallBottomAttach;
      vertices[3].position = prevRWall;
      break;
    case ROOF:
      vertices[3].position = prevLWall;
      vertices[2].position = prevRWall;
      break;
    case OWALLFLOOR:
      vertices[2].position = prevLFloor;
      vertices[3].position = prevRFloor;
      break;
    case LLOWALL:
      vertices[3].position = prevLLOWallBottomAttach;
      vertices[2].position = prevLLOWall;
      break;
    case RLOWALL:
      vertices[2].position = prevRLOWallBottomAttach;
      vertices[3].position = prevRLOWall;
      break;
    case LUOWALL:
      vertices[3].position = prevLLOWall;
      vertices[2].position = prevLUOWall;
      break;
    case RUOWALL:
      vertices[2].position = prevRLOWall;
      vertices[3].position = prevRUOWall;
      break;
    case SELECTED:
      vertices[4].position = prevLUOWall;
      vertices[5].position = prevRUOWall;
      vertices[6].position = prevLLOWallBottomAttach;
      vertices[7].position = prevRLOWallBottomAttach;
      break;
  }

  return vertices;
}

//-------------------------------------------------------------------------------------------------

tVertex *CTrackData::MakeVertsEnvirFloor(uint32 &numVertices)
{
  if (m_chunkAy.empty()) {
    numVertices = 0;
    return NULL;
  }

  numVertices = 4;
  tVertex *vertices = new tVertex[numVertices];
  glm::vec3 prevCenter = glm::vec3(0, 0, 1);
  float fMinX = 0.0f;
  float fMaxX = 0.0f;
  float fMinZ = 0.0f;
  float fMaxZ = 0.0f;
  for (uint32 i = 0; i < m_chunkAy.size(); ++i) {
    glm::vec3 center;
    glm::vec3 pitchAxis;
    glm::vec3 nextChunkPitched;
    glm::mat4 rollMat;
    GetCenter(i, prevCenter, center, pitchAxis, nextChunkPitched, rollMat);
    if (center.x > fMaxX)
      fMaxX = center.x;
    if (center.x < fMinX)
      fMinX = center.x;
    if (center.z > fMaxZ)
      fMaxZ = center.z;
    if (center.z < fMinZ)
      fMinZ = center.z;

    prevCenter = center;
  }

  float fEnvirFloorDepth = (float)m_header.iFloorDepth / m_fScale * -1.0f;
  float fPadding = 10.0f;
  vertices[0].position = glm::vec3(fMinX - fPadding, fEnvirFloorDepth, fMinZ - fPadding);
  vertices[1].position = glm::vec3(fMinX - fPadding, fEnvirFloorDepth, fMaxZ + fPadding);
  vertices[2].position = glm::vec3(fMaxX + fPadding, fEnvirFloorDepth, fMinZ - fPadding);
  vertices[3].position = glm::vec3(fMaxX + fPadding, fEnvirFloorDepth, fMaxZ + fPadding);
  vertices[0].color = glm::vec3(0.235f, 0.317f, 0.157f);
  vertices[1].color = glm::vec3(0.235f, 0.317f, 0.157f);
  vertices[2].color = glm::vec3(0.235f, 0.317f, 0.157f);
  vertices[3].color = glm::vec3(0.235f, 0.317f, 0.157f);
  vertices[0].flags.x = 1.0f;
  vertices[1].flags.x = 1.0f;
  vertices[2].flags.x = 1.0f;
  vertices[3].flags.x = 1.0f;

  return vertices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CTrackData::MakeIndicesSingleSection(uint32 &numIndices, eShapeSection section)
{
  if (m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 4;
  uint32 uiNumIndicesPerChunk = 6;
  numIndices = (uint32)m_chunkAy.size() * uiNumIndicesPerChunk;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  uint32 i = 0;
  for (; i < m_chunkAy.size(); i++) {
    //if (section == eShapeSection::LWALL || section == eShapeSection::RWALL) {
    //  //walls don't draw if forward section isn't drawn
    //  if (!ShouldMakeIndicesForChunk(i, section))
    //    continue;
    //}
    if (i > 0 && !ShouldMakeIndicesForChunk(i - 1, section))
      continue;
    else if (i == 0 && !ShouldMakeIndicesForChunk(((int)m_chunkAy.size() - 1), section))
      continue;
    //else if (i == 0 && !ShouldMakeIndicesForChunk(((int)m_chunkAy.size() - (int)i + 1), section))
    //  continue;
    indices[i * uiNumIndicesPerChunk + 0] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 1] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 2] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 3] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 4] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 5] = (i * uiNumVertsPerChunk) + 0;
  }

  return indices;
}

//-------------------------------------------------------------------------------------------------

uint32 *CTrackData::MakeIndicesSelectedChunks(uint32 &numIndices, int iStart, int iEnd)
{
  if (m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 8;
  uint32 uiNumIndicesPerChunk = 24;
  numIndices = (uint32)m_chunkAy.size() * uiNumIndicesPerChunk;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  if (iStart >= m_chunkAy.size() - 1)
    iStart = 0;
  else
    iStart++;
  if (iEnd >= m_chunkAy.size() - 1)
    iEnd = 0;
  else
    iEnd++;

  int i = iStart;
  for (; i <= iEnd; i++) {
    indices[i * uiNumIndicesPerChunk + 0]  = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 1]  = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 2]  = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 3]  = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 4]  = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 5]  = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 6]  = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 7]  = (i * uiNumVertsPerChunk) + 0;

    indices[i * uiNumIndicesPerChunk + 8]  = (i * uiNumVertsPerChunk) + 4;
    indices[i * uiNumIndicesPerChunk + 9]  = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 10] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 11] = (i * uiNumVertsPerChunk) + 7;
    indices[i * uiNumIndicesPerChunk + 12] = (i * uiNumVertsPerChunk) + 7;
    indices[i * uiNumIndicesPerChunk + 13] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 14] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 15] = (i * uiNumVertsPerChunk) + 4;

    indices[i * uiNumIndicesPerChunk + 16] = (i * uiNumVertsPerChunk) + 4;
    indices[i * uiNumIndicesPerChunk + 17] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 18] = (i * uiNumVertsPerChunk) + 6;
    indices[i * uiNumIndicesPerChunk + 19] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 20] = (i * uiNumVertsPerChunk) + 7;
    indices[i * uiNumIndicesPerChunk + 21] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 22] = (i * uiNumVertsPerChunk) + 5;
    indices[i * uiNumIndicesPerChunk + 23] = (i * uiNumVertsPerChunk) + 1;
  }

  return indices;
}

//-------------------------------------------------------------------------------------------------

bool CTrackData::ShouldMakeIndicesForChunk(int i, eShapeSection section)
{
  if ((section == eShapeSection::LLANE || section == eShapeSection::RLANE)
      && !ShouldDrawSurfaceType(m_chunkAy[i].iCenterSurfaceType))
    return false;
  if (section == eShapeSection::LSHOULDER
      && !ShouldDrawSurfaceType(m_chunkAy[i].iLeftSurfaceType))
    return false;
  if (section == eShapeSection::RSHOULDER
      && !ShouldDrawSurfaceType(m_chunkAy[i].iRightSurfaceType))
    return false;
  if (section == eShapeSection::LWALL 
      && !ShouldDrawSurfaceType(m_chunkAy[i].iLeftWallType))
    return false;
  if (section == eShapeSection::RWALL
      && !ShouldDrawSurfaceType(m_chunkAy[i].iRightWallType))
    return false;
  if (section == eShapeSection::ROOF
      && (!ShouldDrawSurfaceType(m_chunkAy[i].iRoofType)
          || m_chunkAy[i].iLeftWallType == -1
          || m_chunkAy[i].iRightWallType == -1))
    return false;
  if (section == eShapeSection::ENVIRFLOOR
      && !ShouldDrawSurfaceType(m_chunkAy[i].iEnvironmentFloorType))
    return false;
  if (section == eShapeSection::OWALLFLOOR
      && (m_chunkAy[i].iOuterFloorType == -2 
          || !ShouldDrawSurfaceType(m_chunkAy[i].iOuterFloorType)))
    return false;
  if (section == eShapeSection::LLOWALL
      && (!ShouldDrawSurfaceType(m_chunkAy[i].iLLOuterWallType)
          || m_chunkAy[i].iOuterFloorType == -1))
    return false;
  if (section == eShapeSection::RLOWALL
      && (!ShouldDrawSurfaceType(m_chunkAy[i].iRLOuterWallType)
          || m_chunkAy[i].iOuterFloorType == -1))
    return false;
  if (section == eShapeSection::LUOWALL
      && (!ShouldDrawSurfaceType(m_chunkAy[i].iLUOuterWallType)
          || m_chunkAy[i].iOuterFloorType == -1
          || m_chunkAy[i].iLLOuterWallType == -1))
    return false;
  if (section == eShapeSection::RUOWALL
      && (!ShouldDrawSurfaceType(m_chunkAy[i].iRUOuterWallType)
          || m_chunkAy[i].iOuterFloorType == -1
          || m_chunkAy[i].iRLOuterWallType == -1))
    return false;
  return true;
}

//-------------------------------------------------------------------------------------------------

bool CTrackData::ShouldDrawSurfaceType(int iSurfaceType)
{
  if (iSurfaceType == -1)
    return false;
  uint32 uiSurfaceType = GetSignedBitValueFromInt(iSurfaceType);
  if (uiSurfaceType & SURFACE_FLAG_NON_SOLID)
    return false;
  if (!(uiSurfaceType & SURFACE_FLAG_TRANSPARENT)
      && !(uiSurfaceType & SURFACE_FLAG_APPLY_TEXTURE))
    return false;
  return true;
}

//-------------------------------------------------------------------------------------------------

uint32 *CTrackData::MakeIndicesSingleSectionWireframe(uint32 &numIndices)
{
  if (m_chunkAy.empty()) {
    numIndices = 0;
    return NULL;
  }

  uint32 uiNumVertsPerChunk = 4;
  uint32 uiNumIndicesPerChunk = 8;
  numIndices = (uint32)m_chunkAy.size() * uiNumIndicesPerChunk;
  uint32 *indices = new uint32[numIndices];
  memset(indices, 0, numIndices * sizeof(uint32));

  uint32 i = 0;
  for (; i < m_chunkAy.size(); i++) {
    indices[i * uiNumIndicesPerChunk + 0] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 1] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 2] = (i * uiNumVertsPerChunk) + 3;
    indices[i * uiNumIndicesPerChunk + 3] = (i * uiNumVertsPerChunk) + 1;
    indices[i * uiNumIndicesPerChunk + 4] = (i * uiNumVertsPerChunk) + 2;
    indices[i * uiNumIndicesPerChunk + 5] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 6] = (i * uiNumVertsPerChunk) + 0;
    indices[i * uiNumIndicesPerChunk + 7] = (i * uiNumVertsPerChunk) + 1;
  }

  return indices;
}

//-------------------------------------------------------------------------------------------------

void CTrackData::GetCenter(int i, glm::vec3 prevCenter, 
                           glm::vec3 &center, glm::vec3 &pitchAxis, glm::vec3 &nextChunkPitched, glm::mat4 &rollMat)
{
  glm::vec3 nextChunkBase = glm::vec3(0, 0, 1);

  glm::mat4 yawMat = glm::rotate(glm::radians((float)m_chunkAy[i].dYaw), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 nextChunkYawed = glm::vec3(yawMat * glm::vec4(nextChunkBase, 1.0f));
  pitchAxis = glm::normalize(glm::cross(nextChunkYawed, glm::vec3(0.0f, 1.0f, 0.0f)));

  glm::mat4 pitchMat = glm::rotate(glm::radians((float)m_chunkAy[i].dPitch), pitchAxis);
  nextChunkPitched = glm::vec3(pitchMat * glm::vec4(nextChunkYawed, 1.0f));

  glm::mat4 translateMat = glm::mat4(1);
  if (i > 0)
    translateMat = glm::translate(prevCenter);
  //center
  float fLen = (float)m_chunkAy[i].iLength / m_fScale;
  glm::mat4 scaleMat = glm::scale(glm::vec3(fLen, fLen, fLen));
  center = glm::vec3(translateMat * scaleMat * glm::vec4(nextChunkPitched, 1.0f));
  rollMat = glm::rotate(glm::radians((float)m_chunkAy[i].dRoll * -1.0f), glm::normalize(nextChunkPitched));
}

//-------------------------------------------------------------------------------------------------

void CTrackData::GetLane(int i, glm::vec3 center, glm::vec3 pitchAxis, glm::mat4 rollMat, 
                          glm::vec3 &lane, bool bLeft)
{
  glm::mat4 translateMat = glm::translate(center); //translate to centerline
  float fLen;
  if (bLeft)
    fLen = (float)(m_chunkAy[i].iLeftLaneWidth) / m_fScale * -1.0f;
  else
    fLen = (float)(m_chunkAy[i].iRightLaneWidth) / m_fScale;
  glm::mat4 scaleMat = glm::scale(glm::vec3(fLen, fLen, fLen));
  lane = glm::vec3(translateMat * scaleMat * rollMat * glm::vec4(pitchAxis, 1.0f));
}

//-------------------------------------------------------------------------------------------------

void CTrackData::GetShoulder(int i, glm::vec3 lLane, glm::vec3 pitchAxis, glm::mat4 rollMat, glm::vec3 nextChunkPitched, 
                              glm::vec3 &shoulder, bool bLeft, bool bIgnoreHeight)
{
  glm::mat4 translateMat = glm::translate(lLane); //translate to end of left lane
  float fLen = 0.0f;
  float fHeight = 0.0f;
  if (bLeft) {
    fLen = (float)m_chunkAy[i].iLeftShoulderWidth / m_fScale * -1.0f;
    if (!bIgnoreHeight)
      fHeight = (float)m_chunkAy[i].iLeftShoulderHeight / m_fScale * -1.0f;
  } else {
    fLen = (float)m_chunkAy[i].iRightShoulderWidth / m_fScale;
    if (!bIgnoreHeight)
      fHeight = (float)m_chunkAy[i].iRightShoulderHeight / m_fScale * -1.0f;
  }
  glm::mat4 scaleMatWidth = glm::scale(glm::vec3(fLen, fLen, fLen));
  glm::mat4 scaleMatHeight = glm::scale(glm::vec3(fHeight, fHeight, fHeight));
  glm::vec3 widthVec = glm::vec3(scaleMatWidth * rollMat * glm::vec4(pitchAxis, 1.0f));
  glm::vec3 normal = glm::normalize(glm::cross(nextChunkPitched, pitchAxis));
  glm::vec3 heightVec = glm::vec3(scaleMatHeight * rollMat * glm::vec4(normal, 1.0f));
  glm::vec3 shoulderVec = widthVec + heightVec;
  shoulder = glm::vec3(translateMat * glm::vec4(shoulderVec, 1.0f));
}

//-------------------------------------------------------------------------------------------------

void CTrackData::GetEnvirFloor(int i, glm::vec3 lShoulder, glm::vec3 rShoulder,
                               glm::vec3 &lEnvirFloor, glm::vec3 &rEnvirFloor)
{
  float fEnvirFloorDepth = (float)m_header.iFloorDepth / m_fScale * -1.0f;
  lEnvirFloor = lShoulder;
  rEnvirFloor = rShoulder;
  lEnvirFloor.y = fEnvirFloorDepth;
  rEnvirFloor.y = fEnvirFloorDepth;
}

//-------------------------------------------------------------------------------------------------

void CTrackData::GetOWallFloor(int i, glm::vec3 lLane, glm::vec3 rLane, glm::vec3 pitchAxis, glm::vec3 nextChunkPitched,
                               glm::vec3 &lFloor, glm::vec3 &rFloor)
{
  glm::mat4 translateMatL = glm::translate(lLane);
  glm::mat4 translateMatR = glm::translate(rLane);
  float fEnvirFloorDepth = (float)m_header.iFloorDepth / m_fScale * -1.0f;
  float fLOFloorHeight = (float)m_chunkAy[i].iLOuterFloorHeight / m_fScale * 1.0f;
  float fROFloorHeight = (float)m_chunkAy[i].iROuterFloorHeight / m_fScale * 1.0f;
  float fROFloorOffset = (float)m_chunkAy[i].iROuterFloorHOffset / m_fScale * 1.0f;
  float fLOFloorOffset = (float)m_chunkAy[i].iLOuterFloorHOffset / m_fScale * -1.0f;

  glm::mat4 scaleMatRWidth = glm::scale(glm::vec3(fROFloorOffset, fROFloorOffset, fROFloorOffset));
  glm::vec3 rWidthVec = glm::vec3(scaleMatRWidth * glm::vec4(pitchAxis, 1.0f));
  glm::mat4 scaleMatLWidth = glm::scale(glm::vec3(fLOFloorOffset, fLOFloorOffset, fLOFloorOffset));
  glm::vec3 lWidthVec = glm::vec3(scaleMatLWidth * glm::vec4(pitchAxis, 1.0f));

  lFloor = glm::vec3(translateMatL * glm::vec4(lWidthVec, 1.0f));;
  rFloor = glm::vec3(translateMatR * glm::vec4(rWidthVec, 1.0f));;
  lFloor.y = fEnvirFloorDepth + fLOFloorHeight;
  rFloor.y = fEnvirFloorDepth + fROFloorHeight;
}

//-------------------------------------------------------------------------------------------------

void CTrackData::GetWall(int i, glm::vec3 bottomAttach, glm::vec3 pitchAxis, glm::mat4 rollMat, glm::vec3 nextChunkPitched,
                            glm::vec3 &lloWall, eShapeSection wallSection)
{
  glm::mat4 translateMat = glm::translate(bottomAttach);
  float fHOffset = 0.0f;
  float fHeight = 0.0f;
  switch (wallSection) {
    case LWALL:
      if (m_chunkAy[i].iLeftWallType != -1)
        fHeight = (float)m_chunkAy[i].iRoofHeight / m_fScale * -1.0f;
    case RWALL:
      if (m_chunkAy[i].iRightWallType != -1)
        fHeight = (float)m_chunkAy[i].iRoofHeight / m_fScale * -1.0f;
      break;
    case LLOWALL:
      fHOffset = (float)m_chunkAy[i].iLLOuterWallHOffset / m_fScale * -1.0f;
      fHeight = (float)m_chunkAy[i].iLLOuterWallHeight / m_fScale * -1.0f;
      break;
    case RLOWALL:
      fHOffset = (float)m_chunkAy[i].iRLOuterWallHOffset / m_fScale;
      fHeight = (float)m_chunkAy[i].iRLOuterWallHeight / m_fScale * -1.0f;
      break;
    case LUOWALL:
      fHOffset = (float)m_chunkAy[i].iLUOuterWallHOffset / m_fScale * -1.0f;
      fHeight = (float)m_chunkAy[i].iLUOuterWallHeight / m_fScale * -1.0f;
      break;
    case RUOWALL:
      fHOffset = (float)m_chunkAy[i].iRUOuterWallHOffset / m_fScale;
      fHeight = (float)m_chunkAy[i].iRUOuterWallHeight / m_fScale * -1.0f;
      break;
    default:
      assert(0); //only wall sections should use this function
  }
  glm::mat4 scaleMatWidth = glm::scale(glm::vec3(fHOffset, fHOffset, fHOffset));
  glm::mat4 scaleMatHeight = glm::scale(glm::vec3(fHeight, fHeight, fHeight));
  glm::vec3 widthVec = glm::vec3(scaleMatWidth * rollMat * glm::vec4(pitchAxis, 1.0f));
  glm::vec3 normal = glm::normalize(glm::cross(nextChunkPitched, pitchAxis));
  glm::vec3 heightVec = glm::vec3(scaleMatHeight * rollMat * glm::vec4(normal, 1.0f));
  glm::vec3 wallVec = widthVec + heightVec;
  lloWall = glm::vec3(translateMat * glm::vec4(wallVec, 1.0f));
}

//-------------------------------------------------------------------------------------------------

void CTrackData::ApplyVerticesSingleSection(int i, tVertex *vertices, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3)
{
  uint32 uiNumVertsPerChunk = 4;
  vertices[i * uiNumVertsPerChunk + 0].position = v0;
  vertices[i * uiNumVertsPerChunk + 1].position = v1;
  vertices[i * uiNumVertsPerChunk + 2].position = v2;
  vertices[i * uiNumVertsPerChunk + 3].position = v3;
}

//-------------------------------------------------------------------------------------------------

void CTrackData::GetTextureCoordinates(uint32 uiSurfaceType,
                                       tVertex &topLeft, tVertex &topRight, tVertex &bottomLeft, tVertex &bottomRight,
                                       bool bLeftLane, bool bRightLane)
{
    //TEXTURES
  bool bPair = uiSurfaceType & SURFACE_FLAG_TEXTURE_PAIR && uiSurfaceType & SURFACE_FLAG_PAIR_NEXT_TEX; //TODO: having pair but not pair next should double current texture
  bool bFlipVert = uiSurfaceType & SURFACE_FLAG_FLIP_VERT;
  bool bFlipHoriz = uiSurfaceType & SURFACE_FLAG_FLIP_HORIZ;
  bool bTransparent = uiSurfaceType & SURFACE_FLAG_TRANSPARENT;
  bool bPartialTrans = uiSurfaceType & SURFACE_FLAG_PARTIAL_TRANS;
  uint32 uiTexIndex = uiSurfaceType & SURFACE_TEXTURE_INDEX;

  //right lane takes the second texture on center surface
  //both center lanes only draw one texture each when paired
  uint32 uiTexIncVal = (bPair && !(bLeftLane || bRightLane)) ? 2 : 1;
  if (bRightLane && uiSurfaceType & SURFACE_FLAG_TEXTURE_PAIR)
    uiTexIndex++;

  if (!bFlipHoriz && !bFlipVert)
    topLeft.texCoords = glm::vec2(1.0f, (float)uiTexIndex / (float)m_tex.m_iNumTiles);
  else if (bFlipHoriz && !bFlipVert)
    topLeft.texCoords = glm::vec2(1.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_tex.m_iNumTiles);
  else if (!bFlipHoriz && bFlipVert)
    topLeft.texCoords = glm::vec2(0.0f, (float)uiTexIndex / (float)m_tex.m_iNumTiles);
  else if (bFlipHoriz && bFlipVert)
    topLeft.texCoords = glm::vec2(0.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_tex.m_iNumTiles);

  if (!bFlipHoriz && !bFlipVert)
    topRight.texCoords = glm::vec2(1.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_tex.m_iNumTiles);
  else if (bFlipHoriz && !bFlipVert)
    topRight.texCoords = glm::vec2(1.0f, (float)uiTexIndex / (float)m_tex.m_iNumTiles);
  else if (!bFlipHoriz && bFlipVert)
    topRight.texCoords = glm::vec2(0.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_tex.m_iNumTiles);
  else if (bFlipHoriz && bFlipVert)
    topRight.texCoords = glm::vec2(0.0f, (float)uiTexIndex / (float)m_tex.m_iNumTiles);

  if (!bFlipHoriz && !bFlipVert)
    bottomLeft.texCoords = glm::vec2(0.0f, (float)uiTexIndex / (float)m_tex.m_iNumTiles);
  else if (bFlipHoriz && !bFlipVert)
    bottomLeft.texCoords = glm::vec2(0.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_tex.m_iNumTiles);
  else if (!bFlipHoriz && bFlipVert)
    bottomLeft.texCoords = glm::vec2(1.0f, (float)uiTexIndex / (float)m_tex.m_iNumTiles);
  else if (bFlipHoriz && bFlipVert)
    bottomLeft.texCoords = glm::vec2(1.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_tex.m_iNumTiles);

  if (!bFlipHoriz && !bFlipVert)
    bottomRight.texCoords = glm::vec2(0.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_tex.m_iNumTiles);
  else if (bFlipHoriz && !bFlipVert)
    bottomRight.texCoords = glm::vec2(0.0f, (float)uiTexIndex / (float)m_tex.m_iNumTiles);
  else if (!bFlipHoriz && bFlipVert)
    bottomRight.texCoords = glm::vec2(1.0f, (float)(uiTexIndex + uiTexIncVal) / (float)m_tex.m_iNumTiles);
  else if (bFlipHoriz && bFlipVert)
    bottomRight.texCoords = glm::vec2(1.0f, (float)uiTexIndex / (float)m_tex.m_iNumTiles);

  if (bTransparent) {
    //use color
    topLeft.flags.x = 1.0f;
    topRight.flags.x = 1.0f;
    bottomLeft.flags.x = 1.0f;
    bottomRight.flags.x = 1.0f;
    //alpha
    float fAlphaVal = 0.8f;
    topLeft.flags.y = fAlphaVal;
    topRight.flags.y = fAlphaVal;
    bottomLeft.flags.y = fAlphaVal;
    bottomRight.flags.y = fAlphaVal;
    //color
    topLeft.color = glm::vec3(0);
    topRight.color = glm::vec3(0);
    bottomLeft.color = glm::vec3(0);
    bottomRight.color = glm::vec3(0);
  }
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
  snprintf(szBuf, sizeof(szBuf), " %4d %6d %6d %6d\r\n\r\n\r\n", (int)m_chunkAy.size(), m_header.iHeaderUnk1, m_header.iHeaderUnk2, m_header.iFloorDepth);
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