#include "EditStuntWidget.h"
#include "Track.h"
#include "MainWindow.h"
#include "QtHelpers.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

class CEditStuntWidgetPrivate
{
public:
  CEditStuntWidgetPrivate() {};
  ~CEditStuntWidgetPrivate() {};

  CTrack *m_pTrack;
};

//-------------------------------------------------------------------------------------------------

CEditStuntWidget::CEditStuntWidget(QWidget *pParent, CTrack *pTrack)
  : QWidget(pParent)
{
  p = new CEditStuntWidgetPrivate;
  p->m_pTrack = pTrack;

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
  if (!p->m_pTrack || iFrom >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  bool bChunkHasStunt = p->m_pTrack->m_chunkAy[iFrom].stunt.iScaleFactor != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iAngle != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iUnknown != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iTimingGroup != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iHeight != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iTimeBulging != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iTimeFlat != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iSmallerExpandsLargerContracts != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iBulge != 0;

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
    
  BLOCK_SIG_AND_DO(sbScaleFact     , setValue(p->m_pTrack->m_chunkAy[iFrom].stunt.iScaleFactor));
  BLOCK_SIG_AND_DO(sbAngle         , setValue(p->m_pTrack->m_chunkAy[iFrom].stunt.iAngle));
  BLOCK_SIG_AND_DO(sbUnk           , setValue(p->m_pTrack->m_chunkAy[iFrom].stunt.iUnknown));
  BLOCK_SIG_AND_DO(sbTimingGroup   , setValue(p->m_pTrack->m_chunkAy[iFrom].stunt.iTimingGroup));
  BLOCK_SIG_AND_DO(sbHeight        , setValue(p->m_pTrack->m_chunkAy[iFrom].stunt.iHeight));
  BLOCK_SIG_AND_DO(sbTimeBulging   , setValue(p->m_pTrack->m_chunkAy[iFrom].stunt.iTimeBulging));
  BLOCK_SIG_AND_DO(sbTimeFlat      , setValue(p->m_pTrack->m_chunkAy[iFrom].stunt.iTimeFlat));
  BLOCK_SIG_AND_DO(sbExpandContract, setValue(p->m_pTrack->m_chunkAy[iFrom].stunt.iSmallerExpandsLargerContracts));
  BLOCK_SIG_AND_DO(sbBulge         , setValue(p->m_pTrack->m_chunkAy[iFrom].stunt.iBulge));
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::ScaleFactChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].stunt.iScaleFactor = iVal;
  }

  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::AngleChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].stunt.iAngle = iVal;
  }

  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::UnknownChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].stunt.iUnknown = iVal;
  }

  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::TimingGroupChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].stunt.iTimingGroup = iVal;
  }

  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::HeightChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].stunt.iHeight = iVal;
  }

  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::TimeBulgingChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].stunt.iTimeBulging = iVal;
  }

  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::TimeFlatChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].stunt.iTimeFlat = iVal;
  }

  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::ExpandContractChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].stunt.iSmallerExpandsLargerContracts = iVal;
  }

  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::BulgeChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].stunt.iBulge = iVal;
  }

  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditStuntWidget::StuntClicked()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  bool bChunkHasStunt = p->m_pTrack->m_chunkAy[iFrom].stunt.iScaleFactor != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iAngle != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iUnknown != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iTimingGroup != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iHeight != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iTimeBulging != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iTimeFlat != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iSmallerExpandsLargerContracts != 0
    || p->m_pTrack->m_chunkAy[iFrom].stunt.iBulge != 0;
  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[iFrom].stunt.iScaleFactor = bChunkHasStunt ? 0 : 1;
    if (bChunkHasStunt) {
      p->m_pTrack->m_chunkAy[iFrom].stunt.iAngle = 0;
      p->m_pTrack->m_chunkAy[iFrom].stunt.iUnknown = 0;
      p->m_pTrack->m_chunkAy[iFrom].stunt.iTimingGroup = 0;
      p->m_pTrack->m_chunkAy[iFrom].stunt.iHeight = 0;
      p->m_pTrack->m_chunkAy[iFrom].stunt.iTimeBulging = 0;
      p->m_pTrack->m_chunkAy[iFrom].stunt.iTimeFlat = 0;
      p->m_pTrack->m_chunkAy[iFrom].stunt.iSmallerExpandsLargerContracts = 0;
      p->m_pTrack->m_chunkAy[iFrom].stunt.iBulge = 0;;
    }
  }

  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------