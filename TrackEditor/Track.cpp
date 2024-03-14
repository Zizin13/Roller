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
  m_header.iUnk3 = 2048;
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
          m_header.iUnk1 = slLine[1].toInt();
          m_header.iUnk2 = slLine[2].toInt();
          m_header.iUnk3 = slLine[3].toInt();
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
            currChunk.iLeftShoulderWidth = slLine[0].toInt();
            currChunk.iLeftLaneWidth = slLine[1].toInt();
            currChunk.iRightLaneWidth = slLine[2].toInt();
            currChunk.iRightShoulderWidth = slLine[3].toInt();
            currChunk.iUnk1 = slLine[4].toInt();
            currChunk.iUnk2 = slLine[5].toInt();
            currChunk.iUnk3 = slLine[6].toInt();
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
            currChunk.iUnk4 = slLine[17].toInt();
            currChunk.iUnk5 = slLine[18].toInt();
            currChunk.iUnk6 = slLine[19].toInt();
            currChunk.iUnk7 = slLine[20].toInt();
            currChunk.iUnk8 = slLine[21].toInt();
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
          currChunk.iUnk9 = slLine[3].toInt();
          currChunk.iUnk10 = slLine[4].toInt();
          currChunk.iUnk11 = slLine[5].toInt();
          currChunk.iUnk12 = slLine[6].toInt();
          currChunk.iUnk13 = slLine[7].toInt();
          currChunk.iUnk14 = slLine[8].toInt();
          currChunk.iUnk15 = slLine[9].toInt();
          currChunk.iUnk16 = slLine[10].toInt();
          currChunk.iUnk17 = slLine[11].toInt();
          currChunk.iUnk18 = slLine[12].toInt();
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
          currChunk.iUnk21 = slLine[0].toInt();
          currChunk.iUnk22 = slLine[1].toInt();
          currChunk.iUnk23 = slLine[2].toInt();
          currChunk.iUnk24 = slLine[3].toInt();
          currChunk.iUnk25 = slLine[4].toInt();
          currChunk.iUnk26 = slLine[5].toInt();
          currChunk.iUnk27 = slLine[6].toInt();
          currChunk.iUnk28 = slLine[7].toInt();
          currChunk.iUnk29 = slLine[8].toInt();
          currChunk.iUnk30 = slLine[9].toInt();
          currChunk.iUnk31 = slLine[10].toInt();
          currChunk.iUnk32 = slLine[11].toInt();
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
  file.resize(0);
  if (file.open(QIODevice::ReadWrite)) {
    QTextStream stream(&file);

    //write header
    char szBuf[1024];
    snprintf(szBuf, sizeof(szBuf), " %4d %6d %6d  %6d", (int)m_chunkAy.size(), m_header.iUnk1, m_header.iUnk2, m_header.iUnk3);
    stream << szBuf << Qt::endl << Qt::endl << Qt::endl;

    //write chunks
    for (int i = 0; i < m_chunkAy.size(); ++i) {
      stream << m_chunkAy[i].sString.c_str() << Qt::endl;
    }
    
    //write signs
    for (CTupleMap::iterator it = m_tupleMap.begin(); it != m_tupleMap.end(); ++it) {
      memset(szBuf, 0, sizeof(szBuf));
      snprintf(szBuf, sizeof(szBuf), " %4d %6d", it->first, it->second);
      stream << szBuf << Qt::endl;
    }
    memset(szBuf, 0, sizeof(szBuf));
    snprintf(szBuf, sizeof(szBuf), " %4d %6d", -1, -1);
    stream << szBuf << Qt::endl;
    stream << Qt::endl;

    //write stunts
    for (CStuntMap::iterator it = m_stuntMap.begin(); it != m_stuntMap.end(); ++it) {
      memset(szBuf, 0, sizeof(szBuf));
      snprintf(szBuf, sizeof(szBuf), " %4d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
               it->first, it->second.iScaleFactor, it->second.iAngle, it->second.iUnknown,
               it->second.iTimingGroup, it->second.iHeight, it->second.iTimeBulging,
               it->second.iTimeFlat, it->second.iSmallerExpandsLargerContracts, it->second.iBulge);
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
    for (CTupleMap::iterator it = m_backsMap.begin(); it != m_backsMap.end(); ++it) {
      memset(szBuf, 0, sizeof(szBuf));
      snprintf(szBuf, sizeof(szBuf), "%d %d", it->first, it->second);
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

    sVal = QString::number(m_chunkAy[i].iUnk1);
    if (sUnk1.isEmpty()) sUnk1 = sVal;
    else if (sUnk1.compare(sVal) != 0) sUnk1 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk2);
    if (sUnk2.isEmpty()) sUnk2 = sVal;
    else if (sUnk2.compare(sVal) != 0) sUnk2 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk3);
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

    sVal = QString::number(m_chunkAy[i].iUnk4);
    if (sUnk4.isEmpty()) sUnk4 = sVal;
    else if (sUnk4.compare(sVal) != 0) sUnk4 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk5);
    if (sUnk5.isEmpty()) sUnk5 = sVal;
    else if (sUnk5.compare(sVal) != 0) sUnk5 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk6);
    if (sUnk6.isEmpty()) sUnk6 = sVal;
    else if (sUnk6.compare(sVal) != 0) sUnk6 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk7);
    if (sUnk7.isEmpty()) sUnk7 = sVal;
    else if (sUnk7.compare(sVal) != 0) sUnk7 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk8);
    if (sUnk8.isEmpty()) sUnk8 = sVal;
    else if (sUnk8.compare(sVal) != 0) sUnk8 = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iLeftSurfaceType);
    if (sLeftSurfaceType.isEmpty()) sLeftSurfaceType = sVal;
    else if (sLeftSurfaceType.compare(sVal) != 0) sLeftSurfaceType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iCenterSurfaceType);
    if (sCenterSurfaceType.isEmpty()) sCenterSurfaceType = sVal;
    else if (sCenterSurfaceType.compare(sVal) != 0) sCenterSurfaceType = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRightSurfaceType);
    if (sRightSurfaceType.isEmpty()) sRightSurfaceType = sVal;
    else if (sRightSurfaceType.compare(sVal) != 0) sRightSurfaceType = MIXED_DATA;

    sVal = QString::number(m_chunkAy[i].iUnk9);
    if (sUnk9.isEmpty()) sUnk9 = sVal;
    else if (sUnk9.compare(sVal) != 0) sUnk9 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk10);
    if (sUnk10.isEmpty()) sUnk10 = sVal;
    else if (sUnk10.compare(sVal) != 0) sUnk10 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk11);
    if (sUnk11.isEmpty()) sUnk11 = sVal;
    else if (sUnk11.compare(sVal) != 0) sUnk11 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk12);
    if (sUnk12.isEmpty()) sUnk12 = sVal;
    else if (sUnk12.compare(sVal) != 0) sUnk12 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk13);
    if (sUnk13.isEmpty()) sUnk13 = sVal;
    else if (sUnk13.compare(sVal) != 0) sUnk13 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk14);
    if (sUnk14.isEmpty()) sUnk14 = sVal;
    else if (sUnk14.compare(sVal) != 0) sUnk14 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk15);
    if (sUnk15.isEmpty()) sUnk15 = sVal;
    else if (sUnk15.compare(sVal) != 0) sUnk15 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk16);
    if (sUnk16.isEmpty()) sUnk16 = sVal;
    else if (sUnk16.compare(sVal) != 0) sUnk16 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk17);
    if (sUnk17.isEmpty()) sUnk17 = sVal;
    else if (sUnk17.compare(sVal) != 0) sUnk17 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk18);
    if (sUnk18.isEmpty()) sUnk18 = sVal;
    else if (sUnk18.compare(sVal) != 0) sUnk18 = MIXED_DATA;
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
 
    sVal = QString::number(m_chunkAy[i].iUnk21);
    if (sUnk21.isEmpty()) sUnk21 = sVal;
    else if (sUnk21.compare(sVal) != 0) sUnk21 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk22);
    if (sUnk22.isEmpty()) sUnk22 = sVal;
    else if (sUnk22.compare(sVal) != 0) sUnk22 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk23);
    if (sUnk23.isEmpty()) sUnk23 = sVal;
    else if (sUnk23.compare(sVal) != 0) sUnk23 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk24);
    if (sUnk24.isEmpty()) sUnk24 = sVal;
    else if (sUnk24.compare(sVal) != 0) sUnk24 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk25);
    if (sUnk25.isEmpty()) sUnk25 = sVal;
    else if (sUnk25.compare(sVal) != 0) sUnk25 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk26);
    if (sUnk26.isEmpty()) sUnk26 = sVal;
    else if (sUnk26.compare(sVal) != 0) sUnk26 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk27);
    if (sUnk27.isEmpty()) sUnk27 = sVal;
    else if (sUnk27.compare(sVal) != 0) sUnk27 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk28);
    if (sUnk28.isEmpty()) sUnk28 = sVal;
    else if (sUnk28.compare(sVal) != 0) sUnk28 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk29);
    if (sUnk29.isEmpty()) sUnk29 = sVal;
    else if (sUnk29.compare(sVal) != 0) sUnk29 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk30);
    if (sUnk30.isEmpty()) sUnk30 = sVal;
    else if (sUnk30.compare(sVal) != 0) sUnk30 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk31);
    if (sUnk31.isEmpty()) sUnk31 = sVal;
    else if (sUnk31.compare(sVal) != 0) sUnk31 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk32);
    if (sUnk32.isEmpty()) sUnk32 = sVal;
    else if (sUnk32.compare(sVal) != 0) sUnk32 = MIXED_DATA;
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
    if (!sLeftShoulderWidth.isEmpty()) m_chunkAy[i].iLeftShoulderWidth = sLeftShoulderWidth.toInt();
    if (!sLeftLaneWidth.isEmpty()) m_chunkAy[i].iLeftLaneWidth = sLeftLaneWidth.toInt();
    if (!sRightLaneWidth.isEmpty()) m_chunkAy[i].iRightLaneWidth = sRightLaneWidth.toInt();
    if (!sRightShoulderWidth.isEmpty()) m_chunkAy[i].iRightShoulderWidth = sRightShoulderWidth.toInt();
    if (!sUnk1.isEmpty()) m_chunkAy[i].iUnk1 = sUnk1.toInt();
    if (!sUnk2.isEmpty()) m_chunkAy[i].iUnk2 = sUnk2.toInt();
    if (!sUnk3.isEmpty()) m_chunkAy[i].iUnk3 = sUnk3.toInt();
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
    if (!sUnk4.isEmpty()) m_chunkAy[i].iUnk4 = sUnk4.toInt();
    if (!sUnk5.isEmpty()) m_chunkAy[i].iUnk5 = sUnk5.toInt();
    if (!sUnk6.isEmpty()) m_chunkAy[i].iUnk6 = sUnk6.toInt();
    if (!sUnk7.isEmpty()) m_chunkAy[i].iUnk7 = sUnk7.toInt();
    if (!sUnk8.isEmpty()) m_chunkAy[i].iUnk8 = sUnk8.toInt();
    if (!sLeftSurfaceType.isEmpty()) m_chunkAy[i].iLeftSurfaceType = sLeftSurfaceType.toInt();
    if (!sCenterSurfaceType.isEmpty()) m_chunkAy[i].iCenterSurfaceType = sCenterSurfaceType.toInt();
    if (!sRightSurfaceType.isEmpty()) m_chunkAy[i].iRightSurfaceType = sRightSurfaceType.toInt();
    if (!sUnk9.isEmpty()) m_chunkAy[i].iUnk9 = sUnk9.toInt();
    if (!sUnk10.isEmpty()) m_chunkAy[i].iUnk10 = sUnk10.toInt();
    if (!sUnk11.isEmpty()) m_chunkAy[i].iUnk11 = sUnk11.toInt();
    if (!sUnk12.isEmpty()) m_chunkAy[i].iUnk12 = sUnk12.toInt();
    if (!sUnk13.isEmpty()) m_chunkAy[i].iUnk13 = sUnk13.toInt();
    if (!sUnk14.isEmpty()) m_chunkAy[i].iUnk14 = sUnk14.toInt();
    if (!sUnk15.isEmpty()) m_chunkAy[i].iUnk15 = sUnk15.toInt();
    if (!sUnk16.isEmpty()) m_chunkAy[i].iUnk16 = sUnk16.toInt();
    if (!sUnk17.isEmpty()) m_chunkAy[i].iUnk17 = sUnk17.toInt();
    if (!sUnk18.isEmpty()) m_chunkAy[i].iUnk18 = sUnk18.toInt();
    if (!sUnk19.isEmpty()) m_chunkAy[i].iUnk19 = sUnk19.toInt();
    if (!sUnk20.isEmpty()) m_chunkAy[i].iUnk20 = sUnk20.toInt();
    if (!sfUnk1.isEmpty()) m_chunkAy[i].fUnk1 = sfUnk1.toFloat();
    if (!sfUnk2.isEmpty()) m_chunkAy[i].fUnk2 = sfUnk2.toFloat();
    if (!sfUnk3.isEmpty()) m_chunkAy[i].fUnk3 = sfUnk3.toFloat();
    if (!sUnk21.isEmpty()) m_chunkAy[i].iUnk21 = sUnk21.toInt();
    if (!sUnk22.isEmpty()) m_chunkAy[i].iUnk22 = sUnk22.toInt();
    if (!sUnk23.isEmpty()) m_chunkAy[i].iUnk23 = sUnk23.toInt();
    if (!sUnk24.isEmpty()) m_chunkAy[i].iUnk24 = sUnk24.toInt();
    if (!sUnk25.isEmpty()) m_chunkAy[i].iUnk25 = sUnk25.toInt();
    if (!sUnk26.isEmpty()) m_chunkAy[i].iUnk26 = sUnk26.toInt();
    if (!sUnk27.isEmpty()) m_chunkAy[i].iUnk27 = sUnk27.toInt();
    if (!sUnk28.isEmpty()) m_chunkAy[i].iUnk28 = sUnk28.toInt();
    if (!sUnk29.isEmpty()) m_chunkAy[i].iUnk29 = sUnk29.toInt();
    if (!sUnk30.isEmpty()) m_chunkAy[i].iUnk30 = sUnk30.toInt();
    if (!sUnk31.isEmpty()) m_chunkAy[i].iUnk31 = sUnk31.toInt();
    if (!sUnk32.isEmpty()) m_chunkAy[i].iUnk32 = sUnk32.toInt();
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
    newChunk.iLeftShoulderWidth = sLeftShoulderWidth.toInt();
    newChunk.iLeftLaneWidth = sLeftLaneWidth.toInt();
    newChunk.iRightLaneWidth = sRightLaneWidth.toInt();
    newChunk.iRightShoulderWidth = sRightShoulderWidth.toInt();
    newChunk.iUnk1 = sUnk1.toInt();
    newChunk.iUnk2 = sUnk2.toInt();
    newChunk.iUnk3 = sUnk3.toInt();
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
    newChunk.iUnk4 = sUnk4.toInt();
    newChunk.iUnk5 = sUnk5.toInt();
    newChunk.iUnk6 = sUnk6.toInt();
    newChunk.iUnk7 = sUnk7.toInt();
    newChunk.iUnk8 = sUnk8.toInt();
    newChunk.iLeftSurfaceType = sLeftSurfaceType.toInt();
    newChunk.iCenterSurfaceType = sCenterSurfaceType.toInt();
    newChunk.iRightSurfaceType = sRightSurfaceType.toInt();
    newChunk.iUnk9 = sUnk9.toInt();
    newChunk.iUnk10 = sUnk10.toInt();
    newChunk.iUnk11 = sUnk11.toInt();
    newChunk.iUnk12 = sUnk12.toInt();
    newChunk.iUnk13 = sUnk13.toInt();
    newChunk.iUnk14 = sUnk14.toInt();
    newChunk.iUnk15 = sUnk15.toInt();
    newChunk.iUnk16 = sUnk16.toInt();
    newChunk.iUnk17 = sUnk17.toInt();
    newChunk.iUnk18 = sUnk18.toInt();
    newChunk.iUnk19 = sUnk19.toInt();
    newChunk.iUnk20 = sUnk20.toInt();
    newChunk.fUnk1 = sfUnk1.toFloat();
    newChunk.fUnk2 = sfUnk2.toFloat();
    newChunk.fUnk3 = sfUnk3.toFloat();
    newChunk.iUnk21 = sUnk21.toInt();
    newChunk.iUnk22 = sUnk22.toInt();
    newChunk.iUnk23 = sUnk23.toInt();
    newChunk.iUnk24 = sUnk24.toInt();
    newChunk.iUnk25 = sUnk25.toInt();
    newChunk.iUnk26 = sUnk26.toInt();
    newChunk.iUnk27 = sUnk27.toInt();
    newChunk.iUnk28 = sUnk28.toInt();
    newChunk.iUnk29 = sUnk29.toInt();
    newChunk.iUnk30 = sUnk30.toInt();
    newChunk.iUnk31 = sUnk31.toInt();
    newChunk.iUnk32 = sUnk32.toInt();
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
           " %4d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d"                             //line 3
           " %3d %3d %3d %d %d %d %d %d %d %d %d %d %d %d %3d %3d %3d\n"       //line 3 continued
           , chunk.iLeftShoulderWidth
           , chunk.iLeftLaneWidth
           , chunk.iRightLaneWidth
           , chunk.iRightShoulderWidth
           , chunk.iUnk1
           , chunk.iUnk2
           , chunk.iUnk3
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
           , chunk.iUnk4
           , chunk.iUnk5
           , chunk.iUnk6
           , chunk.iUnk7
           , chunk.iUnk8
           , chunk.iLeftSurfaceType
           , chunk.iCenterSurfaceType
           , chunk.iRightSurfaceType
           , chunk.iUnk9
           , chunk.iUnk10
           , chunk.iUnk11
           , chunk.iUnk12
           , chunk.iUnk13
           , chunk.iUnk14
           , chunk.iUnk15
           , chunk.iUnk16
           , chunk.iUnk17
           , chunk.iUnk18
           , chunk.iUnk19
           , chunk.iUnk20
           , chunk.fUnk1
           , chunk.fUnk2
           , chunk.fUnk3
           , chunk.iUnk21
           , chunk.iUnk22
           , chunk.iUnk23
           , chunk.iUnk24
           , chunk.iUnk25
           , chunk.iUnk26
           , chunk.iUnk27
           , chunk.iUnk28
           , chunk.iUnk29
           , chunk.iUnk30
           , chunk.iUnk31
           , chunk.iUnk32
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