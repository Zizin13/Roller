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
  ckNoSurface->setChecked(g_pMainWindow->m_preferences.bPasteNoSurface);

  connect(pbClose       , &QPushButton::clicked, this, &CPreferencesDialog::reject);
  connect(sbMaxHistory  , SIGNAL(valueChanged(int)), this, SLOT(DialogEdited()));
  connect(ckRelYaw      , &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);
  connect(ckRelPitch    , &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);
  connect(ckRelRoll     , &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);
  connect(ckNoSurface   , &QCheckBox::toggled, this, &CPreferencesDialog::DialogEdited);

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
  g_pMainWindow->m_preferences.bPasteNoSurface = ckNoSurface->isChecked();

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