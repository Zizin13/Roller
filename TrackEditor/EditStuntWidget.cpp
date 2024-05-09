#include "EditStuntWidget.h"
#include "Track.h"
#include "MainWindow.h"
#include "QtHelpers.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CEditStuntWidget::CEditStuntWidget(QWidget *pParent)
  : QWidget(pParent)
{
  setupUi(this);

  connect(g_pMainWindow, &CMainWindow::UpdateGeometrySelectionSig, this, &CEditStuntWidget::UpdateGeometrySelection);
  
  connect(pbStunt         , SIGNAL(clicked()),         this, SLOT(StuntClicked()));
  connect(sbScaleFact     , SIGNAL(valueChanged(int)), this, SLOT(ScaleFactChanged(int)));
  connect(sbAngle         , SIGNAL(valueChanged(int)), this, SLOT(AngleChanged(int)));
  connect(sbUnk           , SIGNAL(valueChanged(int)), this, SLOT(UnknownChanged(int)));
  connect(sbTimingGroup   , SIGNAL(valueChanged(int)), this, SLOT(TimingGroupChanged(int)));
  connect(sbHeight        , SIGNAL(valueChanged(int)), this, SLOT(HeightChanged(int)));
  connect(sbTimeBulging   , SIGNAL(valueChanged(int)), this, SLOT(TimeBulgingChanged(int)));
  connect(sbTimeFlat      , SIGNAL(valueChanged(int)), this, SLOT(TimeFlatChanged(int)));
  connect(sbExpandContract, SIGNAL(valueChanged(int)), this, SLOT(ExpandContractChanged(int)));
  connect(sbBulge         , SIGNAL(valueChanged(int)), this, SLOT(BulgeChanged(int)));
}

//-------------------------------------------------------------------------------------------------

CEditStuntWidget::~CEditStuntWidget()
{

}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::UpdateGeometrySelection(int iFrom, int iTo)
{
  (void)(iTo);
  if (!g_pMainWindow->GetCurrentTrack() || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  bool bChunkHasStunt = g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iScaleFactor != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iAngle != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iUnknown != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimingGroup != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iHeight != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeBulging != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeFlat != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iSmallerExpandsLargerContracts != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iBulge != 0;

  pbStunt->setText(bChunkHasStunt ? "Delete Stunt" : "Add Stunt");
  sbScaleFact     ->setEnabled(bChunkHasStunt);
  sbAngle         ->setEnabled(bChunkHasStunt);
  sbUnk           ->setEnabled(bChunkHasStunt);
  sbTimingGroup   ->setEnabled(bChunkHasStunt);
  sbHeight        ->setEnabled(bChunkHasStunt);
  sbTimeBulging   ->setEnabled(bChunkHasStunt);
  sbTimeFlat      ->setEnabled(bChunkHasStunt);
  sbExpandContract->setEnabled(bChunkHasStunt);
  sbBulge         ->setEnabled(bChunkHasStunt);
    
  BLOCK_SIG_AND_DO(sbScaleFact     , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iScaleFactor));
  BLOCK_SIG_AND_DO(sbAngle         , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iAngle));
  BLOCK_SIG_AND_DO(sbUnk           , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iUnknown));
  BLOCK_SIG_AND_DO(sbTimingGroup   , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimingGroup));
  BLOCK_SIG_AND_DO(sbHeight        , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iHeight));
  BLOCK_SIG_AND_DO(sbTimeBulging   , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeBulging));
  BLOCK_SIG_AND_DO(sbTimeFlat      , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeFlat));
  BLOCK_SIG_AND_DO(sbExpandContract, setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iSmallerExpandsLargerContracts));
  BLOCK_SIG_AND_DO(sbBulge         , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iBulge));
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::ScaleFactChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].stunt.iScaleFactor = iVal;
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed stunt scale factor");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::AngleChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].stunt.iAngle = iVal;
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed stunt angle");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::UnknownChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].stunt.iUnknown = iVal;
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed stunt unknown val");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::TimingGroupChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].stunt.iTimingGroup = iVal;
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed stunt timing group");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::HeightChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].stunt.iHeight = iVal;
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed stunt height");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::TimeBulgingChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].stunt.iTimeBulging = iVal;
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed stunt time bulging");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::TimeFlatChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].stunt.iTimeFlat = iVal;
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed stunt time flat");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::ExpandContractChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].stunt.iSmallerExpandsLargerContracts = iVal;
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed stunt expand/contract");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::BulgeChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].stunt.iBulge = iVal;
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed stunt bulge");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::StuntClicked()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  bool bChunkHasStunt = g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iScaleFactor != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iAngle != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iUnknown != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimingGroup != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iHeight != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeBulging != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeFlat != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iSmallerExpandsLargerContracts != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iBulge != 0;
  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iScaleFactor = bChunkHasStunt ? 0 : 1;
    if (bChunkHasStunt) {
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iAngle = 0;
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iUnknown = 0;
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimingGroup = 0;
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iHeight = 0;
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeBulging = 0;
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeFlat = 0;
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iSmallerExpandsLargerContracts = 0;
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iBulge = 0;;
    }
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory(bChunkHasStunt ? "Removed stunt" : "Added stunt");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------