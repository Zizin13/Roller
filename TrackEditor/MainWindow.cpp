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
  QString sLeftShoulderWidth, sLeftLaneWidth, sRightLaneWidth, sRightShoulderWidth
    , sLShoulderHeight, sRShoulderHeight, sLength
    , sYaw, sPitch, sRoll
    , sAILine1, sAILine2, sAILine3, sAILine4
    , sTrackGrip, sLeftShoulderGrip, sRightShoulderGrip
    , sUnk04, sUnk05, sUnk06, sUnk07, sUnk08
    , sLeftSurfaceType, sCenterSurfaceType, sRightSurfaceType
    , sLWallType, sRWallType, sRoofType, sUnk12, sLOuterExtraWallType, sUnk14
    , sROuterExtraWallType, sUnk16, sEnvironmentFloorType, sSignType, sUnk19, sUnk20
    , sfUnk1, sfUnk2, sfUnk3
    , sLOuterUpperExtraWallAngle, sLOuterLowerExtraWallAngle, sUnk23, sUnk24, sROuterLowerExtraWallAngle, sROuterUpperExtraWallAngle
    , sLOuterUpperExtraWallHeight, sLOuterLowerExtraWallHeight, sUnk29, sUnk30, sROuterLowerExtraWallHeight, sROuterUpperExtraWallHeight
    , sUnk33, sUnk34, sUnk35, sUnk36, sUnk37, sUnk38
    , sUnk39, sUnk40, sUnk41, sUnk42, sUnk43, sUnk44
    , sUnk45, sUnk46, sUnk47, sUnk48, sUnk49, sUnk50
    , sSignTexture, sBackTexture;

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
  connect(pbEditLOuterExtraWall, &QPushButton::clicked, this, &CMainWindow::OnEditLOuterExtraWall);
  connect(pbEditROuterExtraWall, &QPushButton::clicked, this, &CMainWindow::OnEditROuterExtraWall);
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
  connect(leUnk12, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLOuterExtraWallType, &QLineEdit::textChanged, this, &CMainWindow::OnLOuterExtraWallLEChanged);
  connect(leUnk14, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leROuterExtraWallType, &QLineEdit::textChanged, this, &CMainWindow::OnROuterExtraWallLEChanged);
  connect(leUnk16, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leEnvironmentFloorType, &QLineEdit::textChanged, this, &CMainWindow::OnEnvirFloorChanged);
  connect(leSignType, &QLineEdit::textChanged, this, &CMainWindow::OnSignTypeLEChanged);
  connect(cbSignType, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSignTypeChanged(int)));
  connect(leUnk19, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk20, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leFloatUnk1, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leFloatUnk2, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leFloatUnk3, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLOuterUpperExtraWallAngle, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLOuterLowerExtraWallAngle, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk23, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk24, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leROuterLowerExtraWallAngle, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leROuterUpperExtraWallAngle, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLOuterUpperExtraWallHeight, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leLOuterLowerExtraWallHeight, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk29, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leUnk30, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leROuterLowerExtraWallHeight, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
  connect(leROuterUpperExtraWallHeight, &QLineEdit::textChanged, this, &CMainWindow::UpdateGeometryEditMode);
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
  p->m_track.InsertGeometryChunk(sbSelChunksFrom->value(), sbInsert->value()
    , leLShoulderWidth->text(), leLLaneWidth->text(), leRLaneWidth->text(), leRShoulderWidth->text()
    , leLShoulderHeight->text(), leRShoulderHeight->text(), leLength->text()
    , leYaw->text(), lePitch->text(), leRoll->text()
    , leAILine1->text(), leAILine2->text(), leAILine3->text(), leAILine4->text()
    , leTrackGrip->text(), leLeftShoulderGrip->text(), leRShoulderGrip->text()
    , leUnk04->text(), leUnk05->text(), leUnk06->text(), leUnk07->text(), leUnk08->text()
    , leLeftSurfaceType->text(), leCenterSurfaceType->text(), leRightSurfaceType->text()
    , leLWallType->text(), leRWallType->text(), leRoofType->text(), leUnk12->text(), leLOuterExtraWallType->text(), leUnk14->text()
    , leROuterExtraWallType->text(), leUnk16->text(), leEnvironmentFloorType->text(), leSignType->text(), leUnk19->text(), leUnk20->text()
    , leFloatUnk1->text(), leFloatUnk2->text(), leFloatUnk3->text()
    , leLOuterUpperExtraWallAngle->text(), leLOuterLowerExtraWallAngle->text(), leUnk23->text(), leUnk24->text(), leROuterLowerExtraWallAngle->text(), leROuterUpperExtraWallAngle->text()
    , leLOuterUpperExtraWallHeight->text(), leLOuterLowerExtraWallHeight->text(), leUnk29->text(), leUnk30->text(), leROuterLowerExtraWallHeight->text(), leROuterUpperExtraWallHeight->text()
    , leUnk33->text(), leUnk34->text(), leUnk35->text(), leUnk36->text(), leUnk37->text(), leUnk38->text()
    , leUnk39->text(), leUnk40->text(), leUnk41->text(), leUnk42->text(), leUnk43->text(), leUnk44->text()
    , leUnk45->text(), leUnk46->text(), leUnk47->text(), leUnk48->text(), leUnk49->text(), leUnk50->text()
    , pbSign->property("value").toString(), pbBack->property("value").toString()
    , leStuntScaleFact->text(), leStuntAngle->text(), leStuntUnk->text(), leStuntTimingGroup->text(), leStuntHeight->text(), leStuntTimeBulging->text()
    , leStuntTimeFlat->text(), leStuntExpandContract->text(), leStuntBulge->text());

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
  p->m_track.InsertGeometryChunk(sbSelChunksTo->value() + 1, sbInsert->value()
    , leLShoulderWidth->text(), leLLaneWidth->text(), leRLaneWidth->text(), leRShoulderWidth->text()
    , leLShoulderHeight->text(), leRShoulderHeight->text(), leLength->text()
    , leYaw->text(), lePitch->text(), leRoll->text()
    , leAILine1->text(), leAILine2->text(), leAILine3->text(), leAILine4->text()
    , leTrackGrip->text(), leLeftShoulderGrip->text(), leRShoulderGrip->text()
    , leUnk04->text(), leUnk05->text(), leUnk06->text(), leUnk07->text(), leUnk08->text()
    , leLeftSurfaceType->text(), leCenterSurfaceType->text(), leRightSurfaceType->text()
    , leLWallType->text(), leRWallType->text(), leRoofType->text(), leUnk12->text(), leLOuterExtraWallType->text(), leUnk14->text()
    , leROuterExtraWallType->text(), leUnk16->text(), leEnvironmentFloorType->text(), leSignType->text(), leUnk19->text(), leUnk20->text()
    , leFloatUnk1->text(), leFloatUnk2->text(), leFloatUnk3->text()
    , leLOuterUpperExtraWallAngle->text(), leLOuterLowerExtraWallAngle->text(), leUnk23->text(), leUnk24->text(), leROuterLowerExtraWallAngle->text(), leROuterUpperExtraWallAngle->text()
    , leLOuterUpperExtraWallHeight->text(), leLOuterLowerExtraWallHeight->text(), leUnk29->text(), leUnk30->text(), leROuterLowerExtraWallHeight->text(), leROuterUpperExtraWallHeight->text()
    , leUnk33->text(), leUnk34->text(), leUnk35->text(), leUnk36->text(), leUnk37->text(), leUnk38->text()
    , leUnk39->text(), leUnk40->text(), leUnk41->text(), leUnk42->text(), leUnk43->text(), leUnk44->text()
    , leUnk45->text(), leUnk46->text(), leUnk47->text(), leUnk48->text(), leUnk49->text(), leUnk50->text()
    , pbSign->property("value").toString(), pbBack->property("value").toString()
    , leStuntScaleFact->text(), leStuntAngle->text(), leStuntUnk->text(), leStuntTimingGroup->text(), leStuntHeight->text(), leStuntTimeBulging->text()
    , leStuntTimeFlat->text(), leStuntExpandContract->text(), leStuntBulge->text());

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
  p->m_track.ApplyGeometrySettings(sbSelChunksFrom->value(), sbSelChunksTo->value()
    , leLShoulderWidth->text(), leLLaneWidth->text(), leRLaneWidth->text(), leRShoulderWidth->text()
    , leLShoulderHeight->text(), leRShoulderHeight->text(), leLength->text()
    , leYaw->text(), lePitch->text(), leRoll->text()
    , leAILine1->text(), leAILine2->text(), leAILine3->text(), leAILine4->text()
    , leTrackGrip->text(), leLeftShoulderGrip->text(), leRShoulderGrip->text()
    , leUnk04->text(), leUnk05->text(), leUnk06->text(), leUnk07->text(), leUnk08->text()
    , leLeftSurfaceType->text(), leCenterSurfaceType->text(), leRightSurfaceType->text()
    , leLWallType->text(), leRWallType->text(), leRoofType->text(), leUnk12->text(), leLOuterExtraWallType->text(), leUnk14->text()
    , leROuterExtraWallType->text(), leUnk16->text(), leEnvironmentFloorType->text(), leSignType->text(), leUnk19->text(), leUnk20->text()
    , leFloatUnk1->text(), leFloatUnk2->text(), leFloatUnk3->text()
    , leLOuterUpperExtraWallAngle->text(), leLOuterLowerExtraWallAngle->text(), leUnk23->text(), leUnk24->text(), leROuterLowerExtraWallAngle->text(), leROuterUpperExtraWallAngle->text()
    , leLOuterUpperExtraWallHeight->text(), leLOuterLowerExtraWallHeight->text(), leUnk29->text(), leUnk30->text(), leROuterLowerExtraWallHeight->text(), leROuterUpperExtraWallHeight->text()
    , leUnk33->text(), leUnk34->text(), leUnk35->text(), leUnk36->text(), leUnk37->text(), leUnk38->text()
    , leUnk39->text(), leUnk40->text(), leUnk41->text(), leUnk42->text(), leUnk43->text(), leUnk44->text()
    , leUnk45->text(), leUnk46->text(), leUnk47->text(), leUnk48->text(), leUnk49->text(), leUnk50->text()
    , pbSign->property("value").toString(), pbBack->property("value").toString()
    , leStuntScaleFact->text(), leStuntAngle->text(), leStuntUnk->text(), leStuntTimingGroup->text(), leStuntHeight->text(), leStuntTimeBulging->text()
    , leStuntTimeFlat->text(), leStuntExpandContract->text(), leStuntBulge->text());
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

void CMainWindow::OnEditLOuterExtraWall()
{
  int iValue = leLOuterExtraWallType->text().toInt();

  CEditSurfaceDialog dlg(this, &p->m_tex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leLOuterExtraWallType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leLOuterExtraWallType, lblLOuterExtraWallTex1, lblLOuterExtraWallTex2);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditROuterExtraWall()
{
  int iValue = leROuterExtraWallType->text().toInt();

  CEditSurfaceDialog dlg(this, &p->m_tex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leROuterExtraWallType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leROuterExtraWallType, lblROuterExtraWallTex1, lblROuterExtraWallTex2);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditEnvirFloor()
{
  int iValue = leEnvironmentFloorType->text().toInt();

  CEditSurfaceDialog dlg(this, &p->m_tex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leROuterExtraWallType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leEnvironmentFloorType, lblEnvirFloorTex1, lblEnvirFloorTex2);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateGeometryEditMode()
{
  bool bEditMode = false;
  bool bMixedData = false;
  UpdateLEEditMode(bEditMode, bMixedData, leLShoulderWidth, p->sLeftShoulderWidth);
  UpdateLEEditMode(bEditMode, bMixedData, leLLaneWidth, p->sLeftLaneWidth);
  UpdateLEEditMode(bEditMode, bMixedData, leRLaneWidth, p->sRightLaneWidth);
  UpdateLEEditMode(bEditMode, bMixedData, leRShoulderWidth, p->sRightShoulderWidth);
  UpdateLEEditMode(bEditMode, bMixedData, leLShoulderHeight, p->sLShoulderHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leRShoulderHeight, p->sRShoulderHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leLength, p->sLength);
  UpdateLEEditMode(bEditMode, bMixedData, leYaw, p->sYaw);
  UpdateLEEditMode(bEditMode, bMixedData, lePitch, p->sPitch);
  UpdateLEEditMode(bEditMode, bMixedData, leRoll, p->sRoll);
  UpdateLEEditMode(bEditMode, bMixedData, leAILine1, p->sAILine1);
  UpdateLEEditMode(bEditMode, bMixedData, leAILine2, p->sAILine2);
  UpdateLEEditMode(bEditMode, bMixedData, leAILine3, p->sAILine3);
  UpdateLEEditMode(bEditMode, bMixedData, leAILine4, p->sAILine4);
  UpdateLEEditMode(bEditMode, bMixedData, leTrackGrip, p->sTrackGrip);
  UpdateLEEditMode(bEditMode, bMixedData, leLeftShoulderGrip, p->sLeftShoulderGrip);
  UpdateLEEditMode(bEditMode, bMixedData, leRShoulderGrip, p->sRightShoulderGrip);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk04, p->sUnk04);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk05, p->sUnk05);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk06, p->sUnk06);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk07, p->sUnk07);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk08, p->sUnk08);
  UpdateLEEditMode(bEditMode, bMixedData, leLeftSurfaceType, p->sLeftSurfaceType);
  UpdateLEEditMode(bEditMode, bMixedData, leCenterSurfaceType, p->sCenterSurfaceType);
  UpdateLEEditMode(bEditMode, bMixedData, leRightSurfaceType, p->sRightSurfaceType);
  UpdateLEEditMode(bEditMode, bMixedData, leLWallType, p->sLWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leRWallType, p->sRWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leRoofType, p->sRoofType);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk12, p->sUnk12);
  UpdateLEEditMode(bEditMode, bMixedData, leLOuterExtraWallType, p->sLOuterExtraWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk14, p->sUnk14);
  UpdateLEEditMode(bEditMode, bMixedData, leROuterExtraWallType, p->sROuterExtraWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk16, p->sUnk16);
  UpdateLEEditMode(bEditMode, bMixedData, leEnvironmentFloorType, p->sEnvironmentFloorType);
  UpdateLEEditMode(bEditMode, bMixedData, leSignType, p->sSignType);
  UpdateCBEditMode(bEditMode, bMixedData, cbSignType, p->sSignType);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk19, p->sUnk19);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk20, p->sUnk20);
  UpdateLEEditMode(bEditMode, bMixedData, leFloatUnk1, p->sfUnk1);
  UpdateLEEditMode(bEditMode, bMixedData, leFloatUnk2, p->sfUnk2);
  UpdateLEEditMode(bEditMode, bMixedData, leFloatUnk3, p->sfUnk3);
  UpdateLEEditMode(bEditMode, bMixedData, leLOuterUpperExtraWallAngle, p->sLOuterUpperExtraWallAngle);
  UpdateLEEditMode(bEditMode, bMixedData, leLOuterLowerExtraWallAngle, p->sLOuterLowerExtraWallAngle);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk23, p->sUnk23);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk24, p->sUnk24);
  UpdateLEEditMode(bEditMode, bMixedData, leROuterLowerExtraWallAngle, p->sROuterLowerExtraWallAngle);
  UpdateLEEditMode(bEditMode, bMixedData, leROuterUpperExtraWallAngle, p->sROuterUpperExtraWallAngle);
  UpdateLEEditMode(bEditMode, bMixedData, leLOuterUpperExtraWallHeight, p->sLOuterUpperExtraWallHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leLOuterLowerExtraWallHeight, p->sLOuterLowerExtraWallHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk29, p->sUnk29);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk30, p->sUnk30);
  UpdateLEEditMode(bEditMode, bMixedData, leROuterLowerExtraWallHeight, p->sROuterLowerExtraWallHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leROuterUpperExtraWallHeight, p->sROuterUpperExtraWallHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk33, p->sUnk33);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk34, p->sUnk34);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk35, p->sUnk35);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk36, p->sUnk36);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk37, p->sUnk37);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk38, p->sUnk38);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk39, p->sUnk39);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk40, p->sUnk40);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk41, p->sUnk41);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk42, p->sUnk42);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk43, p->sUnk43);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk44, p->sUnk44);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk45, p->sUnk45);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk46, p->sUnk46);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk47, p->sUnk47);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk48, p->sUnk48);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk49, p->sUnk49);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk50, p->sUnk50);
  UpdateSignEditMode(bEditMode, bMixedData, leSign, widgetSign, p->sSignTexture);
  UpdateSignEditMode(bEditMode, bMixedData, leBack, widgetBack, p->sBackTexture);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntScaleFact, p->sStuntScaleFactor);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntAngle, p->sStuntAngle);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntUnk, p->sStuntUnknown);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntTimingGroup, p->sStuntTimingGroup);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntHeight, p->sStuntHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntTimeBulging, p->sStuntTimeBulging);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntTimeFlat, p->sStuntTimeFlat);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntExpandContract, p->sStuntExpandsContracts);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntBulge, p->sStuntBulge);

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
  iBldIndex = iBldIndex >> 1;

  CTilePicker dlg(this, &p->m_bld, iBldIndex, true);
  if (dlg.exec()) {
    int iIndex = dlg.GetSelected();
    if (iIndex >= 0) {
      iBldIndex = iIndex;
      iValue = iBldIndex << 1;
      if (ckApplySign->isChecked()) {
        iValue |= SURFACE_FLAG_APPLY_TEXTURE;
      } else {
        iValue &= ~SURFACE_FLAG_APPLY_TEXTURE;
      }
      sValue = QString::number(iValue);
    }
  }

  leSign->setText(sValue);
  UpdateSignButtonDisplay(pbSign, ckApplySign, leSign);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnBackClicked()
{
  QString sValue = leBack->text();
  int iValue = sValue.toInt();
  int iBldIndex = iValue & SURFACE_TEXTURE_INDEX;
  iBldIndex = iBldIndex >> 1;

  CTilePicker dlg(this, &p->m_bld, iBldIndex, true);
  if (dlg.exec()) {
    int iIndex = dlg.GetSelected();
    if (iIndex >= 0) {
      iBldIndex = iIndex;
      iValue = iBldIndex << 1;
      if (ckApplyBack->isChecked()) {
        iValue |= SURFACE_FLAG_APPLY_TEXTURE;
      } else {
        iValue &= ~SURFACE_FLAG_APPLY_TEXTURE;
      }
    }
    sValue = QString::number(iValue);
  }

  leBack->setText(sValue);
  UpdateSignButtonDisplay(pbBack, ckApplyBack, leBack);
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
  UpdateSignButtonDisplay(pbSign, ckApplySign, leSign);
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
  UpdateSignButtonDisplay(pbBack, ckApplyBack, leBack);
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
  UpdateSignButtonDisplay(pbSign, ckApplySign, leSign);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnBackLEChanged()
{
  UpdateSignButtonDisplay(pbBack, ckApplyBack, leBack);
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

void CMainWindow::OnLOuterExtraWallLEChanged()
{
  UpdateTextures(leLOuterExtraWallType, lblLOuterExtraWallTex1, lblLOuterExtraWallTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnROuterExtraWallLEChanged()
{
  UpdateTextures(leROuterExtraWallType, lblROuterExtraWallTex1, lblROuterExtraWallTex2);
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
  p->m_track.GetGeometryValuesFromSelection(sbSelChunksFrom->value(), sbSelChunksTo->value()
    , p->sLeftShoulderWidth, p->sLeftLaneWidth, p->sRightLaneWidth, p->sRightShoulderWidth
    , p->sLShoulderHeight, p->sRShoulderHeight, p->sLength
    , p->sYaw, p->sPitch, p->sRoll
    , p->sAILine1, p->sAILine2, p->sAILine3, p->sAILine4
    , p->sTrackGrip, p->sLeftShoulderGrip, p->sRightShoulderGrip
    , p->sUnk04, p->sUnk05, p->sUnk06, p->sUnk07, p->sUnk08
    , p->sLeftSurfaceType, p->sCenterSurfaceType, p->sRightSurfaceType
    , p->sLWallType, p->sRWallType, p->sRoofType, p->sUnk12, p->sLOuterExtraWallType, p->sUnk14
    , p->sROuterExtraWallType, p->sUnk16, p->sEnvironmentFloorType, p->sSignType, p->sUnk19, p->sUnk20
    , p->sfUnk1, p->sfUnk2, p->sfUnk3
    , p->sLOuterUpperExtraWallAngle, p->sLOuterLowerExtraWallAngle, p->sUnk23, p->sUnk24, p->sROuterLowerExtraWallAngle, p->sROuterUpperExtraWallAngle
    , p->sLOuterUpperExtraWallHeight, p->sLOuterLowerExtraWallHeight, p->sUnk29, p->sUnk30, p->sROuterLowerExtraWallHeight, p->sROuterUpperExtraWallHeight
    , p->sUnk33, p->sUnk34, p->sUnk35, p->sUnk36, p->sUnk37, p->sUnk38
    , p->sUnk39, p->sUnk40, p->sUnk41, p->sUnk42, p->sUnk43, p->sUnk44
    , p->sUnk45, p->sUnk46, p->sUnk47, p->sUnk48, p->sUnk49, p->sUnk50
    , p->sSignTexture, p->sBackTexture
    , p->sStuntScaleFactor, p->sStuntAngle, p->sStuntUnknown, p->sStuntTimingGroup, p->sStuntHeight, p->sStuntTimeBulging
    , p->sStuntTimeFlat, p->sStuntExpandsContracts, p->sStuntBulge);

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

bool CMainWindow::UpdateSignWithSelectionValue(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, const QString &sValue)
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
  UpdateSignButtonDisplay(pPushButton, pCheckbox, pLineEdit);
  return (sValue.compare(MIXED_DATA) == 0) && pLineEdit->text().isEmpty();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateSignButtonDisplay(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit)
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
    iBldIndex = iBldIndex >> 1;
    if (iBldIndex < p->m_bld.m_tileAy.size()) {
      QPixmap pixmap;
      pixmap.convertFromImage(p->m_bld.m_tileAy[iBldIndex]);
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
  bMixedData |= UpdateLEWithSelectionValue(leLShoulderWidth, p->sLeftShoulderWidth);
  bMixedData |= UpdateLEWithSelectionValue(leLLaneWidth, p->sLeftLaneWidth);
  bMixedData |= UpdateLEWithSelectionValue(leRLaneWidth, p->sRightLaneWidth);
  bMixedData |= UpdateLEWithSelectionValue(leRShoulderWidth, p->sRightShoulderWidth);
  bMixedData |= UpdateLEWithSelectionValue(leLShoulderHeight, p->sLShoulderHeight);
  bMixedData |= UpdateLEWithSelectionValue(leRShoulderHeight, p->sRShoulderHeight);
  bMixedData |= UpdateLEWithSelectionValue(leLength, p->sLength);
  bMixedData |= UpdateLEWithSelectionValue(leYaw, p->sYaw);
  bMixedData |= UpdateLEWithSelectionValue(lePitch, p->sPitch);
  bMixedData |= UpdateLEWithSelectionValue(leRoll, p->sRoll);
  bMixedData |= UpdateLEWithSelectionValue(leAILine1, p->sAILine1);
  bMixedData |= UpdateLEWithSelectionValue(leAILine2, p->sAILine2);
  bMixedData |= UpdateLEWithSelectionValue(leAILine3, p->sAILine3);
  bMixedData |= UpdateLEWithSelectionValue(leAILine4, p->sAILine4);
  bMixedData |= UpdateLEWithSelectionValue(leTrackGrip, p->sTrackGrip);
  bMixedData |= UpdateLEWithSelectionValue(leLeftShoulderGrip, p->sLeftShoulderGrip);
  bMixedData |= UpdateLEWithSelectionValue(leRShoulderGrip, p->sRightShoulderGrip);
  bMixedData |= UpdateLEWithSelectionValue(leUnk04, p->sUnk04);
  bMixedData |= UpdateLEWithSelectionValue(leUnk05, p->sUnk05);
  bMixedData |= UpdateLEWithSelectionValue(leUnk06, p->sUnk06);
  bMixedData |= UpdateLEWithSelectionValue(leUnk07, p->sUnk07);
  bMixedData |= UpdateLEWithSelectionValue(leUnk08, p->sUnk08);
  bMixedData |= UpdateLEWithSelectionValue(leLeftSurfaceType, p->sLeftSurfaceType);
  bMixedData |= UpdateLEWithSelectionValue(leCenterSurfaceType, p->sCenterSurfaceType);
  bMixedData |= UpdateLEWithSelectionValue(leRightSurfaceType, p->sRightSurfaceType);
  bMixedData |= UpdateLEWithSelectionValue(leLWallType, p->sLWallType);
  bMixedData |= UpdateLEWithSelectionValue(leRWallType, p->sRWallType);
  bMixedData |= UpdateLEWithSelectionValue(leRoofType, p->sRoofType);
  bMixedData |= UpdateLEWithSelectionValue(leUnk12, p->sUnk12);
  bMixedData |= UpdateLEWithSelectionValue(leLOuterExtraWallType, p->sLOuterExtraWallType);
  bMixedData |= UpdateLEWithSelectionValue(leUnk14, p->sUnk14);
  bMixedData |= UpdateLEWithSelectionValue(leROuterExtraWallType, p->sROuterExtraWallType);
  bMixedData |= UpdateLEWithSelectionValue(leUnk16, p->sUnk16);
  bMixedData |= UpdateLEWithSelectionValue(leEnvironmentFloorType, p->sEnvironmentFloorType);
  bMixedData |= UpdateLEWithSelectionValue(leSignType, p->sSignType);
  bMixedData |= UpdateCBWithSelectionValue(cbSignType, p->sSignType);
  bMixedData |= UpdateLEWithSelectionValue(leUnk19, p->sUnk19);
  bMixedData |= UpdateLEWithSelectionValue(leUnk20, p->sUnk20);
  bMixedData |= UpdateLEWithSelectionValue(leFloatUnk1, p->sfUnk1);
  bMixedData |= UpdateLEWithSelectionValue(leFloatUnk2, p->sfUnk2);
  bMixedData |= UpdateLEWithSelectionValue(leFloatUnk3, p->sfUnk3);
  bMixedData |= UpdateLEWithSelectionValue(leLOuterUpperExtraWallAngle, p->sLOuterUpperExtraWallAngle);
  bMixedData |= UpdateLEWithSelectionValue(leLOuterLowerExtraWallAngle, p->sLOuterLowerExtraWallAngle);
  bMixedData |= UpdateLEWithSelectionValue(leUnk23, p->sUnk23);
  bMixedData |= UpdateLEWithSelectionValue(leUnk24, p->sUnk24);
  bMixedData |= UpdateLEWithSelectionValue(leROuterLowerExtraWallAngle, p->sROuterLowerExtraWallAngle);
  bMixedData |= UpdateLEWithSelectionValue(leROuterUpperExtraWallAngle, p->sROuterUpperExtraWallAngle);
  bMixedData |= UpdateLEWithSelectionValue(leLOuterUpperExtraWallHeight, p->sLOuterUpperExtraWallHeight);
  bMixedData |= UpdateLEWithSelectionValue(leLOuterLowerExtraWallHeight, p->sLOuterLowerExtraWallHeight);
  bMixedData |= UpdateLEWithSelectionValue(leUnk29, p->sUnk29);
  bMixedData |= UpdateLEWithSelectionValue(leUnk30, p->sUnk30);
  bMixedData |= UpdateLEWithSelectionValue(leROuterLowerExtraWallHeight, p->sROuterLowerExtraWallHeight);
  bMixedData |= UpdateLEWithSelectionValue(leROuterUpperExtraWallHeight, p->sROuterUpperExtraWallHeight);
  bMixedData |= UpdateLEWithSelectionValue(leUnk33, p->sUnk33);
  bMixedData |= UpdateLEWithSelectionValue(leUnk34, p->sUnk34);
  bMixedData |= UpdateLEWithSelectionValue(leUnk35, p->sUnk35);
  bMixedData |= UpdateLEWithSelectionValue(leUnk36, p->sUnk36);
  bMixedData |= UpdateLEWithSelectionValue(leUnk37, p->sUnk37);
  bMixedData |= UpdateLEWithSelectionValue(leUnk38, p->sUnk38);
  bMixedData |= UpdateLEWithSelectionValue(leUnk39, p->sUnk39);
  bMixedData |= UpdateLEWithSelectionValue(leUnk40, p->sUnk40);
  bMixedData |= UpdateLEWithSelectionValue(leUnk41, p->sUnk41);
  bMixedData |= UpdateLEWithSelectionValue(leUnk42, p->sUnk42);
  bMixedData |= UpdateLEWithSelectionValue(leUnk43, p->sUnk43);
  bMixedData |= UpdateLEWithSelectionValue(leUnk44, p->sUnk44);
  bMixedData |= UpdateLEWithSelectionValue(leUnk45, p->sUnk45);
  bMixedData |= UpdateLEWithSelectionValue(leUnk46, p->sUnk46);
  bMixedData |= UpdateLEWithSelectionValue(leUnk47, p->sUnk47);
  bMixedData |= UpdateLEWithSelectionValue(leUnk48, p->sUnk48);
  bMixedData |= UpdateLEWithSelectionValue(leUnk49, p->sUnk49);
  bMixedData |= UpdateLEWithSelectionValue(leUnk50, p->sUnk50);
  bMixedData |= UpdateSignWithSelectionValue(pbSign, ckApplySign, leSign, p->sSignTexture);
  bMixedData |= UpdateSignWithSelectionValue(pbBack, ckApplyBack, leBack, p->sBackTexture);
  bMixedData |= UpdateLEWithSelectionValue(leStuntScaleFact, p->sStuntScaleFactor);
  bMixedData |= UpdateLEWithSelectionValue(leStuntAngle, p->sStuntAngle);
  bMixedData |= UpdateLEWithSelectionValue(leStuntUnk, p->sStuntUnknown);
  bMixedData |= UpdateLEWithSelectionValue(leStuntTimingGroup, p->sStuntTimingGroup);
  bMixedData |= UpdateLEWithSelectionValue(leStuntHeight, p->sStuntHeight);
  bMixedData |= UpdateLEWithSelectionValue(leStuntTimeBulging, p->sStuntTimeBulging);
  bMixedData |= UpdateLEWithSelectionValue(leStuntTimeFlat, p->sStuntTimeFlat);
  bMixedData |= UpdateLEWithSelectionValue(leStuntExpandContract, p->sStuntExpandsContracts);
  bMixedData |= UpdateLEWithSelectionValue(leStuntBulge, p->sStuntBulge);

  UpdateTextures(leLeftSurfaceType, lblLSurfaceTex1, lblLSurfaceTex2);
  UpdateTextures(leCenterSurfaceType, lblCSurfaceTex1, lblCSurfaceTex2);
  UpdateTextures(leRightSurfaceType, lblRSurfaceTex1, lblRSurfaceTex2);
  UpdateTextures(leLWallType, lblLWallTex1, lblLWallTex2);
  UpdateTextures(leRWallType, lblRWallTex1, lblRWallTex2);
  UpdateTextures(leRoofType, lblRWallTex1, lblRWallTex2);
  UpdateTextures(leLOuterExtraWallType, lblLOuterExtraWallTex1, lblLOuterExtraWallTex2);
  UpdateTextures(leROuterExtraWallType, lblROuterExtraWallTex1, lblROuterExtraWallTex2);
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
    iIndex = iIndex >> 1;
    if (iIndex < p->m_tex.m_tileAy.size()) {
      pixmap.convertFromImage(p->m_tex.m_tileAy[iIndex]);
      pTex1->setPixmap(pixmap);

      if (iLSurfaceType & SURFACE_FLAG_TEXTURE_PAIR && iIndex > 0) {
        pixmap.convertFromImage(p->m_tex.m_tileAy[iIndex - 1]);
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