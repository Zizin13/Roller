#include "TrackEditor.h"
#include "MainWindow.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include "qsettings.h"
#include "qdesktopwidget.h"
#include "Track.h"
#include "LogDialog.h"
#if defined (IS_WINDOWS)
  #include <Windows.h>
#endif
//-------------------------------------------------------------------------------------------------
CMainWindow *g_pMainWindow = NULL;
//-------------------------------------------------------------------------------------------------

class CMainWindowPrivate
{
public:
  CMainWindowPrivate(CMainWindow *pMainWindow)
    : m_logDialog(pMainWindow)
  {};
  ~CMainWindowPrivate() {};

  CTrack m_track;
  CLogDialog m_logDialog;

  //selected geometry values
  QString sLeftShoulderWidth, sLeftLaneWidth, sRightLaneWidth, sRightShoulderWidth
    , sUnk1, sUnk2, sUnk3
    , sYaw, sPitch, sRoll
    , sAILine1, sAILine2, sAILine3, sAILine4
    , sTrackGrip, sLeftShoulderGrip, sRightShoulderGrip
    , sUnk4, sUnk5, sUnk6, sUnk7, sUnk8
    , sLeftSurfaceType, sCenterSurfaceType, sRightSurfaceType
    , sUnk9, sUnk10, sUnk11, sUnk12, sUnk13, sUnk14
    , sUnk15, sUnk16, sUnk17, sUnk18, sUnk19, sUnk20
    , sfUnk1, sfUnk2, sfUnk3
    , sUnk21, sUnk22, sUnk23, sUnk24, sUnk25, sUnk26
    , sUnk27, sUnk28, sUnk29, sUnk30, sUnk31, sUnk32
    , sUnk33, sUnk34, sUnk35, sUnk36, sUnk37, sUnk38
    , sUnk39, sUnk40, sUnk41, sUnk42, sUnk43, sUnk44
    , sUnk45, sUnk46, sUnk47, sUnk48, sUnk49, sUnk50;

  //selected tuple values
  QString sTupleRVal;

  //selected stunt values
  QString sStuntScaleFactor;
  QString sStuntAngle;
  QString sStuntUnknown;
  QString sStuntTimingGroup;
  QString sStuntHeight;
  QString sStuntTimeBulging;
  QString sStuntTimeFlat;
  QString sStuntExpandsContracts;
  QString sStuntBulge;
};

//-------------------------------------------------------------------------------------------------

