#ifndef _WHIPLIB_SIGNTYPE_H
#define _WHIPLIB_SIGNTYPE_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
//-------------------------------------------------------------------------------------------------

struct tSignType
{
  eWhipModel modelType;
  std::string sDescription;
  bool bCanHaveTexture;
};

static tSignType g_signAy[17] = {
  { eWhipModel::SIGN_TOWER,   "TOWER"    , true },
  { eWhipModel::SIGN_TOWER2,  "TOWER 2"  , false},
  { eWhipModel::SIGN_SIGN01,  "SIGN 1"   , true },
  { eWhipModel::SIGN_SIGN02,  "SIGN 2"   , true },
  { eWhipModel::SIGN_BUILD,   "BUILD"    , false},
  { eWhipModel::SIGN_BUILD1,  "BUILD 1"  , false},
  { eWhipModel::SIGN_BUILD2,  "BUILD 2"  , false},
  { eWhipModel::SIGN_BUILD3,  "BUILD 3"  , true },
  { eWhipModel::SIGN_HEELBAR, "HEELBAR"  , false},
  { eWhipModel::SIGN_BALLOON, "BALLOON"  , true },
  { eWhipModel::SIGN_TREE,    "TREE"     , false},
  { eWhipModel::SIGN_ADVERT,  "ADVERT"   , true },
  { eWhipModel::SIGN_ADVERT2, "ADVERT 2" , true },
  { eWhipModel::SIGN_QUADBLD, "QUAD BLD" , false},
  { eWhipModel::SIGN_BLD0,    "BLD 0"    , false},
  { eWhipModel::SIGN_BALLOON2,"BIG BALL" , true },
  { eWhipModel::SIGN_ADVERT3, "BIG AD"   , true },
};
static int g_signAyCount = sizeof(g_signAy) / sizeof(g_signAy[0]);

//-------------------------------------------------------------------------------------------------

static const tSignType &GetSignTypeFromModel(eWhipModel model)
{
  for (int i = 0; i < g_signAyCount; ++i) {
    if (g_signAy[i].modelType == model)
      return g_signAy[i];
  }
  assert(0);
  return g_signAy[0];
}

//-------------------------------------------------------------------------------------------------

static const bool IsSign(eWhipModel model)
{
  for (int i = 0; i < g_signAyCount; ++i) {
    if (g_signAy[i].modelType == model)
      return true;
  }
  return false;
}

//-------------------------------------------------------------------------------------------------
#endif