#include "TrackEditor.h"
#include "Track.h"
#include "MainWindow.h"
#include "qfile.h"
#include "qtextstream.h"
#include <assert.h>
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

CTrack::CTrack()
{
  ClearData();
}

//-------------------------------------------------------------------------------------------------

CTrack::~CTrack()
{

}

//-------------------------------------------------------------------------------------------------

void CTrack::ClearData()
{
  memset(&m_header, 0, sizeof(m_header));
  m_header.iHeaderUnk3 = 2048;
  m_chunkAy.clear();
  m_sTextureFile = "";
  m_sBuildingFile = "";
  memset(&m_raceInfo, 0, sizeof(m_raceInfo));
}

//-------------------------------------------------------------------------------------------------

bool CTrack::LoadTrack(const QString &sFilename)
{
  ClearData();

  if (sFilename.isEmpty()) {
    g_pMainWindow->LogMessage("Track filename empty: " + sFilename);
    return false;
  }

  QFile file(sFilename);
  if (!file.open(QIODevice::ReadOnly)) {
    g_pMainWindow->LogMessage("Failed to open track: " + sFilename);
    return false;
  }

  bool bSuccess = true;
  int iChunkLine = 0;
  struct tGeometryChunk currChunk;
  eFileSection section = HEADER;
  while (!file.atEnd()) {
    QByteArray baLine = file.readLine();
    QString sLine = baLine.data();
    QStringList slLine = sLine.split(' ', Qt::SkipEmptyParts);
    switch (section) {
      case HEADER:
        //must be first thing in file
        if (slLine.count() == HEADER_ELEMENT_COUNT) {
          //found header
          m_header.iNumChunks = slLine[0].toInt();
          m_header.iHeaderUnk1 = slLine[1].toInt();
          m_header.iHeaderUnk2 = slLine[2].toInt();
          m_header.iHeaderUnk3 = slLine[3].toInt();
          section = GEOMETRY;
        }
        break;
      case GEOMETRY:
        if (iChunkLine == 0) {
          if (slLine.count() == SIGNS_COUNT) {
            if (m_chunkAy.size() != m_header.iNumChunks) {
              QString sLogMsg = "Warning loading file: number of chunks loaded ("
                + QString::number(m_chunkAy.size())
                + ") does not match header ("
                + QString::number(m_header.iNumChunks)
                + ")";
              g_pMainWindow->LogMessage(sLogMsg);
            }
            // reached next section
            // there is no defined end to geometry chunks
            // more chunks than the count at the top of the file is allowed
            // so we must detect the beginning of the next section
            section = SIGNS;
            ProcessSign(slLine, section);
          } else if (slLine.count() == CHUNK_LINE_0_COUNT) {
            //start new chunk
            memset(&currChunk, 0, sizeof(currChunk));
            currChunk.iBackTexture = -1;
            currChunk.iSignTexture = -1;
            //process line 1
            currChunk.iLeftShoulderWidth = slLine[0].toInt();
            currChunk.iLeftLaneWidth = slLine[1].toInt();
            currChunk.iRightLaneWidth = slLine[2].toInt();
            currChunk.iRightShoulderWidth = slLine[3].toInt();
            currChunk.iLeftShoulderHeight = slLine[4].toInt();
            currChunk.iRightShoulderHeight = slLine[5].toInt();
            currChunk.iLength = slLine[6].toInt();
            currChunk.fYaw = slLine[7].toFloat();
            currChunk.fPitch = slLine[8].toFloat();
            currChunk.fRoll = slLine[9].toFloat();
            currChunk.iAILine1 = slLine[10].toInt();
            currChunk.iAILine2 = slLine[11].toInt();
            currChunk.iAILine3 = slLine[12].toInt();
            currChunk.iAILine4 = slLine[13].toInt();
            currChunk.iTrackGrip = slLine[14].toInt();
            currChunk.iLeftShoulderGrip = slLine[15].toInt();
            currChunk.iRightShoulderGrip = slLine[16].toInt();
            currChunk.iUnk04 = slLine[17].toInt();
            currChunk.iUnk05 = slLine[18].toInt();
            currChunk.iUnk06 = slLine[19].toInt();
            currChunk.iUnk07 = slLine[20].toInt();
            currChunk.iUnk08 = slLine[21].toInt();
            //inc chunk index
            ++iChunkLine;
          }
          //TODO: what about invalid number of chunks?
        } else if (iChunkLine == 1) {
          if (slLine.count() != CHUNK_LINE_1_COUNT) {
            assert(0);
            g_pMainWindow->LogMessage("Error loading file: invalid line before chunk completion");
            bSuccess = false;
          }
          //process line 2
          currChunk.iLeftSurfaceType = slLine[0].toInt();
          currChunk.iCenterSurfaceType = slLine[1].toInt();
          currChunk.iRightSurfaceType = slLine[2].toInt();
          currChunk.iLeftWallType = slLine[3].toInt();
          currChunk.iRightWallType = slLine[4].toInt();
          currChunk.iRoofType = slLine[5].toInt();
          currChunk.iUnk12 = slLine[6].toInt();
          currChunk.iLeftOuterExtraWallType = slLine[7].toInt();
          currChunk.iUnk14 = slLine[8].toInt();
          currChunk.iRightOuterExtraWallType = slLine[9].toInt();
          currChunk.iUnk16 = slLine[10].toInt();
          currChunk.iEnvironmentFloorType = slLine[11].toInt();
          currChunk.iSignType = slLine[12].toInt();
          currChunk.iUnk19 = slLine[13].toInt();
          currChunk.iUnk20 = slLine[14].toInt();
          currChunk.fUnk1 = slLine[15].toFloat();
          currChunk.fUnk2 = slLine[16].toFloat();
          currChunk.fUnk3 = slLine[17].toFloat();
          //inc chunk index
          ++iChunkLine;
        } else if (iChunkLine == 2) {
          if (slLine.count() != CHUNK_LINE_2_COUNT) {
            assert(0);
            g_pMainWindow->LogMessage("Error loading file: invalid line before chunk completion");
            bSuccess = false;
          }
          //process line 3
          currChunk.iLeftOuterUpperExtraWallAngle = slLine[0].toInt();
          currChunk.iLeftOuterLowerExtraWallAngle = slLine[1].toInt();
          currChunk.iUnk23 = slLine[2].toInt();
          currChunk.iUnk24 = slLine[3].toInt();
          currChunk.iRightOuterLowerExtraWallAngle = slLine[4].toInt();
          currChunk.iRightOuterUpperExtraWallAngle = slLine[5].toInt();
          currChunk.iLeftOuterUpperExtraWallHeight = slLine[6].toInt();
          currChunk.iLeftOuterLowerExtraWallHeight = slLine[7].toInt();
          currChunk.iUnk29 = slLine[8].toInt();
          currChunk.iUnk30 = slLine[9].toInt();
          currChunk.iRightOuterLowerExtraWallHeight = slLine[10].toInt();
          currChunk.iRightOuterUpperExtraWallHeight = slLine[11].toInt();
          currChunk.iUnk33 = slLine[12].toInt();
          currChunk.iUnk34 = slLine[13].toInt();
          currChunk.iUnk35 = slLine[14].toInt();
          currChunk.iUnk36 = slLine[15].toInt();
          currChunk.iUnk37 = slLine[16].toInt();
          currChunk.iUnk38 = slLine[17].toInt();
          currChunk.iUnk39 = slLine[18].toInt();
          currChunk.iUnk40 = slLine[19].toInt();
          currChunk.iUnk41 = slLine[20].toInt();
          currChunk.iUnk42 = slLine[21].toInt();
          currChunk.iUnk43 = slLine[22].toInt();
          currChunk.iUnk44 = slLine[23].toInt();
          currChunk.iUnk45 = slLine[24].toInt();
          currChunk.iUnk46 = slLine[25].toInt();
          currChunk.iUnk47 = slLine[26].toInt();
          currChunk.iUnk48 = slLine[27].toInt();
          currChunk.iUnk49 = slLine[28].toInt();
          currChunk.iUnk50 = slLine[29].toInt();
          //chunk is complete, add to array and reset index
          m_chunkAy.push_back(currChunk);
          iChunkLine = 0;
        }
        break;
      case SIGNS:
        if (slLine.count() == SIGNS_COUNT) {
          ProcessSign(slLine, section);
        } else {
          assert(0);
          g_pMainWindow->LogMessage("Error loading file: signs section ended before anticipated");
          bSuccess = false;
        }
        break;
      case STUNTS:
        if (slLine.count() == 1) {
          if (!slLine[0].simplified().isEmpty() && slLine[0].toInt() == -1) {
            //stunts section always ends with a single -1 value
            section = TEXTURE;
          }
        } else if (slLine.count() == STUNTS_COUNT) {
          //process stunt
          tStunt stunt;
          memset(&stunt, 0, sizeof(stunt));
          int iGeometryIndex = slLine[0].toInt();
          stunt.iScaleFactor = slLine[1].toInt();
          stunt.iAngle = slLine[2].toInt();
          stunt.iUnknown = slLine[3].toInt();
          stunt.iTimingGroup = slLine[4].toInt();
          stunt.iHeight = slLine[5].toInt();
          stunt.iTimeBulging = slLine[6].toInt();
          stunt.iTimeFlat = slLine[7].toInt();
          stunt.iSmallerExpandsLargerContracts = slLine[8].toInt();
          stunt.iBulge = slLine[9].toInt();
          if (iGeometryIndex < m_chunkAy.size()) {
            memcpy(&m_chunkAy[iGeometryIndex].stunt, &stunt, sizeof(stunt));
          }
        } else {
          assert(0);
          g_pMainWindow->LogMessage("Error loading file: stunts section ended before anticipated");
          bSuccess = false;
        }
        break;
      case TEXTURE:
        if (slLine.count() == 1) {
          if (slLine[0].simplified().isEmpty()) {
            //do nothing
          } if (slLine[0].toInt() == -1) {
            //texture section always ends with a single -1 value
            section = TRACK_NUM;
          } else {
            //texture and building files
            QStringList slTexLine = slLine[0].split(':');
            if (slTexLine.count() == 2) {
              if (slTexLine[0].compare("TEX") == 0) {
                m_sTextureFile = slTexLine[1].simplified();
              } else if (slTexLine[0].compare("BLD") == 0) {
                m_sBuildingFile = slTexLine[1].simplified();
              }
            }
          }
        } else if (slLine.count() == BACKS_COUNT) {
          //process backs
          int iGeometryIndex = slLine[0].toInt();
          if (iGeometryIndex < m_chunkAy.size()) {
            m_chunkAy[iGeometryIndex].iBackTexture = slLine[1].toInt();
          }
        } else {
          assert(0);
          g_pMainWindow->LogMessage("Error loading file: texture section ended before anticipated");
          bSuccess = false;
        }
        break;
      case TRACK_NUM:
        if (slLine.count() == 1) {
          if (slLine[0].simplified().isEmpty()) {
            //do nothing
          } else {
            m_raceInfo.iTrackNumber = slLine[0].toInt();
            section = LAPS;
          }
        }
        break;
      case LAPS:
        if (slLine.count() == LAPS_COUNT) {
          m_raceInfo.iImpossibleLaps = slLine[0].toInt();
          m_raceInfo.iHardLaps = slLine[1].toInt();
          m_raceInfo.iTrickyLaps = slLine[2].toInt();
          m_raceInfo.iMediumLaps = slLine[3].toInt();
          m_raceInfo.iEasyLaps = slLine[4].toInt();
          m_raceInfo.iGirlieLaps = slLine[5].toInt();
          section = MAP;
        }
        break;
      case MAP:
        if (slLine.count() == MAP_COUNT) {
          m_raceInfo.fTrackMapSize = slLine[0].toFloat();
          m_raceInfo.iTrackMapFidelity = slLine[1].toInt();
          m_raceInfo.fUnknown = slLine[2].toFloat();
        }
        break;
    }
  }

  file.close();
  QString sSuccess = (bSuccess ? "Successfully loaded" : "Failed to load");
  QString sLogMsg = sSuccess + " file " + sFilename + "\n"
    + "  geometry chunks: " + QString::number(m_chunkAy.size()) + "\n"
    + "  texture file: " + m_sTextureFile + "\n"
    + "  building file: " + m_sBuildingFile;
  g_pMainWindow->LogMessage(sLogMsg);

  //generate strings
  UpdateChunkStrings();

  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

bool CTrack::SaveTrack(const QString &sFilename)
{
  if (sFilename.isEmpty())
    return false;

  QFile file(sFilename);
  file.resize(0);
  if (file.open(QIODevice::ReadWrite)) {
    QTextStream stream(&file);

    //write header
    char szBuf[1024];
    snprintf(szBuf, sizeof(szBuf), " %4d %6d %6d %6d", (int)m_chunkAy.size(), m_header.iHeaderUnk1, m_header.iHeaderUnk2, m_header.iHeaderUnk3);
    stream << szBuf << Qt::endl << Qt::endl << Qt::endl;

    //write chunks
    CSignMap signMap;
    CSignMap backsMap;
    CStuntMap stuntMap;
    for (int i = 0; i < m_chunkAy.size(); ++i) {
      stream << m_chunkAy[i].sString.c_str() << Qt::endl;
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
      snprintf(szBuf, sizeof(szBuf), " %4d %6d", it->first, (int)it->second);
      stream << szBuf << Qt::endl;
    }
    memset(szBuf, 0, sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), " %4d %6d", -1, -1);
    stream << szBuf << Qt::endl;
    stream << Qt::endl;

    //write stunts
    for (CStuntMap::iterator it = stuntMap.begin(); it != stuntMap.end(); ++it) {
      memset(szBuf, 0, sizeof(szBuf));
      snprintf(szBuf, sizeof(szBuf), " %4d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
               it->first, it->second->iScaleFactor, it->second->iAngle, it->second->iUnknown,
               it->second->iTimingGroup, it->second->iHeight, it->second->iTimeBulging,
               it->second->iTimeFlat, it->second->iSmallerExpandsLargerContracts, it->second->iBulge);
      stream << szBuf << Qt::endl;
    }
    memset(szBuf, 0, sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), " %4d", -1);
    stream << szBuf << Qt::endl;
    stream << Qt::endl;

    //write textures
    stream << "TEX:" << m_sTextureFile << Qt::endl;
    stream << "BLD:" << m_sBuildingFile << Qt::endl;
    stream << "BACKS:" << Qt::endl;
    for (CSignMap::iterator it = backsMap.begin(); it != backsMap.end(); ++it) {
      memset(szBuf, 0, sizeof(szBuf));
      snprintf(szBuf, sizeof(szBuf), "%d %d", it->first, (int)it->second);
      stream << szBuf << Qt::endl;
    }
    memset(szBuf, 0, sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), " %4d", -1);
    stream << szBuf << Qt::endl;
    stream << Qt::endl;

    //write info
    if (!(m_raceInfo.iTrackNumber == 0
       && m_raceInfo.iTrackNumber == 0
       && m_raceInfo.iImpossibleLaps == 0
       && m_raceInfo.iHardLaps == 0
       && m_raceInfo.iTrickyLaps == 0
       && m_raceInfo.iMediumLaps == 0
       && m_raceInfo.iEasyLaps == 0
       && m_raceInfo.iGirlieLaps == 0
       && m_raceInfo.fTrackMapSize == 0
       && m_raceInfo.iTrackMapFidelity == 0
       && m_raceInfo.fUnknown == 0)) {
      stream << QString::number(m_raceInfo.iTrackNumber) << Qt::endl;
      memset(szBuf, 0, sizeof(szBuf));
      snprintf(szBuf, sizeof(szBuf), "%4d %4d %4d %4d %4d %4d",
               m_raceInfo.iImpossibleLaps, m_raceInfo.iHardLaps, m_raceInfo.iTrickyLaps,
               m_raceInfo.iMediumLaps, m_raceInfo.iEasyLaps, m_raceInfo.iGirlieLaps);
      stream << szBuf << Qt::endl;
      memset(szBuf, 0, sizeof(szBuf));
      snprintf(szBuf, sizeof(szBuf), "%.2f %4d %.2f",
               m_raceInfo.fTrackMapSize, m_raceInfo.iTrackMapFidelity, m_raceInfo.fUnknown);
      stream << szBuf << Qt::endl << Qt::endl;
    }

    file.close();
  }
  return true;
}

