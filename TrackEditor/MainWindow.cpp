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

  std::vector<QLabel *> m_lblAy;
  CTrack m_track;
  CPalette m_palette;
  CLogDialog m_logDialog;

  QDockWidget *m_pEditDataDockWidget;
  QDockWidget *m_pGlobalSettingsDockWidget;
  QDockWidget *m_pEditSeriesDockWidget;
  QDockWidget *m_pDisplaySettingsDockWidget;
  CDisplaySettings *m_pDisplaySettings;
  QAction *m_pDebugAction;
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
  frmTex->hide();
  frmBld->hide();

  //setup dock widgets
  p->m_pEditDataDockWidget = new QDockWidget("Edit Chunk Data", this);
  p->m_pEditDataDockWidget->setObjectName("EditChunkData");
  p->m_pEditDataDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pEditDataDockWidget->setWidget(new CEditDataWidget(p->m_pEditDataDockWidget, &p->m_track));

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

  //setup view menu
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
  connect(actImportMangled, &QAction::triggered, this, &CMainWindow::OnImportMangled);
  connect(actExportMangled, &QAction::triggered, this, &CMainWindow::OnExportMangled);
  connect(p->m_pDebugAction, &QAction::triggered, this, &CMainWindow::OnDebug);
  connect(actAbout, &QAction::triggered, this, &CMainWindow::OnAbout);

  connect(ckUnmangleTextures, &QCheckBox::toggled, this, &CMainWindow::OnUnmangleTexturesToggled);

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
  openGLWidget->DeleteModels();
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

  emit ResetSelectionSig();
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
    emit ResetSelectionSig();
    
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
    emit ResetSelectionSig();

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

void CMainWindow::OnAbout()
{
  QMessageBox::information(this, "Git Gud", "YOU NEED MORE PRACTICE");
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
      && settings.contains("show_display_settings")) {
    bool bShowEditData = false;
    bool bShowGlobalSettings = false;
    bool bShowEditSeries = false;
    bool bShowDisplaySettings = false;
    bShowEditData = settings.value("show_edit_data", bShowEditData).toBool();
    bShowGlobalSettings = settings.value("show_global_settings", bShowGlobalSettings).toBool();
    bShowEditSeries = settings.value("show_edit_series", bShowEditSeries).toBool();
    bShowDisplaySettings = settings.value("show_display_settings", bShowDisplaySettings).toBool();
    p->m_pEditDataDockWidget->setVisible(bShowEditData);
    p->m_pGlobalSettingsDockWidget->setVisible(bShowGlobalSettings);
    p->m_pEditSeriesDockWidget->setVisible(bShowEditSeries);
    p->m_pDisplaySettingsDockWidget->setVisible(bShowDisplaySettings);
    restoreDockWidget(p->m_pEditDataDockWidget);
    restoreDockWidget(p->m_pGlobalSettingsDockWidget);
    restoreDockWidget(p->m_pEditSeriesDockWidget);
    restoreDockWidget(p->m_pDisplaySettingsDockWidget);
  } else {
    addDockWidget(Qt::LeftDockWidgetArea, p->m_pEditDataDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, p->m_pGlobalSettingsDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, p->m_pEditSeriesDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, p->m_pDisplaySettingsDockWidget);
    p->m_pEditDataDockWidget->setVisible(false);
    p->m_pGlobalSettingsDockWidget->setVisible(false);
    p->m_pEditSeriesDockWidget->setVisible(false);
    p->m_pDisplaySettingsDockWidget->setVisible(false);
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
  settings.setValue("show_models", p->m_pDisplaySettings->GetDisplaySettings(carModel, aiLine, bMillionPlus));
  settings.setValue("car_model", (int)carModel);
  settings.setValue("car_pos", (int)aiLine);
  settings.setValue("wrong_way", bMillionPlus);
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

  openGLWidget->SetTrack(&p->m_track);
  emit UpdateWindowSig();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::LoadTextures()
{
  bool bMangled = ckUnmangleTextures->isChecked();

  //avoid memory leak
  while (!p->m_lblAy.empty()) {
    std::vector<QLabel *>::iterator it = p->m_lblAy.begin();
    QLabel *pLabel = *it;
    delete pLabel;
    p->m_lblAy.erase(it);
  }
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
  QString sPal = m_sTrackFilesFolder + QDir::separator() + "PALETTE.PAL";
  QString sTex = m_sTrackFilesFolder + QDir::separator() + QString(p->m_track.m_sTextureFile.c_str());
  QString sBld = m_sTrackFilesFolder + QDir::separator() + QString(p->m_track.m_sBuildingFile.c_str());
  bool bPalLoaded = p->m_palette.LoadPalette(sPal.toLatin1().constData());
  bool bTexLoaded = p->m_track.m_tex.LoadTexture(sTex.toLatin1().constData(), &p->m_palette, bMangled);
  bool bBldLoaded = p->m_track.m_bld.LoadTexture(sBld.toLatin1().constData(), &p->m_palette, bMangled);
  lblPalletteLoaded->setVisible(!bPalLoaded);
  frmTex->setVisible(bTexLoaded);
  frmBld->setVisible(bBldLoaded);
  lblTextureLoaded->setText(bTexLoaded ? p->m_track.m_sTextureFile.c_str() : "Texture not loaded");
  lblBuildingsLoaded->setText(bBldLoaded ? p->m_track.m_sBuildingFile.c_str() : "Buildings not loaded");

  //add tiles to viewer layouts
  int iTilesPerLine = (twViewer->width() - 256) / (TILE_WIDTH + 6);
  for (int i = 0; i < p->m_track.m_tex.m_iNumTiles; ++i) {
    QLabel *pImageLabel = new QLabel();
    QPixmap pixmap;
    pixmap.convertFromImage(QtHelpers::GetQImageFromTile(p->m_track.m_tex.m_pTileAy[i]));
    pImageLabel->setPixmap(pixmap);
    texLayout->addWidget(pImageLabel, i / iTilesPerLine, i % iTilesPerLine);
    p->m_lblAy.push_back(pImageLabel);
  }
  for (int i = 0; i < p->m_track.m_bld.m_iNumTiles; ++i) {
    QLabel *pImageLabel = new QLabel();
    QPixmap pixmap;
    pixmap.convertFromImage(QtHelpers::GetQImageFromTile(p->m_track.m_bld.m_pTileAy[i]));
    pImageLabel->setPixmap(pixmap);
    bldLayout->addWidget(pImageLabel, i / iTilesPerLine, i % iTilesPerLine);
    p->m_lblAy.push_back(pImageLabel);
  }

  //make sure car textures are reloaded too
  openGLWidget->ReloadCar();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateGeometrySelection(int iFrom, int iTo)
{
  int iStart = 0, iEnd = 0;
  p->m_track.GetGeometryCursorPos(iFrom, iTo, iStart, iEnd);
  QTextCursor c = txData->textCursor();
  c.setPosition(iStart);
  c.setPosition(iEnd, QTextCursor::KeepAnchor);
  txData->setTextCursor(c);

  openGLWidget->UpdateGeometrySelection(iFrom, iTo);
}

//-------------------------------------------------------------------------------------------------

bool CMainWindow::UnmangleTextures()
{
  return ckUnmangleTextures->isChecked();
}

//-------------------------------------------------------------------------------------------------

const QString &CMainWindow::GetTrackFilesFolder()
{
  return m_sTrackFilesFolder;
}

//-------------------------------------------------------------------------------------------------