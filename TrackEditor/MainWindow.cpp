#include <glew.h>
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
#include "EditSeriesDialog.h"
#include "ChunkEditValues.h"
#include "TrackPreview.h"
#include "EditDataWidget.h"
#include "GlobalTrackSettings.h"
#include "qdockwidget.h"
#include "DisplaySettings.h"
#include "EditGeometryWidget.h"
#include "qtextstream.h"
#include "QtHelpers.h"
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
  CLogDialog m_logDialog;

  QDockWidget *m_pEditDataDockWidget;
  CEditDataWidget *m_pEditData;
  QDockWidget *m_pGlobalSettingsDockWidget;
  QDockWidget *m_pEditSeriesDockWidget;
  QDockWidget *m_pDisplaySettingsDockWidget;
  QDockWidget *m_pEditGeometryDockWidget;
  CDisplaySettings *m_pDisplaySettings;
  QAction *m_pDebugAction;

  CPalette m_palette;
  CTexture m_tex;
  CTexture m_bld;
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
  p->m_logDialog.hide();
  txData->setFont(QFont("Courier", 8));

  //setup dock widgets
  p->m_pEditDataDockWidget = new QDockWidget("Edit Chunk Data", this);
  p->m_pEditDataDockWidget->setObjectName("EditChunkData");
  p->m_pEditDataDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pEditData = new CEditDataWidget(p->m_pEditDataDockWidget, &p->m_track, &p->m_tex, &p->m_bld);
  p->m_pEditDataDockWidget->setWidget(p->m_pEditData);

  p->m_pGlobalSettingsDockWidget = new QDockWidget("Global Track Settings", this);
  p->m_pGlobalSettingsDockWidget->setObjectName("GlobalTrackSettings");
  p->m_pGlobalSettingsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pGlobalSettingsDockWidget->setWidget(new CGlobalTrackSettings(p->m_pGlobalSettingsDockWidget, &p->m_track));

  p->m_pEditSeriesDockWidget = new QDockWidget("Edit Series...", this);
  p->m_pEditSeriesDockWidget->setObjectName("EditSeries");
  p->m_pEditSeriesDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pEditSeriesDockWidget->setWidget(new CEditSeriesDialog(p->m_pEditSeriesDockWidget, &p->m_track));

  p->m_pDisplaySettingsDockWidget = new QDockWidget("Display Settings", this);
  p->m_pDisplaySettingsDockWidget->setObjectName("DisplaySettings");
  p->m_pDisplaySettingsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pDisplaySettings = new CDisplaySettings(p->m_pDisplaySettingsDockWidget, openGLWidget);
  p->m_pDisplaySettingsDockWidget->setWidget(p->m_pDisplaySettings);

  p->m_pEditGeometryDockWidget = new QDockWidget("Edit Geometry", this);
  p->m_pEditGeometryDockWidget->setObjectName("EditGeometry");
  p->m_pEditGeometryDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pEditGeometryDockWidget->setWidget(new CEditGeometryWidget(p->m_pEditGeometryDockWidget, &p->m_track));

  //setup view menu
  menuView->addAction(p->m_pEditGeometryDockWidget->toggleViewAction());
  menuView->addAction(p->m_pEditDataDockWidget->toggleViewAction());
  menuView->addAction(p->m_pEditSeriesDockWidget->toggleViewAction());
  menuView->addAction(p->m_pGlobalSettingsDockWidget->toggleViewAction());
  menuView->addAction(p->m_pDisplaySettingsDockWidget->toggleViewAction());
  menuView->addSeparator();
  p->m_pDebugAction = new QAction("Debug", menuView);
  p->m_pDebugAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
  menuView->addAction(p->m_pDebugAction);

  //signals
  connect(this, &CMainWindow::LogMsgSig, this, &CMainWindow::OnLogMsg, Qt::QueuedConnection);
  connect(actNew, &QAction::triggered, this, &CMainWindow::OnNewTrack);
  connect(actLoad, &QAction::triggered, this, &CMainWindow::OnLoadTrack);
  connect(actSave, &QAction::triggered, this, &CMainWindow::OnSaveTrack);
  connect(actSaveAs, &QAction::triggered, this, &CMainWindow::OnSaveTrackAs);
  connect(actExportMangled, &QAction::triggered, this, &CMainWindow::OnExportMangled);
  connect(p->m_pDebugAction, &QAction::triggered, this, &CMainWindow::OnDebug);
  connect(actAbout, &QAction::triggered, this, &CMainWindow::OnAbout);

  connect(sbSelChunksFrom, SIGNAL(valueChanged(int)), this, SLOT(OnSelChunksFromChanged(int)));
  connect(sbSelChunksTo, SIGNAL(valueChanged(int)), this, SLOT(OnSelChunksToChanged(int)));
  connect(ckTo, &QCheckBox::toggled, this, &CMainWindow::OnToChecked);
  connect(pbDelete, &QPushButton::clicked, this, &CMainWindow::OnDeleteChunkClicked);
  connect(pbAddChunk, &QPushButton::clicked, this, &CMainWindow::OnAddChunkClicked);

  //open window
  LoadSettings();
}