//-------------------------------------------------------------------------------------------------

void CTrack::GetGeometryCursorPos(int iStartIndex, int iEndIndex, int &iStartCursorPos, int &iEndCursorPos)
{
  iStartCursorPos = 0;
  iEndCursorPos = 0;
  if (m_chunkAy.empty()) return;
  if (iEndIndex < iStartIndex || iEndIndex > m_chunkAy.size()) {
    assert(0);
    return;
  }

  for (int i = 0; i <= iEndIndex; ++i) {
    if (i <= iStartIndex)
      iStartCursorPos = iEndCursorPos;
    iEndCursorPos += (int)m_chunkAy[i].sString.size() + 1;
  }
  --iEndCursorPos;
}

//-------------------------------------------------------------------------------------------------

void CTrack::GetGeometryValuesFromSelection(int iStartIndex, int iEndIndex
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
    , QString &sUnk45, QString &sUnk46, QString &sUnk47, QString &sUnk48, QString &sUnk49, QString &sUnk50
    , QString &sSignTexture, QString &sBackTexture
    , QString &sStuntScaleFactor, QString &sStuntAngle, QString &sStuntUnknown, QString &sStuntTimingGroup, QString &sStuntHeight, QString &sStuntTimeBulging
    , QString &sStuntTimeFlat, QString &sStuntExpandsContracts, QString &sStuntBulge)
{
  if (m_chunkAy.empty()) return;
  if (iEndIndex < iStartIndex || iEndIndex >= m_chunkAy.size()) {
    assert(0);
    return;
  }

  sLeftShoulderWidth = ""; sLeftLaneWidth = ""; sRightLaneWidth = ""; sRightShoulderWidth = "";
  sLShoulderHeight = ""; sRShoulderHeight = ""; sLength = "";
  sYaw = ""; sPitch = ""; sRoll = "";
  sAILine1 = ""; sAILine2 = ""; sAILine3 = ""; sAILine4 = "";
  sTrackGrip = ""; sLeftShoulderGrip = ""; sRightShoulderGrip = "";
  sUnk04 = ""; sUnk05 = ""; sUnk06 = ""; sUnk07 = ""; sUnk08 = "";
  sLeftSurfaceType = ""; sCenterSurfaceType = ""; sRightSurfaceType = "";
  sLWallType = ""; sRWallType = ""; sRoofType = ""; sUnk12 = ""; sLOuterExtraWallType = ""; sUnk14 = "";
  sROuterExtraWallType = ""; sUnk16 = ""; sEnvironmentFloorType = ""; sSignType = ""; sUnk19 = ""; sUnk20 = "";
  sfUnk1 = ""; sfUnk2 = ""; sfUnk3 = "";
  sLOuterUpperExtraWallAngle = ""; sLOuterLowerExtraWallAngle = ""; sUnk23 = ""; sUnk24 = ""; sROuterLowerExtraWallAngle = ""; sROuterUpperExtraWallAngle = "";
  sLOuterUpperExtraWallHeight = ""; sLOuterLowerExtraWallHeight = ""; sUnk29 = ""; sUnk30 = ""; sROuterLowerExtraWallHeight = ""; sROuterUpperExtraWallHeight = "";
  sUnk33 = ""; sUnk34 = ""; sUnk35 = ""; sUnk36 = ""; sUnk37 = ""; sUnk38 = "";
  sUnk39 = ""; sUnk40 = ""; sUnk41 = ""; sUnk42 = ""; sUnk43 = ""; sUnk44 = "";
  sUnk45 = ""; sUnk46 = ""; sUnk47 = ""; sUnk48 = ""; sUnk49 = ""; sUnk50 = "";
  sSignTexture = ""; sBackTexture = "";
  sStuntScaleFactor = ""; sStuntAngle = ""; sStuntUnknown = ""; sStuntTimingGroup = ""; sStuntHeight = ""; sStuntTimeBulging = "";
  sStuntTimeFlat = ""; sStuntExpandsContracts = ""; sStuntBulge = "";

  QString sVal;
  for (int i = iStartIndex; i <= iEndIndex; ++i) {
    sVal = QString::number(m_chunkAy[i].iLeftShoulderWidth);
    if (sLeftShoulderWidth.isEmpty()) sLeftShoulderWidth = sVal;
    else if (sLeftShoulderWidth.compare(sVal) != 0) sLeftShoulderWidth = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLeftLaneWidth);
    if (sLeftLaneWidth.isEmpty()) sLeftLaneWidth = sVal;
    else if (sLeftLaneWidth.compare(sVal) != 0) sLeftLaneWidth = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightLaneWidth);
    if (sRightLaneWidth.isEmpty()) sRightLaneWidth = sVal;
    else if (sRightLaneWidth.compare(sVal) != 0) sRightLaneWidth = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightShoulderWidth);
    if (sRightShoulderWidth.isEmpty()) sRightShoulderWidth = sVal;
    else if (sRightShoulderWidth.compare(sVal) != 0) sRightShoulderWidth = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iLeftShoulderHeight);
    if (sLShoulderHeight.isEmpty()) sLShoulderHeight = sVal;
    else if (sLShoulderHeight.compare(sVal) != 0) sLShoulderHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightShoulderHeight);
    if (sRShoulderHeight.isEmpty()) sRShoulderHeight = sVal;
    else if (sRShoulderHeight.compare(sVal) != 0) sRShoulderHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLength);
    if (sLength.isEmpty()) sLength = sVal;
    else if (sLength.compare(sVal) != 0) sLength = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].fYaw, 'f', 5);
    if (sYaw.isEmpty()) sYaw = sVal;
    else if (sYaw.compare(sVal) != 0) sYaw = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].fPitch, 'f', 5);
    if (sPitch.isEmpty()) sPitch = sVal;
    else if (sPitch.compare(sVal) != 0) sPitch = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].fRoll, 'f', 5);
    if (sRoll.isEmpty()) sRoll = sVal;
    else if (sRoll.compare(sVal) != 0) sRoll = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iAILine1);
    if (sAILine1.isEmpty()) sAILine1 = sVal;
    else if (sAILine1.compare(sVal) != 0) sAILine1 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iAILine2);
    if (sAILine2.isEmpty()) sAILine2 = sVal;
    else if (sAILine2.compare(sVal) != 0) sAILine2 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iAILine3);
    if (sAILine3.isEmpty()) sAILine3 = sVal;
    else if (sAILine3.compare(sVal) != 0) sAILine3 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iAILine4);
    if (sAILine4.isEmpty()) sAILine4 = sVal;
    else if (sAILine4.compare(sVal) != 0) sAILine4 = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iTrackGrip);
    if (sTrackGrip.isEmpty()) sTrackGrip = sVal;
    else if (sTrackGrip.compare(sVal) != 0) sTrackGrip = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLeftShoulderGrip);
    if (sLeftShoulderGrip.isEmpty()) sLeftShoulderGrip = sVal;
    else if (sLeftShoulderGrip.compare(sVal) != 0) sLeftShoulderGrip = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightShoulderGrip);
    if (sRightShoulderGrip.isEmpty()) sRightShoulderGrip = sVal;
    else if (sRightShoulderGrip.compare(sVal) != 0) sRightShoulderGrip = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iUnk04);
    if (sUnk04.isEmpty()) sUnk04 = sVal;
    else if (sUnk04.compare(sVal) != 0) sUnk04 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk05);
    if (sUnk05.isEmpty()) sUnk05 = sVal;
    else if (sUnk05.compare(sVal) != 0) sUnk05 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk06);
    if (sUnk06.isEmpty()) sUnk06 = sVal;
    else if (sUnk06.compare(sVal) != 0) sUnk06 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk07);
    if (sUnk07.isEmpty()) sUnk07 = sVal;
    else if (sUnk07.compare(sVal) != 0) sUnk07 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk08);
    if (sUnk08.isEmpty()) sUnk08 = sVal;
    else if (sUnk08.compare(sVal) != 0) sUnk08 = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iLeftSurfaceType);
    if (sLeftSurfaceType.isEmpty()) sLeftSurfaceType = sVal;
    else if (sLeftSurfaceType.compare(sVal) != 0) sLeftSurfaceType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iCenterSurfaceType);
    if (sCenterSurfaceType.isEmpty()) sCenterSurfaceType = sVal;
    else if (sCenterSurfaceType.compare(sVal) != 0) sCenterSurfaceType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightSurfaceType);
    if (sRightSurfaceType.isEmpty()) sRightSurfaceType = sVal;
    else if (sRightSurfaceType.compare(sVal) != 0) sRightSurfaceType = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iLeftWallType);
    if (sLWallType.isEmpty()) sLWallType = sVal;
    else if (sLWallType.compare(sVal) != 0) sLWallType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightWallType);
    if (sRWallType.isEmpty()) sRWallType = sVal;
    else if (sRWallType.compare(sVal) != 0) sRWallType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRoofType);
    if (sRoofType.isEmpty()) sRoofType = sVal;
    else if (sRoofType.compare(sVal) != 0) sRoofType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk12);
    if (sUnk12.isEmpty()) sUnk12 = sVal;
    else if (sUnk12.compare(sVal) != 0) sUnk12 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLeftOuterExtraWallType);
    if (sLOuterExtraWallType.isEmpty()) sLOuterExtraWallType = sVal;
    else if (sLOuterExtraWallType.compare(sVal) != 0) sLOuterExtraWallType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk14);
    if (sUnk14.isEmpty()) sUnk14 = sVal;
    else if (sUnk14.compare(sVal) != 0) sUnk14 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightOuterExtraWallType);
    if (sROuterExtraWallType.isEmpty()) sROuterExtraWallType = sVal;
    else if (sROuterExtraWallType.compare(sVal) != 0) sROuterExtraWallType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk16);
    if (sUnk16.isEmpty()) sUnk16 = sVal;
    else if (sUnk16.compare(sVal) != 0) sUnk16 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iEnvironmentFloorType);
    if (sEnvironmentFloorType.isEmpty()) sEnvironmentFloorType = sVal;
    else if (sEnvironmentFloorType.compare(sVal) != 0) sEnvironmentFloorType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iSignType);
    if (sSignType.isEmpty()) sSignType = sVal;
    else if (sSignType.compare(sVal) != 0) sSignType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk19);
    if (sUnk19.isEmpty()) sUnk19 = sVal;
    else if (sUnk19.compare(sVal) != 0) sUnk19 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk20);
    if (sUnk20.isEmpty()) sUnk20 = sVal;
    else if (sUnk20.compare(sVal) != 0) sUnk20 = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].fUnk1, 'f', 1);
    if (sfUnk1.isEmpty()) sfUnk1 = sVal;
    else if (sfUnk1.compare(sVal) != 0) sfUnk1 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].fUnk2, 'f', 1);
    if (sfUnk2.isEmpty()) sfUnk2 = sVal;
    else if (sfUnk2.compare(sVal) != 0) sfUnk2 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].fUnk3, 'f', 1);
    if (sfUnk3.isEmpty()) sfUnk3 = sVal;
    else if (sfUnk3.compare(sVal) != 0) sfUnk3 = MIXED_DATA;
 
    sVal = QString::number(m_chunkAy[i].iLeftOuterUpperExtraWallAngle);
    if (sLOuterUpperExtraWallAngle.isEmpty()) sLOuterUpperExtraWallAngle = sVal;
    else if (sLOuterUpperExtraWallAngle.compare(sVal) != 0) sLOuterUpperExtraWallAngle = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLeftOuterLowerExtraWallAngle);
    if (sLOuterLowerExtraWallAngle.isEmpty()) sLOuterLowerExtraWallAngle = sVal;
    else if (sLOuterLowerExtraWallAngle.compare(sVal) != 0) sLOuterLowerExtraWallAngle = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk23);
    if (sUnk23.isEmpty()) sUnk23 = sVal;
    else if (sUnk23.compare(sVal) != 0) sUnk23 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk24);
    if (sUnk24.isEmpty()) sUnk24 = sVal;
    else if (sUnk24.compare(sVal) != 0) sUnk24 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightOuterLowerExtraWallAngle);
    if (sROuterLowerExtraWallAngle.isEmpty()) sROuterLowerExtraWallAngle = sVal;
    else if (sROuterLowerExtraWallAngle.compare(sVal) != 0) sROuterLowerExtraWallAngle = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightOuterUpperExtraWallAngle);
    if (sROuterUpperExtraWallAngle.isEmpty()) sROuterUpperExtraWallAngle = sVal;
    else if (sROuterUpperExtraWallAngle.compare(sVal) != 0) sROuterUpperExtraWallAngle = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLeftOuterUpperExtraWallHeight);
    if (sLOuterUpperExtraWallHeight.isEmpty()) sLOuterUpperExtraWallHeight = sVal;
    else if (sLOuterUpperExtraWallHeight.compare(sVal) != 0) sLOuterUpperExtraWallHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLeftOuterLowerExtraWallHeight);
    if (sLOuterLowerExtraWallHeight.isEmpty()) sLOuterLowerExtraWallHeight = sVal;
    else if (sLOuterLowerExtraWallHeight.compare(sVal) != 0) sLOuterLowerExtraWallHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk29);
    if (sUnk29.isEmpty()) sUnk29 = sVal;
    else if (sUnk29.compare(sVal) != 0) sUnk29 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk30);
    if (sUnk30.isEmpty()) sUnk30 = sVal;
    else if (sUnk30.compare(sVal) != 0) sUnk30 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightOuterLowerExtraWallHeight);
    if (sROuterLowerExtraWallHeight.isEmpty()) sROuterLowerExtraWallHeight = sVal;
    else if (sROuterLowerExtraWallHeight.compare(sVal) != 0) sROuterLowerExtraWallHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightOuterUpperExtraWallHeight);
    if (sROuterUpperExtraWallHeight.isEmpty()) sROuterUpperExtraWallHeight = sVal;
    else if (sROuterUpperExtraWallHeight.compare(sVal) != 0) sROuterUpperExtraWallHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk33);
    if (sUnk33.isEmpty()) sUnk33 = sVal;
    else if (sUnk33.compare(sVal) != 0) sUnk33 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk34);
    if (sUnk34.isEmpty()) sUnk34 = sVal;
    else if (sUnk34.compare(sVal) != 0) sUnk34 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk35);
    if (sUnk35.isEmpty()) sUnk35 = sVal;
    else if (sUnk35.compare(sVal) != 0) sUnk35 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk36);
    if (sUnk36.isEmpty()) sUnk36 = sVal;
    else if (sUnk36.compare(sVal) != 0) sUnk36 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk37);
    if (sUnk37.isEmpty()) sUnk37 = sVal;
    else if (sUnk37.compare(sVal) != 0) sUnk37 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk38);
    if (sUnk38.isEmpty()) sUnk38 = sVal;
    else if (sUnk38.compare(sVal) != 0) sUnk38 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk39);
    if (sUnk39.isEmpty()) sUnk39 = sVal;
    else if (sUnk39.compare(sVal) != 0) sUnk39 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk40);
    if (sUnk40.isEmpty()) sUnk40 = sVal;
    else if (sUnk40.compare(sVal) != 0) sUnk40 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk41);
    if (sUnk41.isEmpty()) sUnk41 = sVal;
    else if (sUnk41.compare(sVal) != 0) sUnk41 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk42);
    if (sUnk42.isEmpty()) sUnk42 = sVal;
    else if (sUnk42.compare(sVal) != 0) sUnk42 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk43);
    if (sUnk43.isEmpty()) sUnk43 = sVal;
    else if (sUnk43.compare(sVal) != 0) sUnk43 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk44);
    if (sUnk44.isEmpty()) sUnk44 = sVal;
    else if (sUnk44.compare(sVal) != 0) sUnk44 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk45);
    if (sUnk45.isEmpty()) sUnk45 = sVal;
    else if (sUnk45.compare(sVal) != 0) sUnk45 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk46);
    if (sUnk46.isEmpty()) sUnk46 = sVal;
    else if (sUnk46.compare(sVal) != 0) sUnk46 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk47);
    if (sUnk47.isEmpty()) sUnk47 = sVal;
    else if (sUnk47.compare(sVal) != 0) sUnk47 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk48);
    if (sUnk48.isEmpty()) sUnk48 = sVal;
    else if (sUnk48.compare(sVal) != 0) sUnk48 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk49);
    if (sUnk49.isEmpty()) sUnk49 = sVal;
    else if (sUnk49.compare(sVal) != 0) sUnk49 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk50);
    if (sUnk50.isEmpty()) sUnk50 = sVal;
    else if (sUnk50.compare(sVal) != 0) sUnk50 = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iSignTexture);
    if (sSignTexture.isEmpty()) sSignTexture = sVal;
    else if (sSignTexture.compare(sVal) != 0) sSignTexture = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iBackTexture);
    if (sBackTexture.isEmpty()) sBackTexture = sVal;
    else if (sBackTexture.compare(sVal) != 0) sBackTexture = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].stunt.iScaleFactor);
    if (sStuntScaleFactor.isEmpty()) sStuntScaleFactor = sVal;
    else if (sStuntScaleFactor.compare(sVal) != 0) sStuntScaleFactor = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iAngle);
    if (sStuntAngle.isEmpty()) sStuntAngle = sVal;
    else if (sStuntAngle.compare(sVal) != 0) sStuntAngle = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iUnknown);
    if (sStuntUnknown.isEmpty()) sStuntUnknown = sVal;
    else if (sStuntUnknown.compare(sVal) != 0) sStuntUnknown = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iTimingGroup);
    if (sStuntTimingGroup.isEmpty()) sStuntTimingGroup = sVal;
    else if (sStuntTimingGroup.compare(sVal) != 0) sStuntTimingGroup = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iHeight);
    if (sStuntHeight.isEmpty()) sStuntHeight = sVal;
    else if (sStuntHeight.compare(sVal) != 0) sStuntHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iTimeBulging);
    if (sStuntTimeBulging.isEmpty()) sStuntTimeBulging = sVal;
    else if (sStuntTimeBulging.compare(sVal) != 0) sStuntTimeBulging = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iTimeFlat);
    if (sStuntTimeFlat.isEmpty()) sStuntTimeFlat = sVal;
    else if (sStuntTimeFlat.compare(sVal) != 0) sStuntTimeFlat = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iSmallerExpandsLargerContracts);
    if (sStuntExpandsContracts.isEmpty()) sStuntExpandsContracts = sVal;
    else if (sStuntExpandsContracts.compare(sVal) != 0) sStuntExpandsContracts = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iBulge);
    if (sStuntBulge.isEmpty()) sStuntBulge = sVal;
    else if (sStuntBulge.compare(sVal) != 0) sStuntBulge = MIXED_DATA;
  }
}

