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
                , const QString &sCopyUnk04
                , const QString &sCopyUnk05
                , const QString &sCopyUnk06
                , const QString &sCopyUnk07
                , const QString &sCopyUnk08
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
                , const QString &sCopyLUOuterWallAngle
                , const QString &sCopyLLOuterWallAngle
                , const QString &sCopyUnk23
                , const QString &sCopyUnk24
                , const QString &sCopyRLOuterWallAngle
                , const QString &sCopyRUOuterWallAngle
                , const QString &sCopyLUOuterWallHeight
                , const QString &sCopyLLOuterWallHeight
                , const QString &sCopyUnk29
                , const QString &sCopyUnk30
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
  sUnk04 = sCopyUnk04;
  sUnk05 = sCopyUnk05;
  sUnk06 = sCopyUnk06;
  sUnk07 = sCopyUnk07;
  sUnk08 = sCopyUnk08;
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
  sLUOuterWallAngle = sCopyLUOuterWallAngle;
  sLLOuterWallAngle = sCopyLLOuterWallAngle;
  sUnk23 = sCopyUnk23;
  sUnk24 = sCopyUnk24;
  sRLOuterWallAngle = sCopyRLOuterWallAngle;
  sRUOuterWallAngle = sCopyRUOuterWallAngle;
  sLUOuterWallHeight = sCopyLUOuterWallHeight;
  sLLOuterWallHeight = sCopyLLOuterWallHeight;
  sUnk29 = sCopyUnk29;
  sUnk30 = sCopyUnk30;
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
}

//-------------------------------------------------------------------------------------------------