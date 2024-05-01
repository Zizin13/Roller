#include "EditAudioWidget.h"
#include "Track.h"
#include "MainWindow.h"
#include "QtHelpers.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
#define DEFAULT_AUDIO         33 //LOOPCOME
#define DEFAULT_TRIGGER_SPEED 35
#define TRIGGER_SPEED_TO_MPH (67.0 / 200.0)
//-------------------------------------------------------------------------------------------------

class CEditAudioWidgetPrivate
{
public:
  CEditAudioWidgetPrivate() {};
  ~CEditAudioWidgetPrivate() {};

  CTrack *m_pTrack;

  QString audioAy[119] = {
    "<none>"   ,//ENGINE in sound.ini, appears to mean none in track data
    "ENGINE2"  ,
    "BIGCRASH" ,
    "SKID1"    ,
    "GO"       ,
    "GRSHIFT"  ,
    "EXPLO"    ,
    "FENDER"   ,
    "FENDER2"  ,
    "LANDSKID" ,
    "LIGHTLAN" ,
    "BANK"     ,
    "WALL1"    ,
    "ROOF"     ,
    "FATALITY" ,
    "KEEPGO"   ,
    "FINAL1"   ,
    "FINAL2"   ,
    "FINAL3"   ,
    "YEAHHAH"  ,
    "AAARG"    ,
    "GOTONE"   ,
    "WINNER"   ,
    "HARDER"   ,
    "RUBBISH"  ,
    "POOR"     ,
    "REVERSE"  ,
    "BLOWTIME" ,
    "ESTART"   ,
    "FALSE"    ,
    "RACEOVER" ,
    "YEAHLOOP" ,
    "YEAHCORK" ,
    "LOOPCOME" ,
    "CORKCOME" ,
    "TOOSLOW"  ,
    "TOOFAST"  ,
    "RECORD"   ,
    "SMASH"    ,
    "BEST"     ,
    "FAST"     ,
    "MENDING"  ,
    "FIXED"    ,
    "2LEFT"    ,
    "1LEFT"    ,
    "0LEFT"    ,
    "PITIN"    ,
    "HURTME"   ,
    "HURTYOU"  ,
    "ENGINES"  ,
    "DRIVERS"  ,
    "LAPPED"   ,
    "UNLAPPED" ,
    "2LAPS"    ,
    "3LAPS"    ,
    "4LAPS"    ,
    "5LAPS"    ,
    "WIN"      ,
    "BLOCK"    ,
    "SLOW"     ,
    "BASH"     ,
    "WIN2"     ,
    "BLOCK2"   ,
    "SLOW2"    ,
    "BASH2"    ,
    "REJECT1"  ,
    "REJECT2"  ,
    "TGONE"    ,
    "TDAMAGE"  ,
    "REVERST"  ,
    "TPITS"    ,
    "ARIEL"    ,
    "DESILVA"  ,
    "PULSE"    ,
    "GLOBAL"   ,
    "MILLION"  ,
    "MISSION"  ,
    "ZIZIN"    ,
    "REISE"    ,
    "BRP"      ,
    "UP"       ,
    "BOING"    ,
    "BLOP"     ,
    "BUTTON"   ,
    "CARIN"    ,
    "CAROUT"   ,
    "TRACK"    ,
    "START"    ,
    "WHIPLASH" ,
    "1RACE"    ,
    "2RACE"    ,
    "3RACE"    ,
    "4RACE"    ,
    "5RACE"    ,
    "6RACE"    ,
    "7RACE"    ,
    "8RACE"    ,
    "ONE"      ,
    "TWO"      ,
    "THREE"    ,
    "FOUR"     ,
    "FIVE"     ,
    "SIX"      ,
    "SEVEN"    ,
    "EIGHT"    ,
    "NINE"     ,
    "TEN"      ,
    "ELEVEN"   ,
    "TWELVE"   ,
    "THIRTEEN" ,
    "FOURTEEN" ,
    "FIFTEEN"  ,
    "SIXTEEN"  ,
    "FATLOTS"  ,
    "WON"      ,
    "STAT"     ,
    "NEWLAP"   ,
    "NEWFAST"  ,
    "CONGRAT"  ,
  };
  size_t audioAySize = sizeof(audioAy) / sizeof(audioAy[0]);
};

