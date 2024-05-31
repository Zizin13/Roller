#include "TrackEditor.h"
#include "PreferencesDialog.h"
#include "MainWindow.h"
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
  ckRelYaw->setChecked(g_pMainWindow->m_preferences.bCopyRelativeYaw);
  ckRelPitch->setChecked(g_pMainWindow->m_preferences.bCopyRelativePitch);
  ckRelRoll->setChecked(g_pMainWindow->m_preferences.bCopyRelativeRoll);
  ckPasteSurfaceData->setChecked(g_pMainWindow->m_preferences.bPasteSurfaceData);
  ckPasteAIBehavior->setChecked(g_pMainWindow->m_preferences.bPasteAIBehavior);
  ckPasteDrawOrder->setChecked(g_pMainWindow->m_preferences.bPasteDrawOrder);
  ckPasteSigns->setChecked(g_pMainWindow->m_preferences.bPasteSigns);
  ckPasteAudio->setChecked(g_pMainWindow->m_preferences.bPasteAudio);

  connect(pbClose           , &QPushButton::clicked, this, &CPreferencesDialog::reject);
  connect(sbMaxHistory      , SIGNAL(valueChanged(int)), this, SLOT(DialogEdited()));
  connect(ckRelYaw          , &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);
  connect(ckRelPitch        , &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);
  connect(ckRelRoll         , &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);
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

void CPreferencesDialog::DialogEdited()
{
  g_pMainWindow->m_preferences.iHistoryMaxSize = sbMaxHistory->value();
  g_pMainWindow->m_preferences.bCopyRelativeYaw = ckRelYaw->isChecked();
  g_pMainWindow->m_preferences.bCopyRelativePitch = ckRelPitch->isChecked();
  g_pMainWindow->m_preferences.bCopyRelativeRoll = ckRelRoll->isChecked();
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