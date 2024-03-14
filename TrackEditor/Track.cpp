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
#define TUPLES_COUNT 2
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
  m_chunkAy.clear();
  m_tupleMap.clear();
  m_stuntMap.clear();
  m_sTextureFile = "";
  m_sBuildingFile = "";
  m_backsMap.clear();
  memset(&m_raceInfo, 0, sizeof(m_raceInfo));
}

//-------------------------------------------------------------------------------------------------

bool CTrack::LoadTrack(const QString &sFilename)
{
  if (sFilename.isEmpty())
    return false;

  QFile file(sFilename);
  if (!file.open(QIODevice::ReadOnly))
    return false;

  ClearData();

  bool bSuccess = true;
  int iChunkLine = 0;
  struct tGeometryChunk currChunk;
  eFileSection section = HEADER;
  std::tuple<int, int> firstTuple;
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
          m_header.llUnk1 = slLine[1].toInt();
          m_header.llUnk2 = slLine[2].toInt();
          m_header.llUnk3 = slLine[3].toInt();
          section = GEOMETRY;
        }
        break;
      case GEOMETRY:
        if (iChunkLine == 0) {
          if (slLine.count() == TUPLES_COUNT) {
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
            section = TUPLES;
            ProcessTuple(slLine, section);
          } else if (slLine.count() == CHUNK_LINE_0_COUNT) {
            //start new chunk
            memset(&currChunk, 0, sizeof(currChunk));
            //process line 1
            currChunk.unLeftShoulderWidth = slLine[0].toUShort();
            currChunk.unLeftLaneWidth = slLine[1].toUShort();
            currChunk.unRightLaneWidth = slLine[2].toUShort();
            currChunk.unRightShoulderWidth = slLine[3].toUShort();
            currChunk.llUnk1 = slLine[4].toLongLong();
            currChunk.llUnk2 = slLine[5].toLongLong();
            currChunk.llUnk3 = slLine[6].toLongLong();
            currChunk.fYaw = slLine[7].toFloat();
            currChunk.fPitch = slLine[8].toFloat();
            currChunk.fRoll = slLine[9].toFloat();
            currChunk.iAILine1 = slLine[10].toInt();
            currChunk.iAILine2 = slLine[11].toInt();
            currChunk.iAILine3 = slLine[12].toInt();
            currChunk.iAILine4 = slLine[13].toInt();
            currChunk.unTrackGrip = slLine[14].toUShort();
            currChunk.unLeftShoulderGrip = slLine[15].toUShort();
            currChunk.unRightShoulderGrip = slLine[16].toUShort();
            currChunk.llUnk4 = slLine[17].toLongLong();
            currChunk.llUnk5 = slLine[18].toLongLong();
            currChunk.llUnk6 = slLine[19].toLongLong();
            currChunk.llUnk7 = slLine[20].toLongLong();
            currChunk.llUnk8 = slLine[21].toLongLong();
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
          currChunk.ulLeftSurfaceType = slLine[0].toULong();
          currChunk.ulCenterSurfaceType = slLine[1].toULong();
          currChunk.ulRightSurfaceType = slLine[2].toULong();
          currChunk.llUnk9 = slLine[3].toLongLong();
          currChunk.llUnk10 = slLine[4].toLongLong();
          currChunk.llUnk11 = slLine[5].toLongLong();
          currChunk.llUnk12 = slLine[6].toLongLong();
          currChunk.llUnk13 = slLine[7].toLongLong();
          currChunk.llUnk14 = slLine[8].toLongLong();
          currChunk.llUnk15 = slLine[9].toLongLong();
          currChunk.llUnk16 = slLine[10].toLongLong();
          currChunk.llUnk17 = slLine[11].toLongLong();
          currChunk.llUnk18 = slLine[12].toLongLong();
          currChunk.llUnk19 = slLine[13].toLongLong();
          currChunk.llUnk20 = slLine[14].toLongLong();
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
          currChunk.llUnk21 = slLine[0].toULong();
          currChunk.llUnk22 = slLine[1].toULong();
          currChunk.llUnk23 = slLine[2].toULong();
          currChunk.llUnk24 = slLine[3].toULong();
          currChunk.llUnk25 = slLine[4].toULong();
          currChunk.llUnk26 = slLine[5].toULong();
          currChunk.llUnk27 = slLine[6].toULong();
          currChunk.llUnk28 = slLine[7].toULong();
          currChunk.llUnk29 = slLine[8].toULong();
          currChunk.llUnk30 = slLine[9].toULong();
          currChunk.llUnk31 = slLine[10].toULong();
          currChunk.llUnk32 = slLine[11].toULong();
          currChunk.llUnk33 = slLine[12].toULong();
          currChunk.llUnk34 = slLine[13].toULong();
          currChunk.llUnk35 = slLine[14].toULong();
          currChunk.llUnk36 = slLine[15].toULong();
          currChunk.llUnk37 = slLine[16].toULong();
          currChunk.llUnk38 = slLine[17].toULong();
          currChunk.llUnk39 = slLine[18].toULong();
          currChunk.llUnk40 = slLine[19].toULong();
          currChunk.llUnk41 = slLine[20].toULong();
          currChunk.llUnk42 = slLine[21].toULong();
          currChunk.llUnk43 = slLine[22].toULong();
          currChunk.llUnk44 = slLine[23].toULong();
          currChunk.llUnk45 = slLine[24].toULong();
          currChunk.llUnk46 = slLine[25].toULong();
          currChunk.llUnk47 = slLine[26].toULong();
          currChunk.llUnk48 = slLine[27].toULong();
          currChunk.llUnk49 = slLine[28].toULong();
          currChunk.llUnk50 = slLine[29].toULong();
          //chunk is complete, add to array and reset index
          m_chunkAy.push_back(currChunk);
          iChunkLine = 0;
        }
        break;
      case TUPLES:
        if (slLine.count() == TUPLES_COUNT) {
          ProcessTuple(slLine, section);
        } else {
          assert(0);
          g_pMainWindow->LogMessage("Error loading file: tuples section ended before anticipated");
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
          m_stuntMap[iGeometryIndex] = stunt;
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
          m_backsMap[slLine[0].toInt()] = slLine[1].toInt();
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
    + "  unknown tuples: " + QString::number(m_tupleMap.size()) + "\n"
    + "  stunts: " + QString::number(m_stuntMap.size()) + "\n"
    + "  texture file: " + m_sTextureFile + "\n"
    + "  building file: " + m_sBuildingFile + "\n"
    + "  backs: " + QString::number(m_backsMap.size());
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
  if (file.open(QIODevice::ReadWrite)) {
    QTextStream stream(&file);
    //stream << "something" << endl;
    //write header
    //test
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

void CTrack::GetTextureCursorPos(int iKey, int &iStartCursorPos, int &iEndCursorPos)
{
  iStartCursorPos = 0;
  iEndCursorPos = 0;
  if (m_backsMap.empty()) return;
  CTupleMap::iterator it = m_backsMap.find(iKey);
  if (it == m_backsMap.end())
    return;
  
  QString sTex = "TEX:";
  QString sBld = "BLD:";
  iEndCursorPos = sTex.length() + sBld.length() + m_sTextureFile.length() + m_sBuildingFile.length() + 2;
  it = m_backsMap.begin();
  for (; it != m_backsMap.end(); ++it) {
    iStartCursorPos = iEndCursorPos;

    char szLine[20];
    snprintf(szLine, sizeof(szLine), "%d %d", it->first, it->second);
    iEndCursorPos += (int)strlen(szLine) + 1;

    if (it->first == iKey) {
      break;
    }
  };
  --iEndCursorPos;
}

//-------------------------------------------------------------------------------------------------

void CTrack::GetGeometryValuesFromSelection(int iStartIndex, int iEndIndex
    , QString &sLeftShoulderWidth, QString &sLeftLaneWidth, QString &sRightLaneWidth, QString &sRightShoulderWidth
    , QString &sUnk1, QString &sUnk2, QString &sUnk3
    , QString &sYaw, QString &sPitch, QString &sRoll
    , QString &sAILine1, QString &sAILine2, QString &sAILine3, QString &sAILine4
    , QString &sTrackGrip, QString &sLeftShoulderGrip, QString &sRightShoulderGrip
    , QString &sUnk4, QString &sUnk5, QString &sUnk6, QString &sUnk7, QString &sUnk8
    , QString &sLeftSurfaceType, QString &sCenterSurfaceType, QString &sRightSurfaceType
    , QString &sUnk9, QString &sUnk10, QString &sUnk11, QString &sUnk12, QString &sUnk13, QString &sUnk14
    , QString &sUnk15, QString &sUnk16, QString &sUnk17, QString &sUnk18, QString &sUnk19, QString &sUnk20
    , QString &sfUnk1, QString &sfUnk2, QString &sfUnk3
    , QString &sUnk21, QString &sUnk22, QString &sUnk23, QString &sUnk24, QString &sUnk25, QString &sUnk26
    , QString &sUnk27, QString &sUnk28, QString &sUnk29, QString &sUnk30, QString &sUnk31, QString &sUnk32
    , QString &sUnk33, QString &sUnk34, QString &sUnk35, QString &sUnk36, QString &sUnk37, QString &sUnk38
    , QString &sUnk39, QString &sUnk40, QString &sUnk41, QString &sUnk42, QString &sUnk43, QString &sUnk44
    , QString &sUnk45, QString &sUnk46, QString &sUnk47, QString &sUnk48, QString &sUnk49, QString &sUnk50)
{
  if (m_chunkAy.empty()) return;
  if (iEndIndex < iStartIndex || iEndIndex >= m_chunkAy.size()) {
    assert(0);
    return;
  }

  sLeftShoulderWidth = ""; sLeftLaneWidth = ""; sRightLaneWidth = ""; sRightShoulderWidth = "";
  sUnk1 = ""; sUnk2 = ""; sUnk3 = "";
  sYaw = ""; sPitch = ""; sRoll = "";
  sAILine1 = ""; sAILine2 = ""; sAILine3 = ""; sAILine4 = "";
  sTrackGrip = ""; sLeftShoulderGrip = ""; sRightShoulderGrip = "";
  sUnk4 = ""; sUnk5 = ""; sUnk6 = ""; sUnk7 = ""; sUnk8 = "";
  sLeftSurfaceType = ""; sCenterSurfaceType = ""; sRightSurfaceType = "";
  sUnk9 = ""; sUnk10 = ""; sUnk11 = ""; sUnk12 = ""; sUnk13 = ""; sUnk14 = "";
  sUnk15 = ""; sUnk16 = ""; sUnk17 = ""; sUnk18 = ""; sUnk19 = ""; sUnk20 = "";
  sfUnk1 = ""; sfUnk2 = ""; sfUnk3 = "";
  sUnk21 = ""; sUnk22 = ""; sUnk23 = ""; sUnk24 = ""; sUnk25 = ""; sUnk26 = "";
  sUnk27 = ""; sUnk28 = ""; sUnk29 = ""; sUnk30 = ""; sUnk31 = ""; sUnk32 = "";
  sUnk33 = ""; sUnk34 = ""; sUnk35 = ""; sUnk36 = ""; sUnk37 = ""; sUnk38 = "";
  sUnk39 = ""; sUnk40 = ""; sUnk41 = ""; sUnk42 = ""; sUnk43 = ""; sUnk44 = "";
  sUnk45 = ""; sUnk46 = ""; sUnk47 = ""; sUnk48 = ""; sUnk49 = ""; sUnk50 = "";

  QString sVal;
  for (int i = iStartIndex; i <= iEndIndex; ++i) {
    sVal = QString::number(m_chunkAy[i].unLeftShoulderWidth);
    if (sLeftShoulderWidth.isEmpty()) sLeftShoulderWidth = sVal;
    else if (sLeftShoulderWidth.compare(sVal) != 0) sLeftShoulderWidth = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].unLeftLaneWidth);
    if (sLeftLaneWidth.isEmpty()) sLeftLaneWidth = sVal;
    else if (sLeftLaneWidth.compare(sVal) != 0) sLeftLaneWidth = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].unRightLaneWidth);
    if (sRightLaneWidth.isEmpty()) sRightLaneWidth = sVal;
    else if (sRightLaneWidth.compare(sVal) != 0) sRightLaneWidth = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].unRightShoulderWidth);
    if (sRightShoulderWidth.isEmpty()) sRightShoulderWidth = sVal;
    else if (sRightShoulderWidth.compare(sVal) != 0) sRightShoulderWidth = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].llUnk1);
    if (sUnk1.isEmpty()) sUnk1 = sVal;
    else if (sUnk1.compare(sVal) != 0) sUnk1 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk2);
    if (sUnk2.isEmpty()) sUnk2 = sVal;
    else if (sUnk2.compare(sVal) != 0) sUnk2 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk3);
    if (sUnk3.isEmpty()) sUnk3 = sVal;
    else if (sUnk3.compare(sVal) != 0) sUnk3 = MIXED_DATA;

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

    sVal = QString::number(m_chunkAy[i].unTrackGrip);
    if (sTrackGrip.isEmpty()) sTrackGrip = sVal;
    else if (sTrackGrip.compare(sVal) != 0) sTrackGrip = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].unLeftShoulderGrip);
    if (sLeftShoulderGrip.isEmpty()) sLeftShoulderGrip = sVal;
    else if (sLeftShoulderGrip.compare(sVal) != 0) sLeftShoulderGrip = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].unRightShoulderGrip);
    if (sRightShoulderGrip.isEmpty()) sRightShoulderGrip = sVal;
    else if (sRightShoulderGrip.compare(sVal) != 0) sRightShoulderGrip = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].llUnk4);
    if (sUnk4.isEmpty()) sUnk4 = sVal;
    else if (sUnk4.compare(sVal) != 0) sUnk4 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk5);
    if (sUnk5.isEmpty()) sUnk5 = sVal;
    else if (sUnk5.compare(sVal) != 0) sUnk5 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk6);
    if (sUnk6.isEmpty()) sUnk6 = sVal;
    else if (sUnk6.compare(sVal) != 0) sUnk6 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk7);
    if (sUnk7.isEmpty()) sUnk7 = sVal;
    else if (sUnk7.compare(sVal) != 0) sUnk7 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk8);
    if (sUnk8.isEmpty()) sUnk8 = sVal;
    else if (sUnk8.compare(sVal) != 0) sUnk8 = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].ulLeftSurfaceType);
    if (sLeftSurfaceType.isEmpty()) sLeftSurfaceType = sVal;
    else if (sLeftSurfaceType.compare(sVal) != 0) sLeftSurfaceType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].ulCenterSurfaceType);
    if (sCenterSurfaceType.isEmpty()) sCenterSurfaceType = sVal;
    else if (sCenterSurfaceType.compare(sVal) != 0) sCenterSurfaceType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].ulRightSurfaceType);
    if (sRightSurfaceType.isEmpty()) sRightSurfaceType = sVal;
    else if (sRightSurfaceType.compare(sVal) != 0) sRightSurfaceType = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].llUnk9);
    if (sUnk9.isEmpty()) sUnk9 = sVal;
    else if (sUnk9.compare(sVal) != 0) sUnk9 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk10);
    if (sUnk10.isEmpty()) sUnk10 = sVal;
    else if (sUnk10.compare(sVal) != 0) sUnk10 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk11);
    if (sUnk11.isEmpty()) sUnk11 = sVal;
    else if (sUnk11.compare(sVal) != 0) sUnk11 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk12);
    if (sUnk12.isEmpty()) sUnk12 = sVal;
    else if (sUnk12.compare(sVal) != 0) sUnk12 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk13);
    if (sUnk13.isEmpty()) sUnk13 = sVal;
    else if (sUnk13.compare(sVal) != 0) sUnk13 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk14);
    if (sUnk14.isEmpty()) sUnk14 = sVal;
    else if (sUnk14.compare(sVal) != 0) sUnk14 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk15);
    if (sUnk15.isEmpty()) sUnk15 = sVal;
    else if (sUnk15.compare(sVal) != 0) sUnk15 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk16);
    if (sUnk16.isEmpty()) sUnk16 = sVal;
    else if (sUnk16.compare(sVal) != 0) sUnk16 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk17);
    if (sUnk17.isEmpty()) sUnk17 = sVal;
    else if (sUnk17.compare(sVal) != 0) sUnk17 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk18);
    if (sUnk18.isEmpty()) sUnk18 = sVal;
    else if (sUnk18.compare(sVal) != 0) sUnk18 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk19);
    if (sUnk19.isEmpty()) sUnk19 = sVal;
    else if (sUnk19.compare(sVal) != 0) sUnk19 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk20);
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
 
    sVal = QString::number(m_chunkAy[i].llUnk21);
    if (sUnk21.isEmpty()) sUnk21 = sVal;
    else if (sUnk21.compare(sVal) != 0) sUnk21 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk22);
    if (sUnk22.isEmpty()) sUnk22 = sVal;
    else if (sUnk22.compare(sVal) != 0) sUnk22 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk23);
    if (sUnk23.isEmpty()) sUnk23 = sVal;
    else if (sUnk23.compare(sVal) != 0) sUnk23 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk24);
    if (sUnk24.isEmpty()) sUnk24 = sVal;
    else if (sUnk24.compare(sVal) != 0) sUnk24 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk25);
    if (sUnk25.isEmpty()) sUnk25 = sVal;
    else if (sUnk25.compare(sVal) != 0) sUnk25 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk26);
    if (sUnk26.isEmpty()) sUnk26 = sVal;
    else if (sUnk26.compare(sVal) != 0) sUnk26 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk27);
    if (sUnk27.isEmpty()) sUnk27 = sVal;
    else if (sUnk27.compare(sVal) != 0) sUnk27 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk28);
    if (sUnk28.isEmpty()) sUnk28 = sVal;
    else if (sUnk28.compare(sVal) != 0) sUnk28 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk29);
    if (sUnk29.isEmpty()) sUnk29 = sVal;
    else if (sUnk29.compare(sVal) != 0) sUnk29 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk30);
    if (sUnk30.isEmpty()) sUnk30 = sVal;
    else if (sUnk30.compare(sVal) != 0) sUnk30 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk31);
    if (sUnk31.isEmpty()) sUnk31 = sVal;
    else if (sUnk31.compare(sVal) != 0) sUnk31 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk32);
    if (sUnk32.isEmpty()) sUnk32 = sVal;
    else if (sUnk32.compare(sVal) != 0) sUnk32 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk33);
    if (sUnk33.isEmpty()) sUnk33 = sVal;
    else if (sUnk33.compare(sVal) != 0) sUnk33 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk34);
    if (sUnk34.isEmpty()) sUnk34 = sVal;
    else if (sUnk34.compare(sVal) != 0) sUnk34 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk35);
    if (sUnk35.isEmpty()) sUnk35 = sVal;
    else if (sUnk35.compare(sVal) != 0) sUnk35 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk36);
    if (sUnk36.isEmpty()) sUnk36 = sVal;
    else if (sUnk36.compare(sVal) != 0) sUnk36 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk37);
    if (sUnk37.isEmpty()) sUnk37 = sVal;
    else if (sUnk37.compare(sVal) != 0) sUnk37 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk38);
    if (sUnk38.isEmpty()) sUnk38 = sVal;
    else if (sUnk38.compare(sVal) != 0) sUnk38 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk39);
    if (sUnk39.isEmpty()) sUnk39 = sVal;
    else if (sUnk39.compare(sVal) != 0) sUnk39 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk40);
    if (sUnk40.isEmpty()) sUnk40 = sVal;
    else if (sUnk40.compare(sVal) != 0) sUnk40 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk41);
    if (sUnk41.isEmpty()) sUnk41 = sVal;
    else if (sUnk41.compare(sVal) != 0) sUnk41 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk42);
    if (sUnk42.isEmpty()) sUnk42 = sVal;
    else if (sUnk42.compare(sVal) != 0) sUnk42 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk43);
    if (sUnk43.isEmpty()) sUnk43 = sVal;
    else if (sUnk43.compare(sVal) != 0) sUnk43 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk44);
    if (sUnk44.isEmpty()) sUnk44 = sVal;
    else if (sUnk44.compare(sVal) != 0) sUnk44 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk45);
    if (sUnk45.isEmpty()) sUnk45 = sVal;
    else if (sUnk45.compare(sVal) != 0) sUnk45 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk46);
    if (sUnk46.isEmpty()) sUnk46 = sVal;
    else if (sUnk46.compare(sVal) != 0) sUnk46 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk47);
    if (sUnk47.isEmpty()) sUnk47 = sVal;
    else if (sUnk47.compare(sVal) != 0) sUnk47 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk48);
    if (sUnk48.isEmpty()) sUnk48 = sVal;
    else if (sUnk48.compare(sVal) != 0) sUnk48 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk49);
    if (sUnk49.isEmpty()) sUnk49 = sVal;
    else if (sUnk49.compare(sVal) != 0) sUnk49 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].llUnk50);
    if (sUnk50.isEmpty()) sUnk50 = sVal;
    else if (sUnk50.compare(sVal) != 0) sUnk50 = MIXED_DATA;
  }
}

