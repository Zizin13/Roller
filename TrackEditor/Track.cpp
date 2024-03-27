#include "TrackEditor.h"
#include "Track.h"
#include "MainWindow.h"
#include "qfile.h"
#include "qtextstream.h"
#include "Unmangler.h"
#include "ChunkEditValues.h"
#include <assert.h>
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
  m_bIsMangled = bIsMangled;

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

void CTrack::GetGeometryValuesFromSelection(int iStartIndex, int iEndIndex, CChunkEditValues &editVals)
{
  if (m_chunkAy.empty()) return;
  if (iEndIndex < iStartIndex || iEndIndex >= m_chunkAy.size()) {
    assert(0);
    return;
  }

  editVals.Clear();

  QString sVal;
  for (int i = iStartIndex; i <= iEndIndex; ++i) {
    sVal = QString::number(m_chunkAy[i].iLeftShoulderWidth);
    if (editVals.sLeftShoulderWidth.isEmpty()) editVals.sLeftShoulderWidth = sVal;
    else if (editVals.sLeftShoulderWidth.compare(sVal) != 0) editVals.sLeftShoulderWidth = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLeftLaneWidth);
    if (editVals.sLeftLaneWidth.isEmpty()) editVals.sLeftLaneWidth = sVal;
    else if (editVals.sLeftLaneWidth.compare(sVal) != 0) editVals.sLeftLaneWidth = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightLaneWidth);
    if (editVals.sRightLaneWidth.isEmpty()) editVals.sRightLaneWidth = sVal;
    else if (editVals.sRightLaneWidth.compare(sVal) != 0) editVals.sRightLaneWidth = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightShoulderWidth);
    if (editVals.sRightShoulderWidth.isEmpty()) editVals.sRightShoulderWidth = sVal;
    else if (editVals.sRightShoulderWidth.compare(sVal) != 0) editVals.sRightShoulderWidth = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iLeftShoulderHeight);
    if (editVals.sLShoulderHeight.isEmpty()) editVals.sLShoulderHeight = sVal;
    else if (editVals.sLShoulderHeight.compare(sVal) != 0) editVals.sLShoulderHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightShoulderHeight);
    if (editVals.sRShoulderHeight.isEmpty()) editVals.sRShoulderHeight = sVal;
    else if (editVals.sRShoulderHeight.compare(sVal) != 0) editVals.sRShoulderHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLength);
    if (editVals.sLength.isEmpty()) editVals.sLength = sVal;
    else if (editVals.sLength.compare(sVal) != 0) editVals.sLength = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].dYaw, 'f', 5);
    if (editVals.sYaw.isEmpty()) editVals.sYaw = sVal;
    else if (editVals.sYaw.compare(sVal) != 0) editVals.sYaw = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].dPitch, 'f', 5);
    if (editVals.sPitch.isEmpty()) editVals.sPitch = sVal;
    else if (editVals.sPitch.compare(sVal) != 0) editVals.sPitch = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].dRoll, 'f', 5);
    if (editVals.sRoll.isEmpty()) editVals.sRoll = sVal;
    else if (editVals.sRoll.compare(sVal) != 0) editVals.sRoll = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iAILine1);
    if (editVals.sAILine1.isEmpty()) editVals.sAILine1 = sVal;
    else if (editVals.sAILine1.compare(sVal) != 0) editVals.sAILine1 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iAILine2);
    if (editVals.sAILine2.isEmpty()) editVals.sAILine2 = sVal;
    else if (editVals.sAILine2.compare(sVal) != 0) editVals.sAILine2 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iAILine3);
    if (editVals.sAILine3.isEmpty()) editVals.sAILine3 = sVal;
    else if (editVals.sAILine3.compare(sVal) != 0) editVals.sAILine3 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iAILine4);
    if (editVals.sAILine4.isEmpty()) editVals.sAILine4 = sVal;
    else if (editVals.sAILine4.compare(sVal) != 0) editVals.sAILine4 = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iTrackGrip);
    if (editVals.sTrackGrip.isEmpty()) editVals.sTrackGrip = sVal;
    else if (editVals.sTrackGrip.compare(sVal) != 0) editVals.sTrackGrip = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLeftShoulderGrip);
    if (editVals.sLeftShoulderGrip.isEmpty()) editVals.sLeftShoulderGrip = sVal;
    else if (editVals.sLeftShoulderGrip.compare(sVal) != 0) editVals.sLeftShoulderGrip = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightShoulderGrip);
    if (editVals.sRightShoulderGrip.isEmpty()) editVals.sRightShoulderGrip = sVal;
    else if (editVals.sRightShoulderGrip.compare(sVal) != 0) editVals.sRightShoulderGrip = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iUnk04);
    if (editVals.sUnk04.isEmpty()) editVals.sUnk04 = sVal;
    else if (editVals.sUnk04.compare(sVal) != 0) editVals.sUnk04 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk05);
    if (editVals.sUnk05.isEmpty()) editVals.sUnk05 = sVal;
    else if (editVals.sUnk05.compare(sVal) != 0) editVals.sUnk05 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk06);
    if (editVals.sUnk06.isEmpty()) editVals.sUnk06 = sVal;
    else if (editVals.sUnk06.compare(sVal) != 0) editVals.sUnk06 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk07);
    if (editVals.sUnk07.isEmpty()) editVals.sUnk07 = sVal;
    else if (editVals.sUnk07.compare(sVal) != 0) editVals.sUnk07 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk08);
    if (editVals.sUnk08.isEmpty()) editVals.sUnk08 = sVal;
    else if (editVals.sUnk08.compare(sVal) != 0) editVals.sUnk08 = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iLeftSurfaceType);
    if (editVals.sLeftSurfaceType.isEmpty()) editVals.sLeftSurfaceType = sVal;
    else if (editVals.sLeftSurfaceType.compare(sVal) != 0) editVals.sLeftSurfaceType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iCenterSurfaceType);
    if (editVals.sCenterSurfaceType.isEmpty()) editVals.sCenterSurfaceType = sVal;
    else if (editVals.sCenterSurfaceType.compare(sVal) != 0) editVals.sCenterSurfaceType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightSurfaceType);
    if (editVals.sRightSurfaceType.isEmpty()) editVals.sRightSurfaceType = sVal;
    else if (editVals.sRightSurfaceType.compare(sVal) != 0) editVals.sRightSurfaceType = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iLeftWallType);
    if (editVals.sLWallType.isEmpty()) editVals.sLWallType = sVal;
    else if (editVals.sLWallType.compare(sVal) != 0) editVals.sLWallType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightWallType);
    if (editVals.sRWallType.isEmpty()) editVals.sRWallType = sVal;
    else if (editVals.sRWallType.compare(sVal) != 0) editVals.sRWallType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRoofType);
    if (editVals.sRoofType.isEmpty()) editVals.sRoofType = sVal;
    else if (editVals.sRoofType.compare(sVal) != 0) editVals.sRoofType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLUOuterWallType);
    if (editVals.sLUOuterWallType.isEmpty()) editVals.sLUOuterWallType = sVal;
    else if (editVals.sLUOuterWallType.compare(sVal) != 0) editVals.sLUOuterWallType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLLOuterWallType);
    if (editVals.sLLOuterWallType.isEmpty()) editVals.sLLOuterWallType = sVal;
    else if (editVals.sLLOuterWallType.compare(sVal) != 0) editVals.sLLOuterWallType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iOuterFloorType);
    if (editVals.sOuterFloorType.isEmpty()) editVals.sOuterFloorType = sVal;
    else if (editVals.sOuterFloorType.compare(sVal) != 0) editVals.sOuterFloorType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRLOuterWallType);
    if (editVals.sRLOuterWallType.isEmpty())editVals.sRLOuterWallType = sVal;
    else if (editVals.sRLOuterWallType.compare(sVal) != 0) editVals.sRLOuterWallType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRUOuterWallType);
    if (editVals.sRUOuterWallType.isEmpty()) editVals.sRUOuterWallType = sVal;
    else if (editVals.sRUOuterWallType.compare(sVal) != 0) editVals.sRUOuterWallType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iEnvironmentFloorType);
    if (editVals.sEnvironmentFloorType.isEmpty()) editVals.sEnvironmentFloorType = sVal;
    else if (editVals.sEnvironmentFloorType.compare(sVal) != 0) editVals.sEnvironmentFloorType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iSignType);
    if (editVals.sSignType.isEmpty()) editVals.sSignType = sVal;
    else if (editVals.sSignType.compare(sVal) != 0) editVals.sSignType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iSignHorizOffset);
    if (editVals.sSignHorizOffset.isEmpty()) editVals.sSignHorizOffset = sVal;
    else if (editVals.sSignHorizOffset.compare(sVal) != 0) editVals.sSignHorizOffset = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iSignVertOffset);
    if (editVals.sSignVertOffset.isEmpty()) editVals.sSignVertOffset = sVal;
    else if (editVals.sSignVertOffset.compare(sVal) != 0) editVals.sSignVertOffset = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].dSignYaw, 'f', 1);
    if (editVals.sSignYaw.isEmpty()) editVals.sSignYaw = sVal;
    else if (editVals.sSignYaw.compare(sVal) != 0) editVals.sSignYaw = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].dSignPitch, 'f', 1);
    if (editVals.sSignPitch.isEmpty()) editVals.sSignPitch = sVal;
    else if (editVals.sSignPitch.compare(sVal) != 0) editVals.sSignPitch = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].dSignRoll, 'f', 1);
    if (editVals.sSignRoll.isEmpty()) editVals.sSignRoll = sVal;
    else if (editVals.sSignRoll.compare(sVal) != 0) editVals.sSignRoll = MIXED_DATA;
 
    sVal = QString::number(m_chunkAy[i].iLUOuterWallAngle);
    if (editVals.sLUOuterWallAngle.isEmpty()) editVals.sLUOuterWallAngle = sVal;
    else if (editVals.sLUOuterWallAngle.compare(sVal) != 0) editVals.sLUOuterWallAngle = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLLOuterWallAngle);
    if (editVals.sLLOuterWallAngle.isEmpty()) editVals.sLLOuterWallAngle = sVal;
    else if (editVals.sLLOuterWallAngle.compare(sVal) != 0) editVals.sLLOuterWallAngle = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk23);
    if (editVals.sUnk23.isEmpty()) editVals.sUnk23 = sVal;
    else if (editVals.sUnk23.compare(sVal) != 0) editVals.sUnk23 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk24);
    if (editVals.sUnk24.isEmpty()) editVals.sUnk24 = sVal;
    else if (editVals.sUnk24.compare(sVal) != 0) editVals.sUnk24 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRLOuterWallAngle);
    if (editVals.sRLOuterWallAngle.isEmpty()) editVals.sRLOuterWallAngle = sVal;
    else if (editVals.sRLOuterWallAngle.compare(sVal) != 0) editVals.sRLOuterWallAngle = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRUOuterWallAngle);
    if (editVals.sRUOuterWallAngle.isEmpty()) editVals.sRUOuterWallAngle = sVal;
    else if (editVals.sRUOuterWallAngle.compare(sVal) != 0) editVals.sRUOuterWallAngle = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLUOuterWallHeight);
    if (editVals.sLUOuterWallHeight.isEmpty()) editVals.sLUOuterWallHeight = sVal;
    else if (editVals.sLUOuterWallHeight.compare(sVal) != 0) editVals.sLUOuterWallHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLLOuterWallHeight);
    if (editVals.sLLOuterWallHeight.isEmpty()) editVals.sLLOuterWallHeight = sVal;
    else if (editVals.sLLOuterWallHeight.compare(sVal) != 0) editVals.sLLOuterWallHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk29);
    if (editVals.sUnk29.isEmpty()) editVals.sUnk29 = sVal;
    else if (editVals.sUnk29.compare(sVal) != 0) editVals.sUnk29 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk30);
    if (editVals.sUnk30.isEmpty()) editVals.sUnk30 = sVal;
    else if (editVals.sUnk30.compare(sVal) != 0) editVals.sUnk30 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRLOuterWallHeight);
    if (editVals.sRLOuterWallHeight.isEmpty()) editVals.sRLOuterWallHeight = sVal;
    else if (editVals.sRLOuterWallHeight.compare(sVal) != 0) editVals.sRLOuterWallHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRUOuterWallHeight);
    if (editVals.sRUOuterWallHeight.isEmpty()) editVals.sRUOuterWallHeight = sVal;
    else if (editVals.sRUOuterWallHeight.compare(sVal) != 0) editVals.sRUOuterWallHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRoofHeight);
    if (editVals.sRoofHeight.isEmpty()) editVals.sRoofHeight = sVal;
    else if (editVals.sRoofHeight.compare(sVal) != 0) editVals.sRoofHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iDrawOrder1);
    if (editVals.sDrawOrder1.isEmpty()) editVals.sDrawOrder1 = sVal;
    else if (editVals.sDrawOrder1.compare(sVal) != 0) editVals.sDrawOrder1 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iDrawOrder2);
    if (editVals.sDrawOrder2.isEmpty()) editVals.sDrawOrder2 = sVal;
    else if (editVals.sDrawOrder2.compare(sVal) != 0) editVals.sDrawOrder2 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iDrawOrder3);
    if (editVals.sDrawOrder3.isEmpty()) editVals.sDrawOrder3 = sVal;
    else if (editVals.sDrawOrder3.compare(sVal) != 0) editVals.sDrawOrder3 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk37);
    if (editVals.sUnk37.isEmpty()) editVals.sUnk37 = sVal;
    else if (editVals.sUnk37.compare(sVal) != 0) editVals.sUnk37 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk38);
    if (editVals.sUnk38.isEmpty()) editVals.sUnk38 = sVal;
    else if (editVals.sUnk38.compare(sVal) != 0) editVals.sUnk38 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk39);
    if (editVals.sUnk39.isEmpty()) editVals.sUnk39 = sVal;
    else if (editVals.sUnk39.compare(sVal) != 0) editVals.sUnk39 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk40);
    if (editVals.sUnk40.isEmpty()) editVals.sUnk40 = sVal;
    else if (editVals.sUnk40.compare(sVal) != 0) editVals.sUnk40 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk41);
    if (editVals.sUnk41.isEmpty()) editVals.sUnk41 = sVal;
    else if (editVals.sUnk41.compare(sVal) != 0) editVals.sUnk41 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk42);
    if (editVals.sUnk42.isEmpty()) editVals.sUnk42 = sVal;
    else if (editVals.sUnk42.compare(sVal) != 0) editVals.sUnk42 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk43);
    if (editVals.sUnk43.isEmpty()) editVals.sUnk43 = sVal;
    else if (editVals.sUnk43.compare(sVal) != 0) editVals.sUnk43 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk44);
    if (editVals.sUnk44.isEmpty()) editVals.sUnk44 = sVal;
    else if (editVals.sUnk44.compare(sVal) != 0) editVals.sUnk44 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk45);
    if (editVals.sUnk45.isEmpty()) editVals.sUnk45 = sVal;
    else if (editVals.sUnk45.compare(sVal) != 0) editVals.sUnk45 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk46);
    if (editVals.sUnk46.isEmpty()) editVals.sUnk46 = sVal;
    else if (editVals.sUnk46.compare(sVal) != 0) editVals.sUnk46 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk47);
    if (editVals.sUnk47.isEmpty()) editVals.sUnk47 = sVal;
    else if (editVals.sUnk47.compare(sVal) != 0) editVals.sUnk47 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk48);
    if (editVals.sUnk48.isEmpty()) editVals.sUnk48 = sVal;
    else if (editVals.sUnk48.compare(sVal) != 0) editVals.sUnk48 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk49);
    if (editVals.sUnk49.isEmpty()) editVals.sUnk49 = sVal;
    else if (editVals.sUnk49.compare(sVal) != 0) editVals.sUnk49 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk50);
    if (editVals.sUnk50.isEmpty()) editVals.sUnk50 = sVal;
    else if (editVals.sUnk50.compare(sVal) != 0) editVals.sUnk50 = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iSignTexture);
    if (editVals.sSignTexture.isEmpty()) editVals.sSignTexture = sVal;
    else if (editVals.sSignTexture.compare(sVal) != 0) editVals.sSignTexture = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iBackTexture);
    if (editVals.sBackTexture.isEmpty()) editVals.sBackTexture = sVal;
    else if (editVals.sBackTexture.compare(sVal) != 0) editVals.sBackTexture = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].stunt.iScaleFactor);
    if (editVals.sStuntScaleFactor.isEmpty()) editVals.sStuntScaleFactor = sVal;
    else if (editVals.sStuntScaleFactor.compare(sVal) != 0) editVals.sStuntScaleFactor = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iAngle);
    if (editVals.sStuntAngle.isEmpty()) editVals.sStuntAngle = sVal;
    else if (editVals.sStuntAngle.compare(sVal) != 0) editVals.sStuntAngle = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iUnknown);
    if (editVals.sStuntUnknown.isEmpty()) editVals.sStuntUnknown = sVal;
    else if (editVals.sStuntUnknown.compare(sVal) != 0) editVals.sStuntUnknown = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iTimingGroup);
    if (editVals.sStuntTimingGroup.isEmpty()) editVals.sStuntTimingGroup = sVal;
    else if (editVals.sStuntTimingGroup.compare(sVal) != 0) editVals.sStuntTimingGroup = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iHeight);
    if (editVals.sStuntHeight.isEmpty()) editVals.sStuntHeight = sVal;
    else if (editVals.sStuntHeight.compare(sVal) != 0) editVals.sStuntHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iTimeBulging);
    if (editVals.sStuntTimeBulging.isEmpty()) editVals.sStuntTimeBulging = sVal;
    else if (editVals.sStuntTimeBulging.compare(sVal) != 0) editVals.sStuntTimeBulging = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iTimeFlat);
    if (editVals.sStuntTimeFlat.isEmpty()) editVals.sStuntTimeFlat = sVal;
    else if (editVals.sStuntTimeFlat.compare(sVal) != 0) editVals.sStuntTimeFlat = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iSmallerExpandsLargerContracts);
    if (editVals.sStuntExpandsContracts.isEmpty()) editVals.sStuntExpandsContracts = sVal;
    else if (editVals.sStuntExpandsContracts.compare(sVal) != 0) editVals.sStuntExpandsContracts = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].stunt.iBulge);
    if (editVals.sStuntBulge.isEmpty()) editVals.sStuntBulge = sVal;
    else if (editVals.sStuntBulge.compare(sVal) != 0) editVals.sStuntBulge = MIXED_DATA;
  }
}

