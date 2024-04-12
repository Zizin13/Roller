#include "GlobalTrackSettings.h"
#include "Track.h"
#include "MainWindow.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
class CGlobalTrackSettingsPrivate
{
public:
  CGlobalTrackSettingsPrivate()
    : m_pTrack(NULL)
  {
  };
  ~CGlobalTrackSettingsPrivate()
  {
  };

  CTrack *m_pTrack;

  //header values
  QString sFloorDepth;

  //selected texture values
  QString sTex;
  QString sBld;
  QString sBackVal;

  //selected info values
  QString sTrackNumber;
  QString sImpossibleLaps;
  QString sHardLaps;
  QString sTrickyLaps;
  QString sMediumLaps;
  QString sEasyLaps;
  QString sGirlieLaps;
  QString sTrackMapSize;
  QString sTrackMapFidelity;
  QString sInfoUnknown;
};
//-------------------------------------------------------------------------------------------------

CGlobalTrackSettings::CGlobalTrackSettings(QWidget *pParent, CTrack *pTrack)
  : QWidget(pParent)
{
  p = new CGlobalTrackSettingsPrivate;
  p->m_pTrack = pTrack;
  setupUi(this);

  connect(g_pMainWindow, &CMainWindow::UpdateWindowSig, this, &CGlobalTrackSettings::OnUpdateWindow);

  connect(pbApplyInfo, &QPushButton::clicked, this, &CGlobalTrackSettings::OnApplyInfoClicked);
  connect(pbRevertInfo, &QPushButton::clicked, this, &CGlobalTrackSettings::OnCancelInfoClicked);

  connect(leFloorDepth, &QLineEdit::textChanged, this, &CGlobalTrackSettings::UpdateInfoEditMode);
  connect(leTex, &QLineEdit::textChanged, this, &CGlobalTrackSettings::UpdateInfoEditMode);
  connect(leBld, &QLineEdit::textChanged, this, &CGlobalTrackSettings::UpdateInfoEditMode);
  connect(leTrackNum, &QLineEdit::textChanged, this, &CGlobalTrackSettings::UpdateInfoEditMode);
  connect(leImpossibleLaps, &QLineEdit::textChanged, this, &CGlobalTrackSettings::UpdateInfoEditMode);
  connect(leHardLaps, &QLineEdit::textChanged, this, &CGlobalTrackSettings::UpdateInfoEditMode);
  connect(leTrickyLaps, &QLineEdit::textChanged, this, &CGlobalTrackSettings::UpdateInfoEditMode);
  connect(leMediumLaps, &QLineEdit::textChanged, this, &CGlobalTrackSettings::UpdateInfoEditMode);
  connect(leEasyLaps, &QLineEdit::textChanged, this, &CGlobalTrackSettings::UpdateInfoEditMode);
  connect(leGirlieLaps, &QLineEdit::textChanged, this, &CGlobalTrackSettings::UpdateInfoEditMode);
  connect(leMapSize, &QLineEdit::textChanged, this, &CGlobalTrackSettings::UpdateInfoEditMode);
  connect(leMapFidelity, &QLineEdit::textChanged, this, &CGlobalTrackSettings::UpdateInfoEditMode);
  connect(leInfoUnknown, &QLineEdit::textChanged, this, &CGlobalTrackSettings::UpdateInfoEditMode);
}

//-------------------------------------------------------------------------------------------------

CGlobalTrackSettings::~CGlobalTrackSettings()
{
  if (p) {
    delete p;
    p = NULL;
  }
}

//-------------------------------------------------------------------------------------------------

void CGlobalTrackSettings::OnUpdateWindow()
{
  if (!p->m_pTrack)
    return;

  UpdateInfoSelection();
  UpdateInfoEditMode();
}

//-------------------------------------------------------------------------------------------------

