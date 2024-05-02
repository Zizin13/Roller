#include "ChunkEditValues.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CChunkEditValues::CChunkEditValues()
{
  Clear();
}

//-------------------------------------------------------------------------------------------------

CChunkEditValues::CChunkEditValues(const QString &sCopyLeftShoulderWidth
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
                , const QString &sCopyAIMaxSpeed
                , const QString &sCopyAIAccuracy
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
                , const QString &sCopyStuntBulge)
{
  sLeftShoulderWidth = sCopyLeftShoulderWidth;
  sLeftLaneWidth = sCopyLeftLaneWidth;
  sRightLaneWidth = sCopyRightLaneWidth;
  sRightShoulderWidth = sCopyRightShoulderWidth;
  sLShoulderHeight = sCopyLShoulderHeight;
  sRShoulderHeight = sCopyRShoulderHeight;
  sLength = sCopyLength;
  sYaw = sCopyYaw;
  sPitch = sCopyPitch;
  sRoll = sCopyRoll;
  sAILine1 = sCopyAILine1;
  sAILine2 = sCopyAILine2;
  sAILine3 = sCopyAILine3;
  sAILine4 = sCopyAILine4;
  sTrackGrip = sCopyTrackGrip;
  sLeftShoulderGrip = sCopyLeftShoulderGrip;
  sRightShoulderGrip = sCopyRightShoulderGrip;
  sAIMaxSpeed = sCopyAIMaxSpeed;
  sAIAccuracy = sCopyAIAccuracy;
  sAudioAboveTrigger = sCopyAudioAboveTrigger;
  sAudioTriggerSpeed = sCopyAudioTriggerSpeed;
  sAudioBelowTrigger = sCopyAudioBelowTrigger;
  sLeftSurfaceType = sCopyLeftSurfaceType;
  sCenterSurfaceType = sCopyCenterSurfaceType;
  sRightSurfaceType = sCopyRightSurfaceType;
  sLWallType = sCopyLWallType;
  sRWallType = sCopyRWallType;
  sRoofType = sCopyRoofType;
  sLUOuterWallType = sCopyLUOuterWallType;
  sLLOuterWallType = sCopyLLOuterWallType;
  sOuterFloorType = sCopyOuterFloorType;
  sRLOuterWallType = sCopyRLOuterWallType;
  sRUOuterWallType = sCopyRUOuterWallType;
  sEnvironmentFloorType = sCopyEnvironmentFloorType;
  sSignType = sCopySignType;
  sSignHorizOffset = sCopySignHorizOffset;
  sSignVertOffset = sCopySignVertOffset;
  sSignYaw = sCopySignYaw;
  sSignPitch = sCopySignPitch;
  sSignRoll = sCopySignRoll;
  sLUOuterWallHOffset = sCopyLUOuterWallHOffset;
  sLLOuterWallHOffset = sCopyLLOuterWallHOffset;
  sLOuterFloorHOffset = sCopyLOuterFloorHOffset;
  sROuterFloorHOffset = sCopyROuterFloorHOffset;
  sRLOuterWallHOffset = sCopyRLOuterWallHOffset;
  sRUOuterWallHOffset = sCopyRUOuterWallHOffset;
  sLUOuterWallHeight = sCopyLUOuterWallHeight;
  sLLOuterWallHeight = sCopyLLOuterWallHeight;
  sLOuterFloorHeight = sCopyLOuterFloorHeight;
  sROuterFloorHeight = sCopyROuterFloorHeight;
  sRLOuterWallHeight = sCopyRLOuterWallHeight;
  sRUOuterWallHeight = sCopyRUOuterWallHeight;
  sRoofHeight = sCopyRoofHeight;
  sDrawOrder1 = sCopyDrawOrder1;
  sDrawOrder2 = sCopyDrawOrder2;
  sDrawOrder3 = sCopyDrawOrder3;
  sUnk37 = sCopyUnk37;
  sUnk38 = sCopyUnk38;
  sUnk39 = sCopyUnk39;
  sUnk40 = sCopyUnk40;
  sUnk41 = sCopyUnk41;
  sUnk42 = sCopyUnk42;
  sUnk43 = sCopyUnk43;
  sUnk44 = sCopyUnk44;
  sUnk45 = sCopyUnk45;
  sUnk46 = sCopyUnk46;
  sUnk47 = sCopyUnk47;
  sUnk48 = sCopyUnk48;
  sUnk49 = sCopyUnk49;
  sUnk50 = sCopyUnk50;
  sSignTexture = sCopySignTexture;
  sBackTexture = sCopyBackTexture;
  sStuntScaleFactor = sCopyStuntScaleFactor;
  sStuntAngle = sCopyStuntAngle;
  sStuntUnknown = sCopyStuntUnknown;
  sStuntTimingGroup = sCopyStuntTimingGroup;
  sStuntHeight = sCopyStuntHeight;
  sStuntTimeBulging = sCopyStuntTimeBulging;
  sStuntTimeFlat = sCopyStuntTimeFlat;
  sStuntExpandsContracts = sCopyStuntExpandsContracts;
  sStuntBulge = sCopyStuntBulge;
}