//-------------------------------------------------------------------------------------------------

void CTrack::ApplyGeometrySettings(int iStartIndex, int iEndIndex, const CChunkEditValues &editVals)
{
  for (int i = iStartIndex; i <= iEndIndex; ++i) {
    if (!editVals.sLeftShoulderWidth.isEmpty()) m_chunkAy[i].iLeftShoulderWidth = editVals.sLeftShoulderWidth.toInt();
    if (!editVals.sLeftLaneWidth.isEmpty()) m_chunkAy[i].iLeftLaneWidth = editVals.sLeftLaneWidth.toInt();
    if (!editVals.sRightLaneWidth.isEmpty()) m_chunkAy[i].iRightLaneWidth = editVals.sRightLaneWidth.toInt();
    if (!editVals.sRightShoulderWidth.isEmpty()) m_chunkAy[i].iRightShoulderWidth = editVals.sRightShoulderWidth.toInt();
    if (!editVals.sLShoulderHeight.isEmpty()) m_chunkAy[i].iLeftShoulderHeight = editVals.sLShoulderHeight.toInt();
    if (!editVals.sRShoulderHeight.isEmpty()) m_chunkAy[i].iRightShoulderHeight = editVals.sRShoulderHeight.toInt();
    if (!editVals.sLength.isEmpty()) m_chunkAy[i].iLength = editVals.sLength.toInt();
    if (!editVals.sYaw.isEmpty()) m_chunkAy[i].dYaw = editVals.sYaw.toDouble();
    if (!editVals.sPitch.isEmpty()) m_chunkAy[i].dPitch = editVals.sPitch.toDouble();
    if (!editVals.sRoll.isEmpty()) m_chunkAy[i].dRoll = editVals.sRoll.toDouble();
    if (!editVals.sAILine1.isEmpty()) m_chunkAy[i].iAILine1 = editVals.sAILine1.toInt();
    if (!editVals.sAILine2.isEmpty()) m_chunkAy[i].iAILine2 = editVals.sAILine2.toInt();
    if (!editVals.sAILine3.isEmpty()) m_chunkAy[i].iAILine3 = editVals.sAILine3.toInt();
    if (!editVals.sAILine4.isEmpty()) m_chunkAy[i].iAILine4 = editVals.sAILine4.toInt();
    if (!editVals.sTrackGrip.isEmpty()) m_chunkAy[i].iTrackGrip = editVals.sTrackGrip.toInt();
    if (!editVals.sLeftShoulderGrip.isEmpty()) m_chunkAy[i].iLeftShoulderGrip = editVals.sLeftShoulderGrip.toInt();
    if (!editVals.sRightShoulderGrip.isEmpty()) m_chunkAy[i].iRightShoulderGrip = editVals.sRightShoulderGrip.toInt();
    if (!editVals.sUnk04.isEmpty()) m_chunkAy[i].iUnk04 = editVals.sUnk04.toInt();
    if (!editVals.sUnk05.isEmpty()) m_chunkAy[i].iUnk05 = editVals.sUnk05.toInt();
    if (!editVals.sUnk06.isEmpty()) m_chunkAy[i].iUnk06 = editVals.sUnk06.toInt();
    if (!editVals.sUnk07.isEmpty()) m_chunkAy[i].iUnk07 = editVals.sUnk07.toInt();
    if (!editVals.sUnk08.isEmpty()) m_chunkAy[i].iUnk08 = editVals.sUnk08.toInt();
    if (!editVals.sLeftSurfaceType.isEmpty()) m_chunkAy[i].iLeftSurfaceType = editVals.sLeftSurfaceType.toInt();
    if (!editVals.sCenterSurfaceType.isEmpty()) m_chunkAy[i].iCenterSurfaceType = editVals.sCenterSurfaceType.toInt();
    if (!editVals.sRightSurfaceType.isEmpty()) m_chunkAy[i].iRightSurfaceType = editVals.sRightSurfaceType.toInt();
    if (!editVals.sLWallType.isEmpty()) m_chunkAy[i].iLeftWallType = editVals.sLWallType.toInt();
    if (!editVals.sRWallType.isEmpty()) m_chunkAy[i].iRightWallType = editVals.sRWallType.toInt();
    if (!editVals.sRoofType.isEmpty()) m_chunkAy[i].iRoofType = editVals.sRoofType.toInt();
    if (!editVals.sLUOuterWallType.isEmpty()) m_chunkAy[i].iLUOuterWallType = editVals.sLUOuterWallType.toInt();
    if (!editVals.sLLOuterWallType.isEmpty()) m_chunkAy[i].iLLOuterWallType = editVals.sLLOuterWallType.toInt();
    if (!editVals.sOuterFloorType.isEmpty()) m_chunkAy[i].iOuterFloorType = editVals.sOuterFloorType.toInt();
    if (!editVals.sRLOuterWallType.isEmpty()) m_chunkAy[i].iRLOuterWallType = editVals.sRLOuterWallType.toInt();
    if (!editVals.sRUOuterWallType.isEmpty()) m_chunkAy[i].iRUOuterWallType = editVals.sRUOuterWallType.toInt();
    if (!editVals.sEnvironmentFloorType.isEmpty()) m_chunkAy[i].iEnvironmentFloorType = editVals.sEnvironmentFloorType.toInt();
    if (!editVals.sSignType.isEmpty()) m_chunkAy[i].iSignType = editVals.sSignType.toInt();
    if (!editVals.sSignHorizOffset.isEmpty()) m_chunkAy[i].iSignHorizOffset = editVals.sSignHorizOffset.toInt();
    if (!editVals.sSignVertOffset.isEmpty()) m_chunkAy[i].iSignVertOffset = editVals.sSignVertOffset.toInt();
    if (!editVals.sSignYaw.isEmpty()) m_chunkAy[i].dSignYaw = editVals.sSignYaw.toDouble();
    if (!editVals.sSignPitch.isEmpty()) m_chunkAy[i].dSignPitch = editVals.sSignPitch.toDouble();
    if (!editVals.sSignRoll.isEmpty()) m_chunkAy[i].dSignRoll = editVals.sSignRoll.toDouble();
    if (!editVals.sLUOuterWallAngle.isEmpty()) m_chunkAy[i].iLUOuterWallAngle = editVals.sLUOuterWallAngle.toInt();
    if (!editVals.sLLOuterWallAngle.isEmpty()) m_chunkAy[i].iLLOuterWallAngle = editVals.sLLOuterWallAngle.toInt();
    if (!editVals.sUnk23.isEmpty()) m_chunkAy[i].iUnk23 = editVals.sUnk23.toInt();
    if (!editVals.sUnk24.isEmpty()) m_chunkAy[i].iUnk24 = editVals.sUnk24.toInt();
    if (!editVals.sRLOuterWallAngle.isEmpty()) m_chunkAy[i].iRLOuterWallAngle = editVals.sRLOuterWallAngle.toInt();
    if (!editVals.sRUOuterWallAngle.isEmpty()) m_chunkAy[i].iRUOuterWallAngle = editVals.sRUOuterWallAngle.toInt();
    if (!editVals.sLUOuterWallHeight.isEmpty()) m_chunkAy[i].iLUOuterWallHeight = editVals.sLUOuterWallHeight.toInt();
    if (!editVals.sLLOuterWallHeight.isEmpty()) m_chunkAy[i].iLLOuterWallHeight = editVals.sLLOuterWallHeight.toInt();
    if (!editVals.sUnk29.isEmpty()) m_chunkAy[i].iUnk29 = editVals.sUnk29.toInt();
    if (!editVals.sUnk30.isEmpty()) m_chunkAy[i].iUnk30 = editVals.sUnk30.toInt();
    if (!editVals.sRLOuterWallHeight.isEmpty()) m_chunkAy[i].iRLOuterWallHeight = editVals.sRLOuterWallHeight.toInt();
    if (!editVals.sRUOuterWallHeight.isEmpty()) m_chunkAy[i].iRUOuterWallHeight = editVals.sRUOuterWallHeight.toInt();
    if (!editVals.sRoofHeight.isEmpty()) m_chunkAy[i].iRoofHeight = editVals.sRoofHeight.toInt();
    if (!editVals.sDrawOrder1.isEmpty()) m_chunkAy[i].iDrawOrder1 = editVals.sDrawOrder1.toInt();
    if (!editVals.sDrawOrder2.isEmpty()) m_chunkAy[i].iDrawOrder2 = editVals.sDrawOrder2.toInt();
    if (!editVals.sDrawOrder3.isEmpty()) m_chunkAy[i].iDrawOrder3 = editVals.sDrawOrder3.toInt();
    if (!editVals.sUnk37.isEmpty()) m_chunkAy[i].iUnk37 = editVals.sUnk37.toInt();
    if (!editVals.sUnk38.isEmpty()) m_chunkAy[i].iUnk38 = editVals.sUnk38.toInt();
    if (!editVals.sUnk39.isEmpty()) m_chunkAy[i].iUnk39 = editVals.sUnk39.toInt();
    if (!editVals.sUnk40.isEmpty()) m_chunkAy[i].iUnk40 = editVals.sUnk40.toInt();
    if (!editVals.sUnk41.isEmpty()) m_chunkAy[i].iUnk41 = editVals.sUnk41.toInt();
    if (!editVals.sUnk42.isEmpty()) m_chunkAy[i].iUnk42 = editVals.sUnk42.toInt();
    if (!editVals.sUnk43.isEmpty()) m_chunkAy[i].iUnk43 = editVals.sUnk43.toInt();
    if (!editVals.sUnk44.isEmpty()) m_chunkAy[i].iUnk44 = editVals.sUnk44.toInt();
    if (!editVals.sUnk45.isEmpty()) m_chunkAy[i].iUnk45 = editVals.sUnk45.toInt();
    if (!editVals.sUnk46.isEmpty()) m_chunkAy[i].iUnk46 = editVals.sUnk46.toInt();
    if (!editVals.sUnk47.isEmpty()) m_chunkAy[i].iUnk47 = editVals.sUnk47.toInt();
    if (!editVals.sUnk48.isEmpty()) m_chunkAy[i].iUnk48 = editVals.sUnk48.toInt();
    if (!editVals.sUnk49.isEmpty()) m_chunkAy[i].iUnk49 = editVals.sUnk49.toInt();
    if (!editVals.sUnk50.isEmpty()) m_chunkAy[i].iUnk50 = editVals.sUnk50.toInt();
    if (!editVals.sSignTexture.isEmpty()) m_chunkAy[i].iSignTexture = editVals.sSignTexture.toInt();
    if (!editVals.sBackTexture.isEmpty()) m_chunkAy[i].iBackTexture = editVals.sBackTexture.toInt();
    if (!editVals.sStuntScaleFactor.isEmpty()) m_chunkAy[i].stunt.iScaleFactor = editVals.sStuntScaleFactor.toInt();
    if (!editVals.sStuntAngle.isEmpty()) m_chunkAy[i].stunt.iAngle = editVals.sStuntAngle.toInt();
    if (!editVals.sStuntUnknown.isEmpty()) m_chunkAy[i].stunt.iUnknown = editVals.sStuntUnknown.toInt();
    if (!editVals.sStuntTimingGroup.isEmpty()) m_chunkAy[i].stunt.iTimingGroup = editVals.sStuntTimingGroup.toInt();
    if (!editVals.sStuntHeight.isEmpty()) m_chunkAy[i].stunt.iHeight = editVals.sStuntHeight.toInt();
    if (!editVals.sStuntTimeBulging.isEmpty()) m_chunkAy[i].stunt.iTimeBulging = editVals.sStuntTimeBulging.toInt();
    if (!editVals.sStuntTimeFlat.isEmpty()) m_chunkAy[i].stunt.iTimeFlat = editVals.sStuntTimeFlat.toInt();
    if (!editVals.sStuntExpandsContracts.isEmpty()) m_chunkAy[i].stunt.iSmallerExpandsLargerContracts = editVals.sStuntExpandsContracts.toInt();
    if (!editVals.sStuntBulge.isEmpty()) m_chunkAy[i].stunt.iBulge = editVals.sStuntBulge.toInt();
  }
  UpdateChunkStrings();
  g_pMainWindow->LogMessage("Applied changes to " + QString::number(iEndIndex - iStartIndex + 1) + " geometry chunks");
}