//-------------------------------------------------------------------------------------------------

void CTrack::ApplyGeometrySettings(int iStartIndex, int iEndIndex
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
    , const QString &sUnk45, const QString &sUnk46, const QString &sUnk47, const QString &sUnk48, const QString &sUnk49, const QString &sUnk50
    , const QString &sSignValue, const QString &sBackValue
    , const QString &sStuntScaleFactor, const QString &sStuntAngle, const QString &sStuntUnknown, const QString &sStuntTimingGroup, const QString &sStuntHeight, const QString &sStuntTimeBulging
    , const QString &sStuntTimeFlat, const QString &sStuntExpandsContracts, const QString &sStuntBulge)
{
  for (int i = iStartIndex; i <= iEndIndex; ++i) {
    if (!sLeftShoulderWidth.isEmpty()) m_chunkAy[i].iLeftShoulderWidth = sLeftShoulderWidth.toInt();
    if (!sLeftLaneWidth.isEmpty()) m_chunkAy[i].iLeftLaneWidth = sLeftLaneWidth.toInt();
    if (!sRightLaneWidth.isEmpty()) m_chunkAy[i].iRightLaneWidth = sRightLaneWidth.toInt();
    if (!sRightShoulderWidth.isEmpty()) m_chunkAy[i].iRightShoulderWidth = sRightShoulderWidth.toInt();
    if (!sLShoulderHeight.isEmpty()) m_chunkAy[i].iLeftShoulderHeight = sLShoulderHeight.toInt();
    if (!sRShoulderHeight.isEmpty()) m_chunkAy[i].iRightShoulderHeight = sRShoulderHeight.toInt();
    if (!sLength.isEmpty()) m_chunkAy[i].iLength = sLength.toInt();
    if (!sYaw.isEmpty()) m_chunkAy[i].fYaw = sYaw.toFloat();
    if (!sPitch.isEmpty()) m_chunkAy[i].fPitch = sPitch.toFloat();
    if (!sRoll.isEmpty()) m_chunkAy[i].fRoll = sRoll.toFloat();
    if (!sAILine1.isEmpty()) m_chunkAy[i].iAILine1 = sAILine1.toInt();
    if (!sAILine2.isEmpty()) m_chunkAy[i].iAILine2 = sAILine2.toInt();
    if (!sAILine3.isEmpty()) m_chunkAy[i].iAILine3 = sAILine3.toInt();
    if (!sAILine4.isEmpty()) m_chunkAy[i].iAILine4 = sAILine4.toInt();
    if (!sTrackGrip.isEmpty()) m_chunkAy[i].iTrackGrip = sTrackGrip.toInt();
    if (!sLeftShoulderGrip.isEmpty()) m_chunkAy[i].iLeftShoulderGrip = sLeftShoulderGrip.toInt();
    if (!sRightShoulderGrip.isEmpty()) m_chunkAy[i].iRightShoulderGrip = sRightShoulderGrip.toInt();
    if (!sUnk04.isEmpty()) m_chunkAy[i].iUnk04 = sUnk04.toInt();
    if (!sUnk05.isEmpty()) m_chunkAy[i].iUnk05 = sUnk05.toInt();
    if (!sUnk06.isEmpty()) m_chunkAy[i].iUnk06 = sUnk06.toInt();
    if (!sUnk07.isEmpty()) m_chunkAy[i].iUnk07 = sUnk07.toInt();
    if (!sUnk08.isEmpty()) m_chunkAy[i].iUnk08 = sUnk08.toInt();
    if (!sLeftSurfaceType.isEmpty()) m_chunkAy[i].iLeftSurfaceType = sLeftSurfaceType.toInt();
    if (!sCenterSurfaceType.isEmpty()) m_chunkAy[i].iCenterSurfaceType = sCenterSurfaceType.toInt();
    if (!sRightSurfaceType.isEmpty()) m_chunkAy[i].iRightSurfaceType = sRightSurfaceType.toInt();
    if (!sLWallType.isEmpty()) m_chunkAy[i].iLeftWallType = sLWallType.toInt();
    if (!sRWallType.isEmpty()) m_chunkAy[i].iRightWallType = sRWallType.toInt();
    if (!sRoofType.isEmpty()) m_chunkAy[i].iRoofType = sRoofType.toInt();
    if (!sUnk12.isEmpty()) m_chunkAy[i].iUnk12 = sUnk12.toInt();
    if (!sLOuterExtraWallType.isEmpty()) m_chunkAy[i].iLeftOuterExtraWallType = sLOuterExtraWallType.toInt();
    if (!sUnk14.isEmpty()) m_chunkAy[i].iUnk14 = sUnk14.toInt();
    if (!sROuterExtraWallType.isEmpty()) m_chunkAy[i].iRightOuterExtraWallType = sROuterExtraWallType.toInt();
    if (!sUnk16.isEmpty()) m_chunkAy[i].iUnk16 = sUnk16.toInt();
    if (!sEnvironmentFloorType.isEmpty()) m_chunkAy[i].iEnvironmentFloorType = sEnvironmentFloorType.toInt();
    if (!sSignType.isEmpty()) m_chunkAy[i].iSignType = sSignType.toInt();
    if (!sUnk19.isEmpty()) m_chunkAy[i].iUnk19 = sUnk19.toInt();
    if (!sUnk20.isEmpty()) m_chunkAy[i].iUnk20 = sUnk20.toInt();
    if (!sfUnk1.isEmpty()) m_chunkAy[i].fUnk1 = sfUnk1.toFloat();
    if (!sfUnk2.isEmpty()) m_chunkAy[i].fUnk2 = sfUnk2.toFloat();
    if (!sfUnk3.isEmpty()) m_chunkAy[i].fUnk3 = sfUnk3.toFloat();
    if (!sLOuterUpperExtraWallAngle.isEmpty()) m_chunkAy[i].iLeftOuterUpperExtraWallAngle = sLOuterUpperExtraWallAngle.toInt();
    if (!sLOuterLowerExtraWallAngle.isEmpty()) m_chunkAy[i].iLeftOuterLowerExtraWallAngle = sLOuterLowerExtraWallAngle.toInt();
    if (!sUnk23.isEmpty()) m_chunkAy[i].iUnk23 = sUnk23.toInt();
    if (!sUnk24.isEmpty()) m_chunkAy[i].iUnk24 = sUnk24.toInt();
    if (!sROuterLowerExtraWallAngle.isEmpty()) m_chunkAy[i].iRightOuterLowerExtraWallAngle = sROuterLowerExtraWallAngle.toInt();
    if (!sROuterUpperExtraWallAngle.isEmpty()) m_chunkAy[i].iRightOuterUpperExtraWallAngle = sROuterUpperExtraWallAngle.toInt();
    if (!sLOuterUpperExtraWallHeight.isEmpty()) m_chunkAy[i].iLeftOuterUpperExtraWallHeight = sLOuterUpperExtraWallHeight.toInt();
    if (!sLOuterLowerExtraWallHeight.isEmpty()) m_chunkAy[i].iLeftOuterLowerExtraWallHeight = sLOuterLowerExtraWallHeight.toInt();
    if (!sUnk29.isEmpty()) m_chunkAy[i].iUnk29 = sUnk29.toInt();
    if (!sUnk30.isEmpty()) m_chunkAy[i].iUnk30 = sUnk30.toInt();
    if (!sROuterLowerExtraWallHeight.isEmpty()) m_chunkAy[i].iRightOuterLowerExtraWallHeight = sROuterLowerExtraWallHeight.toInt();
    if (!sROuterUpperExtraWallHeight.isEmpty()) m_chunkAy[i].iRightOuterUpperExtraWallHeight = sROuterUpperExtraWallHeight.toInt();
    if (!sUnk33.isEmpty()) m_chunkAy[i].iUnk33 = sUnk33.toInt();
    if (!sUnk34.isEmpty()) m_chunkAy[i].iUnk34 = sUnk34.toInt();
    if (!sUnk35.isEmpty()) m_chunkAy[i].iUnk35 = sUnk35.toInt();
    if (!sUnk36.isEmpty()) m_chunkAy[i].iUnk36 = sUnk36.toInt();
    if (!sUnk37.isEmpty()) m_chunkAy[i].iUnk37 = sUnk37.toInt();
    if (!sUnk38.isEmpty()) m_chunkAy[i].iUnk38 = sUnk38.toInt();
    if (!sUnk39.isEmpty()) m_chunkAy[i].iUnk39 = sUnk39.toInt();
    if (!sUnk40.isEmpty()) m_chunkAy[i].iUnk40 = sUnk40.toInt();
    if (!sUnk41.isEmpty()) m_chunkAy[i].iUnk41 = sUnk41.toInt();
    if (!sUnk42.isEmpty()) m_chunkAy[i].iUnk42 = sUnk42.toInt();
    if (!sUnk43.isEmpty()) m_chunkAy[i].iUnk43 = sUnk43.toInt();
    if (!sUnk44.isEmpty()) m_chunkAy[i].iUnk44 = sUnk44.toInt();
    if (!sUnk45.isEmpty()) m_chunkAy[i].iUnk45 = sUnk45.toInt();
    if (!sUnk46.isEmpty()) m_chunkAy[i].iUnk46 = sUnk46.toInt();
    if (!sUnk47.isEmpty()) m_chunkAy[i].iUnk47 = sUnk47.toInt();
    if (!sUnk48.isEmpty()) m_chunkAy[i].iUnk48 = sUnk48.toInt();
    if (!sUnk49.isEmpty()) m_chunkAy[i].iUnk49 = sUnk49.toInt();
    if (!sUnk50.isEmpty()) m_chunkAy[i].iUnk50 = sUnk50.toInt();
    if (!sSignValue.isEmpty()) m_chunkAy[i].iSignTexture = sSignValue.toInt();
    if (!sBackValue.isEmpty()) m_chunkAy[i].iBackTexture = sBackValue.toInt();
    if (!sStuntScaleFactor.isEmpty()) m_chunkAy[i].stunt.iScaleFactor = sStuntScaleFactor.toInt();
    if (!sStuntAngle.isEmpty()) m_chunkAy[i].stunt.iAngle = sStuntAngle.toInt();
    if (!sStuntUnknown.isEmpty()) m_chunkAy[i].stunt.iUnknown = sStuntUnknown.toInt();
    if (!sStuntTimingGroup.isEmpty()) m_chunkAy[i].stunt.iTimingGroup = sStuntTimingGroup.toInt();
    if (!sStuntHeight.isEmpty()) m_chunkAy[i].stunt.iHeight = sStuntHeight.toInt();
    if (!sStuntTimeBulging.isEmpty()) m_chunkAy[i].stunt.iTimeBulging = sStuntTimeBulging.toInt();
    if (!sStuntTimeFlat.isEmpty()) m_chunkAy[i].stunt.iTimeFlat = sStuntTimeFlat.toInt();
    if (!sStuntExpandsContracts.isEmpty()) m_chunkAy[i].stunt.iSmallerExpandsLargerContracts = sStuntExpandsContracts.toInt();
    if (!sStuntBulge.isEmpty()) m_chunkAy[i].stunt.iBulge = sStuntBulge.toInt();
  }
  UpdateChunkStrings();
  g_pMainWindow->LogMessage("Applied changes to " + QString::number(iEndIndex - iStartIndex + 1) + " geometry chunks");
}