//-------------------------------------------------------------------------------------------------

void CTrack::ApplyGeometrySettings(int iStartIndex, int iEndIndex
    , const QString &sLeftShoulderWidth, const QString &sLeftLaneWidth, const QString &sRightLaneWidth, const QString &sRightShoulderWidth
    , const QString &sUnk1, const QString &sUnk2, const QString &sUnk3
    , const QString &sYaw, const QString &sPitch, const QString &sRoll
    , const QString &sAILine1, const QString &sAILine2, const QString &sAILine3, const QString &sAILine4
    , const QString &sTrackGrip, const QString &sLeftShoulderGrip, const QString &sRightShoulderGrip
    , const QString &sUnk4, const QString &sUnk5, const QString &sUnk6, const QString &sUnk7, const QString &sUnk8
    , const QString &sLeftSurfaceType, const QString &sCenterSurfaceType, const QString &sRightSurfaceType
    , const QString &sUnk9, const QString &sUnk10, const QString &sUnk11, const QString &sUnk12, const QString &sUnk13, const QString &sUnk14
    , const QString &sUnk15, const QString &sUnk16, const QString &sUnk17, const QString &sUnk18, const QString &sUnk19, const QString &sUnk20
    , const QString &sfUnk1, const QString &sfUnk2, const QString &sfUnk3
    , const QString &sUnk21, const QString &sUnk22, const QString &sUnk23, const QString &sUnk24, const QString &sUnk25, const QString &sUnk26
    , const QString &sUnk27, const QString &sUnk28, const QString &sUnk29, const QString &sUnk30, const QString &sUnk31, const QString &sUnk32
    , const QString &sUnk33, const QString &sUnk34, const QString &sUnk35, const QString &sUnk36, const QString &sUnk37, const QString &sUnk38
    , const QString &sUnk39, const QString &sUnk40, const QString &sUnk41, const QString &sUnk42, const QString &sUnk43, const QString &sUnk44
    , const QString &sUnk45, const QString &sUnk46, const QString &sUnk47, const QString &sUnk48, const QString &sUnk49, const QString &sUnk50)
{
  for (int i = iStartIndex; i <= iEndIndex; ++i) {
    if (!sLeftShoulderWidth.isEmpty()) m_chunkAy[i].unLeftShoulderWidth = sLeftShoulderWidth.toUShort();
    if (!sLeftLaneWidth.isEmpty()) m_chunkAy[i].unLeftLaneWidth = sLeftLaneWidth.toUShort();
    if (!sRightLaneWidth.isEmpty()) m_chunkAy[i].unRightLaneWidth = sRightLaneWidth.toUShort();
    if (!sRightShoulderWidth.isEmpty()) m_chunkAy[i].unRightShoulderWidth = sRightShoulderWidth.toUShort();
    if (!sUnk1.isEmpty()) m_chunkAy[i].llUnk1 = sUnk1.toLongLong();
    if (!sUnk2.isEmpty()) m_chunkAy[i].llUnk2 = sUnk2.toLongLong();
    if (!sUnk3.isEmpty()) m_chunkAy[i].llUnk3 = sUnk3.toLongLong();
    if (!sYaw.isEmpty()) m_chunkAy[i].fYaw = sYaw.toFloat();
    if (!sPitch.isEmpty()) m_chunkAy[i].fPitch = sPitch.toFloat();
    if (!sRoll.isEmpty()) m_chunkAy[i].fRoll = sRoll.toFloat();
    if (!sAILine1.isEmpty()) m_chunkAy[i].iAILine1 = sAILine1.toInt();
    if (!sAILine2.isEmpty()) m_chunkAy[i].iAILine2 = sAILine2.toInt();
    if (!sAILine3.isEmpty()) m_chunkAy[i].iAILine3 = sAILine3.toInt();
    if (!sAILine4.isEmpty()) m_chunkAy[i].iAILine4 = sAILine4.toInt();
    if (!sTrackGrip.isEmpty()) m_chunkAy[i].unTrackGrip = sTrackGrip.toUShort();
    if (!sLeftShoulderGrip.isEmpty()) m_chunkAy[i].unLeftShoulderGrip = sLeftShoulderGrip.toUShort();
    if (!sRightShoulderGrip.isEmpty()) m_chunkAy[i].unRightShoulderGrip = sRightShoulderGrip.toUShort();
    if (!sUnk4.isEmpty()) m_chunkAy[i].llUnk4 = sUnk4.toLongLong();
    if (!sUnk5.isEmpty()) m_chunkAy[i].llUnk5 = sUnk5.toLongLong();
    if (!sUnk6.isEmpty()) m_chunkAy[i].llUnk6 = sUnk6.toLongLong();
    if (!sUnk7.isEmpty()) m_chunkAy[i].llUnk7 = sUnk7.toLongLong();
    if (!sUnk8.isEmpty()) m_chunkAy[i].llUnk8 = sUnk8.toLongLong();
    if (!sLeftSurfaceType.isEmpty()) m_chunkAy[i].ulLeftSurfaceType = sLeftSurfaceType.toULong();
    if (!sCenterSurfaceType.isEmpty()) m_chunkAy[i].ulCenterSurfaceType = sCenterSurfaceType.toULong();
    if (!sRightSurfaceType.isEmpty()) m_chunkAy[i].ulRightSurfaceType = sRightSurfaceType.toULong();
    if (!sUnk9.isEmpty()) m_chunkAy[i].llUnk9 = sUnk9.toLongLong();
    if (!sUnk10.isEmpty()) m_chunkAy[i].llUnk10 = sUnk10.toLongLong();
    if (!sUnk11.isEmpty()) m_chunkAy[i].llUnk11 = sUnk11.toLongLong();
    if (!sUnk12.isEmpty()) m_chunkAy[i].llUnk12 = sUnk12.toLongLong();
    if (!sUnk13.isEmpty()) m_chunkAy[i].llUnk13 = sUnk13.toLongLong();
    if (!sUnk14.isEmpty()) m_chunkAy[i].llUnk14 = sUnk14.toLongLong();
    if (!sUnk15.isEmpty()) m_chunkAy[i].llUnk15 = sUnk15.toLongLong();
    if (!sUnk16.isEmpty()) m_chunkAy[i].llUnk16 = sUnk16.toLongLong();
    if (!sUnk17.isEmpty()) m_chunkAy[i].llUnk17 = sUnk17.toLongLong();
    if (!sUnk18.isEmpty()) m_chunkAy[i].llUnk18 = sUnk18.toLongLong();
    if (!sUnk19.isEmpty()) m_chunkAy[i].llUnk19 = sUnk19.toLongLong();
    if (!sUnk20.isEmpty()) m_chunkAy[i].llUnk20 = sUnk20.toLongLong();
    if (!sfUnk1.isEmpty()) m_chunkAy[i].fUnk1 = sfUnk1.toFloat();
    if (!sfUnk2.isEmpty()) m_chunkAy[i].fUnk2 = sfUnk2.toFloat();
    if (!sfUnk3.isEmpty()) m_chunkAy[i].fUnk3 = sfUnk3.toFloat();
    if (!sUnk21.isEmpty()) m_chunkAy[i].llUnk21 = sUnk21.toLongLong();
    if (!sUnk22.isEmpty()) m_chunkAy[i].llUnk22 = sUnk22.toLongLong();
    if (!sUnk23.isEmpty()) m_chunkAy[i].llUnk23 = sUnk23.toLongLong();
    if (!sUnk24.isEmpty()) m_chunkAy[i].llUnk24 = sUnk24.toLongLong();
    if (!sUnk25.isEmpty()) m_chunkAy[i].llUnk25 = sUnk25.toLongLong();
    if (!sUnk26.isEmpty()) m_chunkAy[i].llUnk26 = sUnk26.toLongLong();
    if (!sUnk27.isEmpty()) m_chunkAy[i].llUnk27 = sUnk27.toLongLong();
    if (!sUnk28.isEmpty()) m_chunkAy[i].llUnk28 = sUnk28.toLongLong();
    if (!sUnk29.isEmpty()) m_chunkAy[i].llUnk29 = sUnk29.toLongLong();
    if (!sUnk30.isEmpty()) m_chunkAy[i].llUnk30 = sUnk30.toLongLong();
    if (!sUnk31.isEmpty()) m_chunkAy[i].llUnk31 = sUnk31.toLongLong();
    if (!sUnk32.isEmpty()) m_chunkAy[i].llUnk32 = sUnk32.toLongLong();
    if (!sUnk33.isEmpty()) m_chunkAy[i].llUnk33 = sUnk33.toLongLong();
    if (!sUnk34.isEmpty()) m_chunkAy[i].llUnk34 = sUnk34.toLongLong();
    if (!sUnk35.isEmpty()) m_chunkAy[i].llUnk35 = sUnk35.toLongLong();
    if (!sUnk36.isEmpty()) m_chunkAy[i].llUnk36 = sUnk36.toLongLong();
    if (!sUnk37.isEmpty()) m_chunkAy[i].llUnk37 = sUnk37.toLongLong();
    if (!sUnk38.isEmpty()) m_chunkAy[i].llUnk38 = sUnk38.toLongLong();
    if (!sUnk39.isEmpty()) m_chunkAy[i].llUnk39 = sUnk39.toLongLong();
    if (!sUnk40.isEmpty()) m_chunkAy[i].llUnk40 = sUnk40.toLongLong();
    if (!sUnk41.isEmpty()) m_chunkAy[i].llUnk41 = sUnk41.toLongLong();
    if (!sUnk42.isEmpty()) m_chunkAy[i].llUnk42 = sUnk42.toLongLong();
    if (!sUnk43.isEmpty()) m_chunkAy[i].llUnk43 = sUnk43.toLongLong();
    if (!sUnk44.isEmpty()) m_chunkAy[i].llUnk44 = sUnk44.toLongLong();
    if (!sUnk45.isEmpty()) m_chunkAy[i].llUnk45 = sUnk45.toLongLong();
    if (!sUnk46.isEmpty()) m_chunkAy[i].llUnk46 = sUnk46.toLongLong();
    if (!sUnk47.isEmpty()) m_chunkAy[i].llUnk47 = sUnk47.toLongLong();
    if (!sUnk48.isEmpty()) m_chunkAy[i].llUnk48 = sUnk48.toLongLong();
    if (!sUnk49.isEmpty()) m_chunkAy[i].llUnk49 = sUnk49.toLongLong();
    if (!sUnk50.isEmpty()) m_chunkAy[i].llUnk50 = sUnk50.toLongLong();
  }
  UpdateChunkStrings();
  g_pMainWindow->LogMessage("Applied changes to " + QString::number(iEndIndex - iStartIndex + 1) + " geometry chunks");
}

