#include "EditSignWidget.h"
#include "Track.h"
#include "Texture.h"
#include "Palette.h"
#include "MainWindow.h"
#include "QtHelpers.h"
#include "EditSurfaceDialog.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

class CEditSignWidgetPrivate
{
public:
  CEditSignWidgetPrivate() {};
  ~CEditSignWidgetPrivate() {};

  QString signAy[17] = { "TOWER"
                       , "TOWER 2"
                       , "SIGN 1"
                       , "SIGN 2"
                       , "BUILD"
                       , "BUILD 1"
                       , "BUILD 2"
                       , "BUILD 3"
                       , "HEELBAR"
                       , "BALLOON"
                       , "TREE"
                       , "ADVERT"
                       , "ADVERT 2"
                       , "QUAD BLD"
                       , "BLD 0"
                       , "BIG BALL"
                       , "BIG AD" };
  size_t signAySize = sizeof(signAy) / sizeof(signAy[0]);
};

//-------------------------------------------------------------------------------------------------

CEditSignWidget::CEditSignWidget(QWidget *pParent)
  : QWidget(pParent)
{
  p = new CEditSignWidgetPrivate;

  setupUi(this);

  for (int i = 0; i < p->signAySize; ++i) {
    cbType->addItem(p->signAy[i], i);
  }
  lblUnk->hide();
  leUnk->hide();

  connect(g_pMainWindow, &CMainWindow::UpdateGeometrySelectionSig, this, &CEditSignWidget::UpdateGeometrySelection);

  connect(dsbYaw    , SIGNAL(valueChanged(double)),     this, SLOT(YawChanged(double)));
  connect(dsbPitch  , SIGNAL(valueChanged(double)),     this, SLOT(PitchChanged(double)));
  connect(dsbRoll   , SIGNAL(valueChanged(double)),     this, SLOT(RollChanged(double)));
  connect(sbHOffset , SIGNAL(valueChanged(int)),        this, SLOT(HOffsetChanged(int)));
  connect(sbVOffset , SIGNAL(valueChanged(int)),        this, SLOT(VOffsetChanged(int)));
  connect(cbType    , SIGNAL(currentIndexChanged(int)), this, SLOT(TypeChanged(int)));
  connect(pbEdit    , SIGNAL(clicked()),                this, SLOT(EditClicked()));
  connect(pbSign    , SIGNAL(clicked()),                this, SLOT(SignClicked()));
  connect(leUnk, &QLineEdit::textChanged, this, &CEditSignWidget::UnkChanged);
}

//-------------------------------------------------------------------------------------------------

CEditSignWidget::~CEditSignWidget()
{
  if (p) {
    delete p;
    p = NULL;
  }
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::UpdateGeometrySelection(int iFrom, int iTo)
{
  (void)(iTo);
  if (!g_pMainWindow->GetCurrentTrack() || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;
  
  BLOCK_SIG_AND_DO(dsbYaw    , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].dSignYaw));
  BLOCK_SIG_AND_DO(dsbPitch  , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].dSignPitch));
  BLOCK_SIG_AND_DO(dsbRoll   , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].dSignRoll));
  BLOCK_SIG_AND_DO(sbHOffset , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].iSignHorizOffset));
  BLOCK_SIG_AND_DO(sbVOffset , setValue(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].iSignVertOffset));
  BLOCK_SIG_AND_DO(cbType    , setCurrentIndex(cbType->findData(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].iSignType)));
  BLOCK_SIG_AND_DO(leUnk     , setText(QString::number(g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].iSignType)));

  bool bChunkHasSign = g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].iSignType != -1;
  dsbYaw    ->setEnabled(bChunkHasSign);
  dsbPitch  ->setEnabled(bChunkHasSign);
  dsbRoll   ->setEnabled(bChunkHasSign);
  sbHOffset ->setEnabled(bChunkHasSign);
  sbVOffset ->setEnabled(bChunkHasSign);
  cbType    ->setEnabled(bChunkHasSign);
  pbEdit    ->setEnabled(bChunkHasSign);
  lblYaw    ->setEnabled(bChunkHasSign);
  lblPitch  ->setEnabled(bChunkHasSign);
  lblRoll   ->setEnabled(bChunkHasSign);
  lblHOffset->setEnabled(bChunkHasSign);
  lblVOffset->setEnabled(bChunkHasSign);
  lblType   ->setEnabled(bChunkHasSign);
  pbSign    ->setText(bChunkHasSign ? "Delete Sign" : "Add Sign");

  bool bUnk = g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].iSignType > 255;
  leUnk->setVisible(bUnk);
  lblUnk->setVisible(bUnk);
  
  QtHelpers::UpdateTextures(lblTex, NULL, g_pMainWindow->GetCurrentTrack()->m_pBld, g_pMainWindow->GetCurrentTrack()->m_pPal, g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].iSignTexture);
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::YawChanged(double dVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].dSignYaw = dVal;
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed sign yaw");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::PitchChanged(double dVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].dSignPitch = dVal;
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed sign pitch");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::RollChanged(double dVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].dSignRoll = dVal;
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed sign roll");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::HOffsetChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iSignHorizOffset = iVal;
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed sign horiz offset");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::VOffsetChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iSignVertOffset = iVal;
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed sign vert offset");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::TypeChanged(int iIndex)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iSignType = cbType->itemData(iIndex).toInt();
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed sign type");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::EditClicked()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, g_pMainWindow->GetCurrentTrack()->m_pBld, g_pMainWindow->GetCurrentTrack()->m_pPal, g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].iSignTexture);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iSignTexture = dlg.GetValue();
    }
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed sign texture");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::SignClicked()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  bool bHasSign = g_pMainWindow->GetCurrentTrack()->m_chunkAy[iFrom].iSignType != -1;
  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iSignType = bHasSign ? -1 : 9; //default sign type of balloon
    if (!bHasSign) {
      g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iSignTexture = SURFACE_FLAG_APPLY_TEXTURE;
    }
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory(bHasSign ? "Removed sign" : "Added sign");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::UnkChanged(const QString &sText)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size()
      || iTo >= (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iSignType = sText.toInt();
  }

  g_pMainWindow->GetCurrentTrack()->UpdateChunkStrings();
  g_pMainWindow->SaveHistory("Changed unk sign value");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------