//-------------------------------------------------------------------------------------------------

void CTrack::InsertGeometryChunk(int iIndex, int iCount
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
  , const QString &sUnk45, const QString &sUnk46, const QString &sUnk47, const QString &sUnk48, const QString &sUnk49, const QString &sUnk50
  , const QString &sSignValue, const QString &sBackValue
  , const QString &sStuntScaleFactor, const QString &sStuntAngle, const QString &sStuntUnknown, const QString &sStuntTimingGroup, const QString &sStuntHeight, const QString &sStuntTimeBulging
  , const QString &sStuntTimeFlat, const QString &sStuntExpandsContracts, const QString &sStuntBulge)
{
  for (int i = 0; i < iCount; ++i) {
    struct tGeometryChunk newChunk;
    memset(&newChunk, sizeof(newChunk), 0);
    newChunk.iLeftShoulderWidth = sLeftShoulderWidth.toInt();
    newChunk.iLeftLaneWidth = sLeftLaneWidth.toInt();
    newChunk.iRightLaneWidth = sRightLaneWidth.toInt();
    newChunk.iRightShoulderWidth = sRightShoulderWidth.toInt();
    newChunk.iLeftShoulderHeight = sLShoulderHeight.toInt();
    newChunk.iRightShoulderHeight = sRShoulderHeight.toInt();
    newChunk.iLength = sLength.toInt();
    newChunk.fYaw = sYaw.toFloat();
    newChunk.fPitch = sPitch.toFloat();
    newChunk.fRoll = sRoll.toFloat();
    newChunk.iAILine1 = sAILine1.toInt();
    newChunk.iAILine2 = sAILine2.toInt();
    newChunk.iAILine3 = sAILine3.toInt();
    newChunk.iAILine4 = sAILine4.toInt();
    newChunk.iTrackGrip = sTrackGrip.toInt();
    newChunk.iLeftShoulderGrip = sLeftShoulderGrip.toInt();
    newChunk.iRightShoulderGrip = sRightShoulderGrip.toInt();
    newChunk.iUnk04 = sUnk04.toInt();
    newChunk.iUnk05 = sUnk05.toInt();
    newChunk.iUnk06 = sUnk06.toInt();
    newChunk.iUnk07 = sUnk07.toInt();
    newChunk.iUnk08 = sUnk08.toInt();
    newChunk.iLeftSurfaceType = sLeftSurfaceType.toInt();
    newChunk.iCenterSurfaceType = sCenterSurfaceType.toInt();
    newChunk.iRightSurfaceType = sRightSurfaceType.toInt();
    newChunk.iLeftWallType = sLWallType.toInt();
    newChunk.iRightWallType = sRWallType.toInt();
    newChunk.iRoofType = sRoofType.toInt();
    newChunk.iUnk12 = sUnk12.toInt();
    newChunk.iLeftOuterExtraWallType = sLOuterExtraWallType.toInt();
    newChunk.iUnk14 = sUnk14.toInt();
    newChunk.iRightOuterExtraWallType = sROuterExtraWallType.toInt();
    newChunk.iUnk16 = sUnk16.toInt();
    newChunk.iEnvironmentFloorType = sEnvironmentFloorType.toInt();
    newChunk.iSignType = sSignType.toInt();
    newChunk.iUnk19 = sUnk19.toInt();
    newChunk.iUnk20 = sUnk20.toInt();
    newChunk.fUnk1 = sfUnk1.toFloat();
    newChunk.fUnk2 = sfUnk2.toFloat();
    newChunk.fUnk3 = sfUnk3.toFloat();
    newChunk.iLeftOuterUpperExtraWallAngle = sLOuterUpperExtraWallAngle.toInt();
    newChunk.iLeftOuterLowerExtraWallAngle = sLOuterLowerExtraWallAngle.toInt();
    newChunk.iUnk23 = sUnk23.toInt();
    newChunk.iUnk24 = sUnk24.toInt();
    newChunk.iRightOuterLowerExtraWallAngle = sROuterLowerExtraWallAngle.toInt();
    newChunk.iRightOuterUpperExtraWallAngle = sROuterUpperExtraWallAngle.toInt();
    newChunk.iLeftOuterUpperExtraWallHeight = sLOuterUpperExtraWallHeight.toInt();
    newChunk.iLeftOuterLowerExtraWallHeight = sLOuterLowerExtraWallHeight.toInt();
    newChunk.iUnk29 = sUnk29.toInt();
    newChunk.iUnk30 = sUnk30.toInt();
    newChunk.iRightOuterLowerExtraWallHeight = sROuterLowerExtraWallHeight.toInt();
    newChunk.iRightOuterUpperExtraWallHeight = sROuterUpperExtraWallHeight.toInt();
    newChunk.iUnk33 = sUnk33.toInt();
    newChunk.iUnk34 = sUnk34.toInt();
    newChunk.iUnk35 = sUnk35.toInt();
    newChunk.iUnk36 = sUnk36.toInt();
    newChunk.iUnk37 = sUnk37.toInt();
    newChunk.iUnk38 = sUnk38.toInt();
    newChunk.iUnk39 = sUnk39.toInt();
    newChunk.iUnk40 = sUnk40.toInt();
    newChunk.iUnk41 = sUnk41.toInt();
    newChunk.iUnk42 = sUnk42.toInt();
    newChunk.iUnk43 = sUnk43.toInt();
    newChunk.iUnk44 = sUnk44.toInt();
    newChunk.iUnk45 = sUnk45.toInt();
    newChunk.iUnk46 = sUnk46.toInt();
    newChunk.iUnk47 = sUnk47.toInt();
    newChunk.iUnk48 = sUnk48.toInt();
    newChunk.iUnk49 = sUnk49.toInt();
    newChunk.iUnk50 = sUnk50.toInt();
    newChunk.iSignTexture = sSignValue.toInt();
    newChunk.iBackTexture = sBackValue.toInt();
    newChunk.stunt.iScaleFactor = sStuntScaleFactor.toInt();
    newChunk.stunt.iAngle = sStuntAngle.toInt();
    newChunk.stunt.iUnknown = sStuntUnknown.toInt();
    newChunk.stunt.iTimingGroup = sStuntTimingGroup.toInt();
    newChunk.stunt.iHeight = sStuntHeight.toInt();
    newChunk.stunt.iTimeBulging = sStuntTimeBulging.toInt();
    newChunk.stunt.iTimeFlat = sStuntTimeFlat.toInt();
    newChunk.stunt.iSmallerExpandsLargerContracts = sStuntExpandsContracts.toInt();
    newChunk.stunt.iBulge = sStuntBulge.toInt();
    if (m_chunkAy.empty())
      m_chunkAy.push_back(newChunk);
    else
      m_chunkAy.insert(m_chunkAy.begin() + iIndex, newChunk);
  }
  UpdateChunkStrings();
  g_pMainWindow->LogMessage("Inserted " + QString::number(iCount) + " geometry chunks");
}

