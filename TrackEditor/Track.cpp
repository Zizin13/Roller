#include "TrackEditor.h"
#include "Track.h"
#include "MainWindow.h"
#include "qfile.h"
#include "qtextstream.h"
#include "Unmangler.h"
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
  m_bIsMangled = false;
  memset(&m_raceInfo, 0, sizeof(m_raceInfo));
}

//-------------------------------------------------------------------------------------------------

bool CTrack::LoadTrack(const QString &sFilename, bool bIsMangled)
{
  ClearData();
  m_bIsMangled = true;

  if (sFilename.isEmpty()) {
    g_pMainWindow->LogMessage("Track filename empty: " + sFilename);
    return false;
  }

  QFile file(sFilename);
  if (!file.open(QIODevice::ReadOnly)) {
    g_pMainWindow->LogMessage("Failed to open track: " + sFilename);
    return false;
  }

  QByteArray baData = file.readAll();
  bool bSuccess = false;

  if (bIsMangled) {
    int iLength = GetUnmangledLength((uint8_t *)baData.constData(), baData.size());
    uint8_t *szData = new uint8_t[iLength];
    UnmangleFile((uint8_t *)baData.constData(), baData.size(), szData, iLength);
    bSuccess = ProcessTrackData(QByteArray((char *)szData, iLength));
    delete[] szData;
  } else {
    bSuccess = ProcessTrackData(baData);
  }

  file.close();
  QString sSuccess = (bSuccess ? "Successfully loaded" : "Failed to load");
  QString sLogMsg = sSuccess + " file " + sFilename + "\n"
    + "  geometry chunks: " + QString::number(m_chunkAy.size()) + "\n"
    + "  texture file: " + m_sTextureFile + "\n"
    + "  building file: " + m_sBuildingFile;
  g_pMainWindow->LogMessage(sLogMsg);

  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

bool CTrack::ProcessTrackData(const QByteArray &baData)
{
  bool bSuccess = true;
  int iChunkLine = 0;
  struct tGeometryChunk currChunk;
  eFileSection section = HEADER;
  QList<QByteArray> listData = baData.split('\n');

  for (const QByteArray &baLine : listData) {
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
            currChunk.dYaw = slLine[7].toDouble();
            currChunk.dPitch = slLine[8].toDouble();
            currChunk.dRoll = slLine[9].toDouble();
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
          currChunk.iLUOuterWallType = slLine[6].toInt();
          currChunk.iLLOuterWallType = slLine[7].toInt();
          currChunk.iOuterFloorType = slLine[8].toInt();
          currChunk.iRLOuterWallType = slLine[9].toInt();
          currChunk.iRUOuterWallType = slLine[10].toInt();
          currChunk.iEnvironmentFloorType = slLine[11].toInt();
          currChunk.iSignType = slLine[12].toInt();
          currChunk.iSignHorizOffset = slLine[13].toInt();
          currChunk.iSignVertOffset = slLine[14].toInt();
          currChunk.dSignYaw = slLine[15].toDouble();
          currChunk.dSignPitch = slLine[16].toDouble();
          currChunk.dSignRoll = slLine[17].toDouble();
          //inc chunk index
          ++iChunkLine;
        } else if (iChunkLine == 2) {
          if (slLine.count() != CHUNK_LINE_2_COUNT) {
            assert(0);
            g_pMainWindow->LogMessage("Error loading file: invalid line before chunk completion");
            bSuccess = false;
          }
          //process line 3
          currChunk.iLUOuterWallAngle = slLine[0].toInt();
          currChunk.iLLOuterWallAngle = slLine[1].toInt();
          currChunk.iUnk23 = slLine[2].toInt();
          currChunk.iUnk24 = slLine[3].toInt();
          currChunk.iRLOuterWallAngle = slLine[4].toInt();
          currChunk.iRUOuterWallAngle = slLine[5].toInt();
          currChunk.iLUOuterWallHeight = slLine[6].toInt();
          currChunk.iLLOuterWallHeight = slLine[7].toInt();
          currChunk.iUnk29 = slLine[8].toInt();
          currChunk.iUnk30 = slLine[9].toInt();
          currChunk.iRLOuterWallHeight = slLine[10].toInt();
          currChunk.iRUOuterWallHeight = slLine[11].toInt();
          currChunk.iRoofHeight = slLine[12].toInt();
          currChunk.iDrawOrder1 = slLine[13].toInt();
          currChunk.iDrawOrder2 = slLine[14].toInt();
          currChunk.iDrawOrder3 = slLine[15].toInt();
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
          m_raceInfo.dTrackMapSize = slLine[0].toDouble();
          m_raceInfo.iTrackMapFidelity = slLine[1].toInt();
          m_raceInfo.dUnknown = slLine[2].toDouble();
        }
        break;
    }
  }

  //generate strings
  UpdateChunkStrings();

  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

