#ifndef _WHIPLIB_SIGNTYPE_H
#define _WHIPLIB_SIGNTYPE_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
//-------------------------------------------------------------------------------------------------

struct tSignType
{
  eWhipModel modelType;
  std::string sDescription;
};

tSignType g_signAy[17] = {
  { eWhipModel::SIGN_TOWER,   "TOWER"    },
  { eWhipModel::SIGN_TOWER2,  "TOWER 2"  },
  { eWhipModel::SIGN_SIGN01,  "SIGN 1"   },
  { eWhipModel::SIGN_SIGN02,  "SIGN 2"   },
  { eWhipModel::SIGN_BUILD,   "BUILD"    },
  { eWhipModel::SIGN_BUILD1,  "BUILD 1"  },
  { eWhipModel::SIGN_BUILD2,  "BUILD 2"  },
  { eWhipModel::SIGN_BUILD3,  "BUILD 3"  },
  { eWhipModel::SIGN_HEELBAR, "HEELBAR"  },
  { eWhipModel::SIGN_BALLOON, "BALLOON"  },
  { eWhipModel::SIGN_TREE,    "TREE"     },
  { eWhipModel::SIGN_ADVERT,  "ADVERT"   },
  { eWhipModel::SIGN_ADVERT2, "ADVERT 2" },
  { eWhipModel::SIGN_QUADBLD, "QUAD BLD" },
  { eWhipModel::SIGN_BLD0,    "BLD 0"    },
  { eWhipModel::SIGN_BALLOON2,"BIG BALL" },
  { eWhipModel::SIGN_ADVERT3, "BIG AD"   },
};
int g_signAyCount = sizeof(g_signAy) / sizeof(g_signAy[0]);

//-------------------------------------------------------------------------------------------------
#endif