//-------------------------------------------------------------------------------------------------

CChunkEditValues::~CChunkEditValues()
{

}

//-------------------------------------------------------------------------------------------------

void CChunkEditValues::Clear()
{
  sLeftShoulderWidth = ""; sLeftLaneWidth = ""; sRightLaneWidth = ""; sRightShoulderWidth = "";
  sLShoulderHeight = ""; sRShoulderHeight = ""; sLength = "";
  sYaw = ""; sPitch = ""; sRoll = "";
  sAILine1 = ""; sAILine2 = ""; sAILine3 = ""; sAILine4 = "";
  sTrackGrip = ""; sLeftShoulderGrip = ""; sRightShoulderGrip = "";
  sAIMaxSpeed = ""; sAIAccuracy = ""; sAudioAboveTrigger = ""; sAudioTriggerSpeed = ""; sAudioBelowTrigger = "";
  sLeftSurfaceType = ""; sCenterSurfaceType = ""; sRightSurfaceType = "";
  sLWallType = ""; sRWallType = ""; sRoofType = ""; sLUOuterWallType = ""; sLLOuterWallType = ""; sOuterFloorType = "";
  sRLOuterWallType = ""; sRUOuterWallType = ""; sEnvironmentFloorType = ""; sSignType = ""; sSignHorizOffset = ""; sSignVertOffset = "";
  sSignYaw = ""; sSignPitch = ""; sSignRoll = "";
  sLUOuterWallHOffset = ""; sLLOuterWallHOffset = ""; sLOuterFloorHOffset = ""; sROuterFloorHOffset = ""; sRLOuterWallHOffset = ""; sRUOuterWallHOffset = "";
  sLUOuterWallHeight = ""; sLLOuterWallHeight = ""; sLOuterFloorHeight = ""; sROuterFloorHeight = ""; sRLOuterWallHeight = ""; sRUOuterWallHeight = "";
  sRoofHeight = ""; sDrawOrder1 = ""; sDrawOrder2 = ""; sDrawOrder3 = ""; sUnk37 = ""; sUnk38 = "";
  sUnk39 = ""; sUnk40 = ""; sUnk41 = ""; sUnk42 = ""; sUnk43 = ""; sUnk44 = "";
  sUnk45 = ""; sUnk46 = ""; sUnk47 = ""; sUnk48 = ""; sUnk49 = ""; sUnk50 = "";
  sSignTexture = ""; sBackTexture = "";
  sStuntScaleFactor = ""; sStuntAngle = ""; sStuntUnknown = ""; sStuntTimingGroup = ""; sStuntHeight = ""; sStuntTimeBulging = "";
  sStuntTimeFlat = ""; sStuntExpandsContracts = ""; sStuntBulge = "";
}

//-------------------------------------------------------------------------------------------------

