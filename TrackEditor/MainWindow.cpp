#include "TrackEditor.h"
#include "MainWindow.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include "qsettings.h"
#include "qdesktopwidget.h"
#include "Track.h"
#include "LogDialog.h"
#include "Palette.h"
#include "Texture.h"
#include "TilePicker.h"
#include "EditSurfaceDialog.h"
#include "ChunkEditValues.h"
#if defined (IS_WINDOWS)
  #include <Windows.h>
#endif
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
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
  ~CMainWindowPrivate() 
  {};

  CTrack m_track;
  CPalette m_palette;
  CTexture m_tex;
  CTexture m_bld;
  CLogDialog m_logDialog;

  //selected geometry values
  CChunkEditValues editVals;

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

CMainWindow::CMainWindow(const QString &sAppPath)
  : QMainWindow(NULL)
  , m_bUnsavedChanges(false)
  , m_bAlreadySaved(false)
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
  p->m_logDialog.hide();
  txData->setFont(QFont("Courier", 8));
  frmTex->hide();
  frmBld->hide();

  cbSignType->addItem("<none>", -1);
  for (int i = 0; i < p->signAySize; ++i) {
    cbSignType->addItem(p->signAy[i], i);
  }

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

  connect(pbInsertBefore, &QPushButton::clicked, this, &CMainWindow::OnInsertBeforeClicked);
  connect(pbInsertAfter, &QPushButton::clicked, this, &CMainWindow::OnInsertAfterClicked);
  connect(sbSelChunksFrom, SIGNAL(valueChanged(int)), this, SLOT(OnSelChunksFromChanged(int)));
  connect(sbSelChunksTo, SIGNAL(valueChanged(int)), this, SLOT(OnSelChunksToChanged(int)));
  connect(ckTo, &QCheckBox::toggled, this, &CMainWindow::OnToChecked);
  connect(pbApply, &QPushButton::clicked, this, &CMainWindow::OnApplyClicked);
  connect(pbApplyTexture, &QPushButton::clicked, this, &CMainWindow::OnApplyTextureClicked);
  connect(pbApplyInfo, &QPushButton::clicked, this, &CMainWindow::OnApplyInfoClicked);
  connect(pbCancel, &QPushButton::clicked, this, &CMainWindow::OnCancelClicked);
  connect(pbRevertTexture, &QPushButton::clicked, this, &CMainWindow::OnCancelTextureClicked);
  connect(pbRevertInfo, &QPushButton::clicked, this, &CMainWindow::OnCancelInfoClicked);
  connect(pbDelete, &QPushButton::clicked, this, &CMainWindow::OnDeleteChunkClicked);
  connect(pbEditLSurface, &QPushButton::clicked, this, &CMainWindow::OnEditLSurface);
  connect(pbEditCSurface, &QPushButton::clicked, this, &CMainWindow::OnEditCSurface);
  connect(pbEditRSurface, &QPushButton::clicked, this, &CMainWindow::OnEditRSurface);
  connect(pbEditLWall, &QPushButton::clicked, this, &CMainWindow::OnEditLWall);
  connect(pbEditRWall, &QPushButton::clicked, this, &CMainWindow::OnEditRWall);
  connect(pbEditRoof, &QPushButton::clicked, this, &CMainWindow::OnEditRoof);
  connect(pbEditLUOuterWall, &QPushButton::clicked, this, &CMainWindow::OnEditLUOuterWall);
  connect(pbEditLLOuterWall, &QPushButton::clicked, this, &CMainWindow::OnEditLLOuterWall);
  connect(pbEditOuterFloor, &QPushButton::clicked, this, &CMainWindow::OnEditOuterFloor);
  connect(pbEditRLOuterWall, &QPushButton::clicked, this, &CMainWindow::OnEditRLOuterWall);
  connect(pbEditRUOuterWall, &QPushButton::clicked, this, &CMainWindow::OnEditRUOuterWall);
  connect(pbEditEnvirFloor, &QPushButton::clicked, this, &CMainWindow::OnEditEnvirFloor);

  connect(leLShoulderWidth, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLLaneWidth, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leRLaneWidth, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leRShoulderWidth, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLShoulderHeight, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leRShoulderHeight, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLength, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
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
  connect(leUnk04, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk05, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk06, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk07, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk08, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLeftSurfaceType, &QLineEdit::textChanged, this, &CMainWindow::OnLSurfaceLEChanged);
  connect(leCenterSurfaceType, &QLineEdit::textChanged, this, &CMainWindow::OnCSurfaceLEChanged);
  connect(leRightSurfaceType, &QLineEdit::textChanged, this, &CMainWindow::OnRSurfaceLEChanged);
  connect(leLWallType, &QLineEdit::textChanged, this, &CMainWindow::OnLWallLEChanged);
  connect(leRWallType, &QLineEdit::textChanged, this, &CMainWindow::OnRWallLEChanged);
  connect(leRoofType, &QLineEdit::textChanged, this, &CMainWindow::OnRoofLEChanged);
  connect(leLUOuterWallType, &QLineEdit::textChanged, this, &CMainWindow::OnLUOuterWallChanged);
  connect(leLLOuterWallType, &QLineEdit::textChanged, this, &CMainWindow::OnLLOuterWallChanged);
  connect(leOuterFloorType, &QLineEdit::textChanged, this, &CMainWindow::OnOuterFloorChanged);
  connect(leRLOuterWallType, &QLineEdit::textChanged, this, &CMainWindow::OnRLOuterWallChanged);
  connect(leRUOuterWallType, &QLineEdit::textChanged, this, &CMainWindow::OnRUOuterWallChanged);
  connect(leEnvironmentFloorType, &QLineEdit::textChanged, this, &CMainWindow::OnEnvirFloorChanged);
  connect(leSignType, &QLineEdit::textChanged, this, &CMainWindow::OnSignTypeLEChanged);
  connect(cbSignType, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSignTypeChanged(int)));
  connect(leSignHorizOffset, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leSignVertOffset, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leSignYaw, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leSignPitch, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leSignRoll, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLUOuterWallAngle, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLLOuterWallAngle, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk23, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk24, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leRLOuterWallAngle, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leRUOuterWallAngle, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLUOuterWallHeight, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLLOuterWallHeight, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk29, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk30, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leRLOuterWallHeight, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leRUOuterWallHeight, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leRoofHeight, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leDrawOrder1, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leDrawOrder2, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leDrawOrder3, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
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
  connect(leSign, &QLineEdit::textChanged, this, &CMainWindow::OnSignLEChanged);
  connect(leBack, &QLineEdit::textChanged, this, &CMainWindow::OnBackLEChanged);
  connect(pbSign, &QPushButton::clicked, this, &CMainWindow::OnSignClicked);
  connect(pbBack, &QPushButton::clicked, this, &CMainWindow::OnBackClicked);
  connect(ckApplySign, &QCheckBox::toggled, this, &CMainWindow::OnApplySignToggled);
  connect(ckApplyBack, &QCheckBox::toggled, this, &CMainWindow::OnApplyBackToggled);

  connect(leStuntScaleFact, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leStuntAngle, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leStuntUnk, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leStuntTimingGroup, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leStuntHeight, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leStuntTimeBulging, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leStuntTimeFlat, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leStuntExpandContract, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leStuntBulge, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(pbDeleteStunt, &QPushButton::clicked, this, &CMainWindow::OnDeleteStuntClicked);

  connect(leTex, &QLineEdit::textChanged, this, &CMainWindow::UpdateTexturesEditMode);
  connect(leBld, &QLineEdit::textChanged, this, &CMainWindow::UpdateTexturesEditMode);

  connect(leTrackNum, &QLineEdit::textChanged, this, &CMainWindow::UpdateInfoEditMode);
  connect(leImpossibleLaps, &QLineEdit::textChanged, this, &CMainWindow::UpdateInfoEditMode);
  connect(leHardLaps, &QLineEdit::textChanged, this, &CMainWindow::UpdateInfoEditMode);
  connect(leTrickyLaps, &QLineEdit::textChanged, this, &CMainWindow::UpdateInfoEditMode);
  connect(leMediumLaps, &QLineEdit::textChanged, this, &CMainWindow::UpdateInfoEditMode);
  connect(leEasyLaps, &QLineEdit::textChanged, this, &CMainWindow::UpdateInfoEditMode);
  connect(leGirlieLaps, &QLineEdit::textChanged, this, &CMainWindow::UpdateInfoEditMode);
  connect(leMapSize, &QLineEdit::textChanged, this, &CMainWindow::UpdateInfoEditMode);
  connect(leMapFidelity, &QLineEdit::textChanged, this, &CMainWindow::UpdateInfoEditMode);
  connect(leInfoUnknown, &QLineEdit::textChanged, this, &CMainWindow::UpdateInfoEditMode);

  connect(ckUnmangleTextures, &QCheckBox::toggled, this, &CMainWindow::OnUnmangleTexturesToggled);

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

  sbSelChunksFrom->setValue(0);
  sbSelChunksTo->setValue(0);
  p->m_track.ClearData();
  m_sTrackFile = "";
  m_bUnsavedChanges = false;
  m_bAlreadySaved = false;
  LoadTextures();
  UpdateWindow();
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
  if (sFilename.isEmpty())
    return;

  if (!p->m_track.LoadTrack(sFilename, false)) {
    //load failed
    m_sTrackFile = "";
  } else { //load successful
    //update ui
    sbSelChunksFrom->setValue(0);
    sbSelChunksTo->setValue(0);
    
    //update variables
    m_sTrackFilesFolder = sFilename.left(sFilename.lastIndexOf(QDir::separator()));
    m_sTrackFile = sFilename;
  }
  m_bAlreadySaved = false;
  m_bUnsavedChanges = false;
  //update app
  ckUnmangleTextures->blockSignals(true);
  ckUnmangleTextures->setChecked(p->m_track.m_bIsMangled);
  ckUnmangleTextures->blockSignals(false);
  LoadTextures();
  UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnSaveTrack()
{
  if (!m_sTrackFile.isEmpty() && m_bAlreadySaved) {
    m_bUnsavedChanges = !p->m_track.SaveTrack(m_sTrackFile, false);
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
  if (!p->m_track.SaveTrack(sFilename, false))
    return;

  //save successful, update app
  m_sTrackFilesFolder = sFilename.left(sFilename.lastIndexOf(QDir::separator()));
  m_sTrackFile = sFilename;
  m_bUnsavedChanges = false;
  m_bAlreadySaved = true;
  UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnImportMangled()
{
  //check for unsaved data
  if (!SaveChangesAndContinue())
    return;

  //load track
  QString sFilename = QDir::toNativeSeparators(QFileDialog::getOpenFileName(
    this, "Load Track", m_sTrackFilesFolder, QString("Track Files (*.TRK)")));
  if (sFilename.isEmpty())
    return;

  if (!p->m_track.LoadTrack(sFilename, true)) {
    //load failed
    m_sTrackFile = "";
  } else { //load successful
    //update ui
    sbSelChunksFrom->setValue(0);
    sbSelChunksTo->setValue(0);

    //update variables
    m_sTrackFilesFolder = sFilename.left(sFilename.lastIndexOf(QDir::separator()));
    m_sTrackFile = sFilename;
  }
  m_bAlreadySaved = false;
  m_bUnsavedChanges = false;
  //update app
  ckUnmangleTextures->blockSignals(true);
  ckUnmangleTextures->setChecked(p->m_track.m_bIsMangled);
  ckUnmangleTextures->blockSignals(false);
  LoadTextures();
  UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnExportMangled()
{
#if !defined(_DEBUG)
  QMessageBox::warning(this, "Fatality!", "Not implemented yet");
#else
  //save track
  QString sFilename = QDir::toNativeSeparators(QFileDialog::getSaveFileName(
    this, "Export Mangled", m_sTrackFilesFolder, "Track Files (*.TRK)"));
  if (!p->m_track.SaveTrack(sFilename, true))
    return;
  
  //save successful, update app
  m_sTrackFilesFolder = sFilename.left(sFilename.lastIndexOf(QDir::separator()));
  UpdateWindow();
#endif
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

void CMainWindow::OnInsertBeforeClicked()
{
  CChunkEditValues editVals(leLShoulderWidth->text(), leLLaneWidth->text(), leRLaneWidth->text(), leRShoulderWidth->text()
    , leLShoulderHeight->text(), leRShoulderHeight->text(), leLength->text()
    , leYaw->text(), lePitch->text(), leRoll->text()
    , leAILine1->text(), leAILine2->text(), leAILine3->text(), leAILine4->text()
    , leTrackGrip->text(), leLeftShoulderGrip->text(), leRShoulderGrip->text()
    , leUnk04->text(), leUnk05->text(), leUnk06->text(), leUnk07->text(), leUnk08->text()
    , leLeftSurfaceType->text(), leCenterSurfaceType->text(), leRightSurfaceType->text()
    , leLWallType->text(), leRWallType->text(), leRoofType->text(), leLUOuterWallType->text(), leLLOuterWallType->text(), leOuterFloorType->text()
    , leRLOuterWallType->text(), leRUOuterWallType->text(), leEnvironmentFloorType->text(), leSignType->text(), leSignHorizOffset->text(), leSignVertOffset->text()
    , leSignYaw->text(), leSignPitch->text(), leSignRoll->text()
    , leLUOuterWallAngle->text(), leLLOuterWallAngle->text(), leUnk23->text(), leUnk24->text(), leRLOuterWallAngle->text(), leRUOuterWallAngle->text()
    , leLUOuterWallHeight->text(), leLLOuterWallHeight->text(), leUnk29->text(), leUnk30->text(), leRLOuterWallHeight->text(), leRUOuterWallHeight->text()
    , leRoofHeight->text(), leDrawOrder1->text(), leDrawOrder2->text(), leDrawOrder3->text(), leUnk37->text(), leUnk38->text()
    , leUnk39->text(), leUnk40->text(), leUnk41->text(), leUnk42->text(), leUnk43->text(), leUnk44->text()
    , leUnk45->text(), leUnk46->text(), leUnk47->text(), leUnk48->text(), leUnk49->text(), leUnk50->text()
    , pbSign->property("value").toString(), pbBack->property("value").toString()
    , leStuntScaleFact->text(), leStuntAngle->text(), leStuntUnk->text(), leStuntTimingGroup->text(), leStuntHeight->text(), leStuntTimeBulging->text()
    , leStuntTimeFlat->text(), leStuntExpandContract->text(), leStuntBulge->text());

  p->m_track.InsertGeometryChunk(sbSelChunksFrom->value(), sbInsert->value(), editVals);

  m_bUnsavedChanges = true;
  sbSelChunksFrom->blockSignals(true);
  sbSelChunksTo->blockSignals(true);
  ckTo->blockSignals(true);
  sbSelChunksFrom->setRange(0, (int)p->m_track.m_chunkAy.size() - 1);
  sbSelChunksTo->setRange(0, (int)p->m_track.m_chunkAy.size() - 1);
  sbSelChunksTo->setValue(sbSelChunksFrom->value() + sbInsert->value() - 1);
  ckTo->setChecked(sbInsert->value() > 1);
  sbSelChunksFrom->blockSignals(false);
  sbSelChunksTo->blockSignals(false);
  ckTo->blockSignals(false);
  UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnInsertAfterClicked()
{
  CChunkEditValues editVals(leLShoulderWidth->text(), leLLaneWidth->text(), leRLaneWidth->text(), leRShoulderWidth->text()
    , leLShoulderHeight->text(), leRShoulderHeight->text(), leLength->text()
    , leYaw->text(), lePitch->text(), leRoll->text()
    , leAILine1->text(), leAILine2->text(), leAILine3->text(), leAILine4->text()
    , leTrackGrip->text(), leLeftShoulderGrip->text(), leRShoulderGrip->text()
    , leUnk04->text(), leUnk05->text(), leUnk06->text(), leUnk07->text(), leUnk08->text()
    , leLeftSurfaceType->text(), leCenterSurfaceType->text(), leRightSurfaceType->text()
    , leLWallType->text(), leRWallType->text(), leRoofType->text(), leLUOuterWallType->text(), leLLOuterWallType->text(), leOuterFloorType->text()
    , leRLOuterWallType->text(), leRUOuterWallType->text(), leEnvironmentFloorType->text(), leSignType->text(), leSignHorizOffset->text(), leSignVertOffset->text()
    , leSignYaw->text(), leSignPitch->text(), leSignRoll->text()
    , leLUOuterWallAngle->text(), leLLOuterWallAngle->text(), leUnk23->text(), leUnk24->text(), leRLOuterWallAngle->text(), leRUOuterWallAngle->text()
    , leLUOuterWallHeight->text(), leLLOuterWallHeight->text(), leUnk29->text(), leUnk30->text(), leRLOuterWallHeight->text(), leRUOuterWallHeight->text()
    , leRoofHeight->text(), leDrawOrder1->text(), leDrawOrder2->text(), leDrawOrder3->text(), leUnk37->text(), leUnk38->text()
    , leUnk39->text(), leUnk40->text(), leUnk41->text(), leUnk42->text(), leUnk43->text(), leUnk44->text()
    , leUnk45->text(), leUnk46->text(), leUnk47->text(), leUnk48->text(), leUnk49->text(), leUnk50->text()
    , leSign->text(), leBack->text()
    , leStuntScaleFact->text(), leStuntAngle->text(), leStuntUnk->text(), leStuntTimingGroup->text(), leStuntHeight->text(), leStuntTimeBulging->text()
    , leStuntTimeFlat->text(), leStuntExpandContract->text(), leStuntBulge->text());

  p->m_track.InsertGeometryChunk(sbSelChunksTo->value() + 1, sbInsert->value(), editVals);

  m_bUnsavedChanges = true;
  sbSelChunksFrom->blockSignals(true);
  sbSelChunksTo->blockSignals(true);
  ckTo->blockSignals(true);
  sbSelChunksFrom->setRange(0, (int)p->m_track.m_chunkAy.size() - 1);
  sbSelChunksTo->setRange(0, (int)p->m_track.m_chunkAy.size() - 1);
  sbSelChunksFrom->setValue(sbSelChunksTo->value() + 1);
  sbSelChunksTo->setValue(sbSelChunksFrom->value() + sbInsert->value() - 1);
  ckTo->setChecked(sbInsert->value() > 1);
  sbSelChunksFrom->blockSignals(false);
  sbSelChunksTo->blockSignals(false);
  ckTo->blockSignals(false);
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
  CChunkEditValues editVals(leLShoulderWidth->text(), leLLaneWidth->text(), leRLaneWidth->text(), leRShoulderWidth->text()
    , leLShoulderHeight->text(), leRShoulderHeight->text(), leLength->text()
    , leYaw->text(), lePitch->text(), leRoll->text()
    , leAILine1->text(), leAILine2->text(), leAILine3->text(), leAILine4->text()
    , leTrackGrip->text(), leLeftShoulderGrip->text(), leRShoulderGrip->text()
    , leUnk04->text(), leUnk05->text(), leUnk06->text(), leUnk07->text(), leUnk08->text()
    , leLeftSurfaceType->text(), leCenterSurfaceType->text(), leRightSurfaceType->text()
    , leLWallType->text(), leRWallType->text(), leRoofType->text(), leLUOuterWallType->text(), leLLOuterWallType->text(), leOuterFloorType->text()
    , leRLOuterWallType->text(), leRUOuterWallType->text(), leEnvironmentFloorType->text(), leSignType->text(), leSignHorizOffset->text(), leSignVertOffset->text()
    , leSignYaw->text(), leSignPitch->text(), leSignRoll->text()
    , leLUOuterWallAngle->text(), leLLOuterWallAngle->text(), leUnk23->text(), leUnk24->text(), leRLOuterWallAngle->text(), leRUOuterWallAngle->text()
    , leLUOuterWallHeight->text(), leLLOuterWallHeight->text(), leUnk29->text(), leUnk30->text(), leRLOuterWallHeight->text(), leRUOuterWallHeight->text()
    , leRoofHeight->text(), leDrawOrder1->text(), leDrawOrder2->text(), leDrawOrder3->text(), leUnk37->text(), leUnk38->text()
    , leUnk39->text(), leUnk40->text(), leUnk41->text(), leUnk42->text(), leUnk43->text(), leUnk44->text()
    , leUnk45->text(), leUnk46->text(), leUnk47->text(), leUnk48->text(), leUnk49->text(), leUnk50->text()
    , leSign->text(), leBack->text()
    , leStuntScaleFact->text(), leStuntAngle->text(), leStuntUnk->text(), leStuntTimingGroup->text(), leStuntHeight->text(), leStuntTimeBulging->text()
    , leStuntTimeFlat->text(), leStuntExpandContract->text(), leStuntBulge->text());

  p->m_track.ApplyGeometrySettings(sbSelChunksFrom->value(), sbSelChunksTo->value(), editVals);
  m_bUnsavedChanges = true;
  UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnApplyTextureClicked()
{
  //update value
  p->m_track.m_sTextureFile = leTex->text();
  p->m_track.m_sBuildingFile = leBld->text();

  m_bUnsavedChanges = true;
  g_pMainWindow->LogMessage("Applied changes to texture");
  LoadTextures();
  UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnApplyInfoClicked()
{
  p->m_track.m_raceInfo.iTrackNumber = leTrackNum->text().toInt();
  p->m_track.m_raceInfo.iImpossibleLaps = leImpossibleLaps->text().toInt();
  p->m_track.m_raceInfo.iHardLaps = leHardLaps->text().toInt();
  p->m_track.m_raceInfo.iTrickyLaps = leTrickyLaps->text().toInt();
  p->m_track.m_raceInfo.iMediumLaps = leMediumLaps->text().toInt();
  p->m_track.m_raceInfo.iEasyLaps = leEasyLaps->text().toInt();
  p->m_track.m_raceInfo.iGirlieLaps = leGirlieLaps->text().toInt();
  p->m_track.m_raceInfo.dTrackMapSize = leMapSize->text().toDouble();
  p->m_track.m_raceInfo.iTrackMapFidelity = leMapFidelity->text().toInt();
  p->m_track.m_raceInfo.dUnknown = leInfoUnknown->text().toDouble();

  m_bUnsavedChanges = true;
  g_pMainWindow->LogMessage("Applied changes to track info");
  UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnCancelClicked()
{
  RevertGeometry();
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnCancelTextureClicked()
{
  p->sTex = p->m_track.m_sTextureFile;
  p->sBld = p->m_track.m_sBuildingFile;
  UpdateLEWithSelectionValue(leTex, p->sTex);
  UpdateLEWithSelectionValue(leBld, p->sBld);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnCancelInfoClicked()
{
  RevertInfo();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnDeleteChunkClicked()
{
  if (p->m_track.m_chunkAy.empty()) return;
  if (sbSelChunksFrom->value() > sbSelChunksTo->value() || sbSelChunksTo->value() > p->m_track.m_chunkAy.size()) {
    assert(0);
    return;
  }
  p->m_track.m_chunkAy.erase(
    p->m_track.m_chunkAy.begin() + sbSelChunksFrom->value(), 
    p->m_track.m_chunkAy.begin() + sbSelChunksTo->value() + 1);

  m_bUnsavedChanges = true;
  g_pMainWindow->LogMessage("Deleted geometry chunk");
  sbSelChunksFrom->blockSignals(true);
  sbSelChunksTo->blockSignals(true);
  UpdateWindow();
  sbSelChunksTo->setValue(sbSelChunksFrom->value());
  sbSelChunksFrom->blockSignals(false);
  sbSelChunksTo->blockSignals(false);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditLSurface()
{
  int iValue = leLeftSurfaceType->text().toInt();

  CEditSurfaceDialog dlg(this, &p->m_tex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leLeftSurfaceType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leLeftSurfaceType, lblLSurfaceTex1, lblLSurfaceTex2);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditCSurface()
{
  int iValue = leCenterSurfaceType->text().toInt();

  CEditSurfaceDialog dlg(this, &p->m_tex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leCenterSurfaceType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leCenterSurfaceType, lblCSurfaceTex1, lblCSurfaceTex2);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditRSurface()
{
  int iValue = leRightSurfaceType->text().toInt();

  CEditSurfaceDialog dlg(this, &p->m_tex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leRightSurfaceType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leRightSurfaceType, lblRSurfaceTex1, lblRSurfaceTex2);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditLWall()
{
  int iValue = leLWallType->text().toInt();

  CEditSurfaceDialog dlg(this, &p->m_tex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leLWallType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leLWallType, lblLWallTex1, lblLWallTex2);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditRWall()
{
  int iValue = leRWallType->text().toInt();

  CEditSurfaceDialog dlg(this, &p->m_tex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leRWallType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leRWallType, lblRWallTex1, lblRWallTex2);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditRoof()
{
  int iValue = leRoofType->text().toInt();

  CEditSurfaceDialog dlg(this, &p->m_tex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leRoofType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leRoofType, lblRoofTex1, lblRoofTex2);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditLUOuterWall()
{
  int iValue = leLUOuterWallType->text().toInt();

  CEditSurfaceDialog dlg(this, &p->m_tex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leLUOuterWallType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leLUOuterWallType, lblLUOuterWallTex1, lblLUOuterWallTex2);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditLLOuterWall()
{
  int iValue = leLLOuterWallType->text().toInt();

  CEditSurfaceDialog dlg(this, &p->m_tex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leLLOuterWallType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leLLOuterWallType, lblLLOuterWallTex1, lblLLOuterWallTex2);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditOuterFloor()
{
  int iValue = leOuterFloorType->text().toInt();

  CEditSurfaceDialog dlg(this, &p->m_tex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leOuterFloorType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leOuterFloorType, lblOuterFloorTex1, lblOuterFloorTex2);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditRLOuterWall()
{
  int iValue = leRLOuterWallType->text().toInt();

  CEditSurfaceDialog dlg(this, &p->m_tex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leRLOuterWallType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leRLOuterWallType, lblRLOuterWallTex1, lblRLOuterWallTex2);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditRUOuterWall()
{
  int iValue = leRUOuterWallType->text().toInt();

  CEditSurfaceDialog dlg(this, &p->m_tex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leRUOuterWallType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leRUOuterWallType, lblRUOuterWallTex1, lblRUOuterWallTex2);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditEnvirFloor()
{
  int iValue = leEnvironmentFloorType->text().toInt();

  CEditSurfaceDialog dlg(this, &p->m_tex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leEnvironmentFloorType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leEnvironmentFloorType, lblEnvirFloorTex1, lblEnvirFloorTex2);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateGeometryEditMode()
{
  bool bEditMode = false;
  bool bMixedData = false;
  UpdateLEEditMode(bEditMode, bMixedData, leLShoulderWidth, p->editVals.sLeftShoulderWidth);
  UpdateLEEditMode(bEditMode, bMixedData, leLLaneWidth, p->editVals.sLeftLaneWidth);
  UpdateLEEditMode(bEditMode, bMixedData, leRLaneWidth, p->editVals.sRightLaneWidth);
  UpdateLEEditMode(bEditMode, bMixedData, leRShoulderWidth, p->editVals.sRightShoulderWidth);
  UpdateLEEditMode(bEditMode, bMixedData, leLShoulderHeight, p->editVals.sLShoulderHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leRShoulderHeight, p->editVals.sRShoulderHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leLength, p->editVals.sLength);
  UpdateLEEditMode(bEditMode, bMixedData, leYaw, p->editVals.sYaw);
  UpdateLEEditMode(bEditMode, bMixedData, lePitch, p->editVals.sPitch);
  UpdateLEEditMode(bEditMode, bMixedData, leRoll, p->editVals.sRoll);
  UpdateLEEditMode(bEditMode, bMixedData, leAILine1, p->editVals.sAILine1);
  UpdateLEEditMode(bEditMode, bMixedData, leAILine2, p->editVals.sAILine2);
  UpdateLEEditMode(bEditMode, bMixedData, leAILine3, p->editVals.sAILine3);
  UpdateLEEditMode(bEditMode, bMixedData, leAILine4, p->editVals.sAILine4);
  UpdateLEEditMode(bEditMode, bMixedData, leTrackGrip, p->editVals.sTrackGrip);
  UpdateLEEditMode(bEditMode, bMixedData, leLeftShoulderGrip, p->editVals.sLeftShoulderGrip);
  UpdateLEEditMode(bEditMode, bMixedData, leRShoulderGrip, p->editVals.sRightShoulderGrip);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk04, p->editVals.sUnk04);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk05, p->editVals.sUnk05);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk06, p->editVals.sUnk06);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk07, p->editVals.sUnk07);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk08, p->editVals.sUnk08);
  UpdateLEEditMode(bEditMode, bMixedData, leLeftSurfaceType, p->editVals.sLeftSurfaceType);
  UpdateLEEditMode(bEditMode, bMixedData, leCenterSurfaceType, p->editVals.sCenterSurfaceType);
  UpdateLEEditMode(bEditMode, bMixedData, leRightSurfaceType, p->editVals.sRightSurfaceType);
  UpdateLEEditMode(bEditMode, bMixedData, leLWallType, p->editVals.sLWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leRWallType, p->editVals.sRWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leRoofType, p->editVals.sRoofType);
  UpdateLEEditMode(bEditMode, bMixedData, leLUOuterWallType, p->editVals.sLUOuterWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leLLOuterWallType, p->editVals.sLLOuterWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leOuterFloorType, p->editVals.sOuterFloorType);
  UpdateLEEditMode(bEditMode, bMixedData, leRLOuterWallType, p->editVals.sRLOuterWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leRUOuterWallType, p->editVals.sRUOuterWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leEnvironmentFloorType, p->editVals.sEnvironmentFloorType);
  UpdateLEEditMode(bEditMode, bMixedData, leSignType, p->editVals.sSignType);
  UpdateCBEditMode(bEditMode, bMixedData, cbSignType, p->editVals.sSignType);
  UpdateLEEditMode(bEditMode, bMixedData, leSignHorizOffset, p->editVals.sSignHorizOffset);
  UpdateLEEditMode(bEditMode, bMixedData, leSignVertOffset, p->editVals.sSignVertOffset);
  UpdateLEEditMode(bEditMode, bMixedData, leSignYaw, p->editVals.sSignYaw);
  UpdateLEEditMode(bEditMode, bMixedData, leSignPitch, p->editVals.sSignPitch);
  UpdateLEEditMode(bEditMode, bMixedData, leSignRoll, p->editVals.sSignRoll);
  UpdateLEEditMode(bEditMode, bMixedData, leLUOuterWallAngle, p->editVals.sLUOuterWallAngle);
  UpdateLEEditMode(bEditMode, bMixedData, leLLOuterWallAngle, p->editVals.sLLOuterWallAngle);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk23, p->editVals.sUnk23);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk24, p->editVals.sUnk24);
  UpdateLEEditMode(bEditMode, bMixedData, leRLOuterWallAngle, p->editVals.sRLOuterWallAngle);
  UpdateLEEditMode(bEditMode, bMixedData, leRUOuterWallAngle, p->editVals.sRUOuterWallAngle);
  UpdateLEEditMode(bEditMode, bMixedData, leLUOuterWallHeight, p->editVals.sLUOuterWallHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leLLOuterWallHeight, p->editVals.sLLOuterWallHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk29, p->editVals.sUnk29);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk30, p->editVals.sUnk30);
  UpdateLEEditMode(bEditMode, bMixedData, leRLOuterWallHeight, p->editVals.sRLOuterWallHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leRUOuterWallHeight, p->editVals.sRUOuterWallHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leRoofHeight, p->editVals.sRoofHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leDrawOrder1, p->editVals.sDrawOrder1);
  UpdateLEEditMode(bEditMode, bMixedData, leDrawOrder2, p->editVals.sDrawOrder2);
  UpdateLEEditMode(bEditMode, bMixedData, leDrawOrder3, p->editVals.sDrawOrder3);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk37, p->editVals.sUnk37);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk38, p->editVals.sUnk38);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk39, p->editVals.sUnk39);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk40, p->editVals.sUnk40);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk41, p->editVals.sUnk41);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk42, p->editVals.sUnk42);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk43, p->editVals.sUnk43);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk44, p->editVals.sUnk44);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk45, p->editVals.sUnk45);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk46, p->editVals.sUnk46);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk47, p->editVals.sUnk47);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk48, p->editVals.sUnk48);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk49, p->editVals.sUnk49);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk50, p->editVals.sUnk50);
  UpdateSignEditMode(bEditMode, bMixedData, leSign, widgetSign, p->editVals.sSignTexture);
  UpdateSignEditMode(bEditMode, bMixedData, leBack, widgetBack, p->editVals.sBackTexture);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntScaleFact, p->editVals.sStuntScaleFactor);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntAngle, p->editVals.sStuntAngle);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntUnk, p->editVals.sStuntUnknown);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntTimingGroup, p->editVals.sStuntTimingGroup);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntHeight, p->editVals.sStuntHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntTimeBulging, p->editVals.sStuntTimeBulging);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntTimeFlat, p->editVals.sStuntTimeFlat);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntExpandContract, p->editVals.sStuntExpandsContracts);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntBulge, p->editVals.sStuntBulge);

  pbApply->setEnabled(bEditMode);
  pbCancel->setEnabled(bEditMode);
  pbInsertAfter->setEnabled(!bMixedData);
  pbInsertBefore->setEnabled(!bMixedData);
  sbSelChunksFrom->setEnabled(!bEditMode);
  ckTo->setEnabled(!bEditMode);
  sbSelChunksTo->setEnabled(!bEditMode && ckTo->isChecked());
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateTexturesEditMode()
{
  bool bLValEdited = false;
  bool bMixedData = false;
  UpdateLEEditMode(bLValEdited, bMixedData, leTex, p->sTex);
  UpdateLEEditMode(bLValEdited, bMixedData, leBld, p->sBld);

  bool bEditMode = bLValEdited;
  pbApplyTexture->setEnabled(bEditMode);
  pbRevertTexture->setEnabled(bLValEdited);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateInfoEditMode()
{
  bool bEditMode = false;
  bool bMixedData = false;
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

void CMainWindow::OnSignClicked()
{
  QString sValue = leSign->text();
  int iValue = sValue.toInt();
  int iBldIndex = iValue & SURFACE_TEXTURE_INDEX;

  CTilePicker dlg(this, &p->m_bld, iBldIndex, true);
  if (dlg.exec()) {
    int iIndex = dlg.GetSelected();
    if (iIndex >= 0) {
      iBldIndex = iIndex;
      iValue = iBldIndex;
      if (ckApplySign->isChecked()) {
        iValue |= SURFACE_FLAG_APPLY_TEXTURE;
      } else {
        iValue &= ~SURFACE_FLAG_APPLY_TEXTURE;
      }
      sValue = QString::number(iValue);
    }
  }

  leSign->setText(sValue);
  UpdateSignButtonDisplay(pbSign, ckApplySign, leSign, &p->m_bld);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnBackClicked()
{
  QString sValue = leBack->text();
  int iValue = sValue.toInt();
  int iBldIndex = iValue & SURFACE_TEXTURE_INDEX;

  CTilePicker dlg(this, &p->m_tex, iBldIndex, true);
  if (dlg.exec()) {
    int iIndex = dlg.GetSelected();
    if (iIndex >= 0) {
      iBldIndex = iIndex;
      iValue = iBldIndex;
      if (ckApplyBack->isChecked()) {
        iValue |= SURFACE_FLAG_APPLY_TEXTURE;
      } else {
        iValue &= ~SURFACE_FLAG_APPLY_TEXTURE;
      }
    }
    sValue = QString::number(iValue);
  }

  leBack->setText(sValue);
  UpdateSignButtonDisplay(pbBack, ckApplyBack, leBack, &p->m_tex);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnApplySignToggled(bool bChecked)
{
  int iValue = leSign->text().toInt();
  if (bChecked) {
    iValue |= SURFACE_FLAG_APPLY_TEXTURE;
  } else {
    iValue &= ~SURFACE_FLAG_APPLY_TEXTURE;
  }
  QString sNewValue = QString::number(iValue);
  leSign->setText(sNewValue);
  UpdateSignButtonDisplay(pbSign, ckApplySign, leSign, &p->m_bld);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnApplyBackToggled(bool bChecked)
{
  int iValue = leBack->text().toInt();
  if (bChecked) {
    iValue |= SURFACE_FLAG_APPLY_TEXTURE;
  } else {
    iValue &= ~SURFACE_FLAG_APPLY_TEXTURE;
  }
  QString sNewValue = QString::number(iValue);
  leBack->setText(sNewValue);
  UpdateSignButtonDisplay(pbBack, ckApplyBack, leBack, &p->m_tex);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnDeleteStuntClicked()
{
  leStuntScaleFact->blockSignals(true);
  leStuntAngle->blockSignals(true);
  leStuntUnk->blockSignals(true);
  leStuntTimingGroup->blockSignals(true);
  leStuntHeight->blockSignals(true);
  leStuntTimeBulging->blockSignals(true);
  leStuntTimeFlat->blockSignals(true);
  leStuntExpandContract->blockSignals(true);
  leStuntBulge->blockSignals(true);

  leStuntScaleFact->setText("0");
  leStuntAngle->setText("0");
  leStuntUnk->setText("0");
  leStuntTimingGroup->setText("0");
  leStuntHeight->setText("0");
  leStuntTimeBulging->setText("0");
  leStuntTimeFlat->setText("0");
  leStuntExpandContract->setText("0");
  leStuntBulge->setText("0");

  leStuntScaleFact->blockSignals(false);
  leStuntAngle->blockSignals(false);
  leStuntUnk->blockSignals(false);
  leStuntTimingGroup->blockSignals(false);
  leStuntHeight->blockSignals(false);
  leStuntTimeBulging->blockSignals(false);
  leStuntTimeFlat->blockSignals(false);
  leStuntExpandContract->blockSignals(false);
  leStuntBulge->blockSignals(false);

  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnSignTypeChanged(int iIndex)
{
  int iData = cbSignType->itemData(iIndex).toInt();
  leSignType->blockSignals(true);
  leSignType->setText(QString::number(iData));
  leSignType->blockSignals(false);

  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnSignTypeLEChanged()
{
  cbSignType->blockSignals(true);
  cbSignType->setCurrentIndex(cbSignType->findData(leSignType->text().toInt()));
  cbSignType->blockSignals(false);

  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnSignLEChanged()
{
  UpdateSignButtonDisplay(pbSign, ckApplySign, leSign, &p->m_bld);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnBackLEChanged()
{
  UpdateSignButtonDisplay(pbBack, ckApplyBack, leBack, &p->m_tex);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnLSurfaceLEChanged()
{
  UpdateTextures(leLeftSurfaceType, lblLSurfaceTex1, lblLSurfaceTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnCSurfaceLEChanged()
{
  UpdateTextures(leCenterSurfaceType, lblCSurfaceTex1, lblCSurfaceTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnRSurfaceLEChanged()
{
  UpdateTextures(leRightSurfaceType, lblRSurfaceTex1, lblRSurfaceTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnLWallLEChanged()
{
  UpdateTextures(leLWallType, lblLWallTex1, lblLWallTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnRWallLEChanged()
{
  UpdateTextures(leRWallType, lblRWallTex1, lblRWallTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnRoofLEChanged()
{
  UpdateTextures(leRoofType, lblRoofTex1, lblRoofTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnLUOuterWallChanged()
{
  UpdateTextures(leLUOuterWallType, lblLUOuterWallTex1, lblLUOuterWallTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnLLOuterWallChanged()
{
  UpdateTextures(leLLOuterWallType, lblLLOuterWallTex1, lblLLOuterWallTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnOuterFloorChanged()
{
  UpdateTextures(leOuterFloorType, lblOuterFloorTex1, lblOuterFloorTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnRLOuterWallChanged()
{
  UpdateTextures(leRLOuterWallType, lblRLOuterWallTex1, lblRLOuterWallTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnRUOuterWallChanged()
{
  UpdateTextures(leRUOuterWallType, lblRUOuterWallTex1, lblRUOuterWallTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEnvirFloorChanged()
{
  UpdateTextures(leEnvironmentFloorType, lblEnvirFloorTex1, lblEnvirFloorTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnUnmangleTexturesToggled(bool bChecked)
{
  (void)(bChecked);
  LoadTextures();
  UpdateWindow();
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
    if (!p->m_track.SaveTrack(sFilename, false))
      return false;
    m_sTrackFilesFolder = sFilename.left(sFilename.lastIndexOf(QDir::separator()));
  }

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
      sbInsert->setRange(1, 65535 - (int)p->m_track.m_chunkAy.size());
      sbSelChunksFrom->setRange(0, (int)p->m_track.m_chunkAy.size() - 1);
      sbSelChunksTo->setRange(0, (int)p->m_track.m_chunkAy.size() - 1);
      sbSelChunksFrom->blockSignals(false);
      sbSelChunksTo->blockSignals(false);
      UpdateGeometrySelection();
      UpdateGeometryEditMode();
    }
      break;
    case 1: //TEXTURES
    {
      //stuff data
      txData->appendPlainText("TEX:" + p->m_track.m_sTextureFile);
      txData->appendPlainText("BLD:" + p->m_track.m_sBuildingFile);

      CSignMap backsMap;
      for (int i = 0; i < p->m_track.m_chunkAy.size(); ++i) {
        if (p->m_track.m_chunkAy[i].iBackTexture > 0)
          backsMap[i] = p->m_track.m_chunkAy[i].iBackTexture;
      }
      
      CSignMap::iterator it = backsMap.begin();
      for (; it != backsMap.end(); ++it) {
        char szLine[20];
        memset(szLine, 0, sizeof(szLine));
        snprintf(szLine, sizeof(szLine), "%d %d", it->first, it->second);
        txData->appendPlainText(szLine);
      }

      //update selection
      p->sTex = p->m_track.m_sTextureFile;
      p->sBld = p->m_track.m_sBuildingFile;
      UpdateLEWithSelectionValue(leTex, p->sTex);
      UpdateLEWithSelectionValue(leBld, p->sBld);
      UpdateTexturesEditMode();
    }
      break;
    case 2: //INFO
    {
      //stuff data
      txData->appendPlainText(QString::number(p->m_track.m_raceInfo.iTrackNumber));
      char szLine[50];
      memset(szLine, 0, sizeof(szLine));
      snprintf(szLine, sizeof(szLine), "%4d %4d %4d %4d %4d %4d",
               p->m_track.m_raceInfo.iImpossibleLaps, p->m_track.m_raceInfo.iHardLaps, p->m_track.m_raceInfo.iTrickyLaps,
               p->m_track.m_raceInfo.iMediumLaps, p->m_track.m_raceInfo.iEasyLaps, p->m_track.m_raceInfo.iGirlieLaps);
      txData->appendPlainText(szLine);
      memset(szLine, 0, sizeof(szLine));
      snprintf(szLine, sizeof(szLine), "%.2lf %4d %.2lf",
               p->m_track.m_raceInfo.dTrackMapSize, p->m_track.m_raceInfo.iTrackMapFidelity, p->m_track.m_raceInfo.dUnknown);
      txData->appendPlainText(szLine);

      //update selection
      UpdateInfoSelection();
      UpdateInfoEditMode();
    }
      break;
  }
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::LoadTextures()
{
  bool bMangled = ckUnmangleTextures->isChecked();

  //avoid memory leak
  QLayoutItem *pItem;
  QLayout *pSublayout;
  QWidget *pWidget;
  while ((pItem = texLayout->takeAt(0))) {
    if ((pSublayout = pItem->layout()) != 0) {
      //should not have any of these
      assert(0);
    } else if ((pWidget = pItem->widget()) != 0) {
      pWidget->hide();
      delete pWidget;
    } else { 
      delete pItem;
    }
  }
  while ((pItem = bldLayout->takeAt(0))) {
    if ((pSublayout = pItem->layout()) != 0) {
      //should not have any of these
      assert(0);
    } else if ((pWidget = pItem->widget()) != 0) {
      pWidget->hide();
      delete pWidget;
    } else {
      delete pItem;
    }
  }
  assert(texLayout->isEmpty());
  assert(bldLayout->isEmpty());

  //load textures
  bool bPalLoaded = p->m_palette.LoadPalette(m_sTrackFilesFolder + QDir::separator() + "PALETTE.PAL");
  bool bTexLoaded = p->m_tex.LoadTexture(m_sTrackFilesFolder + QDir::separator() + p->m_track.m_sTextureFile, p->m_palette, bMangled);
  bool bBldLoaded = p->m_bld.LoadTexture(m_sTrackFilesFolder + QDir::separator() + p->m_track.m_sBuildingFile, p->m_palette, bMangled);
  lblPalletteLoaded->setVisible(!bPalLoaded);
  frmTex->setVisible(bTexLoaded);
  frmBld->setVisible(bBldLoaded);
  lblTextureLoaded->setText(bTexLoaded ? p->m_track.m_sTextureFile : "Texture not loaded");
  lblBuildingsLoaded->setText(bBldLoaded ? p->m_track.m_sBuildingFile : "Buildings not loaded");

  //add tiles to viewer layouts
  int iTilesPerLine = (twViewer->width() - 256) / (TILE_WIDTH + 6);
  for (int i = 0; i < p->m_tex.m_tileAy.size(); ++i) {
    QLabel *pImageLabel = new QLabel();
    QPixmap pixmap;
    pixmap.convertFromImage(p->m_tex.m_tileAy[i]);
    pImageLabel->setPixmap(pixmap);
    texLayout->addWidget(pImageLabel, i / iTilesPerLine, i % iTilesPerLine);
  }
  for (int i = 0; i < p->m_bld.m_tileAy.size(); ++i) {
    QLabel *pImageLabel = new QLabel();
    QPixmap pixmap;
    pixmap.convertFromImage(p->m_bld.m_tileAy[i]);
    pImageLabel->setPixmap(pixmap);
    bldLayout->addWidget(pImageLabel, i / iTilesPerLine, i % iTilesPerLine);
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
  p->m_track.GetGeometryValuesFromSelection(sbSelChunksFrom->value(), sbSelChunksTo->value(), p->editVals);

  RevertGeometry();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateInfoSelection()
{
  p->sTrackNumber = QString::number(p->m_track.m_raceInfo.iTrackNumber);
  p->sImpossibleLaps = QString::number(p->m_track.m_raceInfo.iImpossibleLaps);
  p->sHardLaps = QString::number(p->m_track.m_raceInfo.iHardLaps);
  p->sTrickyLaps = QString::number(p->m_track.m_raceInfo.iTrickyLaps);
  p->sMediumLaps = QString::number(p->m_track.m_raceInfo.iMediumLaps);
  p->sEasyLaps = QString::number(p->m_track.m_raceInfo.iEasyLaps);
  p->sGirlieLaps = QString::number(p->m_track.m_raceInfo.iGirlieLaps);
  p->sTrackMapSize = QString::number(p->m_track.m_raceInfo.dTrackMapSize, 'f', 2);
  p->sTrackMapFidelity = QString::number(p->m_track.m_raceInfo.iTrackMapFidelity);
  p->sInfoUnknown = QString::number(p->m_track.m_raceInfo.dUnknown, 'f', 2);

  RevertInfo();
}

//-------------------------------------------------------------------------------------------------

bool CMainWindow::UpdateLEWithSelectionValue(QLineEdit *pLineEdit, const QString &sValue)
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

bool CMainWindow::UpdateCBWithSelectionValue(QComboBox *pComboBox, const QString &sValue)
{
  std::string sVal = sValue.toLatin1().constData();
  pComboBox->blockSignals(true);
  if (sValue.compare(MIXED_DATA) == 0) {
    if (pComboBox->findData(MIXED_DATA) == -1)
      pComboBox->addItem(MIXED_DATA, MIXED_DATA);
    pComboBox->setCurrentIndex(pComboBox->findData(MIXED_DATA));
  } else {
    pComboBox->removeItem(pComboBox->findData(MIXED_DATA));
    pComboBox->setCurrentIndex(pComboBox->findData(sValue.toInt()));
  }
  pComboBox->blockSignals(false);
  return (sValue.compare(MIXED_DATA) == 0);
}

//-------------------------------------------------------------------------------------------------

bool CMainWindow::UpdateSignWithSelectionValue(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, const QString &sValue, CTexture *pTex)
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
  UpdateSignButtonDisplay(pPushButton, pCheckbox, pLineEdit, pTex);
  return (sValue.compare(MIXED_DATA) == 0) && pLineEdit->text().isEmpty();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateSignButtonDisplay(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, CTexture *pTex)
{
  pCheckbox->blockSignals(true);
  if (pLineEdit->text().compare("-1") == 0) {
    pPushButton->setIcon(QIcon());
    pPushButton->setText("<none>");
    pCheckbox->setEnabled(false);
    pCheckbox->setChecked(false);
  } else if (pLineEdit->text().isEmpty() && pLineEdit->placeholderText().compare(MIXED_DATA) == 0) {
    pPushButton->setIcon(QIcon());
    pPushButton->setText(MIXED_DATA);
    pCheckbox->setEnabled(false);
    pCheckbox->setChecked(false);
  } else {
    int iValue = pLineEdit->text().toInt();
    bool bChecked = iValue & SURFACE_FLAG_APPLY_TEXTURE;
    int iBldIndex = iValue & SURFACE_TEXTURE_INDEX;
    if (iBldIndex < pTex->m_tileAy.size()) {
      QPixmap pixmap;
      pixmap.convertFromImage(pTex->m_tileAy[iBldIndex]);
      pPushButton->setIcon(pixmap);
      pPushButton->setText("");
    }
    pCheckbox->setEnabled(true);
    pCheckbox->setChecked(bChecked);
  }
  pCheckbox->blockSignals(false);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateLEEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, const QString &sValue)
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

void CMainWindow::UpdateCBEditMode(bool &bEdited, bool &bMixedData, QComboBox *pComboBox, const QString &sValue)
{
  QString sCbxVal = pComboBox->currentData().toString();
  if (sCbxVal.compare(sValue) != 0 && pComboBox->currentIndex() != -1) {
    if (sCbxVal.compare(MIXED_DATA) == 0) {
      bMixedData = true;
      pComboBox->setStyleSheet("");
    } else {
      bEdited = true;
      pComboBox->setStyleSheet("background-color: rgb(255,255,0)");
    }
  } else {
    pComboBox->setStyleSheet("");
  }
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateSignEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, QWidget *pWidget, const QString &sValue)
{
  QString sLineEditVal = pLineEdit->text();
  if (sLineEditVal.isEmpty())
    sLineEditVal = "0";

  if (sLineEditVal.compare(sValue) != 0) {
    if (pLineEdit->text().isEmpty() && pLineEdit->placeholderText().compare(MIXED_DATA) == 0) {
      bMixedData = true;
      pWidget->setStyleSheet("");
    } else {
      bEdited = true;
      pWidget->setStyleSheet("background-color: rgb(255,255,0)");
    }
  } else {
    pWidget->setStyleSheet("");
  }
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::RevertGeometry()
{
  pbDelete->setEnabled(!p->m_track.m_chunkAy.empty());

  bool bMixedData = false;
  bMixedData |= UpdateLEWithSelectionValue(leLShoulderWidth, p->editVals.sLeftShoulderWidth);
  bMixedData |= UpdateLEWithSelectionValue(leLLaneWidth, p->editVals.sLeftLaneWidth);
  bMixedData |= UpdateLEWithSelectionValue(leRLaneWidth, p->editVals.sRightLaneWidth);
  bMixedData |= UpdateLEWithSelectionValue(leRShoulderWidth, p->editVals.sRightShoulderWidth);
  bMixedData |= UpdateLEWithSelectionValue(leLShoulderHeight, p->editVals.sLShoulderHeight);
  bMixedData |= UpdateLEWithSelectionValue(leRShoulderHeight, p->editVals.sRShoulderHeight);
  bMixedData |= UpdateLEWithSelectionValue(leLength, p->editVals.sLength);
  bMixedData |= UpdateLEWithSelectionValue(leYaw, p->editVals.sYaw);
  bMixedData |= UpdateLEWithSelectionValue(lePitch, p->editVals.sPitch);
  bMixedData |= UpdateLEWithSelectionValue(leRoll, p->editVals.sRoll);
  bMixedData |= UpdateLEWithSelectionValue(leAILine1, p->editVals.sAILine1);
  bMixedData |= UpdateLEWithSelectionValue(leAILine2, p->editVals.sAILine2);
  bMixedData |= UpdateLEWithSelectionValue(leAILine3, p->editVals.sAILine3);
  bMixedData |= UpdateLEWithSelectionValue(leAILine4, p->editVals.sAILine4);
  bMixedData |= UpdateLEWithSelectionValue(leTrackGrip, p->editVals.sTrackGrip);
  bMixedData |= UpdateLEWithSelectionValue(leLeftShoulderGrip, p->editVals.sLeftShoulderGrip);
  bMixedData |= UpdateLEWithSelectionValue(leRShoulderGrip, p->editVals.sRightShoulderGrip);
  bMixedData |= UpdateLEWithSelectionValue(leUnk04, p->editVals.sUnk04);
  bMixedData |= UpdateLEWithSelectionValue(leUnk05, p->editVals.sUnk05);
  bMixedData |= UpdateLEWithSelectionValue(leUnk06, p->editVals.sUnk06);
  bMixedData |= UpdateLEWithSelectionValue(leUnk07, p->editVals.sUnk07);
  bMixedData |= UpdateLEWithSelectionValue(leUnk08, p->editVals.sUnk08);
  bMixedData |= UpdateLEWithSelectionValue(leLeftSurfaceType, p->editVals.sLeftSurfaceType);
  bMixedData |= UpdateLEWithSelectionValue(leCenterSurfaceType, p->editVals.sCenterSurfaceType);
  bMixedData |= UpdateLEWithSelectionValue(leRightSurfaceType, p->editVals.sRightSurfaceType);
  bMixedData |= UpdateLEWithSelectionValue(leLWallType, p->editVals.sLWallType);
  bMixedData |= UpdateLEWithSelectionValue(leRWallType, p->editVals.sRWallType);
  bMixedData |= UpdateLEWithSelectionValue(leRoofType, p->editVals.sRoofType);
  bMixedData |= UpdateLEWithSelectionValue(leLUOuterWallType, p->editVals.sLUOuterWallType);
  bMixedData |= UpdateLEWithSelectionValue(leLLOuterWallType, p->editVals.sLLOuterWallType);
  bMixedData |= UpdateLEWithSelectionValue(leOuterFloorType, p->editVals.sOuterFloorType);
  bMixedData |= UpdateLEWithSelectionValue(leRLOuterWallType, p->editVals.sRLOuterWallType);
  bMixedData |= UpdateLEWithSelectionValue(leRUOuterWallType, p->editVals.sRUOuterWallType);
  bMixedData |= UpdateLEWithSelectionValue(leEnvironmentFloorType, p->editVals.sEnvironmentFloorType);
  bMixedData |= UpdateLEWithSelectionValue(leSignType, p->editVals.sSignType);
  bMixedData |= UpdateCBWithSelectionValue(cbSignType, p->editVals.sSignType);
  bMixedData |= UpdateLEWithSelectionValue(leSignHorizOffset, p->editVals.sSignHorizOffset);
  bMixedData |= UpdateLEWithSelectionValue(leSignVertOffset, p->editVals.sSignVertOffset);
  bMixedData |= UpdateLEWithSelectionValue(leSignYaw, p->editVals.sSignYaw);
  bMixedData |= UpdateLEWithSelectionValue(leSignPitch, p->editVals.sSignPitch);
  bMixedData |= UpdateLEWithSelectionValue(leSignRoll, p->editVals.sSignRoll);
  bMixedData |= UpdateLEWithSelectionValue(leLUOuterWallAngle, p->editVals.sLUOuterWallAngle);
  bMixedData |= UpdateLEWithSelectionValue(leLLOuterWallAngle, p->editVals.sLLOuterWallAngle);
  bMixedData |= UpdateLEWithSelectionValue(leUnk23, p->editVals.sUnk23);
  bMixedData |= UpdateLEWithSelectionValue(leUnk24, p->editVals.sUnk24);
  bMixedData |= UpdateLEWithSelectionValue(leRLOuterWallAngle, p->editVals.sRLOuterWallAngle);
  bMixedData |= UpdateLEWithSelectionValue(leRUOuterWallAngle, p->editVals.sRUOuterWallAngle);
  bMixedData |= UpdateLEWithSelectionValue(leLUOuterWallHeight, p->editVals.sLUOuterWallHeight);
  bMixedData |= UpdateLEWithSelectionValue(leLLOuterWallHeight, p->editVals.sLLOuterWallHeight);
  bMixedData |= UpdateLEWithSelectionValue(leUnk29, p->editVals.sUnk29);
  bMixedData |= UpdateLEWithSelectionValue(leUnk30, p->editVals.sUnk30);
  bMixedData |= UpdateLEWithSelectionValue(leRLOuterWallHeight, p->editVals.sRLOuterWallHeight);
  bMixedData |= UpdateLEWithSelectionValue(leRUOuterWallHeight, p->editVals.sRUOuterWallHeight);
  bMixedData |= UpdateLEWithSelectionValue(leRoofHeight, p->editVals.sRoofHeight);
  bMixedData |= UpdateLEWithSelectionValue(leDrawOrder1, p->editVals.sDrawOrder1);
  bMixedData |= UpdateLEWithSelectionValue(leDrawOrder2, p->editVals.sDrawOrder2);
  bMixedData |= UpdateLEWithSelectionValue(leDrawOrder3, p->editVals.sDrawOrder3);
  bMixedData |= UpdateLEWithSelectionValue(leUnk37, p->editVals.sUnk37);
  bMixedData |= UpdateLEWithSelectionValue(leUnk38, p->editVals.sUnk38);
  bMixedData |= UpdateLEWithSelectionValue(leUnk39, p->editVals.sUnk39);
  bMixedData |= UpdateLEWithSelectionValue(leUnk40, p->editVals.sUnk40);
  bMixedData |= UpdateLEWithSelectionValue(leUnk41, p->editVals.sUnk41);
  bMixedData |= UpdateLEWithSelectionValue(leUnk42, p->editVals.sUnk42);
  bMixedData |= UpdateLEWithSelectionValue(leUnk43, p->editVals.sUnk43);
  bMixedData |= UpdateLEWithSelectionValue(leUnk44, p->editVals.sUnk44);
  bMixedData |= UpdateLEWithSelectionValue(leUnk45, p->editVals.sUnk45);
  bMixedData |= UpdateLEWithSelectionValue(leUnk46, p->editVals.sUnk46);
  bMixedData |= UpdateLEWithSelectionValue(leUnk47, p->editVals.sUnk47);
  bMixedData |= UpdateLEWithSelectionValue(leUnk48, p->editVals.sUnk48);
  bMixedData |= UpdateLEWithSelectionValue(leUnk49, p->editVals.sUnk49);
  bMixedData |= UpdateLEWithSelectionValue(leUnk50, p->editVals.sUnk50);
  bMixedData |= UpdateSignWithSelectionValue(pbSign, ckApplySign, leSign, p->editVals.sSignTexture, &p->m_bld);
  bMixedData |= UpdateSignWithSelectionValue(pbBack, ckApplyBack, leBack, p->editVals.sBackTexture, &p->m_tex);
  bMixedData |= UpdateLEWithSelectionValue(leStuntScaleFact, p->editVals.sStuntScaleFactor);
  bMixedData |= UpdateLEWithSelectionValue(leStuntAngle, p->editVals.sStuntAngle);
  bMixedData |= UpdateLEWithSelectionValue(leStuntUnk, p->editVals.sStuntUnknown);
  bMixedData |= UpdateLEWithSelectionValue(leStuntTimingGroup, p->editVals.sStuntTimingGroup);
  bMixedData |= UpdateLEWithSelectionValue(leStuntHeight, p->editVals.sStuntHeight);
  bMixedData |= UpdateLEWithSelectionValue(leStuntTimeBulging, p->editVals.sStuntTimeBulging);
  bMixedData |= UpdateLEWithSelectionValue(leStuntTimeFlat, p->editVals.sStuntTimeFlat);
  bMixedData |= UpdateLEWithSelectionValue(leStuntExpandContract, p->editVals.sStuntExpandsContracts);
  bMixedData |= UpdateLEWithSelectionValue(leStuntBulge, p->editVals.sStuntBulge);

  UpdateTextures(leLeftSurfaceType, lblLSurfaceTex1, lblLSurfaceTex2);
  UpdateTextures(leCenterSurfaceType, lblCSurfaceTex1, lblCSurfaceTex2);
  UpdateTextures(leRightSurfaceType, lblRSurfaceTex1, lblRSurfaceTex2);
  UpdateTextures(leLWallType, lblLWallTex1, lblLWallTex2);
  UpdateTextures(leRWallType, lblRWallTex1, lblRWallTex2);
  UpdateTextures(leRoofType, lblRWallTex1, lblRWallTex2);
  UpdateTextures(leLUOuterWallType, lblLUOuterWallTex1, lblLUOuterWallTex2);
  UpdateTextures(leLLOuterWallType, lblLLOuterWallTex1, lblLLOuterWallTex2);
  UpdateTextures(leOuterFloorType, lblOuterFloorTex1, lblOuterFloorTex2);
  UpdateTextures(leRLOuterWallType, lblRLOuterWallTex1, lblRLOuterWallTex2);
  UpdateTextures(leRUOuterWallType, lblRUOuterWallTex1, lblRUOuterWallTex2);
  UpdateTextures(leEnvironmentFloorType, lblEnvirFloorTex1, lblEnvirFloorTex2);

  pbInsertAfter->setEnabled(!bMixedData);
  pbInsertBefore->setEnabled(!bMixedData);
  pbApply->setEnabled(false);
  pbCancel->setEnabled(false);
  sbSelChunksFrom->setEnabled(true);
  ckTo->setEnabled(true);
  sbSelChunksTo->setEnabled(ckTo->isChecked());
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::RevertInfo()
{
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

void CMainWindow::UpdateTextures(QLineEdit *pLineEdit, QLabel *pTex1, QLabel *pTex2)
{
  //textures
  QPixmap pixmap;
  int iIndex;
  if (pLineEdit->text().compare("-1") == 0) {
    pTex1->setPixmap(QPixmap());
    pTex2->setPixmap(QPixmap());
  } else if (!pLineEdit->text().isEmpty() && pLineEdit->placeholderText().compare(MIXED_DATA) != 0) {
    int iLSurfaceType = pLineEdit->text().toInt();
    iIndex = iLSurfaceType & SURFACE_TEXTURE_INDEX;
    if (iIndex < p->m_tex.m_tileAy.size()) {
      pixmap.convertFromImage(p->m_tex.m_tileAy[iIndex]);
      pTex1->setPixmap(pixmap);

      if (iLSurfaceType & SURFACE_FLAG_TEXTURE_PAIR && iIndex > 0) {
        pixmap.convertFromImage(p->m_tex.m_tileAy[iIndex + 1]);
        pTex2->setPixmap(pixmap);
      } else {
        pTex2->setPixmap(QPixmap());
      }
    }
  } else {
    pTex1->setPixmap(QPixmap());
    pTex2->setPixmap(QPixmap());
  }
}

//-------------------------------------------------------------------------------------------------