//-------------------------------------------------------------------------------------------------

void CTrack::InsertGeometryChunk(int iIndex, int iCount, const CChunkEditValues &editVals)
{
  for (int i = 0; i < iCount; ++i) {
    struct tGeometryChunk newChunk;
    memset(&newChunk, sizeof(newChunk), 0);
    newChunk.iLeftShoulderWidth = editVals.sLeftShoulderWidth.toInt();
    newChunk.iLeftLaneWidth = editVals.sLeftLaneWidth.toInt();
    newChunk.iRightLaneWidth = editVals.sRightLaneWidth.toInt();
    newChunk.iRightShoulderWidth = editVals.sRightShoulderWidth.toInt();
    newChunk.iLeftShoulderHeight = editVals.sLShoulderHeight.toInt();
    newChunk.iRightShoulderHeight = editVals.sRShoulderHeight.toInt();
    newChunk.iLength = editVals.sLength.toInt();
    newChunk.dYaw = editVals.sYaw.toDouble();
    newChunk.dPitch = editVals.sPitch.toDouble();
    newChunk.dRoll = editVals.sRoll.toDouble();
    newChunk.iAILine1 = editVals.sAILine1.toInt();
    newChunk.iAILine2 = editVals.sAILine2.toInt();
    newChunk.iAILine3 = editVals.sAILine3.toInt();
    newChunk.iAILine4 = editVals.sAILine4.toInt();
    newChunk.iTrackGrip = editVals.sTrackGrip.toInt();
    newChunk.iLeftShoulderGrip = editVals.sLeftShoulderGrip.toInt();
    newChunk.iRightShoulderGrip = editVals.sRightShoulderGrip.toInt();
    newChunk.iUnk04 = editVals.sUnk04.toInt();
    newChunk.iUnk05 = editVals.sUnk05.toInt();
    newChunk.iUnk06 = editVals.sUnk06.toInt();
    newChunk.iUnk07 = editVals.sUnk07.toInt();
    newChunk.iUnk08 = editVals.sUnk08.toInt();
    newChunk.iLeftSurfaceType = editVals.sLeftSurfaceType.toInt();
    newChunk.iCenterSurfaceType = editVals.sCenterSurfaceType.toInt();
    newChunk.iRightSurfaceType = editVals.sRightSurfaceType.toInt();
    newChunk.iLeftWallType = editVals.sLWallType.toInt();
    newChunk.iRightWallType = editVals.sRWallType.toInt();
    newChunk.iRoofType = editVals.sRoofType.toInt();
    newChunk.iLUOuterWallType = editVals.sLUOuterWallType.toInt();
    newChunk.iLLOuterWallType = editVals.sLLOuterWallType.toInt();
    newChunk.iOuterFloorType = editVals.sOuterFloorType.toInt();
    newChunk.iRLOuterWallType = editVals.sRLOuterWallType.toInt();
    newChunk.iRUOuterWallType = editVals.sRUOuterWallType.toInt();
    newChunk.iEnvironmentFloorType = editVals.sEnvironmentFloorType.toInt();
    newChunk.iSignType = editVals.sSignType.toInt();
    newChunk.iSignHorizOffset = editVals.sSignHorizOffset.toInt();
    newChunk.iSignVertOffset = editVals.sSignVertOffset.toInt();
    newChunk.dSignYaw = editVals.sSignYaw.toDouble();
    newChunk.dSignPitch = editVals.sSignPitch.toDouble();
    newChunk.dSignRoll = editVals.sSignRoll.toDouble();
    newChunk.iLUOuterWallAngle = editVals.sLUOuterWallAngle.toInt();
    newChunk.iLLOuterWallAngle = editVals.sLLOuterWallAngle.toInt();
    newChunk.iUnk23 = editVals.sUnk23.toInt();
    newChunk.iUnk24 = editVals.sUnk24.toInt();
    newChunk.iRLOuterWallAngle = editVals.sRLOuterWallAngle.toInt();
    newChunk.iRUOuterWallAngle = editVals.sRUOuterWallAngle.toInt();
    newChunk.iLUOuterWallHeight = editVals.sLUOuterWallHeight.toInt();
    newChunk.iLLOuterWallHeight = editVals.sLLOuterWallHeight.toInt();
    newChunk.iUnk29 = editVals.sUnk29.toInt();
    newChunk.iUnk30 = editVals.sUnk30.toInt();
    newChunk.iRLOuterWallHeight = editVals.sRLOuterWallHeight.toInt();
    newChunk.iRUOuterWallHeight = editVals.sRUOuterWallHeight.toInt();
    newChunk.iRoofHeight = editVals.sRoofHeight.toInt();
    newChunk.iDrawOrder1 = editVals.sDrawOrder1.toInt();
    newChunk.iDrawOrder2 = editVals.sDrawOrder2.toInt();
    newChunk.iDrawOrder3 = editVals.sDrawOrder3.toInt();
    newChunk.iUnk37 = editVals.sUnk37.toInt();
    newChunk.iUnk38 = editVals.sUnk38.toInt();
    newChunk.iUnk39 = editVals.sUnk39.toInt();
    newChunk.iUnk40 = editVals.sUnk40.toInt();
    newChunk.iUnk41 = editVals.sUnk41.toInt();
    newChunk.iUnk42 = editVals.sUnk42.toInt();
    newChunk.iUnk43 = editVals.sUnk43.toInt();
    newChunk.iUnk44 = editVals.sUnk44.toInt();
    newChunk.iUnk45 = editVals.sUnk45.toInt();
    newChunk.iUnk46 = editVals.sUnk46.toInt();
    newChunk.iUnk47 = editVals.sUnk47.toInt();
    newChunk.iUnk48 = editVals.sUnk48.toInt();
    newChunk.iUnk49 = editVals.sUnk49.toInt();
    newChunk.iUnk50 = editVals.sUnk50.toInt();
    newChunk.iSignTexture = editVals.sSignTexture.toInt();
    newChunk.iBackTexture = editVals.sBackTexture.toInt();
    newChunk.stunt.iScaleFactor = editVals.sStuntScaleFactor.toInt();
    newChunk.stunt.iAngle = editVals.sStuntAngle.toInt();
    newChunk.stunt.iUnknown = editVals.sStuntUnknown.toInt();
    newChunk.stunt.iTimingGroup = editVals.sStuntTimingGroup.toInt();
    newChunk.stunt.iHeight = editVals.sStuntHeight.toInt();
    newChunk.stunt.iTimeBulging = editVals.sStuntTimeBulging.toInt();
    newChunk.stunt.iTimeFlat = editVals.sStuntTimeFlat.toInt();
    newChunk.stunt.iSmallerExpandsLargerContracts = editVals.sStuntExpandsContracts.toInt();
    newChunk.stunt.iBulge = editVals.sStuntBulge.toInt();
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

unsigned int CTrack::GetSignedBitValueFromInt(int iValue)
{
  bool bNegative = iValue < 0;
  unsigned int uiRetVal = (unsigned int)abs(iValue);
  if (bNegative)
    uiRetVal |= 0x80000000;
  return uiRetVal;
}

//-------------------------------------------------------------------------------------------------

int CTrack::GetIntValueFromSignedBit(unsigned int uiValue)
{
  bool bNegative = uiValue & 0x80000000;
  uiValue &= ~0x80000000;
  int iRetVal = (int)uiValue;
  if (bNegative)
    iRetVal = iRetVal * -1;
  return iRetVal;
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