//-------------------------------------------------------------------------------------------------

CMainWindow::~CMainWindow()
{
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::closeEvent(QCloseEvent *pEvent)
{
  SaveSettings();

  //cleanup
  openGLWidget->Shutdown();
  if (p) {
    delete p;
    p = NULL;
  }

  QMainWindow::closeEvent(pEvent);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::LogMessage(const QString &sMsg)
{
  //log messages can come from anywhere like OpenGL
  //so we need to put this onto the GUI thread
  emit LogMsgSig(sMsg);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::SetUnsavedChanges(bool bUnsavedChanges)
{
  m_bUnsavedChanges = bUnsavedChanges;
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnLogMsg(QString sMsg)
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

  if (!p->m_track.LoadTrack(sFilename)) {
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
  p->m_pEditData->OnCancelClicked();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnToChecked(bool bChecked)
{
  sbSelChunksTo->setEnabled(bChecked);
  if (!bChecked) {
    sbSelChunksTo->setValue(sbSelChunksFrom->value());
  }
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
  p->m_track.GenerateTrackMath();

  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->LogMessage("Deleted geometry chunk");
  sbSelChunksFrom->blockSignals(true);
  sbSelChunksTo->blockSignals(true);
  g_pMainWindow->UpdateWindow();
  sbSelChunksTo->setValue(sbSelChunksFrom->value());
  sbSelChunksFrom->blockSignals(false);
  sbSelChunksTo->blockSignals(false);
  p->m_pEditData->UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnAddChunkClicked()
{
  CChunkEditValues editVals;
  int iLastPos = (int)p->m_track.m_chunkAy.size() - 1;
  p->m_track.GetGeometryValuesFromSelection(iLastPos, iLastPos, editVals);
  p->m_track.InsertGeometryChunk(iLastPos, 1, editVals);

  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->LogMessage("Added geometry chunk");
  sbSelChunksFrom->blockSignals(true);
  sbSelChunksTo->blockSignals(true);
  g_pMainWindow->UpdateWindow();
  sbSelChunksTo->setValue(iLastPos + 1);
  sbSelChunksFrom->setValue(iLastPos + 1);
  sbSelChunksFrom->blockSignals(false);
  sbSelChunksTo->blockSignals(false);
  p->m_pEditData->UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::LoadSettings()
{
  QSettings settings(m_sSettingsFile, QSettings::IniFormat);
  m_sTrackFilesFolder = settings.value("track_folder", m_sTrackFilesFolder).toString();

  if (settings.contains("window_geometry") && settings.contains("window_state")) {
    QByteArray geometry = saveGeometry();
    QByteArray state = saveState();
    geometry = settings.value("window_geometry", geometry).toByteArray();
    state = settings.value("window_state", state).toByteArray();
    restoreGeometry(geometry);
    restoreState(state);
  } else {
    move(10, 10);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.8);
  }

  if (settings.contains("show_edit_data")
      && settings.contains("show_global_settings")
      && settings.contains("show_edit_series")
      && settings.contains("show_display_settings")
      && settings.contains("show_edit_geometry")) {
    bool bShowEditData = false;
    bool bShowGlobalSettings = false;
    bool bShowEditSeries = false;
    bool bShowDisplaySettings = false;
    bool bShowEditGeometry = false;
    bShowEditData = settings.value("show_edit_data", bShowEditData).toBool();
    bShowGlobalSettings = settings.value("show_global_settings", bShowGlobalSettings).toBool();
    bShowEditSeries = settings.value("show_edit_series", bShowEditSeries).toBool();
    bShowDisplaySettings = settings.value("show_display_settings", bShowDisplaySettings).toBool();
    bShowEditGeometry = settings.value("show_edit_geometry", bShowEditGeometry).toBool();
    p->m_pEditDataDockWidget->setVisible(bShowEditData);
    p->m_pGlobalSettingsDockWidget->setVisible(bShowGlobalSettings);
    p->m_pEditSeriesDockWidget->setVisible(bShowEditSeries);
    p->m_pDisplaySettingsDockWidget->setVisible(bShowDisplaySettings);
    p->m_pEditGeometryDockWidget->setVisible(bShowEditGeometry);
    restoreDockWidget(p->m_pEditDataDockWidget);
    restoreDockWidget(p->m_pGlobalSettingsDockWidget);
    restoreDockWidget(p->m_pEditSeriesDockWidget);
    restoreDockWidget(p->m_pDisplaySettingsDockWidget);
    restoreDockWidget(p->m_pEditGeometryDockWidget);
  } else {
    addDockWidget(Qt::LeftDockWidgetArea, p->m_pEditDataDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, p->m_pGlobalSettingsDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, p->m_pEditSeriesDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, p->m_pDisplaySettingsDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, p->m_pEditGeometryDockWidget);
    p->m_pEditDataDockWidget->setVisible(false);
    p->m_pGlobalSettingsDockWidget->setVisible(false);
    p->m_pEditSeriesDockWidget->setVisible(false);
    p->m_pDisplaySettingsDockWidget->setVisible(false);
    p->m_pEditGeometryDockWidget->setVisible(false);
  }
  if (settings.contains("show_models")) {
    eWhipModel carModel;
    eShapeSection aiLine;
    bool bMillionPlus;
    uint32 uiShowModels = p->m_pDisplaySettings->GetDisplaySettings(carModel, aiLine, bMillionPlus);
    uiShowModels = settings.value("show_models", uiShowModels).toUInt();
    carModel = (eWhipModel)settings.value("car_model", (int)carModel).toInt();
    aiLine = (eShapeSection)settings.value("car_pos", (int)aiLine).toInt();
    bMillionPlus = settings.value("wrong_way", bMillionPlus).toBool();
    p->m_pDisplaySettings->SetDisplaySettings(uiShowModels, carModel, aiLine, bMillionPlus);
  }
  if (settings.contains("attach_last")) {
    bool bAttachLast = p->m_pDisplaySettings->GetAttachLast();
    bAttachLast = settings.value("attach_last", bAttachLast).toBool();
    p->m_pDisplaySettings->SetAttachLast(bAttachLast);
  }

  show();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::SaveSettings()
{
  QSettings settings(m_sSettingsFile, QSettings::IniFormat);
  settings.setValue("track_folder", m_sTrackFilesFolder);

  eWhipModel carModel;
  eShapeSection aiLine;
  bool bMillionPlus;

  QByteArray geometry = saveGeometry();
  QByteArray state = saveState();
  settings.setValue("window_geometry", geometry);
  settings.setValue("window_state", state);
  settings.setValue("show_edit_data", p->m_pEditDataDockWidget->isVisible());
  settings.setValue("show_global_settings", p->m_pGlobalSettingsDockWidget->isVisible());
  settings.setValue("show_edit_series", p->m_pEditSeriesDockWidget->isVisible());
  settings.setValue("show_display_settings", p->m_pDisplaySettingsDockWidget->isVisible());
  settings.setValue("show_edit_geometry", p->m_pEditGeometryDockWidget->isVisible());
  settings.setValue("show_models", p->m_pDisplaySettings->GetDisplaySettings(carModel, aiLine, bMillionPlus));
  settings.setValue("car_model", (int)carModel);
  settings.setValue("car_pos", (int)aiLine);
  settings.setValue("wrong_way", bMillionPlus);
  settings.setValue("attach_last", p->m_pDisplaySettings->GetAttachLast());
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
  
  //stuff data
  std::vector<uint8_t> trackData;
  p->m_track.GetTrackData(trackData);
  QString sText;
  QTextStream stream(&sText);
  for (int i = 0; i < trackData.size(); ++i) {
    stream << (char)trackData[i];
  }
  txData->insertPlainText(sText);

  openGLWidget->SetTrack(&p->m_track, &p->m_tex, &p->m_bld, &p->m_palette);

  sbSelChunksFrom->blockSignals(true);
  sbSelChunksTo->blockSignals(true);
  sbSelChunksFrom->setRange(0, (int)p->m_track.m_chunkAy.size() - 1);
  sbSelChunksTo->setRange(0,   (int)p->m_track.m_chunkAy.size() - 1);
  sbSelChunksFrom->blockSignals(false);
  sbSelChunksTo->blockSignals(false);
  leChunkCount->setText(QString::number(p->m_track.m_chunkAy.size()));
  UpdateGeometrySelection();
  emit UpdateWindowSig();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::LoadTextures()
{
  //load textures
  QString sPal = m_sTrackFilesFolder + QDir::separator() + "PALETTE.PAL";
  QString sTex = m_sTrackFilesFolder + QDir::separator() + QString(p->m_track.m_sTextureFile.c_str());
  QString sBld = m_sTrackFilesFolder + QDir::separator() + QString(p->m_track.m_sBuildingFile.c_str());
  bool bPalLoaded = p->m_palette.LoadPalette(sPal.toLatin1().constData());
  bool bTexLoaded = p->m_tex.LoadTexture(sTex.toLatin1().constData(), &p->m_palette);
  bool bBldLoaded = p->m_bld.LoadTexture(sBld.toLatin1().constData(), &p->m_palette);

  //make sure car textures are reloaded too
  openGLWidget->ReloadCar();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateGeometrySelection()
{
  int iStart = 0, iEnd = 0;
  p->m_track.GetGeometryCursorPos(sbSelChunksFrom->value(), sbSelChunksTo->value(), iStart, iEnd);
  QTextCursor c = txData->textCursor();
  c.setPosition(iStart);
  c.setPosition(iEnd, QTextCursor::KeepAnchor);
  txData->setTextCursor(c);

  emit UpdateGeometrySelectionSig(sbSelChunksFrom->value(), sbSelChunksTo->value());
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::InsertUIUpdate(int iInsertVal)
{
  sbSelChunksFrom->blockSignals(true);
  sbSelChunksTo->blockSignals(true);
  ckTo->blockSignals(true);
  sbSelChunksFrom->setRange(0, (int)p->m_track.m_chunkAy.size() - 1);
  sbSelChunksTo->setRange(0,   (int)p->m_track.m_chunkAy.size() - 1);
  sbSelChunksTo->setValue(sbSelChunksFrom->value() + iInsertVal - 1);
  ckTo->setChecked(iInsertVal > 1);
  sbSelChunksFrom->blockSignals(false);
  sbSelChunksTo->blockSignals(false);
  ckTo->blockSignals(false);
}

//-------------------------------------------------------------------------------------------------

const QString &CMainWindow::GetTrackFilesFolder()
{
  return m_sTrackFilesFolder;
}

//-------------------------------------------------------------------------------------------------

int CMainWindow::GetSelFrom()
{
  return sbSelChunksFrom->value();
}

//-------------------------------------------------------------------------------------------------

int CMainWindow::GetSelTo()
{
  return sbSelChunksTo->value();
}

//-------------------------------------------------------------------------------------------------