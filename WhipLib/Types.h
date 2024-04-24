#ifndef _WHIPLIB_TYPES_H
#define _WHIPLIB_TYPES_H
//-------------------------------------------------------------------------------------------------
#include <stddef.h>
//-------------------------------------------------------------------------------------------------

typedef signed char      int8;
typedef signed short     int16;
typedef signed int       int32;
typedef signed __int64   int64;
typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned int     uint32;
typedef unsigned __int64 uint64;

//-------------------------------------------------------------------------------------------------

enum eShapeSection
{
  DRIVING_SURFACE = 0,
  CENTER,
  LLANE,
  RLANE,
  LSHOULDER,
  RSHOULDER,
  LWALL,
  RWALL,
  ROOF,
  ENVIRFLOOR,
  OWALLFLOOR,
  LLOWALL,
  RLOWALL,
  LUOWALL,
  RUOWALL,
  SELECTED,
  AILINE1,
  AILINE2,
  AILINE3,
  AILINE4
};

//-------------------------------------------------------------------------------------------------

enum eWhipModel
{
  CAR_F1WACK = 0,
  CAR_AUTO,
  CAR_DESILVA,
  CAR_PULSE,
  CAR_GLOBAL,
  CAR_MILLION,
  CAR_MISSION,
  CAR_ZIZIN,
  CAR_REISE,
  SIGN_TOWER,
  SIGN_TOWER2,
  SIGN_SIGN01,
  SIGN_SIGN02,
  SIGN_BUILD,
  SIGN_BUILD1,
  SIGN_BUILD2,
  SIGN_BUILD3,
  SIGN_HEELBAR,
  SIGN_BALLOON,
  SIGN_BALLOON2,
  SIGN_TREE,
  SIGN_ADVERT,
  SIGN_ADVERT2,
  SIGN_ADVERT3,
  SIGN_QUADBLD,
  SIGN_BLD0
};

//-------------------------------------------------------------------------------------------------
#endif