void CGlobalTrackSettings::OnApplyInfoClicked()
{
  if (!p->m_pTrack)
    return;

  p->m_pTrack->m_raceInfo.iTrackNumber = leTrackNum->text().toInt();
  p->m_pTrack->m_raceInfo.iImpossibleLaps = leImpossibleLaps->text().toInt();
  p->m_pTrack->m_raceInfo.iHardLaps = leHardLaps->text().toInt();
  p->m_pTrack->m_raceInfo.iTrickyLaps = leTrickyLaps->text().toInt();
  p->m_pTrack->m_raceInfo.iMediumLaps = leMediumLaps->text().toInt();
  p->m_pTrack->m_raceInfo.iEasyLaps = leEasyLaps->text().toInt();
  p->m_pTrack->m_raceInfo.iGirlieLaps = leGirlieLaps->text().toInt();
  p->m_pTrack->m_raceInfo.dTrackMapSize = leMapSize->text().toDouble();
  p->m_pTrack->m_raceInfo.iTrackMapFidelity = leMapFidelity->text().toInt();
  p->m_pTrack->m_raceInfo.dUnknown = leInfoUnknown->text().toDouble();
  p->m_pTrack->m_sTextureFile = leTex->text().toLatin1().constData();
  p->m_pTrack->m_sBuildingFile = leBld->text().toLatin1().constData();
  p->m_pTrack->m_header.iFloorDepth = leFloorDepth->text().toInt();

  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->LogMessage("Applied global track settings");
  g_pMainWindow->LoadTextures();
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CGlobalTrackSettings::OnCancelInfoClicked()
{
  RevertInfo();
}

//-------------------------------------------------------------------------------------------------

void CGlobalTrackSettings::UpdateInfoEditMode()
{
  bool bEditMode = false;
  bool bMixedData = false;
  UpdateLEEditMode(bEditMode, bMixedData, leFloorDepth, p->sFloorDepth);
  UpdateLEEditMode(bEditMode, bMixedData, leTex, p->sTex);
  UpdateLEEditMode(bEditMode, bMixedData, leBld, p->sBld);
  UpdateLEEditMode(bEditMode, bMixedData, leTrackNum, p->sTrackNumber);
  UpdateLEEditMode(bEditMode, bMixedData, leImpossibleLaps, p->sImpossibleLaps);
  UpdateLEEditMode(bEditMode, bMixedData, leHardLaps, p->sHardLaps);
  UpdateLEEditMode(bEditMode, bMixedData, leTrickyLaps, p->sTrickyLaps);
  UpdateLEEditMode(bEditMode, bMixedData, leMediumLaps, p->sMediumLaps);
  UpdateLEEditMode(bEditMode, bMixedData, leEasyLaps, p->sEasyLaps);
  UpdateLEEditMode(bEditMode, bMixedData, leGirlieLaps, p->sGirlieLaps);
  UpdateLEEditMode(bEditMode, bMixedData, leMapFidelity, p->sTrackMapFidelity);
  UpdateLEEditMode(bEditMode, bMixedData, leMapSize, p->sTrackMapSize);
  UpdateLEEditMode(bEditMode, bMixedData, leInfoUnknown, p->sInfoUnknown);

  pbApplyInfo->setEnabled(bEditMode);
  pbRevertInfo->setEnabled(bEditMode);
}

//-------------------------------------------------------------------------------------------------

void CGlobalTrackSettings::UpdateInfoSelection()
{
  p->sFloorDepth = QString::number(p->m_pTrack->m_header.iFloorDepth);
  p->sTex = p->m_pTrack->m_sTextureFile.c_str();
  p->sBld = p->m_pTrack->m_sBuildingFile.c_str();
  p->sTrackNumber = QString::number(p->m_pTrack->m_raceInfo.iTrackNumber);
  p->sImpossibleLaps = QString::number(p->m_pTrack->m_raceInfo.iImpossibleLaps);
  p->sHardLaps = QString::number(p->m_pTrack->m_raceInfo.iHardLaps);
  p->sTrickyLaps = QString::number(p->m_pTrack->m_raceInfo.iTrickyLaps);
  p->sMediumLaps = QString::number(p->m_pTrack->m_raceInfo.iMediumLaps);
  p->sEasyLaps = QString::number(p->m_pTrack->m_raceInfo.iEasyLaps);
  p->sGirlieLaps = QString::number(p->m_pTrack->m_raceInfo.iGirlieLaps);
  p->sTrackMapSize = QString::number(p->m_pTrack->m_raceInfo.dTrackMapSize, 'f', 2);
  p->sTrackMapFidelity = QString::number(p->m_pTrack->m_raceInfo.iTrackMapFidelity);
  p->sInfoUnknown = QString::number(p->m_pTrack->m_raceInfo.dUnknown, 'f', 2);

  RevertInfo();
}

//-------------------------------------------------------------------------------------------------

bool CGlobalTrackSettings::UpdateLEWithSelectionValue(QLineEdit *pLineEdit, const QString &sValue)
{
  pLineEdit->blockSignals(true);
  if (sValue.compare(MIXED_DATA) == 0) {
    pLineEdit->setText("");
    pLineEdit->setPlaceholderText(sValue);
    //pLineEdit->setStyleSheet("background-color: rgb(255,0,0)");
  } else {
    pLineEdit->setPlaceholderText("");
    pLineEdit->setText(sValue);
    pLineEdit->setStyleSheet("");
  }
  pLineEdit->blockSignals(false);
  return (sValue.compare(MIXED_DATA) == 0) && pLineEdit->text().isEmpty();
}

//-------------------------------------------------------------------------------------------------

void CGlobalTrackSettings::UpdateLEEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, const QString &sValue)
{
  QString sLineEditVal = pLineEdit->text();
  if (sLineEditVal.isEmpty())
    sLineEditVal = "0";

  if (sLineEditVal.compare(sValue) != 0) {
    if (pLineEdit->text().isEmpty() && pLineEdit->placeholderText().compare(MIXED_DATA) == 0) {
      bMixedData = true;
      pLineEdit->setStyleSheet("");
    } else {
      bEdited = true;
      pLineEdit->setStyleSheet("background-color: rgb(255,255,0)");
    }
  } else {
    pLineEdit->setStyleSheet("");
  }
}

//-------------------------------------------------------------------------------------------------

void CGlobalTrackSettings::RevertInfo()
{
  UpdateLEWithSelectionValue(leFloorDepth, p->sFloorDepth);
  UpdateLEWithSelectionValue(leTex, p->sTex);
  UpdateLEWithSelectionValue(leBld, p->sBld);
  UpdateLEWithSelectionValue(leTrackNum, p->sTrackNumber);
  UpdateLEWithSelectionValue(leImpossibleLaps, p->sImpossibleLaps);
  UpdateLEWithSelectionValue(leHardLaps, p->sHardLaps);
  UpdateLEWithSelectionValue(leTrickyLaps, p->sTrickyLaps);
  UpdateLEWithSelectionValue(leMediumLaps, p->sMediumLaps);
  UpdateLEWithSelectionValue(leEasyLaps, p->sEasyLaps);
  UpdateLEWithSelectionValue(leGirlieLaps, p->sGirlieLaps);
  UpdateLEWithSelectionValue(leMapSize, p->sTrackMapSize);
  UpdateLEWithSelectionValue(leMapFidelity, p->sTrackMapFidelity);
  UpdateLEWithSelectionValue(leInfoUnknown, p->sInfoUnknown);

  pbApplyInfo->setEnabled(false);
  pbRevertInfo->setEnabled(false);
}

//-------------------------------------------------------------------------------------------------