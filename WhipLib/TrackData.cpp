#include "TrackData.h"
#include "Unmangler.h"
#include <assert.h>
#include <fstream>
#include <sstream>
#include "Texture.h"
#include "Palette.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
#include "Logging.h"
#include "SignType.h"
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
  iAIMaxSpeed = 0;
  iAIAccuracy = 0;
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
}

//-------------------------------------------------------------------------------------------------

CTrackData::CTrackData()
  : m_iAILineHeight(100)
  , m_fScale(1000.0f)
  , m_pPal(NULL)
  , m_pTex(NULL)
  , m_pBld(NULL)
{
  ClearData();
}

//-------------------------------------------------------------------------------------------------

CTrackData::~CTrackData()
{
  if (m_pTex) {
    delete m_pTex;
    m_pTex = NULL;
  }
  if (m_pBld) {
    delete m_pBld;
    m_pBld = NULL;
  }
  if (m_pPal) {
    delete m_pPal;
    m_pPal = NULL;
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackData::ClearData()
{
  memset(&m_header, 0, sizeof(m_header));
  m_header.iFloorDepth = 2048;
  m_chunkAy.clear();
  m_sTextureFile = "";
  m_sBuildingFile = "";
  m_sLastLoadedTex = "";
  m_sLastLoadedBld = "";
  m_sLastLoadedPal = "";
  memset(&m_raceInfo, 0, sizeof(m_raceInfo));
}

//-------------------------------------------------------------------------------------------------

bool CTrackData::LoadTrack(const std::string &sFilename)
{
  ClearData();

  if (sFilename.empty()) {
    Logging::LogMessage("Track filename empty");
    return false;
  }

  m_sTrackFileFolder = sFilename;
  size_t pos = sFilename.find_last_of('/');
  if (pos == std::string::npos)
    pos = sFilename.find_last_of('\\');
  if (pos != std::string::npos && pos < sFilename.size())
    m_sTrackFileFolder = sFilename.substr(0, pos + 1);

  //open file
  std::ifstream file(sFilename.c_str(), std::ios::binary);
  if (!file.is_open()) {
    Logging::LogMessage("Failed to open track: %s", sFilename.c_str());
    return false;
  }

  file.seekg(0, file.end);
  size_t length = file.tellg();
  file.seekg(0, file.beg);
  if (length <= 0) {
    Logging::LogMessage("Track file %s is empty", sFilename.c_str());
    return false;
  }

  //read file
  char *szBuf = new char[length];
  memset(szBuf, 0, length);
  file.read(szBuf, length);

  bool bSuccess = false;
  int iUnmangledLength = Unmangler::GetUnmangledLength((uint8 *)szBuf, (int)length);
  //unmangle
  if (iUnmangledLength > 0 && iUnmangledLength < MAX_MANGLED_LENGTH) {
    Logging::LogMessage("Track file %s is mangled", sFilename.c_str());
    uint8 *szUnmangledData = new uint8[iUnmangledLength];
    bSuccess = Unmangler::UnmangleFile((uint8 *)szBuf, (int)length, szUnmangledData, iUnmangledLength);
    Logging::LogMessage("%s track file %s", bSuccess ? "Unmangled" : "Failed to unmangle", sFilename.c_str());

    if (bSuccess)
      bSuccess = ProcessTrackData(szUnmangledData, (size_t)iUnmangledLength);

    delete[] szUnmangledData;
  } else {
    bSuccess = ProcessTrackData((uint8 *)szBuf, length);
  }

  //cleanup
  delete[] szBuf;
  file.close();

  Logging::LogMessage("%s track file: %s", bSuccess ? "Loaded" : "Failed to load", sFilename.c_str());

  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

bool CTrackData::LoadTextures()
{
  bool bSuccess = true;

  std::string sPal = m_sTrackFileFolder + "PALETTE.PAL";
  std::string sTex = m_sTrackFileFolder + m_sTextureFile;
  std::string sBld = m_sTrackFileFolder + m_sBuildingFile;

  if (m_sLastLoadedPal.compare(sPal) != 0) {
    if (m_pPal) {
      delete m_pPal;
      m_pPal = NULL;
    }
    m_pPal = new CPalette;
    bSuccess &= m_pPal->LoadPalette(sPal);
    if (bSuccess)
      m_sLastLoadedPal = sPal;
  }

  if (m_sLastLoadedTex.compare(sTex) != 0) {
    if (m_pTex) {
      delete m_pTex;
      m_pTex = NULL;
    }
    m_pTex = new CTexture;
    bSuccess &= m_pTex->LoadTexture(sTex, m_pPal);
    if (bSuccess)
      m_sLastLoadedTex = sTex;
  }

  if (m_sLastLoadedBld.compare(sBld) != 0) {
    if (m_pBld) {
      delete m_pBld;
      m_pBld = NULL;
    }
    m_pBld = new CTexture;
    bSuccess &= m_pBld->LoadTexture(sBld, m_pPal);
    if (bSuccess)
      m_sLastLoadedBld = sBld;
  }

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

bool CTrackData::ProcessTrackData(const uint8 *pData, size_t length)
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
              Logging::LogMessage("Warning loading file: number of chunks loaded (%d) does not match header (%d)", (int)m_chunkAy.size(), m_header.iNumChunks);
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
            currChunk.dYaw                  = ConstrainAngle(std::stod(lineAy[7]));
            currChunk.dPitch                = ConstrainAngle(std::stod(lineAy[8]));
            currChunk.dRoll                 = ConstrainAngle(std::stod(lineAy[9]));
            currChunk.iAILine1              = std::stoi(lineAy[10]);
            currChunk.iAILine2              = std::stoi(lineAy[11]);
            currChunk.iAILine3              = std::stoi(lineAy[12]);
            currChunk.iAILine4              = std::stoi(lineAy[13]);
            currChunk.iTrackGrip            = std::stoi(lineAy[14]);
            currChunk.iLeftShoulderGrip     = std::stoi(lineAy[15]);
            currChunk.iRightShoulderGrip    = std::stoi(lineAy[16]);
            currChunk.iAIMaxSpeed           = std::stoi(lineAy[17]);
            currChunk.iAIAccuracy           = std::stoi(lineAy[18]);
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
            Logging::LogMessage("Error loading file: invalid line before chunk completion");
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
          currChunk.dSignYaw              = ConstrainAngle(std::stod(lineAy[15]));
          currChunk.dSignPitch            = ConstrainAngle(std::stod(lineAy[16]));
          currChunk.dSignRoll             = ConstrainAngle(std::stod(lineAy[17]));
          //inc chunk index
          ++iChunkLine;
        } else if (iChunkLine == 2) {
          if (lineAy.size() != CHUNK_LINE_2_COUNT) {
            assert(0);
            Logging::LogMessage("Error loading file: invalid line before chunk completion");
            bSuccess = false;
          }
          //process line 3
          currChunk.iLUOuterWallHOffset = std::stoi(lineAy[0]);
          currChunk.iLLOuterWallHOffset = std::stoi(lineAy[1]);
          currChunk.iLOuterFloorHOffset = std::stoi(lineAy[2]);
          currChunk.iROuterFloorHOffset = std::stoi(lineAy[3]);
          currChunk.iRLOuterWallHOffset = std::stoi(lineAy[4]);
          currChunk.iRUOuterWallHOffset = std::stoi(lineAy[5]);
          currChunk.iLUOuterWallHeight  = std::stoi(lineAy[6]);
          currChunk.iLLOuterWallHeight  = std::stoi(lineAy[7]);
          currChunk.iLOuterFloorHeight  = std::stoi(lineAy[8]);
          currChunk.iROuterFloorHeight  = std::stoi(lineAy[9]);
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
          Logging::LogMessage("Error loading file: signs section ended before anticipated");
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
          int iGeometryIndex                    = std::stoi(lineAy[0]);
          tStunt *pStunt = &m_stuntMap[iGeometryIndex];
          memset(pStunt, 0, sizeof(*pStunt));

          pStunt->iChunkCount     = std::stoi(lineAy[1]);
          pStunt->iNumTicks       = std::stoi(lineAy[2]);
          pStunt->iTickStartIdx   = std::stoi(lineAy[3]);
          pStunt->iTimingGroup    = std::stoi(lineAy[4]);
          pStunt->iHeight         = std::stoi(lineAy[5]);
          pStunt->iTimeBulging    = std::stoi(lineAy[6]);
          pStunt->iTimeFlat       = std::stoi(lineAy[7]);
          pStunt->iRampSideLength = std::stoi(lineAy[8]);
          pStunt->iFlags          = std::stoi(lineAy[9]);
        } else {
          assert(0);
          Logging::LogMessage("Error loading file: stunts section ended before anticipated");
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
          Logging::LogMessage("Error loading file: texture section ended before anticipated");
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
          m_raceInfo.dPreviewSize       = std::stod(lineAy[2]);
        }
        break;
    }
  }

  GenerateTrackMath();

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

bool CTrackData::ShouldDrawSurfaceType(int iSurfaceType)
{
  if (iSurfaceType == -1)
    return false;
  uint32 uiSurfaceType = CTrackData::GetSignedBitValueFromInt(iSurfaceType);
  if (uiSurfaceType & SURFACE_FLAG_NON_SOLID)
    return false;
  //if (!(uiSurfaceType & SURFACE_FLAG_TRANSPARENT)
  //    && !(uiSurfaceType & SURFACE_FLAG_APPLY_TEXTURE))
  //  return false;
  return true;
} 

//-------------------------------------------------------------------------------------------------

double CTrackData::ConstrainAngle(double dAngle)
{
  dAngle = fmod(dAngle, 360);
  if (dAngle < 0)
    dAngle += 360;
  return dAngle;
}

//-------------------------------------------------------------------------------------------------

bool CTrackData::ShouldShowChunkSection(int i, eShapeSection section)
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
          || m_chunkAy[i].iRightWallType == -1
          || (!ShouldDrawSurfaceType(m_chunkAy[i].iLeftWallType) && !ShouldDrawSurfaceType(m_chunkAy[i].iRightWallType))))
    return false;
  if (section == eShapeSection::OWALLFLOOR
      && (m_chunkAy[i].iOuterFloorType == -2
          || !ShouldDrawSurfaceType(m_chunkAy[i].iOuterFloorType)))
    return false;
  if (section == eShapeSection::LLOWALL
      && (!ShouldDrawSurfaceType(m_chunkAy[i].iLLOuterWallType)
          || m_chunkAy[i].iOuterFloorType == -1
          || (m_chunkAy[i].iOuterFloorType == -2
              && !ShouldDrawSurfaceType(m_chunkAy[i].iLeftSurfaceType)
              && !ShouldDrawSurfaceType(m_chunkAy[i].iCenterSurfaceType))))
    return false;
  if (section == eShapeSection::RLOWALL
      && (!ShouldDrawSurfaceType(m_chunkAy[i].iRLOuterWallType)
          || m_chunkAy[i].iOuterFloorType == -1
          || (m_chunkAy[i].iOuterFloorType == -2
              && !ShouldDrawSurfaceType(m_chunkAy[i].iRightSurfaceType)
              && !ShouldDrawSurfaceType(m_chunkAy[i].iCenterSurfaceType))))
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
    int iSignable = 0;
    int iChunk = 0;
    //extraneous signs will be lost
    while (iChunk < m_chunkAy.size()) {
      if (m_chunkAy[iChunk].iSignType >= 0 && m_chunkAy[iChunk].iSignType < 256) {
        if (iSignable == iVal0)
          break;
        iSignable++;
      }
      iChunk++;
    }
    if (iChunk < m_chunkAy.size()) {
      m_chunkAy[iChunk].iSignTexture = std::stoi(lineAy[1]);
    }
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackData::GetTrackData(std::vector<uint8> &data)
{
  //write header
  char szBuf[1024];
  snprintf(szBuf, sizeof(szBuf), " %4d %6d %6d %6d\r\n\r\n\r\n", (int)m_chunkAy.size(), m_header.iHeaderUnk1, m_header.iHeaderUnk2, m_header.iFloorDepth);
  WriteToVector(data, szBuf);

  //write chunks
  CSignMap signMap;
  CSignMap backsMap;
  int iSignIndex = 0;
  for (int i = 0; i < m_chunkAy.size(); ++i) {
    //fix angles
    m_chunkAy[i].dYaw = ConstrainAngle(m_chunkAy[i].dYaw);
    m_chunkAy[i].dPitch = ConstrainAngle(m_chunkAy[i].dPitch);
    m_chunkAy[i].dRoll = ConstrainAngle(m_chunkAy[i].dRoll);
    m_chunkAy[i].dSignYaw = ConstrainAngle(m_chunkAy[i].dSignYaw);
    m_chunkAy[i].dSignPitch = ConstrainAngle(m_chunkAy[i].dSignPitch);
    m_chunkAy[i].dSignRoll = ConstrainAngle(m_chunkAy[i].dSignRoll);

    //write chunk data
    char szGenerate[1024];
    GenerateChunkString(m_chunkAy[i], szGenerate, sizeof(szGenerate));
    WriteToVector(data, szGenerate);
    WriteToVector(data, "\r\n");
    if (m_chunkAy[i].iSignType >= 0 && m_chunkAy[i].iSignType < 256) { //signable chunks
      if (m_chunkAy[i].iSignType < g_signAyCount 
          && g_signAy[m_chunkAy[i].iSignType].bCanHaveTexture  //these signable chunks don't have textures
          && m_chunkAy[i].iSignTexture > 0) {
        signMap[iSignIndex] = m_chunkAy[i].iSignTexture;
      }
      iSignIndex++;
    }
    if (m_chunkAy[i].iBackTexture >= 0 
        && (GetSignedBitValueFromInt(m_chunkAy[i].iLeftSurfaceType) & SURFACE_FLAG_BACK ||
            GetSignedBitValueFromInt(m_chunkAy[i].iCenterSurfaceType) & SURFACE_FLAG_BACK ||
            GetSignedBitValueFromInt(m_chunkAy[i].iRightSurfaceType) & SURFACE_FLAG_BACK ||
            GetSignedBitValueFromInt(m_chunkAy[i].iLeftWallType) & SURFACE_FLAG_BACK ||
            GetSignedBitValueFromInt(m_chunkAy[i].iRightWallType) & SURFACE_FLAG_BACK ||
            GetSignedBitValueFromInt(m_chunkAy[i].iRoofType) & SURFACE_FLAG_BACK ||
            GetSignedBitValueFromInt(m_chunkAy[i].iLUOuterWallType) & SURFACE_FLAG_BACK ||
            GetSignedBitValueFromInt(m_chunkAy[i].iLLOuterWallType) & SURFACE_FLAG_BACK ||
            GetSignedBitValueFromInt(m_chunkAy[i].iOuterFloorType) & SURFACE_FLAG_BACK ||
            GetSignedBitValueFromInt(m_chunkAy[i].iRLOuterWallType) & SURFACE_FLAG_BACK ||
            GetSignedBitValueFromInt(m_chunkAy[i].iRUOuterWallType) & SURFACE_FLAG_BACK ||
            GetSignedBitValueFromInt(m_chunkAy[i].iEnvironmentFloorType) & SURFACE_FLAG_BACK ||
            GetSignedBitValueFromInt(m_chunkAy[i].iSignTexture) & SURFACE_FLAG_BACK)) {
      backsMap[i] = m_chunkAy[i].iBackTexture;
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
  for (CStuntMap::iterator it = m_stuntMap.begin(); it != m_stuntMap.end(); ++it) {
    memset(szBuf, 0, sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), " %4d %6d %6d %6d %6d %6d %6d %6d %6d %6d\r\n",
             it->first, it->second.iChunkCount, it->second.iNumTicks, it->second.iTickStartIdx,
             it->second.iTimingGroup, it->second.iHeight, it->second.iTimeBulging,
             it->second.iTimeFlat, it->second.iRampSideLength, it->second.iFlags);
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
        && m_raceInfo.dPreviewSize == 0)) {

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
             m_raceInfo.dTrackMapSize, m_raceInfo.iTrackMapFidelity, m_raceInfo.dPreviewSize);
    WriteToVector(data, szBuf);
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackData::WriteToVector(std::vector<uint8> &data, const char *szText)
{
  int iLength = (int)strlen(szText);
  for (int i = 0; i < iLength; ++i) {
    uint8 val = (uint8)szText[i];
    data.push_back(val);
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackData::GenerateChunkString(tGeometryChunk &chunk, char *szBuf, int iSize)
{
  snprintf(szBuf, iSize,
           "%5d %6d %6d %6d %6d %6d %6d %11.5lf %11.5lf %11.5lf %5d %5d %5d %5d %3d %3d %3d %4d %5d %3d %3d %3d\r\n" //line 1
           "%6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %4d %6d %6d %6.1lf %6.1lf %6.1lf\r\n"   //line 2
           "%5d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d"                             //line 3
           " %3d %3d %3d %d %d %d %d %d %d %d %d %d %d %d %3d %3d %3d\r\n"       //line 3 continued
           , chunk.iLeftShoulderWidth
           , chunk.iLeftLaneWidth
           , chunk.iRightLaneWidth
           , chunk.iRightShoulderWidth
           , chunk.iLeftShoulderHeight
           , chunk.iRightShoulderHeight
           , chunk.iLength
           , chunk.dYaw
           , chunk.dPitch
           , chunk.dRoll
           , chunk.iAILine1
           , chunk.iAILine2
           , chunk.iAILine3
           , chunk.iAILine4
           , chunk.iTrackGrip
           , chunk.iLeftShoulderGrip
           , chunk.iRightShoulderGrip
           , chunk.iAIMaxSpeed
           , chunk.iAIAccuracy
           , chunk.iAudioAboveTrigger
           , chunk.iAudioTriggerSpeed
           , chunk.iAudioBelowTrigger
           , chunk.iLeftSurfaceType
           , chunk.iCenterSurfaceType
           , chunk.iRightSurfaceType
           , chunk.iLeftWallType
           , chunk.iRightWallType
           , chunk.iRoofType
           , chunk.iLUOuterWallType
           , chunk.iLLOuterWallType
           , chunk.iOuterFloorType
           , chunk.iRLOuterWallType
           , chunk.iRUOuterWallType
           , chunk.iEnvironmentFloorType
           , chunk.iSignType
           , chunk.iSignHorizOffset
           , chunk.iSignVertOffset
           , chunk.dSignYaw
           , chunk.dSignPitch
           , chunk.dSignRoll
           , chunk.iLUOuterWallHOffset
           , chunk.iLLOuterWallHOffset
           , chunk.iLOuterFloorHOffset
           , chunk.iROuterFloorHOffset
           , chunk.iRLOuterWallHOffset
           , chunk.iRUOuterWallHOffset
           , chunk.iLUOuterWallHeight
           , chunk.iLLOuterWallHeight
           , chunk.iLOuterFloorHeight
           , chunk.iROuterFloorHeight
           , chunk.iRLOuterWallHeight
           , chunk.iRUOuterWallHeight
           , chunk.iRoofHeight
           , chunk.iDrawOrder1
           , chunk.iDrawOrder2
           , chunk.iDrawOrder3
           , chunk.iUnk37
           , chunk.iUnk38
           , chunk.iUnk39
           , chunk.iUnk40
           , chunk.iUnk41
           , chunk.iUnk42
           , chunk.iUnk43
           , chunk.iUnk44
           , chunk.iUnk45
           , chunk.iUnk46
           , chunk.iUnk47
           , chunk.iUnk48
           , chunk.iUnk49
           , chunk.iUnk50);
}

//-------------------------------------------------------------------------------------------------

void CTrackData::GenerateTrackMath()
{
  if (m_chunkAy.empty()) {
    return;
  }

  ResetStunts();

  m_chunkAy[m_chunkAy.size() - 1].math.center = glm::vec3(0, 0, 1);
  for (uint32 i = 0; i < m_chunkAy.size(); ++i) {
    int iPrevIndex = (int)m_chunkAy.size() - 1;
    if (i > 0)
      iPrevIndex = i - 1;

    glm::mat4 rollMatNoRoll = glm::mat4(1);
    GetCenter(i, m_chunkAy[iPrevIndex].math.center,
              m_chunkAy[i].math.center,
              m_chunkAy[i].math.pitchAxis,
              m_chunkAy[i].math.nextChunkPitched,
              m_chunkAy[i].math.yawMat,
              m_chunkAy[i].math.pitchMat,
              m_chunkAy[i].math.rollMat);

    int iChunkIndex = (int)m_chunkAy.size() - 1;
    if (i > 0)
      iChunkIndex = i - 1;

    //left lane
    GetLane(i,
            m_chunkAy[i].math.center,
            m_chunkAy[i].math.pitchAxis,
            m_chunkAy[i].math.rollMat,
            m_chunkAy[i].math.lLane, true);
    //right lane
    GetLane(i,
            m_chunkAy[i].math.center,
            m_chunkAy[i].math.pitchAxis,
            m_chunkAy[i].math.rollMat,
            m_chunkAy[i].math.rLane, false);
    //left shoulder
    GetShoulder(i,
                m_chunkAy[i].math.lLane,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.lShoulder, true);
    //right shoulder
    GetShoulder(i,
                m_chunkAy[i].math.rLane,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.rShoulder, false);
    //left wall
    m_chunkAy[i].math.bLWallAttachToLane = CTrackData::GetSignedBitValueFromInt(m_chunkAy[i].iLeftWallType) & SURFACE_FLAG_WALL_31;
    if (m_chunkAy[i].iLeftWallType == -1)
      m_chunkAy[i].math.bLWallAttachToLane = m_chunkAy[iPrevIndex].math.bLWallAttachToLane;
    m_chunkAy[i].math.lWallBottomAttach = m_chunkAy[i].math.bLWallAttachToLane ? m_chunkAy[i].math.lLane : m_chunkAy[i].math.lShoulder;
    GetWall(i,
            m_chunkAy[i].math.lWallBottomAttach,
            m_chunkAy[i].math.pitchAxis,
            m_chunkAy[i].math.rollMat,
            m_chunkAy[i].math.nextChunkPitched,
            m_chunkAy[i].math.lWall, eShapeSection::LWALL);
    //right wall
    m_chunkAy[i].math.bRWallAttachToLane = CTrackData::GetSignedBitValueFromInt(m_chunkAy[i].iRightWallType) & SURFACE_FLAG_WALL_31;
    if (m_chunkAy[i].iRightWallType == -1)
      m_chunkAy[i].math.bRWallAttachToLane = m_chunkAy[iPrevIndex].math.bRWallAttachToLane;
    m_chunkAy[i].math.rWallBottomAttach = m_chunkAy[i].math.bRWallAttachToLane ? m_chunkAy[i].math.rLane : m_chunkAy[i].math.rShoulder;
    GetWall(i,
            m_chunkAy[i].math.rWallBottomAttach,
            m_chunkAy[i].math.pitchAxis,
            m_chunkAy[i].math.rollMat,
            m_chunkAy[i].math.nextChunkPitched,
            m_chunkAy[i].math.rWall, eShapeSection::RWALL);
    //outer floor
    glm::vec3 lLaneNoRoll;
    GetLane(i,
            m_chunkAy[i].math.center,
            m_chunkAy[i].math.pitchAxis,
            rollMatNoRoll, lLaneNoRoll, true);
    glm::vec3 rLaneNoRoll;
    GetLane(i,
            m_chunkAy[i].math.center,
            m_chunkAy[i].math.pitchAxis,
            rollMatNoRoll, rLaneNoRoll, false);
    GetOWallFloor(i, lLaneNoRoll, rLaneNoRoll,
                  m_chunkAy[i].math.pitchAxis,
                  m_chunkAy[i].math.nextChunkPitched,
                  m_chunkAy[i].math.lFloor,
                  m_chunkAy[i].math.rFloor);
    //outer wall roll mat
    glm::mat4 oWallRollMat = m_chunkAy[i].iOuterFloorType < 0 ? m_chunkAy[i].math.rollMat : rollMatNoRoll;
    //llowall
    bool bLLOWallNonSolid = CTrackData::GetSignedBitValueFromInt(m_chunkAy[i].iLLOuterWallType) & SURFACE_FLAG_NON_SOLID;
    m_chunkAy[i].math.lloWallBottomAttach = m_chunkAy[i].math.lFloor;
    if (m_chunkAy[i].iOuterFloorType < 0) {
      if (bLLOWallNonSolid) {
        glm::vec3 lShoulderNoHeight;
        GetShoulder(i,
                    m_chunkAy[i].math.lLane,
                    m_chunkAy[i].math.pitchAxis,
                    m_chunkAy[i].math.rollMat,
                    m_chunkAy[i].math.nextChunkPitched,
                    lShoulderNoHeight, true, true);
        m_chunkAy[i].math.lloWallBottomAttach = lShoulderNoHeight;
      } else {
        m_chunkAy[i].math.bLloWallAttachToShoulder = true;
        m_chunkAy[i].math.lloWallBottomAttach = m_chunkAy[i].math.lShoulder;
      }
    }
    GetWall(i,
            m_chunkAy[i].math.lloWallBottomAttach,
            m_chunkAy[i].math.pitchAxis, oWallRollMat,
            m_chunkAy[i].math.nextChunkPitched,
            m_chunkAy[i].math.lloWall, eShapeSection::LLOWALL);
    //rlowall
    bool bRLOWallNonSolid = CTrackData::GetSignedBitValueFromInt(m_chunkAy[i].iRLOuterWallType) & SURFACE_FLAG_NON_SOLID;
    m_chunkAy[i].math.rloWallBottomAttach = m_chunkAy[i].math.rFloor;
    if (m_chunkAy[i].iOuterFloorType < 0) {
      if (bRLOWallNonSolid) {
        glm::vec3 rShoulderNoHeight;
        GetShoulder(i,
                    m_chunkAy[i].math.rLane,
                    m_chunkAy[i].math.pitchAxis,
                    m_chunkAy[i].math.rollMat,
                    m_chunkAy[i].math.nextChunkPitched, rShoulderNoHeight, false, true);
        m_chunkAy[i].math.rloWallBottomAttach = rShoulderNoHeight;
      } else {
        m_chunkAy[i].math.bRloWallAttachToShoulder = true;
        m_chunkAy[i].math.rloWallBottomAttach = m_chunkAy[i].math.rShoulder;
      }
    }
    GetWall(i,
            m_chunkAy[i].math.rloWallBottomAttach,
            m_chunkAy[i].math.pitchAxis, oWallRollMat,
            m_chunkAy[i].math.nextChunkPitched,
            m_chunkAy[i].math.rloWall, eShapeSection::RLOWALL);
    //luowall
    GetWall(i,
            m_chunkAy[i].math.lloWall,
            m_chunkAy[i].math.pitchAxis, oWallRollMat,
            m_chunkAy[i].math.nextChunkPitched,
            m_chunkAy[i].math.luoWall, eShapeSection::LUOWALL);
    //ruowall
    GetWall(i,
            m_chunkAy[i].math.rloWall,
            m_chunkAy[i].math.pitchAxis, oWallRollMat,
            m_chunkAy[i].math.nextChunkPitched,
            m_chunkAy[i].math.ruoWall, eShapeSection::RUOWALL);
    //ailines
    GetAILine(i,
              m_chunkAy[i].math.center,
              m_chunkAy[i].math.pitchAxis,
              m_chunkAy[i].math.rollMat,
              m_chunkAy[i].math.nextChunkPitched,
              m_chunkAy[i].math.aiLine1,
              eShapeSection::AILINE1, m_iAILineHeight);
    GetAILine(i,
              m_chunkAy[i].math.center,
              m_chunkAy[i].math.pitchAxis,
              m_chunkAy[i].math.rollMat,
              m_chunkAy[i].math.nextChunkPitched,
              m_chunkAy[i].math.aiLine2,
              eShapeSection::AILINE2, m_iAILineHeight);
    GetAILine(i,
              m_chunkAy[i].math.center,
              m_chunkAy[i].math.pitchAxis,
              m_chunkAy[i].math.rollMat,
              m_chunkAy[i].math.nextChunkPitched,
              m_chunkAy[i].math.aiLine3, eShapeSection::AILINE3, m_iAILineHeight);
    GetAILine(i,
              m_chunkAy[i].math.center,
              m_chunkAy[i].math.pitchAxis,
              m_chunkAy[i].math.rollMat,
              m_chunkAy[i].math.nextChunkPitched,
              m_chunkAy[i].math.aiLine4,
              eShapeSection::AILINE4, m_iAILineHeight);
    //car positions are ai lines with 0 height
    GetAILine(i,
              m_chunkAy[i].math.center,
              m_chunkAy[i].math.pitchAxis,
              m_chunkAy[i].math.rollMat,
              m_chunkAy[i].math.nextChunkPitched,
              m_chunkAy[i].math.carLine1,
              eShapeSection::AILINE1, 0);
    GetAILine(i,
              m_chunkAy[i].math.center,
              m_chunkAy[i].math.pitchAxis,
              m_chunkAy[i].math.rollMat,
              m_chunkAy[i].math.nextChunkPitched,
              m_chunkAy[i].math.carLine2,
              eShapeSection::AILINE2, 0);
    GetAILine(i,
              m_chunkAy[i].math.center,
              m_chunkAy[i].math.pitchAxis,
              m_chunkAy[i].math.rollMat,
              m_chunkAy[i].math.nextChunkPitched,
              m_chunkAy[i].math.carLine3, 
              eShapeSection::AILINE3, 0);
    GetAILine(i,
              m_chunkAy[i].math.center,
              m_chunkAy[i].math.pitchAxis,
              m_chunkAy[i].math.rollMat,
              m_chunkAy[i].math.nextChunkPitched,
              m_chunkAy[i].math.carLine4,
              eShapeSection::AILINE4, 0);

    m_chunkAy[i].math.centerStunt = m_chunkAy[i].math.center;
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackData::ResetStunts()
{
  CStuntMap::iterator it = m_stuntMap.begin();
  for (; it != m_stuntMap.end(); ++it) {
    it->second.iTickCurrIdx = 0;
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackData::UpdateStunts()
{
  CStuntMap::iterator it = m_stuntMap.begin();
  for (; it != m_stuntMap.end(); ++it) {
    int iStart = it->first - it->second.iChunkCount + 1;
    int iEnd = it->first + it->second.iChunkCount;

    if (iStart < 0)
      iStart = 0;
    if (iEnd > (int)m_chunkAy.size() - 1)
      iEnd = (int)m_chunkAy.size() - 1;

    int iHeight = 0;
    if (it->second.iTickCurrIdx < it->second.iNumTicks)
      iHeight = it->second.iHeight * it->second.iTickCurrIdx;
    else if (it->second.iTickCurrIdx < it->second.iNumTicks + it->second.iTimeBulging)
      iHeight = it->second.iHeight * it->second.iNumTicks;
    else if (it->second.iTickCurrIdx < it->second.iNumTicks + it->second.iTimeBulging + it->second.iNumTicks)
      iHeight = it->second.iHeight * (it->second.iNumTicks - (it->second.iTickCurrIdx - it->second.iNumTicks - it->second.iTimeBulging));
    else if (it->second.iTickCurrIdx < it->second.iNumTicks + it->second.iTimeBulging + it->second.iNumTicks + it->second.iTimeFlat)
      iHeight = 0;
    else
      it->second.iTickCurrIdx = 0;
    it->second.iTickCurrIdx++;
    float fTheta = atan((float)iHeight / (float)m_chunkAy[iStart].iLength);
    
    //ramp before stunt
    for (int i = iStart; i < it->first + 1; ++i) {
      int iPrevIndex = (int)m_chunkAy.size() - 1;
      if (i > 0)
        iPrevIndex = i - 1;
      glm::vec3 prevCenter = (i == iStart) ? m_chunkAy[iPrevIndex].math.center : m_chunkAy[iPrevIndex].math.centerStunt;
      glm::vec3 nextChunkBase = glm::vec3(0, 0, 1);
      glm::mat4 yawMat = m_chunkAy[i].math.yawMat;
      glm::vec3 nextChunkYawed = glm::vec3(yawMat * glm::vec4(nextChunkBase, 1.0f));
      glm::vec3 pitchAxis = glm::normalize(glm::cross(nextChunkYawed, glm::vec3(0.0f, 1.0f, 0.0f)));
      glm::mat4 pitchMat = glm::rotate(fTheta, pitchAxis);
      glm::vec3 nextChunkPitched = glm::vec3(pitchMat * glm::vec4(nextChunkYawed, 1.0f));
      glm::mat4 translateMat = glm::mat4(1);
      if (i > 0)
        translateMat = glm::translate(prevCenter);
      float fLen = (float)m_chunkAy[i].iLength / m_fScale * ((float)it->second.iRampSideLength / 1024.0f);
      glm::mat4 scaleMat = glm::scale(glm::vec3(fLen, fLen, fLen));
      m_chunkAy[i].math.centerStunt = glm::vec3(translateMat * scaleMat * glm::vec4(nextChunkPitched, 1.0f));
      glm::vec3 lLaneStunt;
      glm::vec3 rLaneStunt;
      GetLane(i, m_chunkAy[i].math.centerStunt, pitchAxis, m_chunkAy[i].math.rollMat, lLaneStunt, true);
      GetLane(i, m_chunkAy[i].math.centerStunt, pitchAxis, m_chunkAy[i].math.rollMat, rLaneStunt, false);
      if (it->second.iFlags & STUNT_FLAG_LLANE)
        m_chunkAy[i].math.lLane = lLaneStunt;
      if (it->second.iFlags & STUNT_FLAG_RLANE)
        m_chunkAy[i].math.rLane = rLaneStunt;
      if (it->second.iFlags & STUNT_FLAG_LSHOULDER)
        GetShoulder(i, lLaneStunt,
                    m_chunkAy[i].math.pitchAxis,
                    m_chunkAy[i].math.rollMat,
                    m_chunkAy[i].math.nextChunkPitched,
                    m_chunkAy[i].math.lShoulder, true);
      if (it->second.iFlags & STUNT_FLAG_RSHOULDER)
        GetShoulder(i, rLaneStunt,
                    m_chunkAy[i].math.pitchAxis,
                    m_chunkAy[i].math.rollMat,
                    m_chunkAy[i].math.nextChunkPitched,
                    m_chunkAy[i].math.rShoulder, false);
      m_chunkAy[i].math.lWallBottomAttach = m_chunkAy[i].math.bLWallAttachToLane ? m_chunkAy[i].math.lLane : m_chunkAy[i].math.lShoulder;
      if (it->second.iFlags & STUNT_FLAG_LWALL)
        GetWall(i,
                m_chunkAy[i].math.lWallBottomAttach,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.lWall, eShapeSection::LWALL);
      m_chunkAy[i].math.rWallBottomAttach = m_chunkAy[i].math.bRWallAttachToLane ? m_chunkAy[i].math.rLane : m_chunkAy[i].math.rShoulder;
      if (it->second.iFlags & STUNT_FLAG_RWALL)
        GetWall(i,
                m_chunkAy[i].math.rWallBottomAttach,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.rWall, eShapeSection::RWALL);
      //ailines
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.aiLine1,
                eShapeSection::AILINE1, m_iAILineHeight);
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.aiLine2,
                eShapeSection::AILINE2, m_iAILineHeight);
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.aiLine3, eShapeSection::AILINE3, m_iAILineHeight);
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.aiLine4,
                eShapeSection::AILINE4, m_iAILineHeight);
      //car positions are ai lines with 0 height
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.carLine1,
                eShapeSection::AILINE1, 0);
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.carLine2,
                eShapeSection::AILINE2, 0);
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.carLine3,
                eShapeSection::AILINE3, 0);
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.carLine4,
                eShapeSection::AILINE4, 0);
    }
    //ramp after stunt
    for (int i = iEnd; i > it->first; --i) {
      int iPrevIndex = 0;
      if (i < (int)m_chunkAy.size() - 1)
        iPrevIndex = i + 1;
      glm::vec3 prevCenter = (i == iEnd) ? m_chunkAy[iPrevIndex].math.center : m_chunkAy[iPrevIndex].math.centerStunt;
      glm::vec3 nextChunkBase = glm::vec3(0, 0, 1);
      glm::mat4 yawMat = glm::rotate(glm::radians((float)m_chunkAy[i].dYaw + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
      glm::vec3 nextChunkYawed = glm::vec3(yawMat * glm::vec4(nextChunkBase, 1.0f));
      glm::vec3 pitchAxis = glm::normalize(glm::cross(nextChunkYawed, glm::vec3(0.0f, 1.0f, 0.0f)));
      glm::mat4 pitchMat = glm::rotate(fTheta, pitchAxis);
      glm::vec3 nextChunkPitched = glm::vec3(pitchMat * glm::vec4(nextChunkYawed, 1.0f));
      glm::mat4 translateMat = glm::mat4(1);
      if (i > 0)
        translateMat = glm::translate(prevCenter);
      float fLen = (float)m_chunkAy[i].iLength / m_fScale * ((float)it->second.iRampSideLength / 1024.0f);
      glm::mat4 scaleMat = glm::scale(glm::vec3(fLen, fLen, fLen));
      m_chunkAy[i].math.centerStunt = glm::vec3(translateMat * scaleMat * glm::vec4(nextChunkPitched, 1.0f));
      glm::vec3 lLaneStunt;
      glm::vec3 rLaneStunt;
      GetLane(i, m_chunkAy[i].math.centerStunt, pitchAxis, m_chunkAy[i].math.rollMat, lLaneStunt, false);
      GetLane(i, m_chunkAy[i].math.centerStunt, pitchAxis, m_chunkAy[i].math.rollMat, rLaneStunt, true);
      if (it->second.iFlags & STUNT_FLAG_LLANE)
        m_chunkAy[i].math.lLane = lLaneStunt;
      if (it->second.iFlags & STUNT_FLAG_RLANE)
        m_chunkAy[i].math.rLane = rLaneStunt;
      if (it->second.iFlags & STUNT_FLAG_LSHOULDER)
        GetShoulder(i, lLaneStunt,
                    m_chunkAy[i].math.pitchAxis,
                    m_chunkAy[i].math.rollMat,
                    m_chunkAy[i].math.nextChunkPitched,
                    m_chunkAy[i].math.lShoulder, true);
      if (it->second.iFlags & STUNT_FLAG_RSHOULDER)
        GetShoulder(i,
                    rLaneStunt,
                    m_chunkAy[i].math.pitchAxis,
                    m_chunkAy[i].math.rollMat,
                    m_chunkAy[i].math.nextChunkPitched,
                    m_chunkAy[i].math.rShoulder, false);
      m_chunkAy[i].math.lWallBottomAttach = m_chunkAy[i].math.bLWallAttachToLane ? m_chunkAy[i].math.lLane : m_chunkAy[i].math.lShoulder;
      if (it->second.iFlags & STUNT_FLAG_LWALL)
        GetWall(i,
                m_chunkAy[i].math.lWallBottomAttach,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.lWall, eShapeSection::LWALL);
      m_chunkAy[i].math.rWallBottomAttach = m_chunkAy[i].math.bRWallAttachToLane ? m_chunkAy[i].math.rLane : m_chunkAy[i].math.rShoulder;
      if (it->second.iFlags & STUNT_FLAG_RWALL)
        GetWall(i,
                m_chunkAy[i].math.rWallBottomAttach,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.rWall, eShapeSection::RWALL);
      //ailines
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.aiLine1,
                eShapeSection::AILINE1, m_iAILineHeight);
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.aiLine2,
                eShapeSection::AILINE2, m_iAILineHeight);
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.aiLine3, eShapeSection::AILINE3, m_iAILineHeight);
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.aiLine4,
                eShapeSection::AILINE4, m_iAILineHeight);
      //car positions are ai lines with 0 height
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.carLine1,
                eShapeSection::AILINE1, 0);
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.carLine2,
                eShapeSection::AILINE2, 0);
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.carLine3,
                eShapeSection::AILINE3, 0);
      GetAILine(i,
                m_chunkAy[i].math.centerStunt,
                m_chunkAy[i].math.pitchAxis,
                m_chunkAy[i].math.rollMat,
                m_chunkAy[i].math.nextChunkPitched,
                m_chunkAy[i].math.carLine4,
                eShapeSection::AILINE4, 0);
    }
  }
}

