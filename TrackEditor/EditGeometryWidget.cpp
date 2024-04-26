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


  connect(dsbYaw              , SIGNAL(valueChanged(double)), this, SLOT(ValuesChanged()));
  connect(dsbPitch            , SIGNAL(valueChanged(double)), this, SLOT(ValuesChanged()));
  connect(dsbRoll             , SIGNAL(valueChanged(double)), this, SLOT(ValuesChanged()));
  connect(sbLength            , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbLLaneWidth        , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbRLaneWidth        , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbLShoulderWidth    , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbRShoulderWidth    , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbLShoulderHeight   , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbRShoulderHeight   , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbWallHeight        , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbLLOWallHOffset    , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbLLOWallHeight     , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbLUOWallHOffset    , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbLUOWallHeight     , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbRLOWallHOffset    , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbRLOWallHeight     , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbRUOWallHOffset    , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbRUOWallHeight     , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbLOuterFloorHOffset, SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbROuterFloorHOffset, SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbLOuterFloorHeight , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
  connect(sbROuterFloorHeight , SIGNAL(valueChanged(int)), this, SLOT(ValuesChanged()));
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
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::ValuesChanged()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack 
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].dYaw                = dsbYaw              ->value();
    p->m_pTrack->m_chunkAy[i].dPitch              = dsbPitch            ->value();
    p->m_pTrack->m_chunkAy[i].dRoll               = dsbRoll             ->value();
    p->m_pTrack->m_chunkAy[i].iLength             = sbLength            ->value();
    p->m_pTrack->m_chunkAy[i].iLeftLaneWidth      = sbLLaneWidth        ->value();
    p->m_pTrack->m_chunkAy[i].iRightLaneWidth     = sbRLaneWidth        ->value();
    p->m_pTrack->m_chunkAy[i].iLeftShoulderWidth  = sbLShoulderWidth    ->value();
    p->m_pTrack->m_chunkAy[i].iRightShoulderWidth = sbRShoulderWidth    ->value();
    p->m_pTrack->m_chunkAy[i].iLeftShoulderHeight = sbLShoulderHeight   ->value();
    p->m_pTrack->m_chunkAy[i].iRightShoulderHeight= sbRShoulderHeight   ->value();
    p->m_pTrack->m_chunkAy[i].iRoofHeight         = sbWallHeight        ->value();
    p->m_pTrack->m_chunkAy[i].iLLOuterWallHOffset = sbLLOWallHOffset    ->value();
    p->m_pTrack->m_chunkAy[i].iLLOuterWallHeight  = sbLLOWallHeight     ->value();
    p->m_pTrack->m_chunkAy[i].iLUOuterWallHOffset = sbLUOWallHOffset    ->value();
    p->m_pTrack->m_chunkAy[i].iLUOuterWallHeight  = sbLUOWallHeight     ->value();
    p->m_pTrack->m_chunkAy[i].iRLOuterWallHOffset = sbRLOWallHOffset    ->value();
    p->m_pTrack->m_chunkAy[i].iRLOuterWallHeight  = sbRLOWallHeight     ->value();
    p->m_pTrack->m_chunkAy[i].iRUOuterWallHOffset = sbRUOWallHOffset    ->value();
    p->m_pTrack->m_chunkAy[i].iRUOuterWallHeight  = sbRUOWallHeight     ->value();
    p->m_pTrack->m_chunkAy[i].iLOuterFloorHOffset = sbLOuterFloorHOffset->value();
    p->m_pTrack->m_chunkAy[i].iROuterFloorHOffset = sbROuterFloorHOffset->value();
    p->m_pTrack->m_chunkAy[i].iLOuterFloorHeight  = sbLOuterFloorHeight ->value();
    p->m_pTrack->m_chunkAy[i].iROuterFloorHeight  = sbROuterFloorHeight ->value();
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------