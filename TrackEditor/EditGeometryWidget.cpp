#include "EditGeometryWidget.h"
#include "Track.h"
#include "Texture.h"
#include "MainWindow.h"
#include "ChunkEditValues.h"
#include "QtHelpers.h"
#include "EditSurfaceDialog.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

class CEditGeometryWidgetPrivate
{
public:
  CEditGeometryWidgetPrivate() {};
  ~CEditGeometryWidgetPrivate() {};

  CTrack *m_pTrack;
  CTexture *m_pTex;
};

//-------------------------------------------------------------------------------------------------

CEditGeometryWidget::CEditGeometryWidget(QWidget *pParent, CTrack *pTrack, CTexture *pTex)
  : QWidget(pParent)
{
  p = new CEditGeometryWidgetPrivate;
  p->m_pTrack = pTrack;
  p->m_pTex = pTex;

  setupUi(this);

  connect(g_pMainWindow, &CMainWindow::UpdateGeometrySelectionSig, this, &CEditGeometryWidget::UpdateGeometrySelection);

  
  connect(dsbYaw              , SIGNAL(valueChanged(double)), this, SLOT(YawChanged(double)));
  connect(dsbPitch            , SIGNAL(valueChanged(double)), this, SLOT(PitchChanged(double)));
  connect(dsbRoll             , SIGNAL(valueChanged(double)), this, SLOT(RollChanged(double)));
  connect(sbLength            , SIGNAL(valueChanged(int)), this, SLOT(LengthChanged(int)));
  connect(sbLLaneWidth        , SIGNAL(valueChanged(int)), this, SLOT(LLaneWidthChanged (int)));
  connect(sbRLaneWidth        , SIGNAL(valueChanged(int)), this, SLOT(RLaneWidthChanged (int)));
  connect(sbLShoulderWidth    , SIGNAL(valueChanged(int)), this, SLOT(LShoulderWidthChanged(int)));
  connect(sbRShoulderWidth    , SIGNAL(valueChanged(int)), this, SLOT(RShoulderWidthChanged(int)));
  connect(sbLShoulderHeight   , SIGNAL(valueChanged(int)), this, SLOT(LShoulderHeightChanged(int)));
  connect(sbRShoulderHeight   , SIGNAL(valueChanged(int)), this, SLOT(RShoulderHeightChanged(int)));
  connect(sbWallHeight        , SIGNAL(valueChanged(int)), this, SLOT(WallHeightChanged(int)));
  connect(sbLLOWallHOffset    , SIGNAL(valueChanged(int)), this, SLOT(LLOWallHOffsetChanged(int)));
  connect(sbLLOWallHeight     , SIGNAL(valueChanged(int)), this, SLOT(LLOWallHeightChanged(int)));
  connect(sbLUOWallHOffset    , SIGNAL(valueChanged(int)), this, SLOT(LUOWallHOffsetChanged(int)));
  connect(sbLUOWallHeight     , SIGNAL(valueChanged(int)), this, SLOT(LUOWallHeightChanged(int)));
  connect(sbRLOWallHOffset    , SIGNAL(valueChanged(int)), this, SLOT(RLOWallHOffsetChanged(int)));
  connect(sbRLOWallHeight     , SIGNAL(valueChanged(int)), this, SLOT(RLOWallHeightChanged(int)));
  connect(sbRUOWallHOffset    , SIGNAL(valueChanged(int)), this, SLOT(RUOWallHOffsetChanged(int)));
  connect(sbRUOWallHeight     , SIGNAL(valueChanged(int)), this, SLOT(RUOWallHeightChanged(int)));
  connect(sbLOuterFloorHOffset, SIGNAL(valueChanged(int)), this, SLOT(LOuterFloorHOffsetChanged(int)));
  connect(sbROuterFloorHOffset, SIGNAL(valueChanged(int)), this, SLOT(ROuterFloorHOffsetChanged(int)));
  connect(sbLOuterFloorHeight , SIGNAL(valueChanged(int)), this, SLOT(LOuterFloorHeightChanged(int)));
  connect(sbROuterFloorHeight , SIGNAL(valueChanged(int)), this, SLOT(ROuterFloorHeightChanged(int)));
  connect(sbAILine1           , SIGNAL(valueChanged(int)), this, SLOT(AILine1Changed(int)));
  connect(sbAILine2           , SIGNAL(valueChanged(int)), this, SLOT(AILine2Changed(int)));
  connect(sbAILine3           , SIGNAL(valueChanged(int)), this, SLOT(AILine3Changed(int)));
  connect(sbAILine4           , SIGNAL(valueChanged(int)), this, SLOT(AILine4Changed(int)));

  connect(pbEditCenter,     &QPushButton::clicked, this, &CEditGeometryWidget::EditCSurface);
  connect(pbEditLShoulder,  &QPushButton::clicked, this, &CEditGeometryWidget::EditLShoulder);
  connect(pbEditRShoulder,  &QPushButton::clicked, this, &CEditGeometryWidget::EditRShoulder);
  connect(pbEditLWall,      &QPushButton::clicked, this, &CEditGeometryWidget::EditLWall);
  connect(pbEditRWall,      &QPushButton::clicked, this, &CEditGeometryWidget::EditRWall);
  connect(pbEditBack,       &QPushButton::clicked, this, &CEditGeometryWidget::EditBack);
  connect(pbEditRoofTex,    &QPushButton::clicked, this, &CEditGeometryWidget::EditRoof);
  connect(pbEditLUOWall,    &QPushButton::clicked, this, &CEditGeometryWidget::EditLUOWall);
  connect(pbEditLLOWall,    &QPushButton::clicked, this, &CEditGeometryWidget::EditLLOWall);
  connect(pbEditRLOWall,    &QPushButton::clicked, this, &CEditGeometryWidget::EditRLOWall);
  connect(pbEditRUOWall,    &QPushButton::clicked, this, &CEditGeometryWidget::EditRUOWall);
  connect(pbEditOFloor,     &QPushButton::clicked, this, &CEditGeometryWidget::EditOFloor);

  connect(sldCGrip        , SIGNAL(valueChanged(int)), this, SLOT(CGripChanged(int)));
  connect(sldLShoulderGrip, SIGNAL(valueChanged(int)), this, SLOT(LGripChanged(int)));
  connect(sldRShoulderGrip, SIGNAL(valueChanged(int)), this, SLOT(RGripChanged(int)));
  connect(sldAISpeed,       SIGNAL(valueChanged(int)), this, SLOT(AISpeedChanged(int)));
}