CMainWindow::CMainWindow(const QString &sAppPath)
  : QMainWindow(NULL)
  , m_bUnsavedChanges(false)
  , m_sAppPath(sAppPath)
  , m_sTrackFile("")
  , m_sTrackFilesFolder("")
{
  //init
  p = new CMainWindowPrivate(this);
  g_pMainWindow = this;
  m_sSettingsFile = m_sAppPath + "/TrackEditor.ini";
  m_sSettingsFile = QDir::toNativeSeparators(m_sSettingsFile);
  setupUi(this);
  splitter->setStretchFactor(0, 1);
  splitter->setStretchFactor(1, 3);
  twEditor->setEnabled(false);
  p->m_logDialog.hide();
  txData->setFont(QFont("Courier", 8));

  //signals
  connect(actNew, &QAction::triggered, this, &CMainWindow::OnNewTrack);
  connect(actLoad, &QAction::triggered, this, &CMainWindow::OnLoadTrack);
  connect(actSave, &QAction::triggered, this, &CMainWindow::OnSaveTrack);
  connect(actSaveAs, &QAction::triggered, this, &CMainWindow::OnSaveTrackAs);
  connect(actImportMangled, &QAction::triggered, this, &CMainWindow::OnImportMangled);
  connect(actExportMangled, &QAction::triggered, this, &CMainWindow::OnExportMangled);
  connect(actDebug, &QAction::triggered, this, &CMainWindow::OnDebug);
  connect(actAbout, &QAction::triggered, this, &CMainWindow::OnAbout);

  connect(twEditor, &QTabWidget::currentChanged, this, &CMainWindow::OnEditTabChanged);

  connect(sbSelChunksFrom, SIGNAL(valueChanged(int)), this, SLOT(OnSelChunksFromChanged(int)));
  connect(sbSelChunksTo, SIGNAL(valueChanged(int)), this, SLOT(OnSelChunksToChanged(int)));
  connect(ckTo, &QCheckBox::toggled, this, &CMainWindow::OnToChecked);
  connect(pbApply, &QPushButton::clicked, this, &CMainWindow::OnApplyClicked);
  connect(pbApplyTuple, &QPushButton::clicked, this, &CMainWindow::OnApplyTupleClicked);
  connect(pbApplyStunt, &QPushButton::clicked, this, &CMainWindow::OnApplyStuntClicked);
  connect(pbCancel, &QPushButton::clicked, this, &CMainWindow::OnCancelClicked);
  connect(pbRevertTuple, &QPushButton::clicked, this, &CMainWindow::OnCancelTupleClicked);
  connect(pbRevertStunt, &QPushButton::clicked, this, &CMainWindow::OnCancelStuntClicked);
  connect(pbDeleteTuple, &QPushButton::clicked, this, &CMainWindow::OnDeleteTuplesClicked);
  connect(pbDeleteStunt, &QPushButton::clicked, this, &CMainWindow::OnDeleteStuntClicked);
  connect(pbEditLSurface, &QPushButton::clicked, this, &CMainWindow::OnEditLSurface);
  connect(pbEditCSurface, &QPushButton::clicked, this, &CMainWindow::OnEditCSurface);
  connect(pbEditRSurface, &QPushButton::clicked, this, &CMainWindow::OnEditRSurface);

  connect(leLShoulderWidth, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLLaneWidth, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leRLaneWidth, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk1, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk2, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk3, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leYaw, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(lePitch, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leRoll , &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leAILine1, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leAILine2, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leAILine3, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leAILine4, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leTrackGrip, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLeftShoulderGrip, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leRShoulderGrip, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk4, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk5, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk6, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk7, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk8, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLeftSurfaceType, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leCenterSurfaceType, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leRightSurfaceType, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk9, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk10, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk11, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk12, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk13, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk14, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk15, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk16, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk17, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk18, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk19, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk20, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leFloatUnk1, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leFloatUnk2, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leFloatUnk3, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk21, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk22, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk23, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk24, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk25, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk26, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk27, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk28, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk29, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk30, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk31, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk32, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk33, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk34, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk35, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk36, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk37, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk38, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk39, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk40, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk41, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk42, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk43, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk44, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk45, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk46, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk47, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk48, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk49, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk50, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);

  connect(leLVal, &QLineEdit::textChanged, this, &CMainWindow::OnTupleLValChanged);
  connect(leRVal, &QLineEdit::textChanged, this, &CMainWindow::UpdateTuplesEditMode);

  connect(leStuntIndex, &QLineEdit::textChanged, this, &CMainWindow::OnStuntIndexChanged);
  connect(leStuntScaleFact, &QLineEdit::textChanged, this, &CMainWindow::UpdateStuntsEditMode);
  connect(leStuntAngle, &QLineEdit::textChanged, this, &CMainWindow::UpdateStuntsEditMode);
  connect(leStuntUnk, &QLineEdit::textChanged, this, &CMainWindow::UpdateStuntsEditMode);
  connect(leStuntTimingGroup, &QLineEdit::textChanged, this, &CMainWindow::UpdateStuntsEditMode);
  connect(leStuntHeight, &QLineEdit::textChanged, this, &CMainWindow::UpdateStuntsEditMode);
  connect(leStuntTimeBulging, &QLineEdit::textChanged, this, &CMainWindow::UpdateStuntsEditMode);
  connect(leStuntTimeFlat, &QLineEdit::textChanged, this, &CMainWindow::UpdateStuntsEditMode);
  connect(leStuntExpandContract, &QLineEdit::textChanged, this, &CMainWindow::UpdateStuntsEditMode);
  connect(leStuntBulge, &QLineEdit::textChanged, this, &CMainWindow::UpdateStuntsEditMode);

  //open window
  LoadSettings();
}

//-------------------------------------------------------------------------------------------------

CMainWindow::~CMainWindow()
{
  SaveSettings();

  //cleanup
  if (p) {
    delete p;
    p = NULL;
  }
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::LogMessage(const QString &sMsg)
{
  p->m_logDialog.LogMessage(sMsg);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnNewTrack()
{
  if (!SaveChangesAndContinue())
    return;

  QMessageBox::warning(this, "Fatality!", "Not implemented yet");
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnLoadTrack()
{
  //check for unsaved data
  if (!SaveChangesAndContinue())
    return;

  //load track
  QString sFilename = QDir::toNativeSeparators(QFileDialog::getOpenFileName(
    this, "Load Track", m_sTrackFilesFolder, QString("Track Files (*.TRK)")));
  if (!p->m_track.LoadTrack(sFilename)) {
    //load failed
    m_sTrackFile = "";
    m_bUnsavedChanges = false;
  } else {
    //load successful
    sbSelChunksFrom->setValue(0);
    sbSelChunksTo->setValue(0);
    if (!p->m_track.m_tupleMap.empty())
      leLVal->setText(QString::number(p->m_track.m_tupleMap.begin()->first));
    if (!p->m_track.m_stuntMap.empty())
      leStuntIndex->setText(QString::number(p->m_track.m_stuntMap.begin()->first));
    m_sTrackFilesFolder = sFilename.left(sFilename.lastIndexOf(QDir::separator()));
    m_sTrackFile = sFilename;
    m_bUnsavedChanges = false;
  }
  //update app
  UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnSaveTrack()
{
  if (!m_sTrackFile.isEmpty()) {
    m_bUnsavedChanges = !p->m_track.SaveTrack(m_sTrackFile);
    UpdateWindow();
  } else {
    OnSaveTrackAs();
  }
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnSaveTrackAs()
{
  //save track
  QString sFilename = QDir::toNativeSeparators(QFileDialog::getSaveFileName(
    this, "Save Track As", m_sTrackFilesFolder, "Track Files (*.TRK)"));
  if (!p->m_track.SaveTrack(sFilename))
    return;

  //save successful, update app
  m_sTrackFilesFolder = sFilename.left(sFilename.lastIndexOf(QDir::separator()));
  m_sTrackFile = sFilename;
  m_bUnsavedChanges = false;
  UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnImportMangled()
{
  QMessageBox::warning(this, "Fatality!", "Not implemented yet");
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnExportMangled()
{
  QMessageBox::warning(this, "Fatality!", "Not implemented yet");
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnDebug()
{
  p->m_logDialog.raise();
  p->m_logDialog.show();
  p->m_logDialog.setFocus();
  p->m_logDialog.resize(QDesktopWidget().availableGeometry(this).size() * 0.3);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditTabChanged(int iIndex)
{
  (void)(iIndex);
  UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnAbout()
{
  QMessageBox::information(this, "Git Gud", "YOU NEED MORE PRACTICE");
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnSelChunksFromChanged(int iValue)
{
  if (!ckTo->isChecked() || sbSelChunksTo->value() < iValue) {
    sbSelChunksTo->blockSignals(true);
    sbSelChunksTo->setValue(iValue);
    sbSelChunksTo->blockSignals(false);
  }
  UpdateGeometrySelection();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnSelChunksToChanged(int iValue)
{
  if (sbSelChunksFrom->value() > iValue) {
    sbSelChunksFrom->blockSignals(true);
    sbSelChunksFrom->setValue(iValue);
    sbSelChunksFrom->blockSignals(false);
  }
  UpdateGeometrySelection();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnSelectedTupleChanged(int iValue)
{
  (void)(iValue);
  UpdateTupleSelection();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnToChecked(bool bChecked)
{
  sbSelChunksTo->setEnabled(bChecked && !pbApply->isEnabled());
  if (!bChecked) {
    sbSelChunksTo->setValue(sbSelChunksFrom->value());
  }
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnApplyClicked()
{
  p->m_track.ApplyGeometrySettings(sbSelChunksFrom->value(), sbSelChunksTo->value()
    , leLShoulderWidth->text(), leLLaneWidth->text(), leRLaneWidth->text(), leRShoulderWidth->text()
    , leUnk1->text(), leUnk2->text(), leUnk3->text()
    , leYaw->text(), lePitch->text(), leRoll->text()
    , leAILine1->text(), leAILine2->text(), leAILine3->text(), leAILine4->text()
    , leTrackGrip->text(), leLeftShoulderGrip->text(), leRShoulderGrip->text()
    , leUnk4->text(), leUnk5->text(), leUnk6->text(), leUnk7->text(), leUnk8->text()
    , leLeftSurfaceType->text(), leCenterSurfaceType->text(), leRightSurfaceType->text()
    , leUnk9->text(), leUnk10->text(), leUnk11->text(), leUnk12->text(), leUnk13->text(), leUnk14->text()
    , leUnk15->text(), leUnk16->text(), leUnk17->text(), leUnk18->text(), leUnk19->text(), leUnk20->text()
    , leFloatUnk1->text(), leFloatUnk2->text(), leFloatUnk3->text()
    , leUnk21->text(), leUnk22->text(), leUnk23->text(), leUnk24->text(), leUnk25->text(), leUnk26->text()
    , leUnk27->text(), leUnk28->text(), leUnk29->text(), leUnk30->text(), leUnk31->text(), leUnk32->text()
    , leUnk33->text(), leUnk34->text(), leUnk35->text(), leUnk36->text(), leUnk37->text(), leUnk38->text()
    , leUnk39->text(), leUnk40->text(), leUnk41->text(), leUnk42->text(), leUnk43->text(), leUnk44->text()
    , leUnk45->text(), leUnk46->text(), leUnk47->text(), leUnk48->text(), leUnk49->text(), leUnk50->text());
  m_bUnsavedChanges = true;
  UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnApplyTupleClicked()
{
  //update value
  p->m_track.m_tupleMap[leLVal->text().toInt()] = leRVal->text().toInt();

  m_bUnsavedChanges = true;
  g_pMainWindow->LogMessage("Applied changes to tuples");
  UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnApplyStuntClicked()
{
  //update value
  tStunt *pStunt = &p->m_track.m_stuntMap[leStuntIndex->text().toInt()];
  pStunt->iScaleFactor = leStuntScaleFact->text().toInt();
  pStunt->iAngle = leStuntAngle->text().toInt();
  pStunt->iUnknown = leStuntUnk->text().toInt();
  pStunt->iTimingGroup = leStuntTimingGroup->text().toInt();
  pStunt->iHeight = leStuntHeight->text().toInt();
  pStunt->iTimeBulging = leStuntTimeBulging->text().toInt();
  pStunt->iTimeFlat = leStuntTimeFlat->text().toInt();
  pStunt->iSmallerExpandsLargerContracts = leStuntExpandContract->text().toInt();
  pStunt->iBulge = leStuntBulge->text().toInt();

  m_bUnsavedChanges = true;
  g_pMainWindow->LogMessage("Applied changes to stunts");
  UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnCancelClicked()
{
  RevertGeometry();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnCancelTupleClicked()
{
  RevertTuples();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnCancelStuntClicked()
{
  RevertStunts();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnDeleteTuplesClicked()
{
  //delete value from map
  CTupleMap::iterator it = p->m_track.m_tupleMap.find(leLVal->text().toInt());
  if (it != p->m_track.m_tupleMap.end()) {
    it = p->m_track.m_tupleMap.erase(it);
    if (it != p->m_track.m_tupleMap.end()) {
      leLVal->setText(QString::number(it->first));
    } else if (!p->m_track.m_tupleMap.empty()) {
      --it;
      leLVal->setText(QString::number(it->first));
    }
  }
  g_pMainWindow->LogMessage("Deleted tuple");
  UpdateWindow();
  UpdateTuplesEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnDeleteStuntClicked()
{
  //delete value from map
  CStuntMap::iterator it = p->m_track.m_stuntMap.find(leStuntIndex->text().toInt());
  if (it != p->m_track.m_stuntMap.end()) {
    it = p->m_track.m_stuntMap.erase(it);
    if (it != p->m_track.m_stuntMap.end()) {
      leStuntIndex->setText(QString::number(it->first));
    } else if (!p->m_track.m_stuntMap.empty()) {
      --it;
      leStuntIndex->setText(QString::number(it->first));
    }
  }
  g_pMainWindow->LogMessage("Deleted stunt");
  UpdateWindow();
  UpdateStuntsEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditLSurface()
{
  QMessageBox::warning(this, "Fatality!", "Not implemented yet");
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditCSurface()
{
  QMessageBox::warning(this, "Fatality!", "Not implemented yet");
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditRSurface()
{
  QMessageBox::warning(this, "Fatality!", "Not implemented yet");
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnTupleLValChanged()
{
  UpdateTupleSelection();
  UpdateTuplesEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateGeometryEditMode()
{
  bool bEditMode = false;
  bEditMode |= UpdateLEEditMode(leLShoulderWidth, p->sLeftShoulderWidth);
  bEditMode |= UpdateLEEditMode(leLLaneWidth, p->sLeftLaneWidth);
  bEditMode |= UpdateLEEditMode(leRLaneWidth, p->sRightLaneWidth);
  bEditMode |= UpdateLEEditMode(leRShoulderWidth, p->sRightShoulderWidth);
  bEditMode |= UpdateLEEditMode(leUnk1, p->sUnk1);
  bEditMode |= UpdateLEEditMode(leUnk2, p->sUnk2);
  bEditMode |= UpdateLEEditMode(leUnk3, p->sUnk3);
  bEditMode |= UpdateLEEditMode(leYaw, p->sYaw);
  bEditMode |= UpdateLEEditMode(lePitch, p->sPitch);
  bEditMode |= UpdateLEEditMode(leRoll, p->sRoll);
  bEditMode |= UpdateLEEditMode(leAILine1, p->sAILine1);
  bEditMode |= UpdateLEEditMode(leAILine2, p->sAILine2);
  bEditMode |= UpdateLEEditMode(leAILine3, p->sAILine3);
  bEditMode |= UpdateLEEditMode(leAILine4, p->sAILine4);
  bEditMode |= UpdateLEEditMode(leTrackGrip, p->sTrackGrip);
  bEditMode |= UpdateLEEditMode(leLeftShoulderGrip, p->sLeftShoulderGrip);
  bEditMode |= UpdateLEEditMode(leRShoulderGrip, p->sRightShoulderGrip);
  bEditMode |= UpdateLEEditMode(leUnk4, p->sUnk4);
  bEditMode |= UpdateLEEditMode(leUnk5, p->sUnk5);
  bEditMode |= UpdateLEEditMode(leUnk6, p->sUnk6);
  bEditMode |= UpdateLEEditMode(leUnk7, p->sUnk7);
  bEditMode |= UpdateLEEditMode(leUnk8, p->sUnk8);
  bEditMode |= UpdateLEEditMode(leLeftSurfaceType, p->sLeftSurfaceType);
  bEditMode |= UpdateLEEditMode(leCenterSurfaceType, p->sCenterSurfaceType);
  bEditMode |= UpdateLEEditMode(leRightSurfaceType, p->sRightSurfaceType);
  bEditMode |= UpdateLEEditMode(leUnk9, p->sUnk9);
  bEditMode |= UpdateLEEditMode(leUnk10, p->sUnk10);
  bEditMode |= UpdateLEEditMode(leUnk11, p->sUnk11);
  bEditMode |= UpdateLEEditMode(leUnk12, p->sUnk12);
  bEditMode |= UpdateLEEditMode(leUnk13, p->sUnk13);
  bEditMode |= UpdateLEEditMode(leUnk14, p->sUnk14);
  bEditMode |= UpdateLEEditMode(leUnk15, p->sUnk15);
  bEditMode |= UpdateLEEditMode(leUnk16, p->sUnk16);
  bEditMode |= UpdateLEEditMode(leUnk17, p->sUnk17);
  bEditMode |= UpdateLEEditMode(leUnk18, p->sUnk18);
  bEditMode |= UpdateLEEditMode(leUnk19, p->sUnk19);
  bEditMode |= UpdateLEEditMode(leUnk20, p->sUnk20);
  bEditMode |= UpdateLEEditMode(leFloatUnk1, p->sfUnk1);
  bEditMode |= UpdateLEEditMode(leFloatUnk2, p->sfUnk2);
  bEditMode |= UpdateLEEditMode(leFloatUnk3, p->sfUnk3);
  bEditMode |= UpdateLEEditMode(leUnk21, p->sUnk21);
  bEditMode |= UpdateLEEditMode(leUnk22, p->sUnk22);
  bEditMode |= UpdateLEEditMode(leUnk23, p->sUnk23);
  bEditMode |= UpdateLEEditMode(leUnk24, p->sUnk24);
  bEditMode |= UpdateLEEditMode(leUnk25, p->sUnk25);
  bEditMode |= UpdateLEEditMode(leUnk26, p->sUnk26);
  bEditMode |= UpdateLEEditMode(leUnk27, p->sUnk27);
  bEditMode |= UpdateLEEditMode(leUnk28, p->sUnk28);
  bEditMode |= UpdateLEEditMode(leUnk29, p->sUnk29);
  bEditMode |= UpdateLEEditMode(leUnk30, p->sUnk30);
  bEditMode |= UpdateLEEditMode(leUnk31, p->sUnk31);
  bEditMode |= UpdateLEEditMode(leUnk32, p->sUnk32);
  bEditMode |= UpdateLEEditMode(leUnk33, p->sUnk33);
  bEditMode |= UpdateLEEditMode(leUnk34, p->sUnk34);
  bEditMode |= UpdateLEEditMode(leUnk35, p->sUnk35);
  bEditMode |= UpdateLEEditMode(leUnk36, p->sUnk36);
  bEditMode |= UpdateLEEditMode(leUnk37, p->sUnk37);
  bEditMode |= UpdateLEEditMode(leUnk38, p->sUnk38);
  bEditMode |= UpdateLEEditMode(leUnk39, p->sUnk39);
  bEditMode |= UpdateLEEditMode(leUnk40, p->sUnk40);
  bEditMode |= UpdateLEEditMode(leUnk41, p->sUnk41);
  bEditMode |= UpdateLEEditMode(leUnk42, p->sUnk42);
  bEditMode |= UpdateLEEditMode(leUnk43, p->sUnk43);
  bEditMode |= UpdateLEEditMode(leUnk44, p->sUnk44);
  bEditMode |= UpdateLEEditMode(leUnk45, p->sUnk45);
  bEditMode |= UpdateLEEditMode(leUnk46, p->sUnk46);
  bEditMode |= UpdateLEEditMode(leUnk47, p->sUnk47);
  bEditMode |= UpdateLEEditMode(leUnk48, p->sUnk48);
  bEditMode |= UpdateLEEditMode(leUnk49, p->sUnk49);
  bEditMode |= UpdateLEEditMode(leUnk50, p->sUnk50);
  pbApply->setEnabled(bEditMode);
  pbCancel->setEnabled(bEditMode);
  sbSelChunksFrom->setEnabled(!bEditMode);
  ckTo->setEnabled(!bEditMode);
  sbSelChunksTo->setEnabled(!bEditMode && ckTo->isChecked());
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateTuplesEditMode()
{
  CTupleMap::iterator it = p->m_track.m_tupleMap.find(leLVal->text().toInt());
  bool bNew = (it == p->m_track.m_tupleMap.end());

  bool bLValEdited = UpdateLEEditMode(leRVal, p->sTupleRVal, bNew);

  bool bEditMode = bNew || bLValEdited;
  pbApplyTuple->setEnabled(bEditMode);
  pbRevertTuple->setEnabled(bLValEdited);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnStuntIndexChanged()
{
  UpdateStuntSelection();
  UpdateStuntsEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateStuntsEditMode()
{
  CStuntMap::iterator it = p->m_track.m_stuntMap.find(leStuntIndex->text().toInt());
  bool bNew = (it == p->m_track.m_stuntMap.end());

  bool bLValEdited = false;
  bLValEdited |= UpdateLEEditMode(leStuntScaleFact, p->sStuntScaleFactor, bNew);
  bLValEdited |= UpdateLEEditMode(leStuntAngle, p->sStuntAngle, bNew);
  bLValEdited |= UpdateLEEditMode(leStuntUnk, p->sStuntUnknown, bNew);
  bLValEdited |= UpdateLEEditMode(leStuntTimingGroup, p->sStuntTimingGroup, bNew);
  bLValEdited |= UpdateLEEditMode(leStuntHeight, p->sStuntHeight, bNew);
  bLValEdited |= UpdateLEEditMode(leStuntTimeBulging, p->sStuntTimeBulging, bNew);
  bLValEdited |= UpdateLEEditMode(leStuntTimeFlat, p->sStuntTimeFlat, bNew);
  bLValEdited |= UpdateLEEditMode(leStuntExpandContract, p->sStuntExpandsContracts, bNew);
  bLValEdited |= UpdateLEEditMode(leStuntBulge, p->sStuntBulge, bNew);

  bool bEditMode = bNew || bLValEdited;
  pbApplyStunt->setEnabled(bEditMode);
  pbRevertStunt->setEnabled(bLValEdited);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::LoadSettings()
{
  QSettings settings(m_sSettingsFile, QSettings::IniFormat);
  m_sTrackFilesFolder = settings.value("track_folder", m_sTrackFilesFolder).toString();

  if (settings.contains("window_geometry") && settings.contains("window_state")) {
    QByteArray geometry = saveGeometry();
    QByteArray state = saveState();
    QByteArray splitterGeometry = splitter->saveGeometry();
    QByteArray splitterState = splitter->saveState();
    geometry = settings.value("window_geometry", geometry).toByteArray();
    state = settings.value("window_state", state).toByteArray();
    splitterGeometry = settings.value("splitter_geometry", splitterGeometry).toByteArray();
    splitterState = settings.value("splitter_state", splitterState).toByteArray();
    restoreGeometry(geometry);
    restoreState(state);
    splitter->restoreGeometry(splitterGeometry);
    splitter->restoreState(splitterState);
  } else {
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
  }

  show();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::SaveSettings()
{
  QSettings settings(m_sSettingsFile, QSettings::IniFormat);
  settings.setValue("track_folder", m_sTrackFilesFolder);

  QByteArray geometry = saveGeometry();
  QByteArray state = saveState();
  QByteArray splitterGeometry = splitter->saveGeometry();
  QByteArray splitterState = splitter->saveState();
  settings.setValue("window_geometry", geometry);
  settings.setValue("window_state", state);
  settings.setValue("splitter_geometry", splitterGeometry);
  settings.setValue("splitter_state", splitterState);
}

//-------------------------------------------------------------------------------------------------

bool CMainWindow::SaveChangesAndContinue()
{
  if (!m_bUnsavedChanges)
    return true;

  //init
  QMessageBox saveDiscardCancelBox(QMessageBox::Warning, "Unsaved Changes",
                                   "There are unsaved changes to the current track. Save them?",
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                   this);
  int iButton = saveDiscardCancelBox.exec();

  //cancel
  if (iButton == QMessageBox::Cancel || iButton == QMessageBox::NoButton)
    return false;

  //save
  QString sFilename = m_sTrackFile;
  if (iButton == QMessageBox::Save) {
    if (sFilename.isEmpty()) {
      QString sFilename = QDir::toNativeSeparators(QFileDialog::getSaveFileName(
        this, "Save Track File As", m_sTrackFilesFolder, QString("Track Files (*.TRK)")));
    }
    if (sFilename.isEmpty()) {
      return false;
    }
    if (!p->m_track.SaveTrack(sFilename))
      return false;
  }

  //save successful, update app
  m_sTrackFilesFolder = sFilename.left(sFilename.lastIndexOf(QDir::separator()));
  m_sTrackFile = sFilename;
  UpdateWindow();
  return true;
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateWindow()
{
  //update title bar
  QString sTitle = "Track Editor";
  if (!m_sTrackFile.isEmpty())
    sTitle = sTitle + QString(" - ") + m_sTrackFile;
  if (m_bUnsavedChanges)
    sTitle = QString("* ") + sTitle;
  setWindowTitle(sTitle);

  //update edit pane
  twEditor->setEnabled(!m_sTrackFile.isEmpty());

  //update view pane
  txData->clear();

  switch (twEditor->currentIndex()) {
    case 0: //CHUNKS
    {
      leChunkCount->setText(QString::number(p->m_track.m_chunkAy.size()));
      //stuff data
      for (int i = 0; i < p->m_track.m_chunkAy.size(); ++i) {
        txData->appendPlainText(p->m_track.m_chunkAy[i].sString.c_str());
      }

      //update selection
      sbSelChunksFrom->blockSignals(true);
      sbSelChunksTo->blockSignals(true);
      sbSelChunksFrom->setRange(0, (int)p->m_track.m_chunkAy.size() - 1);
      sbSelChunksTo->setRange(0, (int)p->m_track.m_chunkAy.size() - 1);
      sbSelChunksFrom->blockSignals(false);
      sbSelChunksTo->blockSignals(false);
      UpdateGeometrySelection();
    }
      break;
    case 1: //TUPLES
    {
      leTuplesCount->setText(QString::number(p->m_track.m_tupleMap.size()));
      //stuff data
      CTupleMap::iterator it = p->m_track.m_tupleMap.begin();
      for (; it != p->m_track.m_tupleMap.end(); ++it) {
        char szLine[20];
        snprintf(szLine, sizeof(szLine), "%5d%7d", it->first, it->second);
        txData->appendPlainText(szLine);
      }

      //update selection
      UpdateTupleSelection();
    }
      break;
    case 2: //STUNTS
    {
      leStuntsCount->setText(QString::number(p->m_track.m_stuntMap.size()));
      //stuff data
      CStuntMap::iterator it = p->m_track.m_stuntMap.begin();
      for (; it != p->m_track.m_stuntMap.end(); ++it) {
        char szLine[70];
        snprintf(szLine, sizeof(szLine), "%5d%7d%7d%7d%7d%7d%7d%7d%7d%7d", 
                 it->first, it->second.iScaleFactor, it->second.iAngle, it->second.iUnknown,
                 it->second.iTimingGroup, it->second.iHeight, it->second.iTimeBulging,
                 it->second.iTimeFlat, it->second.iSmallerExpandsLargerContracts, it->second.iBulge);
        txData->appendPlainText(szLine);
      }

      //update selection
      UpdateStuntSelection();
    }
      break;
    case 3: //TEXTURES
    {
      txData->appendPlainText("todo");
    }
      break;
    case 4: //INFO
    {
      txData->appendPlainText("todo");
    }
      break;
  }
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateGeometrySelection()
{
  //update view window selection
  int iStart = 0, iEnd = 0;
  p->m_track.GetGeometryCursorPos(sbSelChunksFrom->value(), sbSelChunksTo->value(), iStart, iEnd);
  QTextCursor c = txData->textCursor();
  c.setPosition(iStart);
  c.setPosition(iEnd, QTextCursor::KeepAnchor);
  txData->setTextCursor(c);

  //update values in edit window
  p->m_track.GetGeometryValuesFromSelection(sbSelChunksFrom->value(), sbSelChunksTo->value()
    , p->sLeftShoulderWidth, p->sLeftLaneWidth, p->sRightLaneWidth, p->sRightShoulderWidth
    , p->sUnk1, p->sUnk2, p->sUnk3
    , p->sYaw, p->sPitch, p->sRoll
    , p->sAILine1, p->sAILine2, p->sAILine3, p->sAILine4
    , p->sTrackGrip, p->sLeftShoulderGrip, p->sRightShoulderGrip
    , p->sUnk4, p->sUnk5, p->sUnk6, p->sUnk7, p->sUnk8
    , p->sLeftSurfaceType, p->sCenterSurfaceType, p->sRightSurfaceType
    , p->sUnk9, p->sUnk10, p->sUnk11, p->sUnk12, p->sUnk13, p->sUnk14
    , p->sUnk15, p->sUnk16, p->sUnk17, p->sUnk18, p->sUnk19, p->sUnk20
    , p->sfUnk1, p->sfUnk2, p->sfUnk3
    , p->sUnk21, p->sUnk22, p->sUnk23, p->sUnk24, p->sUnk25, p->sUnk26
    , p->sUnk27, p->sUnk28, p->sUnk29, p->sUnk30, p->sUnk31, p->sUnk32
    , p->sUnk33, p->sUnk34, p->sUnk35, p->sUnk36, p->sUnk37, p->sUnk38
    , p->sUnk39, p->sUnk40, p->sUnk41, p->sUnk42, p->sUnk43, p->sUnk44
    , p->sUnk45, p->sUnk46, p->sUnk47, p->sUnk48, p->sUnk49, p->sUnk50);

  RevertGeometry();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateTupleSelection()
{
  //update values in edit window
  int i = 0;
  CTupleMap::iterator it = p->m_track.m_tupleMap.begin();
  for (; it != p->m_track.m_tupleMap.end(); ++it, ++i) {
    if (leLVal->text().toInt() == it->first) {
      p->sTupleRVal = QString::number(it->second);
      break;
    }
  }

  //update view window selection
  QTextCursor c = txData->textCursor();
  c.setPosition(i * TUPLE_LINE_LENGTH);
  c.setPosition((i + 1) * TUPLE_LINE_LENGTH - 1, QTextCursor::KeepAnchor);
  txData->setTextCursor(c);
  
  RevertTuples();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateStuntSelection()
{
  //update values in edit window
  int i = 0;
  CStuntMap::iterator it = p->m_track.m_stuntMap.begin();
  for (; it != p->m_track.m_stuntMap.end(); ++it, ++i) {
    if (leStuntIndex->text().toInt() == it->first) {
      p->sStuntScaleFactor = QString::number(it->second.iScaleFactor);
      p->sStuntAngle = QString::number(it->second.iAngle);
      p->sStuntUnknown = QString::number(it->second.iUnknown);
      p->sStuntTimingGroup = QString::number(it->second.iTimingGroup);
      p->sStuntHeight = QString::number(it->second.iHeight);
      p->sStuntTimeBulging = QString::number(it->second.iTimeBulging);
      p->sStuntTimeFlat = QString::number(it->second.iTimeFlat);
      p->sStuntExpandsContracts = QString::number(it->second.iSmallerExpandsLargerContracts);
      p->sStuntBulge = QString::number(it->second.iBulge);
      break;
    }
  }

  //update view window selection
  QTextCursor c = txData->textCursor();
  c.setPosition(i * STUNT_LINE_LENGTH);
  c.setPosition((i + 1) * STUNT_LINE_LENGTH - 1, QTextCursor::KeepAnchor);
  txData->setTextCursor(c);

  RevertStunts();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateLEWithSelectionValue(QLineEdit *pLineEdit, const QString &sValue)
{
  pLineEdit->blockSignals(true);
  pLineEdit->setText(sValue);
  if (sValue.compare(MIXED_DATA) == 0) {
    pLineEdit->setStyleSheet("background-color: rgb(255,0,0)");
  } else {
    pLineEdit->setStyleSheet("");
  }
  pLineEdit->blockSignals(false);
}

//-------------------------------------------------------------------------------------------------

bool CMainWindow::UpdateLEEditMode(QLineEdit *pLineEdit, const QString &sValue, bool bNew)
{
  bool bEditMode = (pLineEdit->text().compare(sValue) != 0);
  if (bNew) {
    pLineEdit->setStyleSheet("background-color: rgb(0,255,0)");
  } else if (bEditMode) {
    pLineEdit->setStyleSheet("background-color: rgb(255,255,0)");
  } else {
    pLineEdit->setStyleSheet("");
  }
  return bEditMode;
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::RevertGeometry()
{
  UpdateLEWithSelectionValue(leLShoulderWidth, p->sLeftShoulderWidth);
  UpdateLEWithSelectionValue(leLLaneWidth, p->sLeftLaneWidth);
  UpdateLEWithSelectionValue(leRLaneWidth, p->sRightLaneWidth);
  UpdateLEWithSelectionValue(leRShoulderWidth, p->sRightShoulderWidth);
  UpdateLEWithSelectionValue(leUnk1, p->sUnk1);
  UpdateLEWithSelectionValue(leUnk2, p->sUnk2);
  UpdateLEWithSelectionValue(leUnk3, p->sUnk3);
  UpdateLEWithSelectionValue(leYaw, p->sYaw);
  UpdateLEWithSelectionValue(lePitch, p->sPitch);
  UpdateLEWithSelectionValue(leRoll, p->sRoll);
  UpdateLEWithSelectionValue(leAILine1, p->sAILine1);
  UpdateLEWithSelectionValue(leAILine2, p->sAILine2);
  UpdateLEWithSelectionValue(leAILine3, p->sAILine3);
  UpdateLEWithSelectionValue(leAILine4, p->sAILine4);
  UpdateLEWithSelectionValue(leTrackGrip, p->sTrackGrip);
  UpdateLEWithSelectionValue(leLeftShoulderGrip, p->sLeftShoulderGrip);
  UpdateLEWithSelectionValue(leRShoulderGrip, p->sRightShoulderGrip);
  UpdateLEWithSelectionValue(leUnk4, p->sUnk4);
  UpdateLEWithSelectionValue(leUnk5, p->sUnk5);
  UpdateLEWithSelectionValue(leUnk6, p->sUnk6);
  UpdateLEWithSelectionValue(leUnk7, p->sUnk7);
  UpdateLEWithSelectionValue(leUnk8, p->sUnk8);
  UpdateLEWithSelectionValue(leLeftSurfaceType, p->sLeftSurfaceType);
  UpdateLEWithSelectionValue(leCenterSurfaceType, p->sCenterSurfaceType);
  UpdateLEWithSelectionValue(leRightSurfaceType, p->sRightSurfaceType);
  UpdateLEWithSelectionValue(leUnk9, p->sUnk9);
  UpdateLEWithSelectionValue(leUnk10, p->sUnk10);
  UpdateLEWithSelectionValue(leUnk11, p->sUnk11);
  UpdateLEWithSelectionValue(leUnk12, p->sUnk12);
  UpdateLEWithSelectionValue(leUnk13, p->sUnk13);
  UpdateLEWithSelectionValue(leUnk14, p->sUnk14);
  UpdateLEWithSelectionValue(leUnk15, p->sUnk15);
  UpdateLEWithSelectionValue(leUnk16, p->sUnk16);
  UpdateLEWithSelectionValue(leUnk17, p->sUnk17);
  UpdateLEWithSelectionValue(leUnk18, p->sUnk18);
  UpdateLEWithSelectionValue(leUnk19, p->sUnk19);
  UpdateLEWithSelectionValue(leUnk20, p->sUnk20);
  UpdateLEWithSelectionValue(leFloatUnk1, p->sfUnk1);
  UpdateLEWithSelectionValue(leFloatUnk2, p->sfUnk2);
  UpdateLEWithSelectionValue(leFloatUnk3, p->sfUnk3);
  UpdateLEWithSelectionValue(leUnk21, p->sUnk21);
  UpdateLEWithSelectionValue(leUnk22, p->sUnk22);
  UpdateLEWithSelectionValue(leUnk23, p->sUnk23);
  UpdateLEWithSelectionValue(leUnk24, p->sUnk24);
  UpdateLEWithSelectionValue(leUnk25, p->sUnk25);
  UpdateLEWithSelectionValue(leUnk26, p->sUnk26);
  UpdateLEWithSelectionValue(leUnk27, p->sUnk27);
  UpdateLEWithSelectionValue(leUnk28, p->sUnk28);
  UpdateLEWithSelectionValue(leUnk29, p->sUnk29);
  UpdateLEWithSelectionValue(leUnk30, p->sUnk30);
  UpdateLEWithSelectionValue(leUnk31, p->sUnk31);
  UpdateLEWithSelectionValue(leUnk32, p->sUnk32);
  UpdateLEWithSelectionValue(leUnk33, p->sUnk33);
  UpdateLEWithSelectionValue(leUnk34, p->sUnk34);
  UpdateLEWithSelectionValue(leUnk35, p->sUnk35);
  UpdateLEWithSelectionValue(leUnk36, p->sUnk36);
  UpdateLEWithSelectionValue(leUnk37, p->sUnk37);
  UpdateLEWithSelectionValue(leUnk38, p->sUnk38);
  UpdateLEWithSelectionValue(leUnk39, p->sUnk39);
  UpdateLEWithSelectionValue(leUnk40, p->sUnk40);
  UpdateLEWithSelectionValue(leUnk41, p->sUnk41);
  UpdateLEWithSelectionValue(leUnk42, p->sUnk42);
  UpdateLEWithSelectionValue(leUnk43, p->sUnk43);
  UpdateLEWithSelectionValue(leUnk44, p->sUnk44);
  UpdateLEWithSelectionValue(leUnk45, p->sUnk45);
  UpdateLEWithSelectionValue(leUnk46, p->sUnk46);
  UpdateLEWithSelectionValue(leUnk47, p->sUnk47);
  UpdateLEWithSelectionValue(leUnk48, p->sUnk48);
  UpdateLEWithSelectionValue(leUnk49, p->sUnk49);
  UpdateLEWithSelectionValue(leUnk50, p->sUnk50);

  pbApply->setEnabled(false);
  pbCancel->setEnabled(false);
  sbSelChunksFrom->setEnabled(true);
  ckTo->setEnabled(true);
  sbSelChunksTo->setEnabled(ckTo->isChecked());
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::RevertTuples()
{
  CTupleMap::iterator it = p->m_track.m_tupleMap.find(leLVal->text().toInt());
  if (it == p->m_track.m_tupleMap.end()) {
    leLVal->setStyleSheet("background-color: rgb(0,255,0)");
    pbDeleteTuple->setEnabled(false);
  } else {
    leLVal->setStyleSheet("");
    pbDeleteTuple->setEnabled(true);
  }

  UpdateLEWithSelectionValue(leRVal, p->sTupleRVal);

  pbApplyTuple->setEnabled(false);
  pbRevertTuple->setEnabled(false);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::RevertStunts()
{
  CStuntMap::iterator it = p->m_track.m_stuntMap.find(leStuntIndex->text().toInt());
  if (it == p->m_track.m_stuntMap.end()) {
    leStuntIndex->setStyleSheet("background-color: rgb(0,255,0)");
    pbDeleteStunt->setEnabled(false);
  } else {
    leStuntIndex->setStyleSheet("");
    pbDeleteStunt->setEnabled(true);
  }

  UpdateLEWithSelectionValue(leStuntScaleFact, p->sStuntScaleFactor);
  UpdateLEWithSelectionValue(leStuntAngle, p->sStuntAngle);
  UpdateLEWithSelectionValue(leStuntUnk, p->sStuntUnknown);
  UpdateLEWithSelectionValue(leStuntTimingGroup, p->sStuntTimingGroup);
  UpdateLEWithSelectionValue(leStuntHeight, p->sStuntHeight);
  UpdateLEWithSelectionValue(leStuntTimeBulging, p->sStuntTimeBulging);
  UpdateLEWithSelectionValue(leStuntTimeFlat, p->sStuntTimeFlat);
  UpdateLEWithSelectionValue(leStuntExpandContract, p->sStuntExpandsContracts);
  UpdateLEWithSelectionValue(leStuntBulge, p->sStuntBulge);

  pbApplyStunt->setEnabled(false);
  pbRevertStunt->setEnabled(false);
}

//-------------------------------------------------------------------------------------------------