//-------------------------------------------------------------------------------------------------

void CTrack::UpdateChunkStrings()
{
  for (int i = 0; i < m_chunkAy.size(); ++i) {
    GenerateChunkString(m_chunkAy[i]);
  }
}

//-------------------------------------------------------------------------------------------------

void CTrack::GenerateChunkString(tGeometryChunk &chunk)
{
  char szGenerate[1024];
  snprintf(szGenerate, sizeof(szGenerate),
           " %4d %6d %6d %6d %6d %6d %6d %11.5f %11.5f %11.5f %5d %5d %5d %5d %3d %3d %3d %4d %5d %3d %3d %3d\n" //line 1
           "%6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %4d %6d %6d %6.1f %6.1f %6.1f\n"   //line 2
           " %4d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d"                             //line 3 TODO: first val does not have space if negative
           " %3d %3d %3d %d %d %d %d %d %d %d %d %d %d %d %3d %3d %3d\n"       //line 3 continued
           , chunk.iLeftShoulderWidth
           , chunk.iLeftLaneWidth
           , chunk.iRightLaneWidth
           , chunk.iRightShoulderWidth
           , chunk.iLeftShoulderHeight
           , chunk.iRightShoulderHeight
           , chunk.iLength
           , chunk.fYaw
           , chunk.fPitch
           , chunk.fRoll
           , chunk.iAILine1
           , chunk.iAILine2
           , chunk.iAILine3
           , chunk.iAILine4
           , chunk.iTrackGrip
           , chunk.iLeftShoulderGrip
           , chunk.iRightShoulderGrip
           , chunk.iUnk04
           , chunk.iUnk05
           , chunk.iUnk06
           , chunk.iUnk07
           , chunk.iUnk08
           , chunk.iLeftSurfaceType
           , chunk.iCenterSurfaceType
           , chunk.iRightSurfaceType
           , chunk.iLeftWallType
           , chunk.iRightWallType
           , chunk.iRoofType
           , chunk.iUnk12
           , chunk.iLeftOuterExtraWallType
           , chunk.iUnk14
           , chunk.iRightOuterExtraWallType
           , chunk.iUnk16
           , chunk.iEnvironmentFloorType
           , chunk.iSignType
           , chunk.iUnk19
           , chunk.iUnk20
           , chunk.fUnk1
           , chunk.fUnk2
           , chunk.fUnk3
           , chunk.iLeftOuterUpperExtraWallAngle
           , chunk.iLeftOuterLowerExtraWallAngle
           , chunk.iUnk23
           , chunk.iUnk24
           , chunk.iRightOuterLowerExtraWallAngle
           , chunk.iRightOuterUpperExtraWallAngle
           , chunk.iLeftOuterUpperExtraWallHeight
           , chunk.iLeftOuterLowerExtraWallHeight
           , chunk.iUnk29
           , chunk.iUnk30
           , chunk.iRightOuterLowerExtraWallHeight
           , chunk.iRightOuterUpperExtraWallHeight
           , chunk.iUnk33
           , chunk.iUnk34
           , chunk.iUnk35
           , chunk.iUnk36
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
  chunk.sString = szGenerate;
}

//-------------------------------------------------------------------------------------------------

void CTrack::ProcessSign(const QStringList &slLine, eFileSection &section)
{
  //helper function because this process must be done in two places
  int iVal0 = slLine[0].toInt();
  int iVal1 = slLine[1].toInt();
  if (iVal0 == -1 || iVal1 == -1) {
    //sign section always ends in two -1 values
    section = STUNTS;
  } else {
    //process sign
    if (iVal0 < m_chunkAy.size()) {
      m_chunkAy[iVal0].iSignTexture = slLine[1].toInt();
    }
  }
}

//-------------------------------------------------------------------------------------------------