//-------------------------------------------------------------------------------------------------

CEditAudioWidget::CEditAudioWidget(QWidget *pParent, CTrack *pTrack)
  : QWidget(pParent)
{
  p = new CEditAudioWidgetPrivate;
  p->m_pTrack = pTrack;

  setupUi(this);

  for (int i = 0; i < p->audioAySize; ++i) {
    cbBelow->addItem(p->audioAy[i], i);
  }
  for (int i = 0; i < p->audioAySize; ++i) {
    cbAbove->addItem(p->audioAy[i], i);
  }

  connect(g_pMainWindow, &CMainWindow::UpdateGeometrySelectionSig, this, &CEditAudioWidget::UpdateGeometrySelection);
  
  connect(pbAudio, SIGNAL(clicked()),                this, SLOT(AudioClicked()));
  connect(sbSpeed, SIGNAL(valueChanged(int)),        this, SLOT(SpeedChanged(int)));
  connect(cbBelow, SIGNAL(currentIndexChanged(int)), this, SLOT(BelowChanged(int)));
  connect(cbAbove, SIGNAL(currentIndexChanged(int)), this, SLOT(AboveChanged(int)));
}

//-------------------------------------------------------------------------------------------------

CEditAudioWidget::~CEditAudioWidget()
{

}

//-------------------------------------------------------------------------------------------------

void CEditAudioWidget::UpdateGeometrySelection(int iFrom, int iTo)
{
  (void)(iTo);
  if (!p->m_pTrack || iFrom >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  bool bChunkHasAudio = p->m_pTrack->m_chunkAy[iFrom].iAudioTriggerSpeed != 0;
  sbSpeed->setEnabled(bChunkHasAudio);
  cbBelow->setEnabled(bChunkHasAudio);
  cbAbove->setEnabled(bChunkHasAudio);
  lblSpeed->setEnabled(bChunkHasAudio);
  lblBelow->setEnabled(bChunkHasAudio);
  lblAbove->setEnabled(bChunkHasAudio);
  lblMph->setEnabled(bChunkHasAudio);
  pbAudio->setText(bChunkHasAudio ? "Delete Audio" : "Add Audio");

  sbSpeed->blockSignals(true);
  cbBelow->blockSignals(true);
  cbAbove->blockSignals(true);

  sbSpeed->setMinimum(bChunkHasAudio ? 1 : 0);
  sbSpeed->setValue(p->m_pTrack->m_chunkAy[iFrom].iAudioTriggerSpeed);
  cbBelow->setCurrentIndex(cbBelow->findData(p->m_pTrack->m_chunkAy[iFrom].iAudioBelowTrigger));
  cbAbove->setCurrentIndex(cbAbove->findData(p->m_pTrack->m_chunkAy[iFrom].iAudioAboveTrigger));

  sbSpeed->blockSignals(false);
  cbBelow->blockSignals(false);
  cbAbove->blockSignals(false);

  int iSpeedMph = (int)(p->m_pTrack->m_chunkAy[iFrom].iAudioTriggerSpeed * TRIGGER_SPEED_TO_MPH);
  lblMph->setText("(" + QString::number(iSpeedMph) + " mph)");
}

//-------------------------------------------------------------------------------------------------

void CEditAudioWidget::SpeedChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iAudioTriggerSpeed = iVal;
  }

  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditAudioWidget::BelowChanged(int iIndex)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iAudioBelowTrigger = cbBelow->itemData(iIndex).toInt();
  }

  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditAudioWidget::AboveChanged(int iIndex)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iSignType = cbAbove->itemData(iIndex).toInt();
  }

  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditAudioWidget::AudioClicked()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  bool bHasAudio = p->m_pTrack->m_chunkAy[iFrom].iAudioTriggerSpeed != 0;
  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iAudioTriggerSpeed = bHasAudio ? 0 : DEFAULT_TRIGGER_SPEED;
    p->m_pTrack->m_chunkAy[i].iAudioAboveTrigger = bHasAudio ? 0 : DEFAULT_AUDIO;
    if (bHasAudio)
      p->m_pTrack->m_chunkAy[i].iAudioBelowTrigger = 0;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------