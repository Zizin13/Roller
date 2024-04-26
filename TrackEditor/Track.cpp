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

CTrack::CTrack()
{
  ClearData();
}

//-------------------------------------------------------------------------------------------------

CTrack::~CTrack()
{

}

//-------------------------------------------------------------------------------------------------

bool CTrack::LoadTrack(const QString &sFilename, bool bIsMangled)
{
  bool bSuccess = CTrackData::LoadTrack(sFilename.toLatin1().constData(), bIsMangled);

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
  char szBuf[1024];
  snprintf(szBuf, sizeof(szBuf), " %4d %6d %6d %6d\r\n", (int)m_chunkAy.size(), m_header.iHeaderUnk1, m_header.iHeaderUnk2, m_header.iFloorDepth);
  QString sHeader = szBuf;

  iStartCursorPos = sHeader.size();
  iEndCursorPos = sHeader.size();
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
  //return a default track piece
  if (m_chunkAy.empty()) {
    editVals.sLeftShoulderWidth = "3000";
    editVals.sLeftLaneWidth = "3000";
    editVals.sRightLaneWidth = "3000";
    editVals.sRightShoulderWidth = "3000";
    editVals.sLShoulderHeight = "100";
    editVals.sRShoulderHeight = "100";
    editVals.sLength = "2000";
    editVals.sYaw = "0.0";
    editVals.sPitch = "0.0";
    editVals.sRoll = "0.0";
    editVals.sAILine1 = "1000";
    editVals.sAILine2 = "2000";
    editVals.sAILine3 = "-1000";
    editVals.sAILine4 = "-2000";
    editVals.sAIMaxAccel = "1000";
    editVals.sLeftSurfaceType = "134545758";
    editVals.sCenterSurfaceType = "82326";
    editVals.sRightSurfaceType = "134545758";
    editVals.sLWallType = "525605";
    editVals.sRWallType = "525605";
    editVals.sRoofType = "-1";
    editVals.sLUOuterWallType = "1351";
    editVals.sLLOuterWallType = "134545758";
    editVals.sOuterFloorType = "-2";
    editVals.sRLOuterWallType = "134545758";
    editVals.sRUOuterWallType = "263495";
    editVals.sEnvironmentFloorType = "267";
    editVals.sLUOuterWallHOffset = "2000";
    editVals.sLLOuterWallHOffset = "6000";
    editVals.sLOuterFloorHOffset = "1000";
    editVals.sROuterFloorHOffset = "1000";
    editVals.sRLOuterWallHOffset = "6000";
    editVals.sRUOuterWallHOffset = "2000";
    editVals.sLUOuterWallHeight = "4000";
    editVals.sLLOuterWallHeight = "1000";
    editVals.sLOuterFloorHeight = "1792";
    editVals.sROuterFloorHeight = "1792";
    editVals.sRLOuterWallHeight = "1000";
    editVals.sRUOuterWallHeight = "4000";
    editVals.sRoofHeight = "1000";
    editVals.sDrawOrder1 = "39";
    editVals.sDrawOrder2 = "-1";
    editVals.sUnk37 = "20";
    editVals.sUnk38 = "20";
    editVals.sUnk39 = "20";
    editVals.sUnk40 = "20";
    editVals.sUnk41 = "20";
    editVals.sUnk42 = "20";
    editVals.sUnk43 = "1";
    editVals.sUnk44 = "20";
    editVals.sUnk45 = "1";
    editVals.sUnk46 = "20";
    editVals.sUnk47 = "1";
    editVals.sUnk48 = "64";
    editVals.sUnk49 = "-1";
    return;
  }

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

    sVal = QString::number(m_chunkAy[i].iAIMaxAccel);
    if (editVals.sAIMaxAccel.isEmpty()) editVals.sAIMaxAccel = sVal;
    else if (editVals.sAIMaxAccel.compare(sVal) != 0) editVals.sAIMaxAccel = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iUnk05);
    if (editVals.sUnk05.isEmpty()) editVals.sUnk05 = sVal;
    else if (editVals.sUnk05.compare(sVal) != 0) editVals.sUnk05 = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iAudioAboveTrigger);
    if (editVals.sAudioAboveTrigger.isEmpty()) editVals.sAudioAboveTrigger = sVal;
    else if (editVals.sAudioAboveTrigger.compare(sVal) != 0) editVals.sAudioAboveTrigger = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iAudioTriggerSpeed);
    if (editVals.sAudioTriggerSpeed.isEmpty()) editVals.sAudioTriggerSpeed = sVal;
    else if (editVals.sAudioTriggerSpeed.compare(sVal) != 0) editVals.sAudioTriggerSpeed = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iAudioBelowTrigger);
    if (editVals.sAudioBelowTrigger.isEmpty()) editVals.sAudioBelowTrigger = sVal;
    else if (editVals.sAudioBelowTrigger.compare(sVal) != 0) editVals.sAudioBelowTrigger = MIXED_DATA;

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
 
    sVal = QString::number(m_chunkAy[i].iLUOuterWallHOffset);
    if (editVals.sLUOuterWallHOffset.isEmpty()) editVals.sLUOuterWallHOffset = sVal;
    else if (editVals.sLUOuterWallHOffset.compare(sVal) != 0) editVals.sLUOuterWallHOffset = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLLOuterWallHOffset);
    if (editVals.sLLOuterWallHOffset.isEmpty()) editVals.sLLOuterWallHOffset = sVal;
    else if (editVals.sLLOuterWallHOffset.compare(sVal) != 0) editVals.sLLOuterWallHOffset = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLOuterFloorHOffset);
    if (editVals.sLOuterFloorHOffset.isEmpty()) editVals.sLOuterFloorHOffset = sVal;
    else if (editVals.sLOuterFloorHOffset.compare(sVal) != 0) editVals.sLOuterFloorHOffset = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iROuterFloorHOffset);
    if (editVals.sROuterFloorHOffset.isEmpty()) editVals.sROuterFloorHOffset = sVal;
    else if (editVals.sROuterFloorHOffset.compare(sVal) != 0) editVals.sROuterFloorHOffset = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRLOuterWallHOffset);
    if (editVals.sRLOuterWallHOffset.isEmpty()) editVals.sRLOuterWallHOffset = sVal;
    else if (editVals.sRLOuterWallHOffset.compare(sVal) != 0) editVals.sRLOuterWallHOffset = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iRUOuterWallHOffset);
    if (editVals.sRUOuterWallHOffset.isEmpty()) editVals.sRUOuterWallHOffset = sVal;
    else if (editVals.sRUOuterWallHOffset.compare(sVal) != 0) editVals.sRUOuterWallHOffset = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLUOuterWallHeight);
    if (editVals.sLUOuterWallHeight.isEmpty()) editVals.sLUOuterWallHeight = sVal;
    else if (editVals.sLUOuterWallHeight.compare(sVal) != 0) editVals.sLUOuterWallHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLLOuterWallHeight);
    if (editVals.sLLOuterWallHeight.isEmpty()) editVals.sLLOuterWallHeight = sVal;
    else if (editVals.sLLOuterWallHeight.compare(sVal) != 0) editVals.sLLOuterWallHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iLOuterFloorHeight);
    if (editVals.sLOuterFloorHeight.isEmpty()) editVals.sLOuterFloorHeight = sVal;
    else if (editVals.sLOuterFloorHeight.compare(sVal) != 0) editVals.sLOuterFloorHeight = MIXED_DATA;
    sVal = QString::number(m_chunkAy[i].iROuterFloorHeight);
    if (editVals.sROuterFloorHeight.isEmpty()) editVals.sROuterFloorHeight = sVal;
    else if (editVals.sROuterFloorHeight.compare(sVal) != 0) editVals.sROuterFloorHeight = MIXED_DATA;
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
    if (!editVals.sAIMaxAccel.isEmpty()) m_chunkAy[i].iAIMaxAccel = editVals.sAIMaxAccel.toInt();
    if (!editVals.sUnk05.isEmpty()) m_chunkAy[i].iUnk05 = editVals.sUnk05.toInt();
    if (!editVals.sAudioAboveTrigger.isEmpty()) m_chunkAy[i].iAudioAboveTrigger = editVals.sAudioAboveTrigger.toInt();
    if (!editVals.sAudioTriggerSpeed.isEmpty()) m_chunkAy[i].iAudioTriggerSpeed = editVals.sAudioTriggerSpeed.toInt();
    if (!editVals.sAudioBelowTrigger.isEmpty()) m_chunkAy[i].iAudioBelowTrigger = editVals.sAudioBelowTrigger.toInt();
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
    if (!editVals.sLUOuterWallHOffset.isEmpty()) m_chunkAy[i].iLUOuterWallHOffset = editVals.sLUOuterWallHOffset.toInt();
    if (!editVals.sLLOuterWallHOffset.isEmpty()) m_chunkAy[i].iLLOuterWallHOffset = editVals.sLLOuterWallHOffset.toInt();
    if (!editVals.sLOuterFloorHOffset.isEmpty()) m_chunkAy[i].iLOuterFloorHOffset = editVals.sLOuterFloorHOffset.toInt();
    if (!editVals.sROuterFloorHOffset.isEmpty()) m_chunkAy[i].iROuterFloorHOffset = editVals.sROuterFloorHOffset.toInt();
    if (!editVals.sRLOuterWallHOffset.isEmpty()) m_chunkAy[i].iRLOuterWallHOffset = editVals.sRLOuterWallHOffset.toInt();
    if (!editVals.sRUOuterWallHOffset.isEmpty()) m_chunkAy[i].iRUOuterWallHOffset = editVals.sRUOuterWallHOffset.toInt();
    if (!editVals.sLUOuterWallHeight.isEmpty()) m_chunkAy[i].iLUOuterWallHeight = editVals.sLUOuterWallHeight.toInt();
    if (!editVals.sLLOuterWallHeight.isEmpty()) m_chunkAy[i].iLLOuterWallHeight = editVals.sLLOuterWallHeight.toInt();
    if (!editVals.sLOuterFloorHeight.isEmpty()) m_chunkAy[i].iLOuterFloorHeight = editVals.sLOuterFloorHeight.toInt();
    if (!editVals.sROuterFloorHeight.isEmpty()) m_chunkAy[i].iROuterFloorHeight = editVals.sROuterFloorHeight.toInt();
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
  GenerateTrackMath();
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
    newChunk.iAIMaxAccel = editVals.sAIMaxAccel.toInt();
    newChunk.iUnk05 = editVals.sUnk05.toInt();
    newChunk.iAudioAboveTrigger = editVals.sAudioAboveTrigger.toInt();
    newChunk.iAudioTriggerSpeed = editVals.sAudioTriggerSpeed.toInt();
    newChunk.iAudioBelowTrigger = editVals.sAudioBelowTrigger.toInt();
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
    newChunk.iLUOuterWallHOffset = editVals.sLUOuterWallHOffset.toInt();
    newChunk.iLLOuterWallHOffset = editVals.sLLOuterWallHOffset.toInt();
    newChunk.iLOuterFloorHOffset = editVals.sLOuterFloorHOffset.toInt();
    newChunk.iROuterFloorHOffset = editVals.sROuterFloorHOffset.toInt();
    newChunk.iRLOuterWallHOffset = editVals.sRLOuterWallHOffset.toInt();
    newChunk.iRUOuterWallHOffset = editVals.sRUOuterWallHOffset.toInt();
    newChunk.iLUOuterWallHeight = editVals.sLUOuterWallHeight.toInt();
    newChunk.iLLOuterWallHeight = editVals.sLLOuterWallHeight.toInt();
    newChunk.iLOuterFloorHeight = editVals.sLOuterFloorHeight.toInt();
    newChunk.iROuterFloorHeight = editVals.sROuterFloorHeight.toInt();
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
  GenerateTrackMath();
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
           , chunk.iAIMaxAccel
           , chunk.iUnk05
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
  chunk.sString = szGenerate;
}

//-------------------------------------------------------------------------------------------------