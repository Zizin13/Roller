#include "Types.h"
#include <assert.h>
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

std::string CarHelpers::GetCarNameFromModel(eWhipModel model)
{
  switch (model) {
    case CAR_F1WACK:
      return "F1WACK";
      break;
    case CAR_XAUTO:
      return "XAUTO";
      break;
    case CAR_XDESILVA:
      return "XDESILVA";
      break;
    case CAR_XPULSE:
      return "XPULSE";
      break;
    case CAR_XGLOBAL:
      return "XGLOBAL";
      break;
    case CAR_XMILLION:
      return "XMILLION";
      break;
    case CAR_XMISSION:
      return "XMISSION";
      break;
    case CAR_XZIZIN:
      return "XZIZIN";
      break;
    case CAR_XREISE:
      return "XREISE";
      break;
    case CAR_YAUTO:
      return "YAUTO";
      break;
    case CAR_YDESILVA:
      return "YDESILVA";
      break;
    case CAR_YPULSE:
      return "YPULSE";
      break;
    case CAR_YGLOBAL:
      return "YGLOBAL";
      break;
    case CAR_YMILLION:
      return "YMILLION";
      break;
    case CAR_YMISSION:
      return "YMISSION";
      break;
    case CAR_YZIZIN:
      return "YZIZIN";
      break;
    case CAR_YREISE:
      return "YREISE";
      break;
    case CAR_DEATH:
      return "DEATH";
      break;
    default:
      assert(0);
      return "ERROR";
  }
};

//-------------------------------------------------------------------------------------------------

std::string CarHelpers::GetCarTextureFromModel(eWhipModel model)
{
  if (model == CAR_F1WACK)
    return "RED28.BM";
  else
    return GetCarNameFromModel(model) + ".BM";
}

//-------------------------------------------------------------------------------------------------