//-------------------------------------------------------------------------------------------------

bool CTrackData::HasPitchedStunt()
{
  CStuntMap::iterator it = m_stuntMap.begin();
  for (; it != m_stuntMap.end(); ++it) {
    int iStart = it->first - it->second.iChunkCount;
    int iEnd = it->first + it->second.iChunkCount;

    if (iStart < 0)
      iStart = 0;
    if (iEnd > (int)m_chunkAy.size() - 1)
      iEnd = (int)m_chunkAy.size() - 1;

    for (int i = iStart; i <= iEnd; ++i) {
      if (m_chunkAy[i].dPitch > 5.0 && m_chunkAy[i].dPitch < 355.0)
        return true;
    }
  }

  return false;
}

//-------------------------------------------------------------------------------------------------

bool CTrackData::UseCenterStunt(int i)
{
  bool bUseCenterStunt = false;
  CStuntMap::iterator it = m_stuntMap.begin();
  for (; it != m_stuntMap.end(); ++it) {
    int iStart = it->first - it->second.iChunkCount;
    int iEnd = it->first + it->second.iChunkCount;

    if (iStart < 0)
      iStart = 0;
    if (iEnd > (int)m_chunkAy.size() - 1)
      iEnd = (int)m_chunkAy.size() - 1;

    if (i >= iStart && i <= iEnd) {
      //chunk is in stunt
      bUseCenterStunt |= (it->second.iFlags & STUNT_FLAG_LLANE || it->second.iFlags & STUNT_FLAG_RLANE);
    }
  }
  return bUseCenterStunt;
}

