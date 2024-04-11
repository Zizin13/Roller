#ifndef _TRACKEDITOR_CHUNK_EDIT_VALUES_H
#define _TRACKEDITOR_CHUNK_EDIT_VALUES_H
//-------------------------------------------------------------------------------------------------
#include "qstring.h"
//-------------------------------------------------------------------------------------------------

class CChunkEditValues
{
public:
  CChunkEditValues();
  CChunkEditValues(const QString &sCopyLeftShoulderWidth
                  , const QString &sCopyLeftLaneWidth
                  , const QString &sCopyRightLaneWidth
                  , const QString &sCopyRightShoulderWidth
                  , const QString &sCopyLShoulderHeight
                  , const QString &sCopyRShoulderHeight
                  , const QString &sCopyLength
                  , const QString &sCopyYaw
                  , const QString &sCopyPitch
                  , const QString &sCopyRoll
                  , const QString &sCopyAILine1
                  , const QString &sCopyAILine2
                  , const QString &sCopyAILine3
                  , const QString &sCopyAILine4
                  , const QString &sCopyTrackGrip
                  , const QString &sCopyLeftShoulderGrip
                  , const QString &sCopyRightShoulderGrip
                  , const QString &sCopyUnk04
                  , const QString &sCopyUnk05
                  , const QString &sCopyAudioAboveTrigger
                  , const QString &sCopyAudioTriggerSpeed
                  , const QString &sCopyAudioBelowTrigger
                  , const QString &sCopyLeftSurfaceType
                  , const QString &sCopyCenterSurfaceType
                  , const QString &sCopyRightSurfaceType
                  , const QString &sCopyLWallType
                  , const QString &sCopyRWallType
                  , const QString &sCopyRoofType
                  , const QString &sCopyLUOuterWallType
                  , const QString &sCopyLLOuterWallType
                  , const QString &sCopyOuterFloorType
                  , const QString &sCopyRLOuterWallType
                  , const QString &sCopyRUOuterWallType
                  , const QString &sCopyEnvironmentFloorType
                  , const QString &sCopySignType
                  , const QString &sCopySignHorizOffset
                  , const QString &sCopySignVertOffset
                  , const QString &sCopySignYaw
                  , const QString &sCopySignPitch
                  , const QString &sCopySignRoll
                  , const QString &sCopyLUOuterWallHOffset
                  , const QString &sCopyLLOuterWallHOffset
                  , const QString &sCopyLOuterFloorHOffset
                  , const QString &sCopyROuterFloorHOffset
                  , const QString &sCopyRLOuterWallHOffset
                  , const QString &sCopyRUOuterWallHOffset
                  , const QString &sCopyLUOuterWallHeight
                  , const QString &sCopyLLOuterWallHeight
                  , const QString &sCopyLOuterFloorHeight
                  , const QString &sCopyROuterFloorHeight
                  , const QString &sCopyRLOuterWallHeight
                  , const QString &sCopyRUOuterWallHeight
                  , const QString &sCopyRoofHeight
                  , const QString &sCopyDrawOrder1
                  , const QString &sCopyDrawOrder2
                  , const QString &sCopyDrawOrder3
                  , const QString &sCopyUnk37
                  , const QString &sCopyUnk38
                  , const QString &sCopyUnk39
                  , const QString &sCopyUnk40
                  , const QString &sCopyUnk41
                  , const QString &sCopyUnk42
                  , const QString &sCopyUnk43
                  , const QString &sCopyUnk44
                  , const QString &sCopyUnk45
                  , const QString &sCopyUnk46
                  , const QString &sCopyUnk47
                  , const QString &sCopyUnk48
                  , const QString &sCopyUnk49
                  , const QString &sCopyUnk50
                  , const QString &sCopySignTexture
                  , const QString &sCopyBackTexture
                  , const QString &sCopyStuntScaleFactor
                  , const QString &sCopyStuntAngle
                  , const QString &sCopyStuntUnknown
                  , const QString &sCopyStuntTimingGroup
                  , const QString &sCopyStuntHeight
                  , const QString &sCopyStuntTimeBulging
                  , const QString &sCopyStuntTimeFlat
                  , const QString &sCopyStuntExpandsContracts
                  , const QString &sCopyStuntBulge);
  ~CChunkEditValues();

  void Clear();
  void Set(int index, QString value);

  QString sLeftShoulderWidth;
  QString sLeftLaneWidth;
  QString sRightLaneWidth;
  QString sRightShoulderWidth;
  QString sLShoulderHeight;
  QString sRShoulderHeight;
  QString sLength;
  QString sYaw;
  QString sPitch;
  QString sRoll;
  QString sAILine1;
  QString sAILine2;
  QString sAILine3;
  QString sAILine4;
  QString sTrackGrip;
  QString sLeftShoulderGrip;
  QString sRightShoulderGrip;
  QString sUnk04;
  QString sUnk05;
  QString sAudioAboveTrigger;
  QString sAudioTriggerSpeed;
  QString sAudioBelowTrigger;
  QString sLeftSurfaceType;
  QString sCenterSurfaceType;
  QString sRightSurfaceType;
  QString sLWallType;
  QString sRWallType;
  QString sRoofType;
  QString sLUOuterWallType;
  QString sLLOuterWallType;
  QString sOuterFloorType;
  QString sRLOuterWallType;
  QString sRUOuterWallType;
  QString sEnvironmentFloorType;
  QString sSignType;
  QString sSignHorizOffset;
  QString sSignVertOffset;
  QString sSignYaw;
  QString sSignPitch;
  QString sSignRoll;
  QString sLUOuterWallHOffset;
  QString sLLOuterWallHOffset;
  QString sLOuterFloorHOffset;
  QString sROuterFloorHOffset;
  QString sRLOuterWallHOffset;
  QString sRUOuterWallHOffset;
  QString sLUOuterWallHeight;
  QString sLLOuterWallHeight;
  QString sLOuterFloorHeight;
  QString sROuterFloorHeight;
  QString sRLOuterWallHeight;
  QString sRUOuterWallHeight;
  QString sRoofHeight;
  QString sDrawOrder1;
  QString sDrawOrder2;
  QString sDrawOrder3;
  QString sUnk37;
  QString sUnk38;
  QString sUnk39;
  QString sUnk40;
  QString sUnk41;
  QString sUnk42;
  QString sUnk43;
  QString sUnk44;
  QString sUnk45;
  QString sUnk46;
  QString sUnk47;
  QString sUnk48;
  QString sUnk49;
  QString sUnk50;
  QString sSignTexture;
  QString sBackTexture;
  QString sStuntScaleFactor;
  QString sStuntAngle;
  QString sStuntUnknown;
  QString sStuntTimingGroup;
  QString sStuntHeight;
  QString sStuntTimeBulging;
  QString sStuntTimeFlat;
  QString sStuntExpandsContracts;
  QString sStuntBulge;
};

//-------------------------------------------------------------------------------------------------
#endif