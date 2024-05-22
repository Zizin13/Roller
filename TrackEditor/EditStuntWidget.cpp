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
  connect(rbGroup1        , SIGNAL(toggled(bool))    , this, SLOT(TimingGroupChanged()));
  connect(rbGroup2        , SIGNAL(toggled(bool))    , this, SLOT(TimingGroupChanged()));
  connect(sbHeight        , SIGNAL(valueChanged(int)), this, SLOT(HeightChanged(int)));
  connect(sbTimeBulging   , SIGNAL(valueChanged(int)), this, SLOT(TimeBulgingChanged(int)));
  connect(sbTimeFlat      , SIGNAL(valueChanged(int)), this, SLOT(TimeFlatChanged(int)));
  connect(sldLength       , SIGNAL(valueChanged(int)), this, SLOT(LengthChanged(int)));
  connect(ck0LShoulder    , SIGNAL(toggled(bool))    , this, SLOT(FlagsChanged()));
  connect(ck1LWall        , SIGNAL(toggled(bool))    , this, SLOT(FlagsChanged()));
  connect(ck2LLane        , SIGNAL(toggled(bool))    , this, SLOT(FlagsChanged()));
  connect(ck3RLane        , SIGNAL(toggled(bool))    , this, SLOT(FlagsChanged()));
  connect(ck4RShoulder    , SIGNAL(toggled(bool))    , this, SLOT(FlagsChanged()));
  connect(ck5RWall        , SIGNAL(toggled(bool))    , this, SLOT(FlagsChanged()));

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
  if (!g_pMainWindow->GetCurrentTrack() || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  tStunt stunt;
  bool bChunkHasStunt = g_pMainWindow->GetCurrentTrack()->GetStunt(iFrom, stunt);

  pbStunt->setText(bChunkHasStunt ? "Delete Stunt" : "Add Stunt");
  sbChunkCount    ->setEnabled(bChunkHasStunt);
  sbNumTicks      ->setEnabled(bChunkHasStunt);
  sbTickStartIdx  ->setEnabled(bChunkHasStunt);
  rbGroup1        ->setEnabled(bChunkHasStunt);
  rbGroup2        ->setEnabled(bChunkHasStunt);
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

  if (bChunkHasStunt) {
    int iChunksAfter = g_pMainWindow->GetCurrentTrack()->GetChunkCount() - iFrom - 2;
    BLOCK_SIG_AND_DO(sbChunkCount, setRange(0, std::min(iFrom, iChunksAfter)));
    BLOCK_SIG_AND_DO(sbChunkCount, setValue(stunt.iChunkCount));
    BLOCK_SIG_AND_DO(sbNumTicks, setValue(stunt.iNumTicks));
    BLOCK_SIG_AND_DO(sbTickStartIdx, setRange(0, sbNumTicks->value() - 1));
    BLOCK_SIG_AND_DO(sbTickStartIdx, setValue(stunt.iTickStartIdx));
    BLOCK_SIG_AND_DO(rbGroup1, setChecked(stunt.iTimingGroup == 1));
    BLOCK_SIG_AND_DO(rbGroup2, setChecked(stunt.iTimingGroup != 1));
    BLOCK_SIG_AND_DO(sbHeight, setValue(stunt.iHeight));
    BLOCK_SIG_AND_DO(sbTimeBulging, setValue(stunt.iTimeBulging));
    BLOCK_SIG_AND_DO(sbTimeFlat, setValue(stunt.iTimeFlat));
    BLOCK_SIG_AND_DO(sldLength, setValue(stunt.iRampSideLength * 100 / STUNT_LENGTH_100_PERCENT));
    BLOCK_SIG_AND_DO(ck0LShoulder, setChecked(stunt.iFlags & STUNT_FLAG_LSHOULDER));
    BLOCK_SIG_AND_DO(ck1LWall, setChecked(stunt.iFlags & STUNT_FLAG_LWALL));
    BLOCK_SIG_AND_DO(ck2LLane, setChecked(stunt.iFlags & STUNT_FLAG_LLANE));
    BLOCK_SIG_AND_DO(ck3RLane, setChecked(stunt.iFlags & STUNT_FLAG_RLANE));
    BLOCK_SIG_AND_DO(ck4RShoulder, setChecked(stunt.iFlags & STUNT_FLAG_RSHOULDER));
    BLOCK_SIG_AND_DO(ck5RWall, setChecked(stunt.iFlags & STUNT_FLAG_RWALL));
  }

  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::ChunkCountChanged(int iVal)
{
  if (!g_pMainWindow->GetCurrentTrack())
    return;

  int iFrom = g_pMainWindow->GetSelFrom();

  tStunt stunt;
  if (!g_pMainWindow->GetCurrentTrack()->GetStunt(iFrom, stunt))
    return;

  stunt.iChunkCount = iVal;
  g_pMainWindow->GetCurrentTrack()->SetStunt(iFrom, stunt);

  g_pMainWindow->SaveHistory("Changed stunt chunk count");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::NumTicksChanged(int iVal)
{
  if (!g_pMainWindow->GetCurrentTrack())
    return;

  int iFrom = g_pMainWindow->GetSelFrom();

  tStunt stunt;
  if (!g_pMainWindow->GetCurrentTrack()->GetStunt(iFrom, stunt))
    return;

  stunt.iNumTicks = iVal;
  g_pMainWindow->GetCurrentTrack()->SetStunt(iFrom, stunt);

  g_pMainWindow->SaveHistory("Changed stunt num ticks");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::TickStartIdxChanged(int iVal)
{
  if (!g_pMainWindow->GetCurrentTrack())
    return;

  int iFrom = g_pMainWindow->GetSelFrom();

  tStunt stunt;
  if (!g_pMainWindow->GetCurrentTrack()->GetStunt(iFrom, stunt))
    return;

  stunt.iTickStartIdx = iVal;
  g_pMainWindow->GetCurrentTrack()->SetStunt(iFrom, stunt);

  g_pMainWindow->SaveHistory("Changed stunt tick start idx");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::Group1Toggled(bool bChecked)
{
  if (!g_pMainWindow->GetCurrentTrack())
    return;

  int iFrom = g_pMainWindow->GetSelFrom();

  tStunt stunt;
  if (!g_pMainWindow->GetCurrentTrack()->GetStunt(iFrom, stunt))
    return;

  stunt.iTimingGroup = bChecked ? 1 : 0;
  g_pMainWindow->GetCurrentTrack()->SetStunt(iFrom, stunt);

  g_pMainWindow->SaveHistory("Changed stunt timing group");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::Group2Toggled(bool bChecked)
{
  if (!g_pMainWindow->GetCurrentTrack())
    return;

  int iFrom = g_pMainWindow->GetSelFrom();

  tStunt stunt;
  if (!g_pMainWindow->GetCurrentTrack()->GetStunt(iFrom, stunt))
    return;

  stunt.iTimingGroup = bChecked ? 0 : 1;
  g_pMainWindow->GetCurrentTrack()->SetStunt(iFrom, stunt);

  g_pMainWindow->SaveHistory("Changed stunt timing group");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::HeightChanged(int iVal)
{
  if (!g_pMainWindow->GetCurrentTrack())
    return;

  int iFrom = g_pMainWindow->GetSelFrom();

  tStunt stunt;
  if (!g_pMainWindow->GetCurrentTrack()->GetStunt(iFrom, stunt))
    return;

  stunt.iHeight = iVal;
  g_pMainWindow->GetCurrentTrack()->SetStunt(iFrom, stunt);

  g_pMainWindow->SaveHistory("Changed stunt height");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::TimeBulgingChanged(int iVal)
{
  if (!g_pMainWindow->GetCurrentTrack())
    return;

  int iFrom = g_pMainWindow->GetSelFrom();

  tStunt stunt;
  if (!g_pMainWindow->GetCurrentTrack()->GetStunt(iFrom, stunt))
    return;

  stunt.iTimeBulging = iVal;
  g_pMainWindow->GetCurrentTrack()->SetStunt(iFrom, stunt);

  g_pMainWindow->SaveHistory("Changed stunt time bulging");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::TimeFlatChanged(int iVal)
{
  if (!g_pMainWindow->GetCurrentTrack())
    return;

  int iFrom = g_pMainWindow->GetSelFrom();

  tStunt stunt;
  if (!g_pMainWindow->GetCurrentTrack()->GetStunt(iFrom, stunt))
    return;

  stunt.iTimeFlat = iVal;
  g_pMainWindow->GetCurrentTrack()->SetStunt(iFrom, stunt);

  g_pMainWindow->SaveHistory("Changed stunt time flat");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::LengthChanged(int iVal)
{
  if (!g_pMainWindow->GetCurrentTrack())
    return;

  int iFrom = g_pMainWindow->GetSelFrom();

  tStunt stunt;
  if (!g_pMainWindow->GetCurrentTrack()->GetStunt(iFrom, stunt))
    return;

  stunt.iRampSideLength = iVal * STUNT_LENGTH_100_PERCENT / 100;
  g_pMainWindow->GetCurrentTrack()->SetStunt(iFrom, stunt);

  g_pMainWindow->SaveHistory("Changed stunt ramp length");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::FlagsChanged()
{
  if (!g_pMainWindow->GetCurrentTrack())
    return;

  int iFrom = g_pMainWindow->GetSelFrom();

  tStunt stunt;
  if (!g_pMainWindow->GetCurrentTrack()->GetStunt(iFrom, stunt))
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

  stunt.iFlags = (int)byFlags;
  g_pMainWindow->GetCurrentTrack()->SetStunt(iFrom, stunt);

  g_pMainWindow->SaveHistory("Changed stunt flags");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::StuntClicked()
{
  if (!g_pMainWindow->GetCurrentTrack())
    return;

  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  tStunt stunt;
  bool bChunkHasStunt = g_pMainWindow->GetCurrentTrack()->GetStunt(iFrom, stunt);

  if (!bChunkHasStunt) {
    int iAllFlags = (STUNT_FLAG_LLANE | STUNT_FLAG_RLANE | STUNT_FLAG_LWALL | STUNT_FLAG_RWALL | STUNT_FLAG_LSHOULDER | STUNT_FLAG_RSHOULDER);
    stunt.iChunkCount = bChunkHasStunt ? 0 : 1;
    stunt.iNumTicks = bChunkHasStunt ? 0 : 30;
    stunt.iTickStartIdx = 0;
    stunt.iTimingGroup = bChunkHasStunt ? 0 : 1;
    stunt.iHeight = bChunkHasStunt ? 0 : 10;
    stunt.iTimeBulging = bChunkHasStunt ? 0 : 30;
    stunt.iTimeFlat = bChunkHasStunt ? 0 : 30;
    stunt.iRampSideLength = bChunkHasStunt ? 0 : STUNT_LENGTH_100_PERCENT;
    stunt.iFlags = bChunkHasStunt ? 0 : iAllFlags;
    g_pMainWindow->GetCurrentTrack()->SetStunt(iFrom, stunt);
  } else {
    g_pMainWindow->GetCurrentTrack()->EraseStunt(iFrom);
  }

  g_pMainWindow->SaveHistory(bChunkHasStunt ? "Removed stunt" : "Added stunt");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::UpdateDialog()
{
  if (!g_pMainWindow->GetCurrentTrack())
    return;

  int iFrom = g_pMainWindow->GetSelFrom();
  tStunt stunt;
  if (!g_pMainWindow->GetCurrentTrack()->GetStunt(iFrom, stunt))
    return;

  int iValue = stunt.iFlags;
  uint32 uiSignedBitValue = CTrackData::GetSignedBitValueFromInt(iValue);
  char szBuf[128];
  snprintf(szBuf, sizeof(szBuf), " (%#010x)", uiSignedBitValue);

  leFlags->setFont(QFont("Courier", 8));
  leFlags->setText(QString::number(iValue).leftJustified(4, ' ') + szBuf);

  lblLengthPercent->setText("(" + QString::number(stunt.iRampSideLength * 100 / STUNT_LENGTH_100_PERCENT) + "%)");
  lblTicksTimeS->setText("(" + QString::number(stunt.iNumTicks * 2.768 / 100, 'f', 2) + " s)");
  lblBulgeTimeS->setText("(" + QString::number(stunt.iTimeBulging * 2.768 / 100, 'f', 2) + " s)");
  lblFlatTimeS->setText("(" + QString::number(stunt.iTimeFlat * 2.768 / 100, 'f', 2) + " s)");
}

//-------------------------------------------------------------------------------------------------