#include "EditGeometryWidget.h"
#include "Track.h"
#include "MainWindow.h"
#include "ChunkEditValues.h"
#include "QtHelpers.h"
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
};

//-------------------------------------------------------------------------------------------------

CEditGeometryWidget::CEditGeometryWidget(QWidget *pParent, CTrack *pTrack)
  : QWidget(pParent)
{
  p = new CEditGeometryWidgetPrivate;
  p->m_pTrack = pTrack;

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