//-------------------------------------------------------------------------------------------------

CEditGeometryWidget::~CEditGeometryWidget()
{

}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::UpdateGeometrySelection(int iFrom, int iTo)
{
  (void)(iTo);
  if (!p->m_pTrack || iFrom >= (int)p->m_pTrack->m_chunkAy.size())
    return;
  dsbYaw              ->blockSignals(true);
  dsbPitch            ->blockSignals(true);
  dsbRoll             ->blockSignals(true);
  sbLength            ->blockSignals(true);
  sbLLaneWidth        ->blockSignals(true);
  sbRLaneWidth        ->blockSignals(true);
  sbLShoulderWidth    ->blockSignals(true);
  sbRShoulderWidth    ->blockSignals(true);
  sbLShoulderHeight   ->blockSignals(true);
  sbRShoulderHeight   ->blockSignals(true);
  sbWallHeight        ->blockSignals(true);
  sbLLOWallHOffset    ->blockSignals(true);
  sbLLOWallHeight     ->blockSignals(true);
  sbLUOWallHOffset    ->blockSignals(true);
  sbLUOWallHeight     ->blockSignals(true);
  sbRLOWallHOffset    ->blockSignals(true);
  sbRLOWallHeight     ->blockSignals(true);
  sbRUOWallHOffset    ->blockSignals(true);
  sbRUOWallHeight     ->blockSignals(true);
  sbLOuterFloorHOffset->blockSignals(true);
  sbROuterFloorHOffset->blockSignals(true);
  sbLOuterFloorHeight ->blockSignals(true);
  sbROuterFloorHeight ->blockSignals(true);
  sbAILine1           ->blockSignals(true);
  sbAILine2           ->blockSignals(true);
  sbAILine3           ->blockSignals(true);
  sbAILine4           ->blockSignals(true);
  sldCGrip            ->blockSignals(true);
  sldLShoulderGrip    ->blockSignals(true);
  sldRShoulderGrip    ->blockSignals(true);
  sldAISpeed          ->blockSignals(true);
  dsbYaw->setValue(               p->m_pTrack->m_chunkAy[iFrom].dYaw);
  dsbPitch->setValue(             p->m_pTrack->m_chunkAy[iFrom].dPitch);
  dsbRoll->setValue(              p->m_pTrack->m_chunkAy[iFrom].dRoll);
  sbLength->setValue(             p->m_pTrack->m_chunkAy[iFrom].iLength);
  sbLLaneWidth->setValue(         p->m_pTrack->m_chunkAy[iFrom].iLeftLaneWidth);
  sbRLaneWidth->setValue(         p->m_pTrack->m_chunkAy[iFrom].iRightLaneWidth);
  sbLShoulderWidth->setValue(     p->m_pTrack->m_chunkAy[iFrom].iLeftShoulderWidth);
  sbRShoulderWidth->setValue(     p->m_pTrack->m_chunkAy[iFrom].iRightShoulderWidth);
  sbLShoulderHeight->setValue(    p->m_pTrack->m_chunkAy[iFrom].iLeftShoulderHeight);
  sbRShoulderHeight->setValue(    p->m_pTrack->m_chunkAy[iFrom].iRightShoulderHeight);
  sbWallHeight->setValue(         p->m_pTrack->m_chunkAy[iFrom].iRoofHeight);
  sbLLOWallHOffset->setValue(     p->m_pTrack->m_chunkAy[iFrom].iLLOuterWallHOffset);
  sbLLOWallHeight->setValue(      p->m_pTrack->m_chunkAy[iFrom].iLLOuterWallHeight);
  sbLUOWallHOffset->setValue(     p->m_pTrack->m_chunkAy[iFrom].iLUOuterWallHOffset);
  sbLUOWallHeight->setValue(      p->m_pTrack->m_chunkAy[iFrom].iLUOuterWallHeight);
  sbRLOWallHOffset->setValue(     p->m_pTrack->m_chunkAy[iFrom].iRLOuterWallHOffset);
  sbRLOWallHeight->setValue(      p->m_pTrack->m_chunkAy[iFrom].iRLOuterWallHeight);
  sbRUOWallHOffset->setValue(     p->m_pTrack->m_chunkAy[iFrom].iRUOuterWallHOffset);
  sbRUOWallHeight->setValue(      p->m_pTrack->m_chunkAy[iFrom].iRUOuterWallHeight);
  sbLOuterFloorHOffset->setValue( p->m_pTrack->m_chunkAy[iFrom].iLOuterFloorHOffset);
  sbROuterFloorHOffset->setValue( p->m_pTrack->m_chunkAy[iFrom].iROuterFloorHOffset);
  sbLOuterFloorHeight->setValue(  p->m_pTrack->m_chunkAy[iFrom].iLOuterFloorHeight);
  sbROuterFloorHeight->setValue(  p->m_pTrack->m_chunkAy[iFrom].iROuterFloorHeight);
  sbAILine1->setValue(            p->m_pTrack->m_chunkAy[iFrom].iAILine1);
  sbAILine2->setValue(            p->m_pTrack->m_chunkAy[iFrom].iAILine2);
  sbAILine3->setValue(            p->m_pTrack->m_chunkAy[iFrom].iAILine3);
  sbAILine4->setValue(            p->m_pTrack->m_chunkAy[iFrom].iAILine4);
  sldCGrip->setValue(             p->m_pTrack->m_chunkAy[iFrom].iTrackGrip);
  sldLShoulderGrip->setValue(     p->m_pTrack->m_chunkAy[iFrom].iLeftShoulderGrip);
  sldRShoulderGrip->setValue(     p->m_pTrack->m_chunkAy[iFrom].iRightShoulderGrip);
  sldAISpeed->setValue(           p->m_pTrack->m_chunkAy[iFrom].iAIMaxSpeed / 10);
  dsbYaw              ->blockSignals(false);
  dsbPitch            ->blockSignals(false);
  dsbRoll             ->blockSignals(false);
  sbLength            ->blockSignals(false);
  sbLLaneWidth        ->blockSignals(false);
  sbRLaneWidth        ->blockSignals(false);
  sbLShoulderWidth    ->blockSignals(false);
  sbRShoulderWidth    ->blockSignals(false);
  sbLShoulderHeight   ->blockSignals(false);
  sbRShoulderHeight   ->blockSignals(false);
  sbWallHeight        ->blockSignals(false);
  sbLLOWallHOffset    ->blockSignals(false);
  sbLLOWallHeight     ->blockSignals(false);
  sbLUOWallHOffset    ->blockSignals(false);
  sbLUOWallHeight     ->blockSignals(false);
  sbRLOWallHOffset    ->blockSignals(false);
  sbRLOWallHeight     ->blockSignals(false);
  sbRUOWallHOffset    ->blockSignals(false);
  sbRUOWallHeight     ->blockSignals(false);
  sbLOuterFloorHOffset->blockSignals(false);
  sbROuterFloorHOffset->blockSignals(false);
  sbLOuterFloorHeight ->blockSignals(false);
  sbROuterFloorHeight ->blockSignals(false);
  sbAILine1           ->blockSignals(false);
  sbAILine2           ->blockSignals(false);
  sbAILine3           ->blockSignals(false);
  sbAILine4           ->blockSignals(false);
  sldCGrip            ->blockSignals(false);
  sldLShoulderGrip    ->blockSignals(false);
  sldRShoulderGrip    ->blockSignals(false);
  sldAISpeed          ->blockSignals(false);

  lblCGrip->setText(        "(" + QString::number(p->m_pTrack->m_chunkAy[iFrom].iTrackGrip) + ")");
  lblLShoulderGrip->setText("(" + QString::number(p->m_pTrack->m_chunkAy[iFrom].iLeftShoulderGrip) + ")");
  lblRShoulderGrip->setText("(" + QString::number(p->m_pTrack->m_chunkAy[iFrom].iRightShoulderGrip) + ")");
  lblAISpeed->setText(      "(" + QString::number(p->m_pTrack->m_chunkAy[iFrom].iAIMaxSpeed / 10) + "%)");

  QtHelpers::UpdateTextures(lblCTex1, lblCTex2, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iCenterSurfaceType);
  QtHelpers::UpdateTextures(lblLShoulderTex1, lblLShoulderTex2, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iLeftSurfaceType);
  QtHelpers::UpdateTextures(lblRShoulderTex1, lblRShoulderTex2, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iRightSurfaceType);
  QtHelpers::UpdateTextures(lblLWallTex1, lblLWallTex2, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iLeftWallType);
  QtHelpers::UpdateTextures(lblRWallTex1, lblRWallTex2, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iRightWallType);
  QtHelpers::UpdateTextures(lblBackTex1, lblBackTex2, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iBackTexture);
  QtHelpers::UpdateTextures(lblRoofTex1, lblRoofTex2, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iRoofType);
  QtHelpers::UpdateTextures(lblLLOWallTex1, lblLLOWallTex2, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iLLOuterWallType);
  QtHelpers::UpdateTextures(lblLUOWallTex1, lblLUOWallTex2, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iLUOuterWallType);
  QtHelpers::UpdateTextures(lblRLOWallTex1, lblRLOWallTex2, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iRLOuterWallType);
  QtHelpers::UpdateTextures(lblRUOWallTex1, lblRUOWallTex2, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iRUOuterWallType);
  QtHelpers::UpdateTextures(lblOFloorTex1, lblOFloorTex2, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iOuterFloorType);
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::YawChanged(double dValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].dYaw = dValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::PitchChanged(double dValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].dPitch = dValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RollChanged(double dValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].dRoll = dValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LengthChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLength = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LLaneWidthChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLeftLaneWidth = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RLaneWidthChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRightLaneWidth = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LShoulderWidthChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLeftShoulderWidth = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RShoulderWidthChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRightShoulderWidth = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LShoulderHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLeftShoulderHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RShoulderHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRightShoulderHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::WallHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRoofHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LLOWallHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLLOuterWallHOffset = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LLOWallHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLLOuterWallHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LUOWallHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLUOuterWallHOffset = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LUOWallHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLUOuterWallHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RLOWallHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRLOuterWallHOffset = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RLOWallHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRLOuterWallHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RUOWallHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRUOuterWallHOffset = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RUOWallHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRUOuterWallHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LOuterFloorHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLOuterFloorHOffset = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::ROuterFloorHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iROuterFloorHOffset = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LOuterFloorHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLOuterFloorHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::ROuterFloorHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iROuterFloorHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AILine1Changed(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iAILine1 = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AILine2Changed(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iAILine2 = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AILine3Changed(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iAILine3 = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AILine4Changed(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iAILine4 = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditCSurface()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();
  
  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iCenterSurfaceType);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iCenterSurfaceType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditLShoulder()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iLeftSurfaceType);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iLeftSurfaceType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditRShoulder()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iRightSurfaceType);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iRightSurfaceType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditLWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iLeftWallType);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iLeftWallType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditRWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iRightWallType);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iRightWallType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditBack()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iBackTexture);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iBackTexture = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditRoof()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iRoofType);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iRoofType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditLUOWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iLUOuterWallType);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iLUOuterWallType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditLLOWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iLLOuterWallType);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iLLOuterWallType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditRLOWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iRLOuterWallType);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iRLOuterWallType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditRUOWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iRUOuterWallType);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iRUOuterWallType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditOFloor()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pTrack->m_chunkAy[iFrom].iOuterFloorType);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iOuterFloorType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::CGripChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iTrackGrip = iValue;
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LGripChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLeftShoulderGrip = iValue;
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RGripChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRightShoulderGrip = iValue;
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AISpeedChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iAIMaxSpeed = iValue * 10;
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------