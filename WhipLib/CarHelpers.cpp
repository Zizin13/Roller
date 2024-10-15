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
    case eWhipModel::CAR_F1WACK:
      return "F1WACK";
      break;
    case eWhipModel::CAR_XAUTO:
      return "XAUTO";
      break;
    case eWhipModel::CAR_XDESILVA:
      return "XDESILVA";
      break;
    case eWhipModel::CAR_XPULSE:
      return "XPULSE";
      break;
    case eWhipModel::CAR_XGLOBAL:
      return "XGLOBAL";
      break;
    case eWhipModel::CAR_XMILLION:
      return "XMILLION";
      break;
    case eWhipModel::CAR_XMISSION:
      return "XMISSION";
      break;
    case eWhipModel::CAR_XZIZIN:
      return "XZIZIN";
      break;
    case eWhipModel::CAR_XREISE:
      return "XREISE";
      break;
    case eWhipModel::CAR_YAUTO:
      return "YAUTO";
      break;
    case eWhipModel::CAR_YDESILVA:
      return "YDESILVA";
      break;
    case eWhipModel::CAR_YPULSE:
      return "YPULSE";
      break;
    case eWhipModel::CAR_YGLOBAL:
      return "YGLOBAL";
      break;
    case eWhipModel::CAR_YMILLION:
      return "YMILLION";
      break;
    case eWhipModel::CAR_YMISSION:
      return "YMISSION";
      break;
    case eWhipModel::CAR_YZIZIN:
      return "YZIZIN";
      break;
    case eWhipModel::CAR_YREISE:
      return "YREISE";
      break;
    case eWhipModel::CAR_DEATH:
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
  if (model == eWhipModel::CAR_F1WACK)
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
    case eWhipModel::CAR_YDESILVA:
      if (uiColor == 0x000000cf)
        uiColor = 0x000000c3;
      break;
    case eWhipModel::CAR_YPULSE:
      if (uiColor == 0x000000e7)
        uiColor = 0x000000ab;
      break;
    case eWhipModel::CAR_YGLOBAL:
      if (uiColor == 0x000000e7)
        uiColor = 0x000000ff;
      break;
    case eWhipModel::CAR_YMISSION:
      if (uiColor == 0x000000c3)
        uiColor = 0x000000ab;
      break;
    case eWhipModel::CAR_YZIZIN:
      if (uiColor == 0x0000008f)
        uiColor = 0x000000db;
      break;
  }
  //apply to texture
  uiTex &= ~SURFACE_MASK_TEXTURE_INDEX;
  uiTex |= uiColor;
}

//-------------------------------------------------------------------------------------------------