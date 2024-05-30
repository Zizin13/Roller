#include "EditDataWidget.h"
#include "Track.h"
#include "MainWindow.h"
#include "EditSurfaceDialog.h"
#include "TilePicker.h"
#include "Texture.h"
#include "Palette.h"
#include "QtHelpers.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CEditDataWidget::CEditDataWidget(QWidget *pParent)
  : QWidget(pParent)
{
  setupUi(this);

  connect(g_pMainWindow, &CMainWindow::UpdateGeometrySelectionSig, this, &CEditDataWidget::UpdateGeometrySelection);
}

//-------------------------------------------------------------------------------------------------

CEditDataWidget::~CEditDataWidget()
{
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::UpdateGeometrySelection(int iFrom, int iTo)
{
  if (!g_pMainWindow->GetCurrentTrack())
    return;

  //bool bMixedData = false;
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLShoulderWidth, p->editVals.sLeftShoulderWidth);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLLaneWidth, p->editVals.sLeftLaneWidth);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRLaneWidth, p->editVals.sRightLaneWidth);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRShoulderWidth, p->editVals.sRightShoulderWidth);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLShoulderHeight, p->editVals.sLShoulderHeight);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRShoulderHeight, p->editVals.sRShoulderHeight);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLength, p->editVals.sLength);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leYaw, p->editVals.sYaw);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(lePitch, p->editVals.sPitch);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRoll, p->editVals.sRoll);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAILine1, p->editVals.sAILine1);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAILine2, p->editVals.sAILine2);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAILine3, p->editVals.sAILine3);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAILine4, p->editVals.sAILine4);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leTrackGrip, p->editVals.sTrackGrip);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLeftShoulderGrip, p->editVals.sLeftShoulderGrip);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRShoulderGrip, p->editVals.sRightShoulderGrip);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAIMaxSpeed, p->editVals.sAIMaxSpeed);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAIAccuracy, p->editVals.sAIAccuracy);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAudioAboveTrigger, p->editVals.sAudioAboveTrigger);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAudioTriggerSpeed, p->editVals.sAudioTriggerSpeed);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAudioBelowTrigger, p->editVals.sAudioBelowTrigger);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLeftSurfaceType, p->editVals.sLeftSurfaceType);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leCenterSurfaceType, p->editVals.sCenterSurfaceType);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRightSurfaceType, p->editVals.sRightSurfaceType);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLWallType, p->editVals.sLWallType);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRWallType, p->editVals.sRWallType);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRoofType, p->editVals.sRoofType);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLUOuterWallType, p->editVals.sLUOuterWallType);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLLOuterWallType, p->editVals.sLLOuterWallType);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leOuterFloorType, p->editVals.sOuterFloorType);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRLOuterWallType, p->editVals.sRLOuterWallType);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRUOuterWallType, p->editVals.sRUOuterWallType);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leEnvironmentFloorType, p->editVals.sEnvironmentFloorType);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leSignType, p->editVals.sSignType);
  //bMixedData |= QtHelpers::UpdateCBWithSelectionValue(cbSignType, p->editVals.sSignType);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leSignHorizOffset, p->editVals.sSignHorizOffset);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leSignVertOffset, p->editVals.sSignVertOffset);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leSignYaw, p->editVals.sSignYaw);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leSignPitch, p->editVals.sSignPitch);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leSignRoll, p->editVals.sSignRoll);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLUOuterWallHOffset, p->editVals.sLUOuterWallHOffset);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLLOuterWallHOffset, p->editVals.sLLOuterWallHOffset);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLOuterFloorHOffset, p->editVals.sLOuterFloorHOffset);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leROuterFloorHOffset, p->editVals.sROuterFloorHOffset);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRLOuterWallHOffset, p->editVals.sRLOuterWallHOffset);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRUOuterWallHOffset, p->editVals.sRUOuterWallHOffset);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLUOuterWallHeight, p->editVals.sLUOuterWallHeight);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLLOuterWallHeight, p->editVals.sLLOuterWallHeight);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLOuterFloorHeight, p->editVals.sLOuterFloorHeight);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leROuterFloorHeight, p->editVals.sROuterFloorHeight);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRLOuterWallHeight, p->editVals.sRLOuterWallHeight);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRUOuterWallHeight, p->editVals.sRUOuterWallHeight);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRoofHeight, p->editVals.sRoofHeight);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leDrawOrder1, p->editVals.sDrawOrder1);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leDrawOrder2, p->editVals.sDrawOrder2);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leDrawOrder3, p->editVals.sDrawOrder3);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk37, p->editVals.sUnk37);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk38, p->editVals.sUnk38);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk39, p->editVals.sUnk39);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk40, p->editVals.sUnk40);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk41, p->editVals.sUnk41);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk42, p->editVals.sUnk42);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk43, p->editVals.sUnk43);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk44, p->editVals.sUnk44);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk45, p->editVals.sUnk45);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk46, p->editVals.sUnk46);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk47, p->editVals.sUnk47);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk48, p->editVals.sUnk48);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk49, p->editVals.sUnk49);
  //bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk50, p->editVals.sUnk50);
  //bMixedData |= QtHelpers::UpdateSignWithSelectionValue(pbSign, ckApplySign, leSign, p->editVals.sSignTexture, g_pMainWindow->GetCurrentTrack()->m_pBld);

  UpdateTextures(leLeftSurfaceType, lblLSurfaceTex1, lblLSurfaceTex2);
  UpdateTextures(leCenterSurfaceType, lblCSurfaceTex1, lblCSurfaceTex2);
  UpdateTextures(leRightSurfaceType, lblRSurfaceTex1, lblRSurfaceTex2);
  UpdateTextures(leLWallType, lblLWallTex1, lblLWallTex2);
  UpdateTextures(leRWallType, lblRWallTex1, lblRWallTex2);
  UpdateTextures(leRoofType, lblRoofTex1, lblRoofTex2);
  UpdateTextures(leLUOuterWallType, lblLUOuterWallTex1, lblLUOuterWallTex2);
  UpdateTextures(leLLOuterWallType, lblLLOuterWallTex1, lblLLOuterWallTex2);
  UpdateTextures(leOuterFloorType, lblOuterFloorTex1, lblOuterFloorTex2);
  UpdateTextures(leRLOuterWallType, lblRLOuterWallTex1, lblRLOuterWallTex2);
  UpdateTextures(leRUOuterWallType, lblRUOuterWallTex1, lblRUOuterWallTex2);
  UpdateTextures(leEnvironmentFloorType, lblEnvirFloorTex1, lblEnvirFloorTex2);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::UpdateTextures(QLineEdit *pLineEdit, QLabel *pTex1, QLabel *pTex2)
{
  if (!g_pMainWindow->GetCurrentTrack() || !g_pMainWindow->GetCurrentTrack()->m_pTex)
    return;

  //textures
  QPixmap pixmap;
  int iIndex;
  if (pLineEdit->text().compare("-1") == 0) {
    pTex1->setPixmap(QPixmap());
    pTex2->setPixmap(QPixmap());
  } else if (!pLineEdit->text().isEmpty() && pLineEdit->placeholderText().compare(MIXED_DATA) != 0) {
    int iValue = pLineEdit->text().toInt();
    unsigned int uiSignedBitVal = CTrack::GetSignedBitValueFromInt(iValue);
    iIndex = CTrack::GetIntValueFromSignedBit(uiSignedBitVal & SURFACE_TEXTURE_INDEX);
    if (iIndex < g_pMainWindow->GetCurrentTrack()->m_pTex->m_iNumTiles) {
      pixmap.convertFromImage(QtHelpers::GetQImageFromTile(g_pMainWindow->GetCurrentTrack()->m_pTex->m_pTileAy[iIndex]));
      pTex1->setPixmap(pixmap);

      if (uiSignedBitVal & SURFACE_FLAG_TEXTURE_PAIR && iIndex > 0) {
        if (uiSignedBitVal & SURFACE_FLAG_PAIR_NEXT_TEX)
          pixmap.convertFromImage(QtHelpers::GetQImageFromTile(g_pMainWindow->GetCurrentTrack()->m_pTex->m_pTileAy[iIndex + 1]));
        pTex2->setPixmap(pixmap);
      } else {
        pTex2->setPixmap(QPixmap());
      }
    }
  } else {
    pTex1->setPixmap(QPixmap());
    pTex2->setPixmap(QPixmap());
  }
}

//-------------------------------------------------------------------------------------------------