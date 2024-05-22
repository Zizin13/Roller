#include "TrackEditor.h"
#include "Track.h"
#include "MainWindow.h"
#include "qfile.h"
#include "qtextstream.h"
#include "Unmangler.h"
#include "ChunkEditValues.h"
#include "TrackPreview.h"
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

bool CTrack::SaveTrack(const QString &sFilename)
{
  if (sFilename.isEmpty())
    return false;

  std::vector<uint8> data;
  std::vector<uint8> mangledData;
  GetTrackData(data);

  std::vector<uint8> *pOutData;
  //if (bIsMangled) {
  //  MangleFile(data, mangledData);
  //  pOutData = &mangledData;
  //} else {
    pOutData = &data;
  //}

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

void CTrack::GetGeometryValuesFromSelection(int iStartIndex, int iEndIndex, CChunkEditValues &editVals)
{
  //return a default track piece
  if (GetChunkCount() == 0) {
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
    editVals.sAIMaxSpeed = "1000";
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
    editVals.sSignType = "-1";
    return;
  }

  if (iEndIndex < iStartIndex || iEndIndex >= GetChunkCount()) {
    assert(0);
    return;
  }

  editVals.Clear();

  tGeometryChunk chunk;
  QString sVal;
  for (int i = iStartIndex; i <= iEndIndex; ++i) {
    GetChunk(i, chunk);
    sVal = QString::number(chunk.iLeftShoulderWidth);
    if (editVals.sLeftShoulderWidth.isEmpty()) editVals.sLeftShoulderWidth = sVal;
    else if (editVals.sLeftShoulderWidth.compare(sVal) != 0) editVals.sLeftShoulderWidth = MIXED_DATA;
    sVal = QString::number(chunk.iLeftLaneWidth);
    if (editVals.sLeftLaneWidth.isEmpty()) editVals.sLeftLaneWidth = sVal;
    else if (editVals.sLeftLaneWidth.compare(sVal) != 0) editVals.sLeftLaneWidth = MIXED_DATA;
    sVal = QString::number(chunk.iRightLaneWidth);
    if (editVals.sRightLaneWidth.isEmpty()) editVals.sRightLaneWidth = sVal;
    else if (editVals.sRightLaneWidth.compare(sVal) != 0) editVals.sRightLaneWidth = MIXED_DATA;
    sVal = QString::number(chunk.iRightShoulderWidth);
    if (editVals.sRightShoulderWidth.isEmpty()) editVals.sRightShoulderWidth = sVal;
    else if (editVals.sRightShoulderWidth.compare(sVal) != 0) editVals.sRightShoulderWidth = MIXED_DATA;

    sVal = QString::number(chunk.iLeftShoulderHeight);
    if (editVals.sLShoulderHeight.isEmpty()) editVals.sLShoulderHeight = sVal;
    else if (editVals.sLShoulderHeight.compare(sVal) != 0) editVals.sLShoulderHeight = MIXED_DATA;
    sVal = QString::number(chunk.iRightShoulderHeight);
    if (editVals.sRShoulderHeight.isEmpty()) editVals.sRShoulderHeight = sVal;
    else if (editVals.sRShoulderHeight.compare(sVal) != 0) editVals.sRShoulderHeight = MIXED_DATA;
    sVal = QString::number(chunk.iLength);
    if (editVals.sLength.isEmpty()) editVals.sLength = sVal;
    else if (editVals.sLength.compare(sVal) != 0) editVals.sLength = MIXED_DATA;

    sVal = QString::number(chunk.dYaw, 'f', 5);
    if (editVals.sYaw.isEmpty()) editVals.sYaw = sVal;
    else if (editVals.sYaw.compare(sVal) != 0) editVals.sYaw = MIXED_DATA;
    sVal = QString::number(chunk.dPitch, 'f', 5);
    if (editVals.sPitch.isEmpty()) editVals.sPitch = sVal;
    else if (editVals.sPitch.compare(sVal) != 0) editVals.sPitch = MIXED_DATA;
    sVal = QString::number(chunk.dRoll, 'f', 5);
    if (editVals.sRoll.isEmpty()) editVals.sRoll = sVal;
    else if (editVals.sRoll.compare(sVal) != 0) editVals.sRoll = MIXED_DATA;

    sVal = QString::number(chunk.iAILine1);
    if (editVals.sAILine1.isEmpty()) editVals.sAILine1 = sVal;
    else if (editVals.sAILine1.compare(sVal) != 0) editVals.sAILine1 = MIXED_DATA;
    sVal = QString::number(chunk.iAILine2);
    if (editVals.sAILine2.isEmpty()) editVals.sAILine2 = sVal;
    else if (editVals.sAILine2.compare(sVal) != 0) editVals.sAILine2 = MIXED_DATA;
    sVal = QString::number(chunk.iAILine3);
    if (editVals.sAILine3.isEmpty()) editVals.sAILine3 = sVal;
    else if (editVals.sAILine3.compare(sVal) != 0) editVals.sAILine3 = MIXED_DATA;
    sVal = QString::number(chunk.iAILine4);
    if (editVals.sAILine4.isEmpty()) editVals.sAILine4 = sVal;
    else if (editVals.sAILine4.compare(sVal) != 0) editVals.sAILine4 = MIXED_DATA;

    sVal = QString::number(chunk.iTrackGrip);
    if (editVals.sTrackGrip.isEmpty()) editVals.sTrackGrip = sVal;
    else if (editVals.sTrackGrip.compare(sVal) != 0) editVals.sTrackGrip = MIXED_DATA;
    sVal = QString::number(chunk.iLeftShoulderGrip);
    if (editVals.sLeftShoulderGrip.isEmpty()) editVals.sLeftShoulderGrip = sVal;
    else if (editVals.sLeftShoulderGrip.compare(sVal) != 0) editVals.sLeftShoulderGrip = MIXED_DATA;
    sVal = QString::number(chunk.iRightShoulderGrip);
    if (editVals.sRightShoulderGrip.isEmpty()) editVals.sRightShoulderGrip = sVal;
    else if (editVals.sRightShoulderGrip.compare(sVal) != 0) editVals.sRightShoulderGrip = MIXED_DATA;

    sVal = QString::number(chunk.iAIMaxSpeed);
    if (editVals.sAIMaxSpeed.isEmpty()) editVals.sAIMaxSpeed = sVal;
    else if (editVals.sAIMaxSpeed.compare(sVal) != 0) editVals.sAIMaxSpeed = MIXED_DATA;
    sVal = QString::number(chunk.iAIAccuracy);
    if (editVals.sAIAccuracy.isEmpty()) editVals.sAIAccuracy = sVal;
    else if (editVals.sAIAccuracy.compare(sVal) != 0) editVals.sAIAccuracy = MIXED_DATA;
    sVal = QString::number(chunk.iAudioAboveTrigger);
    if (editVals.sAudioAboveTrigger.isEmpty()) editVals.sAudioAboveTrigger = sVal;
    else if (editVals.sAudioAboveTrigger.compare(sVal) != 0) editVals.sAudioAboveTrigger = MIXED_DATA;
    sVal = QString::number(chunk.iAudioTriggerSpeed);
    if (editVals.sAudioTriggerSpeed.isEmpty()) editVals.sAudioTriggerSpeed = sVal;
    else if (editVals.sAudioTriggerSpeed.compare(sVal) != 0) editVals.sAudioTriggerSpeed = MIXED_DATA;
    sVal = QString::number(chunk.iAudioBelowTrigger);
    if (editVals.sAudioBelowTrigger.isEmpty()) editVals.sAudioBelowTrigger = sVal;
    else if (editVals.sAudioBelowTrigger.compare(sVal) != 0) editVals.sAudioBelowTrigger = MIXED_DATA;

    sVal = QString::number(chunk.iLeftSurfaceType);
    if (editVals.sLeftSurfaceType.isEmpty()) editVals.sLeftSurfaceType = sVal;
    else if (editVals.sLeftSurfaceType.compare(sVal) != 0) editVals.sLeftSurfaceType = MIXED_DATA;
    sVal = QString::number(chunk.iCenterSurfaceType);
    if (editVals.sCenterSurfaceType.isEmpty()) editVals.sCenterSurfaceType = sVal;
    else if (editVals.sCenterSurfaceType.compare(sVal) != 0) editVals.sCenterSurfaceType = MIXED_DATA;
    sVal = QString::number(chunk.iRightSurfaceType);
    if (editVals.sRightSurfaceType.isEmpty()) editVals.sRightSurfaceType = sVal;
    else if (editVals.sRightSurfaceType.compare(sVal) != 0) editVals.sRightSurfaceType = MIXED_DATA;

    sVal = QString::number(chunk.iLeftWallType);
    if (editVals.sLWallType.isEmpty()) editVals.sLWallType = sVal;
    else if (editVals.sLWallType.compare(sVal) != 0) editVals.sLWallType = MIXED_DATA;
    sVal = QString::number(chunk.iRightWallType);
    if (editVals.sRWallType.isEmpty()) editVals.sRWallType = sVal;
    else if (editVals.sRWallType.compare(sVal) != 0) editVals.sRWallType = MIXED_DATA;
    sVal = QString::number(chunk.iRoofType);
    if (editVals.sRoofType.isEmpty()) editVals.sRoofType = sVal;
    else if (editVals.sRoofType.compare(sVal) != 0) editVals.sRoofType = MIXED_DATA;
    sVal = QString::number(chunk.iLUOuterWallType);
    if (editVals.sLUOuterWallType.isEmpty()) editVals.sLUOuterWallType = sVal;
    else if (editVals.sLUOuterWallType.compare(sVal) != 0) editVals.sLUOuterWallType = MIXED_DATA;
    sVal = QString::number(chunk.iLLOuterWallType);
    if (editVals.sLLOuterWallType.isEmpty()) editVals.sLLOuterWallType = sVal;
    else if (editVals.sLLOuterWallType.compare(sVal) != 0) editVals.sLLOuterWallType = MIXED_DATA;
    sVal = QString::number(chunk.iOuterFloorType);
    if (editVals.sOuterFloorType.isEmpty()) editVals.sOuterFloorType = sVal;
    else if (editVals.sOuterFloorType.compare(sVal) != 0) editVals.sOuterFloorType = MIXED_DATA;
    sVal = QString::number(chunk.iRLOuterWallType);
    if (editVals.sRLOuterWallType.isEmpty())editVals.sRLOuterWallType = sVal;
    else if (editVals.sRLOuterWallType.compare(sVal) != 0) editVals.sRLOuterWallType = MIXED_DATA;
    sVal = QString::number(chunk.iRUOuterWallType);
    if (editVals.sRUOuterWallType.isEmpty()) editVals.sRUOuterWallType = sVal;
    else if (editVals.sRUOuterWallType.compare(sVal) != 0) editVals.sRUOuterWallType = MIXED_DATA;
    sVal = QString::number(chunk.iEnvironmentFloorType);
    if (editVals.sEnvironmentFloorType.isEmpty()) editVals.sEnvironmentFloorType = sVal;
    else if (editVals.sEnvironmentFloorType.compare(sVal) != 0) editVals.sEnvironmentFloorType = MIXED_DATA;
    sVal = QString::number(chunk.iSignType);
    if (editVals.sSignType.isEmpty()) editVals.sSignType = sVal;
    else if (editVals.sSignType.compare(sVal) != 0) editVals.sSignType = MIXED_DATA;
    sVal = QString::number(chunk.iSignHorizOffset);
    if (editVals.sSignHorizOffset.isEmpty()) editVals.sSignHorizOffset = sVal;
    else if (editVals.sSignHorizOffset.compare(sVal) != 0) editVals.sSignHorizOffset = MIXED_DATA;
    sVal = QString::number(chunk.iSignVertOffset);
    if (editVals.sSignVertOffset.isEmpty()) editVals.sSignVertOffset = sVal;
    else if (editVals.sSignVertOffset.compare(sVal) != 0) editVals.sSignVertOffset = MIXED_DATA;

    sVal = QString::number(chunk.dSignYaw, 'f', 1);
    if (editVals.sSignYaw.isEmpty()) editVals.sSignYaw = sVal;
    else if (editVals.sSignYaw.compare(sVal) != 0) editVals.sSignYaw = MIXED_DATA;
    sVal = QString::number(chunk.dSignPitch, 'f', 1);
    if (editVals.sSignPitch.isEmpty()) editVals.sSignPitch = sVal;
    else if (editVals.sSignPitch.compare(sVal) != 0) editVals.sSignPitch = MIXED_DATA;
    sVal = QString::number(chunk.dSignRoll, 'f', 1);
    if (editVals.sSignRoll.isEmpty()) editVals.sSignRoll = sVal;
    else if (editVals.sSignRoll.compare(sVal) != 0) editVals.sSignRoll = MIXED_DATA;
 
    sVal = QString::number(chunk.iLUOuterWallHOffset);
    if (editVals.sLUOuterWallHOffset.isEmpty()) editVals.sLUOuterWallHOffset = sVal;
    else if (editVals.sLUOuterWallHOffset.compare(sVal) != 0) editVals.sLUOuterWallHOffset = MIXED_DATA;
    sVal = QString::number(chunk.iLLOuterWallHOffset);
    if (editVals.sLLOuterWallHOffset.isEmpty()) editVals.sLLOuterWallHOffset = sVal;
    else if (editVals.sLLOuterWallHOffset.compare(sVal) != 0) editVals.sLLOuterWallHOffset = MIXED_DATA;
    sVal = QString::number(chunk.iLOuterFloorHOffset);
    if (editVals.sLOuterFloorHOffset.isEmpty()) editVals.sLOuterFloorHOffset = sVal;
    else if (editVals.sLOuterFloorHOffset.compare(sVal) != 0) editVals.sLOuterFloorHOffset = MIXED_DATA;
    sVal = QString::number(chunk.iROuterFloorHOffset);
    if (editVals.sROuterFloorHOffset.isEmpty()) editVals.sROuterFloorHOffset = sVal;
    else if (editVals.sROuterFloorHOffset.compare(sVal) != 0) editVals.sROuterFloorHOffset = MIXED_DATA;
    sVal = QString::number(chunk.iRLOuterWallHOffset);
    if (editVals.sRLOuterWallHOffset.isEmpty()) editVals.sRLOuterWallHOffset = sVal;
    else if (editVals.sRLOuterWallHOffset.compare(sVal) != 0) editVals.sRLOuterWallHOffset = MIXED_DATA;
    sVal = QString::number(chunk.iRUOuterWallHOffset);
    if (editVals.sRUOuterWallHOffset.isEmpty()) editVals.sRUOuterWallHOffset = sVal;
    else if (editVals.sRUOuterWallHOffset.compare(sVal) != 0) editVals.sRUOuterWallHOffset = MIXED_DATA;
    sVal = QString::number(chunk.iLUOuterWallHeight);
    if (editVals.sLUOuterWallHeight.isEmpty()) editVals.sLUOuterWallHeight = sVal;
    else if (editVals.sLUOuterWallHeight.compare(sVal) != 0) editVals.sLUOuterWallHeight = MIXED_DATA;
    sVal = QString::number(chunk.iLLOuterWallHeight);
    if (editVals.sLLOuterWallHeight.isEmpty()) editVals.sLLOuterWallHeight = sVal;
    else if (editVals.sLLOuterWallHeight.compare(sVal) != 0) editVals.sLLOuterWallHeight = MIXED_DATA;
    sVal = QString::number(chunk.iLOuterFloorHeight);
    if (editVals.sLOuterFloorHeight.isEmpty()) editVals.sLOuterFloorHeight = sVal;
    else if (editVals.sLOuterFloorHeight.compare(sVal) != 0) editVals.sLOuterFloorHeight = MIXED_DATA;
    sVal = QString::number(chunk.iROuterFloorHeight);
    if (editVals.sROuterFloorHeight.isEmpty()) editVals.sROuterFloorHeight = sVal;
    else if (editVals.sROuterFloorHeight.compare(sVal) != 0) editVals.sROuterFloorHeight = MIXED_DATA;
    sVal = QString::number(chunk.iRLOuterWallHeight);
    if (editVals.sRLOuterWallHeight.isEmpty()) editVals.sRLOuterWallHeight = sVal;
    else if (editVals.sRLOuterWallHeight.compare(sVal) != 0) editVals.sRLOuterWallHeight = MIXED_DATA;
    sVal = QString::number(chunk.iRUOuterWallHeight);
    if (editVals.sRUOuterWallHeight.isEmpty()) editVals.sRUOuterWallHeight = sVal;
    else if (editVals.sRUOuterWallHeight.compare(sVal) != 0) editVals.sRUOuterWallHeight = MIXED_DATA;
    sVal = QString::number(chunk.iRoofHeight);
    if (editVals.sRoofHeight.isEmpty()) editVals.sRoofHeight = sVal;
    else if (editVals.sRoofHeight.compare(sVal) != 0) editVals.sRoofHeight = MIXED_DATA;
    sVal = QString::number(chunk.iDrawOrder1);
    if (editVals.sDrawOrder1.isEmpty()) editVals.sDrawOrder1 = sVal;
    else if (editVals.sDrawOrder1.compare(sVal) != 0) editVals.sDrawOrder1 = MIXED_DATA;
    sVal = QString::number(chunk.iDrawOrder2);
    if (editVals.sDrawOrder2.isEmpty()) editVals.sDrawOrder2 = sVal;
    else if (editVals.sDrawOrder2.compare(sVal) != 0) editVals.sDrawOrder2 = MIXED_DATA;
    sVal = QString::number(chunk.iDrawOrder3);
    if (editVals.sDrawOrder3.isEmpty()) editVals.sDrawOrder3 = sVal;
    else if (editVals.sDrawOrder3.compare(sVal) != 0) editVals.sDrawOrder3 = MIXED_DATA;
    sVal = QString::number(chunk.iUnk37);
    if (editVals.sUnk37.isEmpty()) editVals.sUnk37 = sVal;
    else if (editVals.sUnk37.compare(sVal) != 0) editVals.sUnk37 = MIXED_DATA;
    sVal = QString::number(chunk.iUnk38);
    if (editVals.sUnk38.isEmpty()) editVals.sUnk38 = sVal;
    else if (editVals.sUnk38.compare(sVal) != 0) editVals.sUnk38 = MIXED_DATA;
    sVal = QString::number(chunk.iUnk39);
    if (editVals.sUnk39.isEmpty()) editVals.sUnk39 = sVal;
    else if (editVals.sUnk39.compare(sVal) != 0) editVals.sUnk39 = MIXED_DATA;
    sVal = QString::number(chunk.iUnk40);
    if (editVals.sUnk40.isEmpty()) editVals.sUnk40 = sVal;
    else if (editVals.sUnk40.compare(sVal) != 0) editVals.sUnk40 = MIXED_DATA;
    sVal = QString::number(chunk.iUnk41);
    if (editVals.sUnk41.isEmpty()) editVals.sUnk41 = sVal;
    else if (editVals.sUnk41.compare(sVal) != 0) editVals.sUnk41 = MIXED_DATA;
    sVal = QString::number(chunk.iUnk42);
    if (editVals.sUnk42.isEmpty()) editVals.sUnk42 = sVal;
    else if (editVals.sUnk42.compare(sVal) != 0) editVals.sUnk42 = MIXED_DATA;
    sVal = QString::number(chunk.iUnk43);
    if (editVals.sUnk43.isEmpty()) editVals.sUnk43 = sVal;
    else if (editVals.sUnk43.compare(sVal) != 0) editVals.sUnk43 = MIXED_DATA;
    sVal = QString::number(chunk.iUnk44);
    if (editVals.sUnk44.isEmpty()) editVals.sUnk44 = sVal;
    else if (editVals.sUnk44.compare(sVal) != 0) editVals.sUnk44 = MIXED_DATA;
    sVal = QString::number(chunk.iUnk45);
    if (editVals.sUnk45.isEmpty()) editVals.sUnk45 = sVal;
    else if (editVals.sUnk45.compare(sVal) != 0) editVals.sUnk45 = MIXED_DATA;
    sVal = QString::number(chunk.iUnk46);
    if (editVals.sUnk46.isEmpty()) editVals.sUnk46 = sVal;
    else if (editVals.sUnk46.compare(sVal) != 0) editVals.sUnk46 = MIXED_DATA;
    sVal = QString::number(chunk.iUnk47);
    if (editVals.sUnk47.isEmpty()) editVals.sUnk47 = sVal;
    else if (editVals.sUnk47.compare(sVal) != 0) editVals.sUnk47 = MIXED_DATA;
    sVal = QString::number(chunk.iUnk48);
    if (editVals.sUnk48.isEmpty()) editVals.sUnk48 = sVal;
    else if (editVals.sUnk48.compare(sVal) != 0) editVals.sUnk48 = MIXED_DATA;
    sVal = QString::number(chunk.iUnk49);
    if (editVals.sUnk49.isEmpty()) editVals.sUnk49 = sVal;
    else if (editVals.sUnk49.compare(sVal) != 0) editVals.sUnk49 = MIXED_DATA;
    sVal = QString::number(chunk.iUnk50);
    if (editVals.sUnk50.isEmpty()) editVals.sUnk50 = sVal;
    else if (editVals.sUnk50.compare(sVal) != 0) editVals.sUnk50 = MIXED_DATA;

    sVal = QString::number(chunk.iSignTexture);
    if (editVals.sSignTexture.isEmpty()) editVals.sSignTexture = sVal;
    else if (editVals.sSignTexture.compare(sVal) != 0) editVals.sSignTexture = MIXED_DATA;
    sVal = QString::number(chunk.iBackTexture);
    if (editVals.sBackTexture.isEmpty()) editVals.sBackTexture = sVal;
    else if (editVals.sBackTexture.compare(sVal) != 0) editVals.sBackTexture = MIXED_DATA;
  }
}

//-------------------------------------------------------------------------------------------------

void CTrack::ApplyGeometrySettings(int iStartIndex, int iEndIndex, const CChunkEditValues &editVals)
{
  tGeometryChunk chunk;
  for (int i = iStartIndex; i <= iEndIndex; ++i) {
    GetChunk(i, chunk);
    if (!editVals.sLeftShoulderWidth.isEmpty()) chunk.iLeftShoulderWidth = editVals.sLeftShoulderWidth.toInt();
    if (!editVals.sLeftLaneWidth.isEmpty()) chunk.iLeftLaneWidth = editVals.sLeftLaneWidth.toInt();
    if (!editVals.sRightLaneWidth.isEmpty()) chunk.iRightLaneWidth = editVals.sRightLaneWidth.toInt();
    if (!editVals.sRightShoulderWidth.isEmpty()) chunk.iRightShoulderWidth = editVals.sRightShoulderWidth.toInt();
    if (!editVals.sLShoulderHeight.isEmpty()) chunk.iLeftShoulderHeight = editVals.sLShoulderHeight.toInt();
    if (!editVals.sRShoulderHeight.isEmpty()) chunk.iRightShoulderHeight = editVals.sRShoulderHeight.toInt();
    if (!editVals.sLength.isEmpty()) chunk.iLength = editVals.sLength.toInt();
    if (!editVals.sYaw.isEmpty()) chunk.dYaw = editVals.sYaw.toDouble();
    if (!editVals.sPitch.isEmpty()) chunk.dPitch = editVals.sPitch.toDouble();
    if (!editVals.sRoll.isEmpty()) chunk.dRoll = editVals.sRoll.toDouble();
    if (!editVals.sAILine1.isEmpty()) chunk.iAILine1 = editVals.sAILine1.toInt();
    if (!editVals.sAILine2.isEmpty()) chunk.iAILine2 = editVals.sAILine2.toInt();
    if (!editVals.sAILine3.isEmpty()) chunk.iAILine3 = editVals.sAILine3.toInt();
    if (!editVals.sAILine4.isEmpty()) chunk.iAILine4 = editVals.sAILine4.toInt();
    if (!editVals.sTrackGrip.isEmpty()) chunk.iTrackGrip = editVals.sTrackGrip.toInt();
    if (!editVals.sLeftShoulderGrip.isEmpty()) chunk.iLeftShoulderGrip = editVals.sLeftShoulderGrip.toInt();
    if (!editVals.sRightShoulderGrip.isEmpty()) chunk.iRightShoulderGrip = editVals.sRightShoulderGrip.toInt();
    if (!editVals.sAIMaxSpeed.isEmpty()) chunk.iAIMaxSpeed = editVals.sAIMaxSpeed.toInt();
    if (!editVals.sAIAccuracy.isEmpty()) chunk.iAIAccuracy = editVals.sAIAccuracy.toInt();
    if (!editVals.sAudioAboveTrigger.isEmpty()) chunk.iAudioAboveTrigger = editVals.sAudioAboveTrigger.toInt();
    if (!editVals.sAudioTriggerSpeed.isEmpty()) chunk.iAudioTriggerSpeed = editVals.sAudioTriggerSpeed.toInt();
    if (!editVals.sAudioBelowTrigger.isEmpty()) chunk.iAudioBelowTrigger = editVals.sAudioBelowTrigger.toInt();
    if (!editVals.sLeftSurfaceType.isEmpty()) chunk.iLeftSurfaceType = editVals.sLeftSurfaceType.toInt();
    if (!editVals.sCenterSurfaceType.isEmpty()) chunk.iCenterSurfaceType = editVals.sCenterSurfaceType.toInt();
    if (!editVals.sRightSurfaceType.isEmpty()) chunk.iRightSurfaceType = editVals.sRightSurfaceType.toInt();
    if (!editVals.sLWallType.isEmpty()) chunk.iLeftWallType = editVals.sLWallType.toInt();
    if (!editVals.sRWallType.isEmpty()) chunk.iRightWallType = editVals.sRWallType.toInt();
    if (!editVals.sRoofType.isEmpty()) chunk.iRoofType = editVals.sRoofType.toInt();
    if (!editVals.sLUOuterWallType.isEmpty()) chunk.iLUOuterWallType = editVals.sLUOuterWallType.toInt();
    if (!editVals.sLLOuterWallType.isEmpty()) chunk.iLLOuterWallType = editVals.sLLOuterWallType.toInt();
    if (!editVals.sOuterFloorType.isEmpty()) chunk.iOuterFloorType = editVals.sOuterFloorType.toInt();
    if (!editVals.sRLOuterWallType.isEmpty()) chunk.iRLOuterWallType = editVals.sRLOuterWallType.toInt();
    if (!editVals.sRUOuterWallType.isEmpty()) chunk.iRUOuterWallType = editVals.sRUOuterWallType.toInt();
    if (!editVals.sEnvironmentFloorType.isEmpty()) chunk.iEnvironmentFloorType = editVals.sEnvironmentFloorType.toInt();
    if (!editVals.sSignType.isEmpty()) chunk.iSignType = editVals.sSignType.toInt();
    if (!editVals.sSignHorizOffset.isEmpty()) chunk.iSignHorizOffset = editVals.sSignHorizOffset.toInt();
    if (!editVals.sSignVertOffset.isEmpty()) chunk.iSignVertOffset = editVals.sSignVertOffset.toInt();
    if (!editVals.sSignYaw.isEmpty()) chunk.dSignYaw = editVals.sSignYaw.toDouble();
    if (!editVals.sSignPitch.isEmpty()) chunk.dSignPitch = editVals.sSignPitch.toDouble();
    if (!editVals.sSignRoll.isEmpty()) chunk.dSignRoll = editVals.sSignRoll.toDouble();
    if (!editVals.sLUOuterWallHOffset.isEmpty()) chunk.iLUOuterWallHOffset = editVals.sLUOuterWallHOffset.toInt();
    if (!editVals.sLLOuterWallHOffset.isEmpty()) chunk.iLLOuterWallHOffset = editVals.sLLOuterWallHOffset.toInt();
    if (!editVals.sLOuterFloorHOffset.isEmpty()) chunk.iLOuterFloorHOffset = editVals.sLOuterFloorHOffset.toInt();
    if (!editVals.sROuterFloorHOffset.isEmpty()) chunk.iROuterFloorHOffset = editVals.sROuterFloorHOffset.toInt();
    if (!editVals.sRLOuterWallHOffset.isEmpty()) chunk.iRLOuterWallHOffset = editVals.sRLOuterWallHOffset.toInt();
    if (!editVals.sRUOuterWallHOffset.isEmpty()) chunk.iRUOuterWallHOffset = editVals.sRUOuterWallHOffset.toInt();
    if (!editVals.sLUOuterWallHeight.isEmpty()) chunk.iLUOuterWallHeight = editVals.sLUOuterWallHeight.toInt();
    if (!editVals.sLLOuterWallHeight.isEmpty()) chunk.iLLOuterWallHeight = editVals.sLLOuterWallHeight.toInt();
    if (!editVals.sLOuterFloorHeight.isEmpty()) chunk.iLOuterFloorHeight = editVals.sLOuterFloorHeight.toInt();
    if (!editVals.sROuterFloorHeight.isEmpty()) chunk.iROuterFloorHeight = editVals.sROuterFloorHeight.toInt();
    if (!editVals.sRLOuterWallHeight.isEmpty()) chunk.iRLOuterWallHeight = editVals.sRLOuterWallHeight.toInt();
    if (!editVals.sRUOuterWallHeight.isEmpty()) chunk.iRUOuterWallHeight = editVals.sRUOuterWallHeight.toInt();
    if (!editVals.sRoofHeight.isEmpty()) chunk.iRoofHeight = editVals.sRoofHeight.toInt();
    if (!editVals.sDrawOrder1.isEmpty()) chunk.iDrawOrder1 = editVals.sDrawOrder1.toInt();
    if (!editVals.sDrawOrder2.isEmpty()) chunk.iDrawOrder2 = editVals.sDrawOrder2.toInt();
    if (!editVals.sDrawOrder3.isEmpty()) chunk.iDrawOrder3 = editVals.sDrawOrder3.toInt();
    if (!editVals.sUnk37.isEmpty()) chunk.iUnk37 = editVals.sUnk37.toInt();
    if (!editVals.sUnk38.isEmpty()) chunk.iUnk38 = editVals.sUnk38.toInt();
    if (!editVals.sUnk39.isEmpty()) chunk.iUnk39 = editVals.sUnk39.toInt();
    if (!editVals.sUnk40.isEmpty()) chunk.iUnk40 = editVals.sUnk40.toInt();
    if (!editVals.sUnk41.isEmpty()) chunk.iUnk41 = editVals.sUnk41.toInt();
    if (!editVals.sUnk42.isEmpty()) chunk.iUnk42 = editVals.sUnk42.toInt();
    if (!editVals.sUnk43.isEmpty()) chunk.iUnk43 = editVals.sUnk43.toInt();
    if (!editVals.sUnk44.isEmpty()) chunk.iUnk44 = editVals.sUnk44.toInt();
    if (!editVals.sUnk45.isEmpty()) chunk.iUnk45 = editVals.sUnk45.toInt();
    if (!editVals.sUnk46.isEmpty()) chunk.iUnk46 = editVals.sUnk46.toInt();
    if (!editVals.sUnk47.isEmpty()) chunk.iUnk47 = editVals.sUnk47.toInt();
    if (!editVals.sUnk48.isEmpty()) chunk.iUnk48 = editVals.sUnk48.toInt();
    if (!editVals.sUnk49.isEmpty()) chunk.iUnk49 = editVals.sUnk49.toInt();
    if (!editVals.sUnk50.isEmpty()) chunk.iUnk50 = editVals.sUnk50.toInt();
    if (!editVals.sSignTexture.isEmpty()) chunk.iSignTexture = editVals.sSignTexture.toInt();
    if (!editVals.sBackTexture.isEmpty()) chunk.iBackTexture = editVals.sBackTexture.toInt();
    SetChunk(i, chunk);
  }
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
    newChunk.iAIMaxSpeed = editVals.sAIMaxSpeed.toInt();
    newChunk.iAIAccuracy = editVals.sAIAccuracy.toInt();
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
    InsertChunk(iIndex, newChunk);
  }
  g_pMainWindow->LogMessage("Inserted " + QString::number(iCount) + " geometry chunks");
  g_pMainWindow->GetCurrentPreview()->DeleteTrackModels();
}

//-------------------------------------------------------------------------------------------------