//-------------------------------------------------------------------------------------------------

void CTrack::InsertGeometryChunk(int iIndex, int iCount
  , const QString &sLeftShoulderWidth, const QString &sLeftLaneWidth, const QString &sRightLaneWidth, const QString &sRightShoulderWidth
  , const QString &sUnk1, const QString &sUnk2, const QString &sUnk3
  , const QString &sYaw, const QString &sPitch, const QString &sRoll
  , const QString &sAILine1, const QString &sAILine2, const QString &sAILine3, const QString &sAILine4
  , const QString &sTrackGrip, const QString &sLeftShoulderGrip, const QString &sRightShoulderGrip
  , const QString &sUnk4, const QString &sUnk5, const QString &sUnk6, const QString &sUnk7, const QString &sUnk8
  , const QString &sLeftSurfaceType, const QString &sCenterSurfaceType, const QString &sRightSurfaceType
  , const QString &sUnk9, const QString &sUnk10, const QString &sUnk11, const QString &sUnk12, const QString &sUnk13, const QString &sUnk14
  , const QString &sUnk15, const QString &sUnk16, const QString &sUnk17, const QString &sUnk18, const QString &sUnk19, const QString &sUnk20
  , const QString &sfUnk1, const QString &sfUnk2, const QString &sfUnk3
  , const QString &sUnk21, const QString &sUnk22, const QString &sUnk23, const QString &sUnk24, const QString &sUnk25, const QString &sUnk26
  , const QString &sUnk27, const QString &sUnk28, const QString &sUnk29, const QString &sUnk30, const QString &sUnk31, const QString &sUnk32
  , const QString &sUnk33, const QString &sUnk34, const QString &sUnk35, const QString &sUnk36, const QString &sUnk37, const QString &sUnk38
  , const QString &sUnk39, const QString &sUnk40, const QString &sUnk41, const QString &sUnk42, const QString &sUnk43, const QString &sUnk44
  , const QString &sUnk45, const QString &sUnk46, const QString &sUnk47, const QString &sUnk48, const QString &sUnk49, const QString &sUnk50)
{
  for (int i = 0; i < iCount; ++i) {
    struct tGeometryChunk newChunk;
    memset(&newChunk, sizeof(newChunk), 0);
    newChunk.unLeftShoulderWidth = sLeftShoulderWidth.toUShort();
    newChunk.unLeftLaneWidth = sLeftLaneWidth.toUShort();
    newChunk.unRightLaneWidth = sRightLaneWidth.toUShort();
    newChunk.unRightShoulderWidth = sRightShoulderWidth.toUShort();
    newChunk.llUnk1 = sUnk1.toLongLong();
    newChunk.llUnk2 = sUnk2.toLongLong();
    newChunk.llUnk3 = sUnk3.toLongLong();
    newChunk.fYaw = sYaw.toFloat();
    newChunk.fPitch = sPitch.toFloat();
    newChunk.fRoll = sRoll.toFloat();
    newChunk.iAILine1 = sAILine1.toInt();
    newChunk.iAILine2 = sAILine2.toInt();
    newChunk.iAILine3 = sAILine3.toInt();
    newChunk.iAILine4 = sAILine4.toInt();
    newChunk.unTrackGrip = sTrackGrip.toUShort();
    newChunk.unLeftShoulderGrip = sLeftShoulderGrip.toUShort();
    newChunk.unRightShoulderGrip = sRightShoulderGrip.toUShort();
    newChunk.llUnk4 = sUnk4.toLongLong();
    newChunk.llUnk5 = sUnk5.toLongLong();
    newChunk.llUnk6 = sUnk6.toLongLong();
    newChunk.llUnk7 = sUnk7.toLongLong();
    newChunk.llUnk8 = sUnk8.toLongLong();
    newChunk.ulLeftSurfaceType = sLeftSurfaceType.toULong();
    newChunk.ulCenterSurfaceType = sCenterSurfaceType.toULong();
    newChunk.ulRightSurfaceType = sRightSurfaceType.toULong();
    newChunk.llUnk9 = sUnk9.toLongLong();
    newChunk.llUnk10 = sUnk10.toLongLong();
    newChunk.llUnk11 = sUnk11.toLongLong();
    newChunk.llUnk12 = sUnk12.toLongLong();
    newChunk.llUnk13 = sUnk13.toLongLong();
    newChunk.llUnk14 = sUnk14.toLongLong();
    newChunk.llUnk15 = sUnk15.toLongLong();
    newChunk.llUnk16 = sUnk16.toLongLong();
    newChunk.llUnk17 = sUnk17.toLongLong();
    newChunk.llUnk18 = sUnk18.toLongLong();
    newChunk.llUnk19 = sUnk19.toLongLong();
    newChunk.llUnk20 = sUnk20.toLongLong();
    newChunk.fUnk1 = sfUnk1.toFloat();
    newChunk.fUnk2 = sfUnk2.toFloat();
    newChunk.fUnk3 = sfUnk3.toFloat();
    newChunk.llUnk21 = sUnk21.toLongLong();
    newChunk.llUnk22 = sUnk22.toLongLong();
    newChunk.llUnk23 = sUnk23.toLongLong();
    newChunk.llUnk24 = sUnk24.toLongLong();
    newChunk.llUnk25 = sUnk25.toLongLong();
    newChunk.llUnk26 = sUnk26.toLongLong();
    newChunk.llUnk27 = sUnk27.toLongLong();
    newChunk.llUnk28 = sUnk28.toLongLong();
    newChunk.llUnk29 = sUnk29.toLongLong();
    newChunk.llUnk30 = sUnk30.toLongLong();
    newChunk.llUnk31 = sUnk31.toLongLong();
    newChunk.llUnk32 = sUnk32.toLongLong();
    newChunk.llUnk33 = sUnk33.toLongLong();
    newChunk.llUnk34 = sUnk34.toLongLong();
    newChunk.llUnk35 = sUnk35.toLongLong();
    newChunk.llUnk36 = sUnk36.toLongLong();
    newChunk.llUnk37 = sUnk37.toLongLong();
    newChunk.llUnk38 = sUnk38.toLongLong();
    newChunk.llUnk39 = sUnk39.toLongLong();
    newChunk.llUnk40 = sUnk40.toLongLong();
    newChunk.llUnk41 = sUnk41.toLongLong();
    newChunk.llUnk42 = sUnk42.toLongLong();
    newChunk.llUnk43 = sUnk43.toLongLong();
    newChunk.llUnk44 = sUnk44.toLongLong();
    newChunk.llUnk45 = sUnk45.toLongLong();
    newChunk.llUnk46 = sUnk46.toLongLong();
    newChunk.llUnk47 = sUnk47.toLongLong();
    newChunk.llUnk48 = sUnk48.toLongLong();
    newChunk.llUnk49 = sUnk49.toLongLong();
    newChunk.llUnk50 = sUnk50.toLongLong();
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
           "%5d%7d%7d%7d%7lld%7lld%7lld%12.5f%12.5f%12.5f%6d%6d%6d%6d%5d%4d%4d%5lld%6lld%4lld%4lld%4lld\n" //line 1
           "%9lu%7lu%7lu%7lld%7lld%7lld%7lld%10lld%7lld%10lld%7lld%7lld%5lld%7lld%7lld%7.1f%7.1f%7.1f\n"   //line 2
           "%5lld%7lld%7lld%7lld%7lld%7lld%7lld%7lld%7lld%7lld%7lld%7lld%7lld"                             //line 3
           "%4lld%4lld%4lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld%4lld%4lld%4lld\n"       //line 3 continued
           , chunk.unLeftShoulderWidth
           , chunk.unLeftLaneWidth
           , chunk.unRightLaneWidth
           , chunk.unRightShoulderWidth
           , chunk.llUnk1
           , chunk.llUnk2
           , chunk.llUnk3
           , chunk.fYaw
           , chunk.fPitch
           , chunk.fRoll
           , chunk.iAILine1
           , chunk.iAILine2
           , chunk.iAILine3
           , chunk.iAILine4
           , chunk.unTrackGrip
           , chunk.unLeftShoulderGrip
           , chunk.unRightShoulderGrip
           , chunk.llUnk4
           , chunk.llUnk5
           , chunk.llUnk6
           , chunk.llUnk7
           , chunk.llUnk8
           , chunk.ulLeftSurfaceType
           , chunk.ulCenterSurfaceType
           , chunk.ulRightSurfaceType
           , chunk.llUnk9
           , chunk.llUnk10
           , chunk.llUnk11
           , chunk.llUnk12
           , chunk.llUnk13
           , chunk.llUnk14
           , chunk.llUnk15
           , chunk.llUnk16
           , chunk.llUnk17
           , chunk.llUnk18
           , chunk.llUnk19
           , chunk.llUnk20
           , chunk.fUnk1
           , chunk.fUnk2
           , chunk.fUnk3
           , chunk.llUnk21
           , chunk.llUnk22
           , chunk.llUnk23
           , chunk.llUnk24
           , chunk.llUnk25
           , chunk.llUnk26
           , chunk.llUnk27
           , chunk.llUnk28
           , chunk.llUnk29
           , chunk.llUnk30
           , chunk.llUnk31
           , chunk.llUnk32
           , chunk.llUnk33
           , chunk.llUnk34
           , chunk.llUnk35
           , chunk.llUnk36
           , chunk.llUnk37
           , chunk.llUnk38
           , chunk.llUnk39
           , chunk.llUnk40
           , chunk.llUnk41
           , chunk.llUnk42
           , chunk.llUnk43
           , chunk.llUnk44
           , chunk.llUnk45
           , chunk.llUnk46
           , chunk.llUnk47
           , chunk.llUnk48
           , chunk.llUnk49
           , chunk.llUnk50);
  chunk.sString = szGenerate;
}

//-------------------------------------------------------------------------------------------------

void CTrack::ProcessTuple(const QStringList &slLine, eFileSection &section)
{
  //helper function because this process must be done in two places
  int iVal0 = slLine[0].toInt();
  int iVal1 = slLine[1].toInt();
  if (iVal0 == -1 || iVal1 == -1) {
    //tuple section always ends in two -1 values
    section = STUNTS;
  } else {
    //process tuple
    m_tupleMap[iVal0] = iVal1;
  }
}

//-------------------------------------------------------------------------------------------------