void CChunkEditValues::Set(int index, QString value)
{
  switch (index) {
    case 0: sLeftShoulderWidth = value; break;
    case 1: sLeftLaneWidth = value; break;
    case 2: sRightLaneWidth = value; break;
    case 3: sRightShoulderWidth = value; break;
    case 4: sLShoulderHeight = value; break;
    case 5: sRShoulderHeight = value; break;
    case 6: sLength = value; break;
    case 7: sYaw = value; break;
    case 8: sPitch = value; break;
    case 9: sRoll = value; break;
    case 10: sAILine1 = value; break;
    case 11: sAILine2 = value; break;
    case 12: sAILine3 = value; break;
    case 13: sAILine4 = value; break;
    case 14: sTrackGrip = value; break;
    case 15: sLeftShoulderGrip = value; break;
    case 16: sRightShoulderGrip = value; break;
    case 17: sAIMaxSpeed = value; break;
    case 18: sAIAccuracy = value; break;
    case 19: sAudioAboveTrigger = value; break;
    case 20: sAudioTriggerSpeed = value; break;
    case 21: sAudioBelowTrigger = value; break;
    case 22: sLeftSurfaceType = value; break;
    case 23: sCenterSurfaceType = value; break;
    case 24: sRightSurfaceType = value; break;
    case 25: sLWallType = value; break;
    case 26: sRWallType = value; break;
    case 27: sRoofType = value; break;
    case 28: sLUOuterWallType = value; break;
    case 29: sLLOuterWallType = value; break;
    case 30: sOuterFloorType = value; break;
    case 31: sRLOuterWallType = value; break;
    case 32: sRUOuterWallType = value; break;
    case 33: sEnvironmentFloorType = value; break;
    case 34: sSignType = value; break;
    case 35: sSignHorizOffset = value; break;
    case 36: sSignVertOffset = value; break;
    case 37: sSignYaw = value; break;
    case 38: sSignPitch = value; break;
    case 39: sSignRoll = value; break;
    case 40: sLUOuterWallHOffset = value; break;
    case 41: sLLOuterWallHOffset = value; break;
    case 42: sLOuterFloorHOffset = value; break;
    case 43: sROuterFloorHOffset = value; break;
    case 44: sRLOuterWallHOffset = value; break;
    case 45: sRUOuterWallHOffset = value; break;
    case 46: sLUOuterWallHeight = value; break;
    case 47: sLLOuterWallHeight = value; break;
    case 48: sLOuterFloorHeight = value; break;
    case 49: sROuterFloorHeight = value; break;
    case 50: sRLOuterWallHeight = value; break;
    case 51: sRUOuterWallHeight = value; break;
    case 52: sRoofHeight = value; break;
    case 53: sDrawOrder1 = value; break;
    case 54: sDrawOrder2 = value; break;
    case 55: sDrawOrder3 = value; break;
    case 56: sUnk37 = value; break;
    case 57: sUnk38 = value; break;
    case 58: sUnk39 = value; break;
    case 59: sUnk40 = value; break;
    case 60: sUnk41 = value; break;
    case 61: sUnk42 = value; break;
    case 62: sUnk43 = value; break;
    case 63: sUnk44 = value; break;
    case 64: sUnk45 = value; break;
    case 65: sUnk46 = value; break;
    case 66: sUnk47 = value; break;
    case 67: sUnk48 = value; break;
    case 68: sUnk49 = value; break;
    case 69: sUnk50 = value; break;
    case 70: sSignTexture = value; break;
    case 71: sBackTexture = value; break;
    case 72: sStuntScaleFactor = value; break;
    case 73: sStuntAngle = value; break;
    case 74: sStuntUnknown = value; break;
    case 75: sStuntTimingGroup = value; break;
    case 76: sStuntHeight = value; break;
    case 77: sStuntTimeBulging = value; break;
    case 78: sStuntTimeFlat = value; break;
    case 79: sStuntExpandsContracts = value; break;
    case 80: sStuntBulge = value; break;
  }
}

//-------------------------------------------------------------------------------------------------