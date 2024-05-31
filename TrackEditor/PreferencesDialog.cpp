#include "TrackEditor.h"
#include "PreferencesDialog.h"
#include "MainWindow.h"
#include "QtHelpers.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CPreferencesDialog::CPreferencesDialog(QWidget *pParent)
  : QDialog(pParent)
{
  setupUi(this);

  sbMaxHistory->setValue(g_pMainWindow->m_preferences.iHistoryMaxSize);
  rbPasteNew->setChecked(g_pMainWindow->m_preferences.bPasteNewChunks);
  rbPasteData->setChecked(!g_pMainWindow->m_preferences.bPasteNewChunks);
  ckRelYaw->setChecked(g_pMainWindow->m_preferences.bCopyRelativeYaw);
  ckRelPitch->setChecked(g_pMainWindow->m_preferences.bCopyRelativePitch);
  ckRelRoll->setChecked(g_pMainWindow->m_preferences.bCopyRelativeRoll);
  ckPasteGeometry->setChecked(g_pMainWindow->m_preferences.bPasteGeometry);
  ckPasteSurfaceData->setChecked(g_pMainWindow->m_preferences.bPasteSurfaceData);
  ckPasteAIBehavior->setChecked(g_pMainWindow->m_preferences.bPasteAIBehavior);
  ckPasteDrawOrder->setChecked(g_pMainWindow->m_preferences.bPasteDrawOrder);
  ckPasteSigns->setChecked(g_pMainWindow->m_preferences.bPasteSigns);
  ckPasteAudio->setChecked(g_pMainWindow->m_preferences.bPasteAudio);

  connect(pbClose           , &QPushButton::clicked, this, &CPreferencesDialog::reject);
  connect(sbMaxHistory      , SIGNAL(valueChanged(int)), this, SLOT(DialogEdited()));
  connect(rbPasteNew        , &QCheckBox::toggled, this, &CPreferencesDialog::PasteNewToggled);
  connect(rbPasteData       , &QCheckBox::toggled, this, &CPreferencesDialog::PasteDataToggled);
  connect(ckRelYaw          , &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);
  connect(ckRelPitch        , &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);
  connect(ckRelRoll         , &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);
  connect(ckPasteGeometry   , &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);
  connect(ckPasteSurfaceData, &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);
  connect(ckPasteAIBehavior , &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);
  connect(ckPasteDrawOrder  , &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);
  connect(ckPasteSigns      , &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);
  connect(ckPasteAudio      , &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);

  UpdateText();
}

//-------------------------------------------------------------------------------------------------

CPreferencesDialog::~CPreferencesDialog()
{

}

//-------------------------------------------------------------------------------------------------

void CPreferencesDialog::PasteNewToggled(bool bChecked)
{
  BLOCK_SIG_AND_DO(rbPasteData, setChecked(!bChecked));
  DialogEdited();
}

//-------------------------------------------------------------------------------------------------

void CPreferencesDialog::PasteDataToggled(bool bChecked)
{
  BLOCK_SIG_AND_DO(rbPasteNew, setChecked(!bChecked));
  DialogEdited();
}

//-------------------------------------------------------------------------------------------------

void CPreferencesDialog::DialogEdited()
{
  g_pMainWindow->m_preferences.iHistoryMaxSize = sbMaxHistory->value();
  g_pMainWindow->m_preferences.bPasteNewChunks = rbPasteNew->isChecked();
  g_pMainWindow->m_preferences.bCopyRelativeYaw = ckRelYaw->isChecked();
  g_pMainWindow->m_preferences.bCopyRelativePitch = ckRelPitch->isChecked();
  g_pMainWindow->m_preferences.bCopyRelativeRoll = ckRelRoll->isChecked();
  g_pMainWindow->m_preferences.bPasteGeometry = ckPasteGeometry->isChecked();
  g_pMainWindow->m_preferences.bPasteSurfaceData = ckPasteSurfaceData->isChecked();
  g_pMainWindow->m_preferences.bPasteAIBehavior = ckPasteAIBehavior->isChecked();
  g_pMainWindow->m_preferences.bPasteDrawOrder = ckPasteDrawOrder->isChecked();
  g_pMainWindow->m_preferences.bPasteSigns = ckPasteSigns->isChecked();
  g_pMainWindow->m_preferences.bPasteAudio = ckPasteAudio->isChecked();

  UpdateText();
}

//-------------------------------------------------------------------------------------------------

void CPreferencesDialog::UpdateText()
{
  int iMemSize = 214 * sbMaxHistory->value() / 1024; //~500 chunk track = ~214KB byte array
  QString sHistorySizeText = "(approx " + QString::number(iMemSize) + "MB ram per opened ~500-chunk track)";
  lblHistorySize->setText(sHistorySizeText);
}

//-------------------------------------------------------------------------------------------------