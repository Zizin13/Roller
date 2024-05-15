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
  connect(sbChunkCount    , SIGNAL(valueChanged(int)), this, SLOT(ChunkCountChanged(int)));
  connect(sbNumTicks      , SIGNAL(valueChanged(int)), this, SLOT(NumTicksChanged(int)));
  connect(sbTickStartIdx  , SIGNAL(valueChanged(int)), this, SLOT(TickStartIdxChanged(int)));
  connect(sbTimingGroup   , SIGNAL(valueChanged(int)), this, SLOT(TimingGroupChanged(int)));
  connect(sbHeight        , SIGNAL(valueChanged(int)), this, SLOT(HeightChanged(int)));
  connect(sbTimeBulging   , SIGNAL(valueChanged(int)), this, SLOT(TimeBulgingChanged(int)));
  connect(sbTimeFlat      , SIGNAL(valueChanged(int)), this, SLOT(TimeFlatChanged(int)));
  connect(sldLength       , SIGNAL(valueChanged(int)), this, SLOT(LengthChanged(int)));
  connect(ck0LShoulder    , SIGNAL(toggled(bool)),     this, SLOT(FlagsChanged()));
  connect(ck1LWall        , SIGNAL(toggled(bool)), this, SLOT(FlagsChanged()));
  connect(ck2LLane        , SIGNAL(toggled(bool)), this, SLOT(FlagsChanged()));
  connect(ck3RLane        , SIGNAL(toggled(bool)), this, SLOT(FlagsChanged()));
  connect(ck4RShoulder    , SIGNAL(toggled(bool)), this, SLOT(FlagsChanged()));
  connect(ck5RWall             , SIGNAL(toggled(bool)), this, SLOT(FlagsChanged()));

  UpdateDialog();
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

  bool bChunkHasStunt = g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iChunkCount != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iNumTicks != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTickStartIdx != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimingGroup != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iHeight != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeBulging != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeFlat != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iRampSideLength != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iFlags != 0;

  pbStunt->setText(bChunkHasStunt ? "Delete Stunt" : "Add Stunt");
  sbChunkCount    ->setEnabled(bChunkHasStunt);
  sbNumTicks      ->setEnabled(bChunkHasStunt);
  sbTickStartIdx  ->setEnabled(bChunkHasStunt);
  sbTimingGroup   ->setEnabled(bChunkHasStunt);
  sbHeight        ->setEnabled(bChunkHasStunt);
  sbTimeBulging   ->setEnabled(bChunkHasStunt);
  sbTimeFlat      ->setEnabled(bChunkHasStunt);
  sldLength       ->setEnabled(bChunkHasStunt);
  ck0LShoulder    ->setEnabled(bChunkHasStunt);
  ck1LWall        ->setEnabled(bChunkHasStunt);
  ck2LLane        ->setEnabled(bChunkHasStunt);
  ck3RLane        ->setEnabled(bChunkHasStunt);
  ck4RShoulder    ->setEnabled(bChunkHasStunt);
  ck5RWall        ->setEnabled(bChunkHasStunt);
    
  BLOCK_SIG_AND_DO(sbChunkCount    , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iChunkCount));
  BLOCK_SIG_AND_DO(sbNumTicks      , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iNumTicks));
  BLOCK_SIG_AND_DO(sbTickStartIdx  , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTickStartIdx));
  BLOCK_SIG_AND_DO(sbTimingGroup   , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimingGroup));
  BLOCK_SIG_AND_DO(sbHeight        , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iHeight));
  BLOCK_SIG_AND_DO(sbTimeBulging   , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeBulging));
  BLOCK_SIG_AND_DO(sbTimeFlat      , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeFlat));
  BLOCK_SIG_AND_DO(sldLength       , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iRampSideLength * 100 / 1024));
  BLOCK_SIG_AND_DO(ck0LShoulder    , setChecked(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iFlags & STUNT_FLAG_LSHOULDER));
  BLOCK_SIG_AND_DO(ck1LWall        , setChecked(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iFlags & STUNT_FLAG_LWALL));
  BLOCK_SIG_AND_DO(ck2LLane        , setChecked(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iFlags & STUNT_FLAG_LLANE));
  BLOCK_SIG_AND_DO(ck3RLane        , setChecked(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iFlags & STUNT_FLAG_RLANE));
  BLOCK_SIG_AND_DO(ck4RShoulder    , setChecked(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iFlags & STUNT_FLAG_RSHOULDER));
  BLOCK_SIG_AND_DO(ck5RWall        , setChecked(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iFlags & STUNT_FLAG_RWALL));

  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::ChunkCountChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].stunt.iChunkCount = iVal;
  }

  g_pMainWindow->SaveHistory("Changed stunt chunk count");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::NumTicksChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].stunt.iNumTicks = iVal;
  }

  g_pMainWindow->SaveHistory("Changed stunt num ticks");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::TickStartIdxChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].stunt.iTickStartIdx = iVal;
  }

  g_pMainWindow->SaveHistory("Changed stunt tick start idx");
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

  g_pMainWindow->SaveHistory("Changed stunt time flat");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::LengthChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].stunt.iRampSideLength = iVal * 1024 / 100;
  }

  g_pMainWindow->SaveHistory("Changed stunt ramp length");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::FlagsChanged()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  uint8 byFlags = 0;
  if (ck0LShoulder->isChecked())
    byFlags |= STUNT_FLAG_LSHOULDER;
  if (ck1LWall->isChecked())
    byFlags |= STUNT_FLAG_LWALL;
  if (ck2LLane->isChecked())
    byFlags |= STUNT_FLAG_LLANE;
  if (ck3RLane->isChecked())
    byFlags |= STUNT_FLAG_RLANE;
  if (ck4RShoulder->isChecked())
    byFlags |= STUNT_FLAG_RSHOULDER;
  if (ck5RWall->isChecked())
    byFlags |= STUNT_FLAG_RWALL;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].stunt.iFlags = (int)byFlags;
  }

  g_pMainWindow->SaveHistory("Changed stunt flags");
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

  bool bChunkHasStunt = g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iChunkCount != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iNumTicks != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTickStartIdx != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimingGroup != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iHeight != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeBulging != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeFlat != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iRampSideLength != 0
    || g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iFlags != 0;
  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iChunkCount = bChunkHasStunt ? 0 : 1;
    if (bChunkHasStunt) {
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iNumTicks = 0;
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTickStartIdx = 0;
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimingGroup = 0;
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iHeight = 0;
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeBulging = 0;
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iTimeFlat = 0;
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iRampSideLength = 0;
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iFlags = 0;;
    }
  }

  g_pMainWindow->SaveHistory(bChunkHasStunt ? "Removed stunt" : "Added stunt");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::UpdateDialog()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  int iValue = g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].stunt.iFlags;
  uint32 uiSignedBitValue = CTrackData::GetSignedBitValueFromInt(iValue);
  char szBuf[128];
  snprintf(szBuf, sizeof(szBuf), " (%#010x)", uiSignedBitValue);

  leFlags->setFont(QFont("Courier", 8));
  leFlags->setText(QString::number(iValue).leftJustified(10, ' ') + szBuf);

  sbTickStartIdx->setRange(0, sbNumTicks->value() - 1);
}

//-------------------------------------------------------------------------------------------------