//-------------------------------------------------------------------------------------------------

void CTrackData::GetCenter(int i, glm::vec3 prevCenter,
                           glm::vec3 &center, glm::vec3 &pitchAxis, glm::vec3 &nextChunkPitched,
                           glm::mat4 &yawMat, glm::mat4 &pitchMat, glm::mat4 &rollMat)
{
  glm::vec3 nextChunkBase = glm::vec3(0, 0, 1);

  yawMat = glm::rotate(glm::radians((float)m_chunkAy[i].dYaw), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 nextChunkYawed = glm::vec3(yawMat * glm::vec4(nextChunkBase, 1.0f));
  pitchAxis = glm::normalize(glm::cross(nextChunkYawed, glm::vec3(0.0f, 1.0f, 0.0f)));

  pitchMat = glm::rotate(glm::radians((float)m_chunkAy[i].dPitch), pitchAxis);
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

void CTrackData::GetAILine(int i, glm::vec3 center, glm::vec3 pitchAxis, glm::mat4 rollMat, glm::vec3 nextChunkPitched,
                           glm::vec3 &aiLine, eShapeSection lineSection, int iHeight)
{
  glm::mat4 translateMat = glm::translate(center);
  float fLen = 0.0f;
  int iUseAILine;
  switch (lineSection) {
    case eShapeSection::AILINE1:
      iUseAILine = m_chunkAy[i].iAILine1;
      break;
    case eShapeSection::AILINE2:
      iUseAILine = m_chunkAy[i].iAILine2;
      break;
    case eShapeSection::AILINE3:
      iUseAILine = m_chunkAy[i].iAILine3;
      break;
    case eShapeSection::AILINE4:
      iUseAILine = m_chunkAy[i].iAILine4;
      break;
    default:
      assert(0);
  }
  int iShoulderHeight = 0;
  if (iUseAILine > 0 && iUseAILine > m_chunkAy[i].iLeftLaneWidth) {
    //ai line must be on left shoulder
    float fTheta = atan((float)m_chunkAy[i].iLeftShoulderHeight / (float)m_chunkAy[i].iLeftShoulderWidth);
    int iLengthIntoShoulder = abs(iUseAILine) - m_chunkAy[i].iLeftLaneWidth;
    iShoulderHeight = (int)(tan(fTheta) * (float)iLengthIntoShoulder);
  }
  if (iUseAILine < 0 && abs(iUseAILine) > m_chunkAy[i].iRightLaneWidth) {
    //ai line must be on right shoulder
    float fTheta = atan((float)m_chunkAy[i].iRightShoulderHeight / (float)m_chunkAy[i].iRightShoulderWidth);
    int iLengthIntoShoulder = abs(iUseAILine) - m_chunkAy[i].iRightLaneWidth;
    iShoulderHeight = (int)(tan(fTheta) * (float)iLengthIntoShoulder);
  }

  fLen = (float)iUseAILine / m_fScale * -1.0f;
  float fHeight = (float)(iHeight + iShoulderHeight) / m_fScale * -1.0f;

  glm::mat4 scaleMatWidth = glm::scale(glm::vec3(fLen, fLen, fLen));
  glm::mat4 scaleMatHeight = glm::scale(glm::vec3(fHeight, fHeight, fHeight));
  glm::vec3 widthVec = glm::vec3(scaleMatWidth * rollMat * glm::vec4(pitchAxis, 1.0f));
  glm::vec3 normal = glm::normalize(glm::cross(nextChunkPitched, pitchAxis));
  glm::vec3 heightVec = glm::vec3(scaleMatHeight * rollMat * glm::vec4(normal, 1.0f));
  glm::vec3 lineVec = widthVec + heightVec;
  aiLine = glm::vec3(translateMat * glm::vec4(lineVec, 1.0f));
}

//-------------------------------------------------------------------------------------------------