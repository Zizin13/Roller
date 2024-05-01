#include "EditSignWidget.h"
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

class CEditSignWidgetPrivate
{
public:
  CEditSignWidgetPrivate() {};
  ~CEditSignWidgetPrivate() {};

  CTrack *m_pTrack;
  CTexture *m_pBld;


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

CEditSignWidget::CEditSignWidget(QWidget *pParent, CTrack *pTrack, CTexture *pBld)
  : QWidget(pParent)
{
  p = new CEditSignWidgetPrivate;
  p->m_pTrack = pTrack;
  p->m_pBld = pBld;

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

}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::UpdateGeometrySelection(int iFrom, int iTo)
{
  (void)(iTo);
  if (!p->m_pTrack || iFrom >= (int)p->m_pTrack->m_chunkAy.size())
    return;
  dsbYaw    ->blockSignals(true);
  dsbPitch  ->blockSignals(true);
  dsbRoll   ->blockSignals(true);
  sbHOffset ->blockSignals(true);
  sbVOffset ->blockSignals(true);
  cbType    ->blockSignals(true);
  leUnk     ->blockSignals(true);
  
  dsbYaw    ->setValue(p->m_pTrack->m_chunkAy[iFrom].dSignYaw);
  dsbPitch  ->setValue(p->m_pTrack->m_chunkAy[iFrom].dSignPitch);
  dsbRoll   ->setValue(p->m_pTrack->m_chunkAy[iFrom].dSignRoll);
  sbHOffset ->setValue(p->m_pTrack->m_chunkAy[iFrom].iSignHorizOffset);
  sbVOffset ->setValue(p->m_pTrack->m_chunkAy[iFrom].iSignVertOffset);
  cbType    ->setCurrentIndex(cbType->findData(p->m_pTrack->m_chunkAy[iFrom].iSignType));
  leUnk     ->setText(QString::number(p->m_pTrack->m_chunkAy[iFrom].iSignType));

  
  dsbYaw    ->blockSignals(false);
  dsbPitch  ->blockSignals(false);
  dsbRoll   ->blockSignals(false);
  sbHOffset ->blockSignals(false);
  sbVOffset ->blockSignals(false);
  cbType    ->blockSignals(false);
  leUnk     ->blockSignals(false);

  bool bChunkHasSign = p->m_pTrack->m_chunkAy[iFrom].iSignType != -1;
  dsbYaw    ->setEnabled(bChunkHasSign);
  dsbPitch  ->setEnabled(bChunkHasSign);
  dsbRoll   ->setEnabled(bChunkHasSign);
  sbHOffset ->setEnabled(bChunkHasSign);
  sbVOffset ->setEnabled(bChunkHasSign);
  cbType    ->setEnabled(bChunkHasSign);
  pbEdit    ->setEnabled(bChunkHasSign);
  pbSign    ->setText(bChunkHasSign ? "Delete Sign" : "Add Sign");

  bool bUnk = p->m_pTrack->m_chunkAy[iFrom].iSignType > 255;
  leUnk->setVisible(bUnk);
  lblUnk->setVisible(bUnk);
  
  QtHelpers::UpdateTextures(lblTex, NULL, p->m_pBld, p->m_pTrack->m_chunkAy[iFrom].iSignTexture);
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::YawChanged(double dVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].dSignYaw = dVal;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::PitchChanged(double dVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].dSignPitch = dVal;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::RollChanged(double dVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].dSignRoll = dVal;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::HOffsetChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iSignHorizOffset = iVal;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::VOffsetChanged(int iVal)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iSignVertOffset = iVal;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::TypeChanged(int iIndex)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iSignType = cbType->itemData(iIndex).toInt();
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::EditClicked()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pBld, p->m_pTrack->m_chunkAy[iFrom].iSignTexture);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iSignTexture = dlg.GetValue();
    }
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::SignClicked()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  bool bHasSign = p->m_pTrack->m_chunkAy[iFrom].iSignType != -1;
  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iSignType = bHasSign ? -1 : 9; //default sign type of balloon
    if (!bHasSign) {
      p->m_pTrack->m_chunkAy[i].iSignTexture = SURFACE_FLAG_APPLY_TEXTURE;
    }
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditSignWidget::UnkChanged(const QString &sText)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iSignType = sText.toInt();
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------