bool CTrack::SaveTrack(const QString &sFilename, bool bIsMangled)
{
  if (sFilename.isEmpty())
    return false;

  std::vector<uint8_t> data;
  std::vector<uint8_t> mangledData;
  GetTrackData(data);

  std::vector<uint8_t> *pOutData;
  if (bIsMangled) {
    MangleFile(data, mangledData);
    pOutData = &mangledData;
  } else {
    pOutData = &data;
  }

  QFile file(sFilename);
  file.resize(0);
  if (file.open(QIODevice::ReadWrite)) {
    QTextStream stream(&file);
    for (int i = 0; i < pOutData->size(); ++i) {
      stream << (char)((*pOutData)[i]);
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
    iEndCursorPos += (int)m_chunkAy[i].sString.size() - 2;
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
    , QString &sLWallType, QString &sRWallType, QString &sRoofType, QString &sLUOuterWallType, QString &sLLOuterWallType, QString &sOuterFloorType
    , QString &sRLOuterWallType, QString &sRUOuterWallType, QString &sEnvironmentFloorType, QString &sSignType, QString &sSignHorizOffset, QString &sSignVertOffset
    , QString &sSignYaw, QString &sSignPitch, QString &sSignRoll
    , QString &sLUOuterWallAngle, QString &sLLOuterWallAngle, QString &sUnk23, QString &sUnk24, QString &sRLOuterWallAngle, QString &sRUOuterWallAngle
    , QString &sLUOuterWallHeight, QString &sLLOuterWallHeight, QString &sUnk29, QString &sUnk30, QString &sRLOuterWallHeight, QString &sRUOuterWallHeight
    , QString &sRoofHeight, QString &sDrawOrder1, QString &sDrawOrder2, QString &sDrawOrder3, QString &sUnk37, QString &sUnk38
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
  sLWallType = ""; sRWallType = ""; sRoofType = ""; sLUOuterWallType = ""; sLLOuterWallType = ""; sOuterFloorType = "";
  sRLOuterWallType = ""; sRUOuterWallType = ""; sEnvironmentFloorType = ""; sSignType = ""; sSignHorizOffset = ""; sSignVertOffset = "";
  sSignYaw = ""; sSignPitch = ""; sSignRoll = "";
  sLUOuterWallAngle = ""; sLLOuterWallAngle = ""; sUnk23 = ""; sUnk24 = ""; sRLOuterWallAngle = ""; sRUOuterWallAngle = "";
  sLUOuterWallHeight = ""; sLLOuterWallHeight = ""; sUnk29 = ""; sUnk30 = ""; sRLOuterWallHeight = ""; sRUOuterWallHeight = "";
  sRoofHeight = ""; sDrawOrder1 = ""; sDrawOrder2 = ""; sDrawOrder3 = ""; sUnk37 = ""; sUnk38 = "";
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

    sVal = QString::number(m_chunkAy[i].dYaw, 'f', 5);
    if (sYaw.isEmpty()) sYaw = sVal;
    else if (sYaw.compare(sVal) != 0) sYaw = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].dPitch, 'f', 5);
    if (sPitch.isEmpty()) sPitch = sVal;
    else if (sPitch.compare(sVal) != 0) sPitch = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].dRoll, 'f', 5);
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
    sVal = QString::number(m_chunkAy[i].iLUOuterWallType);
    if (sLUOuterWallType.isEmpty()) sLUOuterWallType = sVal;
    else if (sLUOuterWallType.compare(sVal) != 0) sLUOuterWallType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLLOuterWallType);
    if (sLLOuterWallType.isEmpty()) sLLOuterWallType = sVal;
    else if (sLLOuterWallType.compare(sVal) != 0) sLLOuterWallType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iOuterFloorType);
    if (sOuterFloorType.isEmpty()) sOuterFloorType = sVal;
    else if (sOuterFloorType.compare(sVal) != 0) sOuterFloorType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRLOuterWallType);
    if (sRLOuterWallType.isEmpty()) sRLOuterWallType = sVal;
    else if (sRLOuterWallType.compare(sVal) != 0) sRLOuterWallType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRUOuterWallType);
    if (sRUOuterWallType.isEmpty()) sRUOuterWallType = sVal;
    else if (sRUOuterWallType.compare(sVal) != 0) sRUOuterWallType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iEnvironmentFloorType);
    if (sEnvironmentFloorType.isEmpty()) sEnvironmentFloorType = sVal;
    else if (sEnvironmentFloorType.compare(sVal) != 0) sEnvironmentFloorType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iSignType);
    if (sSignType.isEmpty()) sSignType = sVal;
    else if (sSignType.compare(sVal) != 0) sSignType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iSignHorizOffset);
    if (sSignHorizOffset.isEmpty()) sSignHorizOffset = sVal;
    else if (sSignHorizOffset.compare(sVal) != 0) sSignHorizOffset = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iSignVertOffset);
    if (sSignVertOffset.isEmpty()) sSignVertOffset = sVal;
    else if (sSignVertOffset.compare(sVal) != 0) sSignVertOffset = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].dSignYaw, 'f', 1);
    if (sSignYaw.isEmpty()) sSignYaw = sVal;
    else if (sSignYaw.compare(sVal) != 0) sSignYaw = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].dSignPitch, 'f', 1);
    if (sSignPitch.isEmpty()) sSignPitch = sVal;
    else if (sSignPitch.compare(sVal) != 0) sSignPitch = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].dSignRoll, 'f', 1);
    if (sSignRoll.isEmpty()) sSignRoll = sVal;
    else if (sSignRoll.compare(sVal) != 0) sSignRoll = MIXED_DATA;
 
    sVal = QString::number(m_chunkAy[i].iLUOuterWallAngle);
    if (sLUOuterWallAngle.isEmpty()) sLUOuterWallAngle = sVal;
    else if (sLUOuterWallAngle.compare(sVal) != 0) sLUOuterWallAngle = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLLOuterWallAngle);
    if (sLLOuterWallAngle.isEmpty()) sLLOuterWallAngle = sVal;
    else if (sLLOuterWallAngle.compare(sVal) != 0) sLLOuterWallAngle = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk23);
    if (sUnk23.isEmpty()) sUnk23 = sVal;
    else if (sUnk23.compare(sVal) != 0) sUnk23 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk24);
    if (sUnk24.isEmpty()) sUnk24 = sVal;
    else if (sUnk24.compare(sVal) != 0) sUnk24 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRLOuterWallAngle);
    if (sRLOuterWallAngle.isEmpty()) sRLOuterWallAngle = sVal;
    else if (sRLOuterWallAngle.compare(sVal) != 0) sRLOuterWallAngle = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRUOuterWallAngle);
    if (sRUOuterWallAngle.isEmpty()) sRUOuterWallAngle = sVal;
    else if (sRUOuterWallAngle.compare(sVal) != 0) sRUOuterWallAngle = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLUOuterWallHeight);
    if (sLUOuterWallHeight.isEmpty()) sLUOuterWallHeight = sVal;
    else if (sLUOuterWallHeight.compare(sVal) != 0) sLUOuterWallHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLLOuterWallHeight);
    if (sLLOuterWallHeight.isEmpty()) sLLOuterWallHeight = sVal;
    else if (sLLOuterWallHeight.compare(sVal) != 0) sLLOuterWallHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk29);
    if (sUnk29.isEmpty()) sUnk29 = sVal;
    else if (sUnk29.compare(sVal) != 0) sUnk29 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk30);
    if (sUnk30.isEmpty()) sUnk30 = sVal;
    else if (sUnk30.compare(sVal) != 0) sUnk30 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRLOuterWallHeight);
    if (sRLOuterWallHeight.isEmpty()) sRLOuterWallHeight = sVal;
    else if (sRLOuterWallHeight.compare(sVal) != 0) sRLOuterWallHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRUOuterWallHeight);
    if (sRUOuterWallHeight.isEmpty()) sRUOuterWallHeight = sVal;
    else if (sRUOuterWallHeight.compare(sVal) != 0) sRUOuterWallHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRoofHeight);
    if (sRoofHeight.isEmpty()) sRoofHeight = sVal;
    else if (sRoofHeight.compare(sVal) != 0) sRoofHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iDrawOrder1);
    if (sDrawOrder1.isEmpty()) sDrawOrder1 = sVal;
    else if (sDrawOrder1.compare(sVal) != 0) sDrawOrder1 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iDrawOrder2);
    if (sDrawOrder2.isEmpty()) sDrawOrder2 = sVal;
    else if (sDrawOrder2.compare(sVal) != 0) sDrawOrder2 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iDrawOrder3);
    if (sDrawOrder3.isEmpty()) sDrawOrder3 = sVal;
    else if (sDrawOrder3.compare(sVal) != 0) sDrawOrder3 = MIXED_DATA;
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
                           , const QString &sLeftShoulderWidth
                           , const QString &sLeftLaneWidth
                           , const QString &sRightLaneWidth
                           , const QString &sRightShoulderWidth
                           , const QString &sLShoulderHeight
                           , const QString &sRShoulderHeight
                           , const QString &sLength
                           , const QString &sYaw
                           , const QString &sPitch
                           , const QString &sRoll
                           , const QString &sAILine1
                           , const QString &sAILine2
                           , const QString &sAILine3
                           , const QString &sAILine4
                           , const QString &sTrackGrip
                           , const QString &sLeftShoulderGrip
                           , const QString &sRightShoulderGrip
                           , const QString &sUnk04
                           , const QString &sUnk05
                           , const QString &sUnk06
                           , const QString &sUnk07
                           , const QString &sUnk08
                           , const QString &sLeftSurfaceType
                           , const QString &sCenterSurfaceType
                           , const QString &sRightSurfaceType
                           , const QString &sLWallType
                           , const QString &sRWallType
                           , const QString &sRoofType
                           , const QString &sLUOuterWallType
                           , const QString &sLLOuterWallType
                           , const QString &sOuterFloorType
                           , const QString &sRLOuterWallType
                           , const QString &sRUOuterWallType
                           , const QString &sEnvironmentFloorType
                           , const QString &sSignType
                           , const QString &sSignHorizOffset
                           , const QString &sSignVertOffset
                           , const QString &sSignYaw
                           , const QString &sSignPitch
                           , const QString &sSignRoll
                           , const QString &sLUOuterWallAngle
                           , const QString &sLLOuterWallAngle
                           , const QString &sUnk23
                           , const QString &sUnk24
                           , const QString &sRLOuterWallAngle
                           , const QString &sRUOuterWallAngle
                           , const QString &sLUOuterWallHeight
                           , const QString &sLLOuterWallHeight
                           , const QString &sUnk29
                           , const QString &sUnk30
                           , const QString &sRLOuterWallHeight
                           , const QString &sRUOuterWallHeight
                           , const QString &sRoofHeight
                           , const QString &sDrawOrder1
                           , const QString &sDrawOrder2
                           , const QString &sDrawOrder3
                           , const QString &sUnk37
                           , const QString &sUnk38
                           , const QString &sUnk39
                           , const QString &sUnk40
                           , const QString &sUnk41
                           , const QString &sUnk42
                           , const QString &sUnk43
                           , const QString &sUnk44
                           , const QString &sUnk45
                           , const QString &sUnk46
                           , const QString &sUnk47
                           , const QString &sUnk48
                           , const QString &sUnk49
                           , const QString &sUnk50
                           , const QString &sSignTexture
                           , const QString &sBackTexture
                           , const QString &sStuntScaleFactor
                           , const QString &sStuntAngle
                           , const QString &sStuntUnknown
                           , const QString &sStuntTimingGroup
                           , const QString &sStuntHeight
                           , const QString &sStuntTimeBulging
                           , const QString &sStuntTimeFlat
                           , const QString &sStuntExpandsContracts
                           , const QString &sStuntBulge)
{
  for (int i = iStartIndex; i <= iEndIndex; ++i) {
    if (!sLeftShoulderWidth.isEmpty()) m_chunkAy[i].iLeftShoulderWidth = sLeftShoulderWidth.toInt();
    if (!sLeftLaneWidth.isEmpty()) m_chunkAy[i].iLeftLaneWidth = sLeftLaneWidth.toInt();
    if (!sRightLaneWidth.isEmpty()) m_chunkAy[i].iRightLaneWidth = sRightLaneWidth.toInt();
    if (!sRightShoulderWidth.isEmpty()) m_chunkAy[i].iRightShoulderWidth = sRightShoulderWidth.toInt();
    if (!sLShoulderHeight.isEmpty()) m_chunkAy[i].iLeftShoulderHeight = sLShoulderHeight.toInt();
    if (!sRShoulderHeight.isEmpty()) m_chunkAy[i].iRightShoulderHeight = sRShoulderHeight.toInt();
    if (!sLength.isEmpty()) m_chunkAy[i].iLength = sLength.toInt();
    if (!sYaw.isEmpty()) m_chunkAy[i].dYaw = sYaw.toDouble();
    if (!sPitch.isEmpty()) m_chunkAy[i].dPitch = sPitch.toDouble();
    if (!sRoll.isEmpty()) m_chunkAy[i].dRoll = sRoll.toDouble();
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
    if (!sLUOuterWallType.isEmpty()) m_chunkAy[i].iLUOuterWallType = sLUOuterWallType.toInt();
    if (!sLLOuterWallType.isEmpty()) m_chunkAy[i].iLLOuterWallType = sLLOuterWallType.toInt();
    if (!sOuterFloorType.isEmpty()) m_chunkAy[i].iOuterFloorType = sOuterFloorType.toInt();
    if (!sRLOuterWallType.isEmpty()) m_chunkAy[i].iRLOuterWallType = sRLOuterWallType.toInt();
    if (!sRUOuterWallType.isEmpty()) m_chunkAy[i].iRUOuterWallType = sRUOuterWallType.toInt();
    if (!sEnvironmentFloorType.isEmpty()) m_chunkAy[i].iEnvironmentFloorType = sEnvironmentFloorType.toInt();
    if (!sSignType.isEmpty()) m_chunkAy[i].iSignType = sSignType.toInt();
    if (!sSignHorizOffset.isEmpty()) m_chunkAy[i].iSignHorizOffset = sSignHorizOffset.toInt();
    if (!sSignVertOffset.isEmpty()) m_chunkAy[i].iSignVertOffset = sSignVertOffset.toInt();
    if (!sSignYaw.isEmpty()) m_chunkAy[i].dSignYaw = sSignYaw.toDouble();
    if (!sSignPitch.isEmpty()) m_chunkAy[i].dSignPitch = sSignPitch.toDouble();
    if (!sSignRoll.isEmpty()) m_chunkAy[i].dSignRoll = sSignRoll.toDouble();
    if (!sLUOuterWallAngle.isEmpty()) m_chunkAy[i].iLUOuterWallAngle = sLUOuterWallAngle.toInt();
    if (!sLLOuterWallAngle.isEmpty()) m_chunkAy[i].iLLOuterWallAngle = sLLOuterWallAngle.toInt();
    if (!sUnk23.isEmpty()) m_chunkAy[i].iUnk23 = sUnk23.toInt();
    if (!sUnk24.isEmpty()) m_chunkAy[i].iUnk24 = sUnk24.toInt();
    if (!sRLOuterWallAngle.isEmpty()) m_chunkAy[i].iRLOuterWallAngle = sRLOuterWallAngle.toInt();
    if (!sRUOuterWallAngle.isEmpty()) m_chunkAy[i].iRUOuterWallAngle = sRUOuterWallAngle.toInt();
    if (!sLUOuterWallHeight.isEmpty()) m_chunkAy[i].iLUOuterWallHeight = sLUOuterWallHeight.toInt();
    if (!sLLOuterWallHeight.isEmpty()) m_chunkAy[i].iLLOuterWallHeight = sLLOuterWallHeight.toInt();
    if (!sUnk29.isEmpty()) m_chunkAy[i].iUnk29 = sUnk29.toInt();
    if (!sUnk30.isEmpty()) m_chunkAy[i].iUnk30 = sUnk30.toInt();
    if (!sRLOuterWallHeight.isEmpty()) m_chunkAy[i].iRLOuterWallHeight = sRLOuterWallHeight.toInt();
    if (!sRUOuterWallHeight.isEmpty()) m_chunkAy[i].iRUOuterWallHeight = sRUOuterWallHeight.toInt();
    if (!sRoofHeight.isEmpty()) m_chunkAy[i].iRoofHeight = sRoofHeight.toInt();
    if (!sDrawOrder1.isEmpty()) m_chunkAy[i].iDrawOrder1 = sDrawOrder1.toInt();
    if (!sDrawOrder2.isEmpty()) m_chunkAy[i].iDrawOrder2 = sDrawOrder2.toInt();
    if (!sDrawOrder3.isEmpty()) m_chunkAy[i].iDrawOrder3 = sDrawOrder3.toInt();
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
    if (!sSignTexture.isEmpty()) m_chunkAy[i].iSignTexture = sSignTexture.toInt();
    if (!sBackTexture.isEmpty()) m_chunkAy[i].iBackTexture = sBackTexture.toInt();
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
                           , const QString &sLeftShoulderWidth
                           , const QString &sLeftLaneWidth
                           , const QString &sRightLaneWidth
                           , const QString &sRightShoulderWidth
                           , const QString &sLShoulderHeight
                           , const QString &sRShoulderHeight
                           , const QString &sLength
                           , const QString &sYaw
                           , const QString &sPitch
                           , const QString &sRoll
                           , const QString &sAILine1
                           , const QString &sAILine2
                           , const QString &sAILine3
                           , const QString &sAILine4
                           , const QString &sTrackGrip
                           , const QString &sLeftShoulderGrip
                           , const QString &sRightShoulderGrip
                           , const QString &sUnk04
                           , const QString &sUnk05
                           , const QString &sUnk06
                           , const QString &sUnk07
                           , const QString &sUnk08
                           , const QString &sLeftSurfaceType
                           , const QString &sCenterSurfaceType
                           , const QString &sRightSurfaceType
                           , const QString &sLWallType
                           , const QString &sRWallType
                           , const QString &sRoofType
                           , const QString &sLUOuterWallType
                           , const QString &sLLOuterWallType
                           , const QString &sOuterFloorType
                           , const QString &sRLOuterWallType
                           , const QString &sRUOuterWallType
                           , const QString &sEnvironmentFloorType
                           , const QString &sSignType
                           , const QString &sSignHorizOffset
                           , const QString &sSignVertOffset
                           , const QString &sSignYaw
                           , const QString &sSignPitch
                           , const QString &sSignRoll
                           , const QString &sLUOuterWallAngle
                           , const QString &sLLOuterWallAngle
                           , const QString &sUnk23
                           , const QString &sUnk24
                           , const QString &sRLOuterWallAngle
                           , const QString &sRUOuterWallAngle
                           , const QString &sLUOuterWallHeight
                           , const QString &sLLOuterWallHeight
                           , const QString &sUnk29
                           , const QString &sUnk30
                           , const QString &sRLOuterWallHeight
                           , const QString &sRUOuterWallHeight
                           , const QString &sRoofHeight
                           , const QString &sDrawOrder1
                           , const QString &sDrawOrder2
                           , const QString &sDrawOrder3
                           , const QString &sUnk37
                           , const QString &sUnk38
                           , const QString &sUnk39
                           , const QString &sUnk40
                           , const QString &sUnk41
                           , const QString &sUnk42
                           , const QString &sUnk43
                           , const QString &sUnk44
                           , const QString &sUnk45
                           , const QString &sUnk46
                           , const QString &sUnk47
                           , const QString &sUnk48
                           , const QString &sUnk49
                           , const QString &sUnk50
                           , const QString &sSignTexture
                           , const QString &sBackTexture
                           , const QString &sStuntScaleFactor
                           , const QString &sStuntAngle
                           , const QString &sStuntUnknown
                           , const QString &sStuntTimingGroup
                           , const QString &sStuntHeight
                           , const QString &sStuntTimeBulging
                           , const QString &sStuntTimeFlat
                           , const QString &sStuntExpandsContracts
                           , const QString &sStuntBulge)
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
    newChunk.dYaw = sYaw.toDouble();
    newChunk.dPitch = sPitch.toDouble();
    newChunk.dRoll = sRoll.toDouble();
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
    newChunk.iLUOuterWallType = sLUOuterWallType.toInt();
    newChunk.iLLOuterWallType = sLLOuterWallType.toInt();
    newChunk.iOuterFloorType = sOuterFloorType.toInt();
    newChunk.iRLOuterWallType = sRLOuterWallType.toInt();
    newChunk.iRUOuterWallType = sRUOuterWallType.toInt();
    newChunk.iEnvironmentFloorType = sEnvironmentFloorType.toInt();
    newChunk.iSignType = sSignType.toInt();
    newChunk.iSignHorizOffset = sSignHorizOffset.toInt();
    newChunk.iSignVertOffset = sSignVertOffset.toInt();
    newChunk.dSignYaw = sSignYaw.toDouble();
    newChunk.dSignPitch = sSignPitch.toDouble();
    newChunk.dSignRoll = sSignRoll.toDouble();
    newChunk.iLUOuterWallAngle = sLUOuterWallAngle.toInt();
    newChunk.iLLOuterWallAngle = sLLOuterWallAngle.toInt();
    newChunk.iUnk23 = sUnk23.toInt();
    newChunk.iUnk24 = sUnk24.toInt();
    newChunk.iRLOuterWallAngle = sRLOuterWallAngle.toInt();
    newChunk.iRUOuterWallAngle = sRUOuterWallAngle.toInt();
    newChunk.iLUOuterWallHeight = sLUOuterWallHeight.toInt();
    newChunk.iLLOuterWallHeight = sLLOuterWallHeight.toInt();
    newChunk.iUnk29 = sUnk29.toInt();
    newChunk.iUnk30 = sUnk30.toInt();
    newChunk.iRLOuterWallHeight = sRLOuterWallHeight.toInt();
    newChunk.iRUOuterWallHeight = sRUOuterWallHeight.toInt();
    newChunk.iRoofHeight = sRoofHeight.toInt();
    newChunk.iDrawOrder1 = sDrawOrder1.toInt();
    newChunk.iDrawOrder2 = sDrawOrder2.toInt();
    newChunk.iDrawOrder3 = sDrawOrder3.toInt();
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
    newChunk.iSignTexture = sSignTexture.toInt();
    newChunk.iBackTexture = sBackTexture.toInt();
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
           , chunk.iLUOuterWallAngle
           , chunk.iLLOuterWallAngle
           , chunk.iUnk23
           , chunk.iUnk24
           , chunk.iRLOuterWallAngle
           , chunk.iRUOuterWallAngle
           , chunk.iLUOuterWallHeight
           , chunk.iLLOuterWallHeight
           , chunk.iUnk29
           , chunk.iUnk30
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

void CTrack::GetTrackData(std::vector<uint8_t> &data)
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
  WriteToVector(data, m_sTextureFile.toLatin1().constData());
  WriteToVector(data, "\r\n");
  WriteToVector(data, "BLD:");
  WriteToVector(data, m_sBuildingFile.toLatin1().constData());
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
    WriteToVector(data, QString::number(m_raceInfo.iTrackNumber).toLatin1().constData());
    WriteToVector(data, "\r\n");
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

void CTrack::WriteToVector(std::vector<uint8_t> &data, const char *szText)
{
  int iLength = (int)strlen(szText);
  for (int i = 0; i < iLength; ++i) {
    uint8_t val = (uint8_t)szText[i];
    data.push_back(val);
  }
}

//-------------------------------------------------------------------------------------------------