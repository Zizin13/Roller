#include "CarHelpers.h"
#include "Types.h"
#include "Texture.h"
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
//taken from _car_flat_remap_variable_1 in disasm_object_3
void CarHelpers::RemapColor(eWhipModel model, uint32 &uiTex)
{
  if (uiTex & SURFACE_FLAG_APPLY_TEXTURE)
    return; //surface is using a texture, don't change it

  uint32 uiColor = uiTex & SURFACE_MASK_TEXTURE_INDEX;

  //other cars are unchanged
  switch (model) {
    case CAR_YDESILVA:
      if (uiColor == 0x000000cf)
        uiColor = 0x000000c3;
      break;
    case CAR_YPULSE:
      if (uiColor == 0x000000e7)
        uiColor = 0x000000ab;
      break;
    case CAR_YGLOBAL:
      if (uiColor == 0x000000e7)
        uiColor = 0x000000ff;
      break;
    case CAR_YMISSION:
      if (uiColor == 0x000000c3)
        uiColor = 0x000000ab;
      break;
    case CAR_YZIZIN:
      if (uiColor == 0x0000008f)
        uiColor = 0x000000db;
      break;
  }
  //apply to texture
  uiTex &= ~SURFACE_MASK_TEXTURE_INDEX;
  uiTex |= uiColor;
}

//-------------------------------------------------------------------------------------------------