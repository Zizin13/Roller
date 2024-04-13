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
  CTexture m_tex;
  CTexture m_bld;
  CLogDialog m_logDialog;

  QDockWidget *m_pEditDataDockWidget;
  QDockWidget *m_pGlobalSettingsDockWidget;
  QDockWidget *m_pEditSeriesDockWidget;
  QDockWidget *m_pDisplaySettingsDockWidget;
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

  p->m_pEditDataDockWidget = new QDockWidget("Edit Chunk Data", this);
  p->m_pEditDataDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pEditDataDockWidget->setWidget(new CEditDataWidget(p->m_pEditDataDockWidget, &p->m_track, &p->m_tex, &p->m_bld));
  addDockWidget(Qt::LeftDockWidgetArea, p->m_pEditDataDockWidget);
  p->m_pEditDataDockWidget->hide();

  p->m_pGlobalSettingsDockWidget = new QDockWidget("Global Track Settings", this);
  p->m_pGlobalSettingsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pGlobalSettingsDockWidget->setWidget(new CGlobalTrackSettings(p->m_pGlobalSettingsDockWidget, &p->m_track));
  addDockWidget(Qt::LeftDockWidgetArea, p->m_pGlobalSettingsDockWidget);
  p->m_pGlobalSettingsDockWidget->hide();

  p->m_pEditSeriesDockWidget = new QDockWidget("Edit Series...", this);
  p->m_pEditSeriesDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pEditSeriesDockWidget->setWidget(new CEditSeriesDialog(p->m_pEditSeriesDockWidget, &p->m_track));
  addDockWidget(Qt::LeftDockWidgetArea, p->m_pEditSeriesDockWidget);
  p->m_pEditSeriesDockWidget->hide();

  p->m_pDisplaySettingsDockWidget = new QDockWidget("Display Settings", this);
  p->m_pDisplaySettingsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pDisplaySettingsDockWidget->setWidget(new CDisplaySettings(p->m_pDisplaySettingsDockWidget, openGLWidget));
  addDockWidget(Qt::RightDockWidgetArea, p->m_pDisplaySettingsDockWidget);
  p->m_pDisplaySettingsDockWidget->hide();

  //signals
  connect(this, &CMainWindow::LogMsgSig, this, &CMainWindow::OnLogMsg, Qt::QueuedConnection);
  connect(actNew, &QAction::triggered, this, &CMainWindow::OnNewTrack);
  connect(actLoad, &QAction::triggered, this, &CMainWindow::OnLoadTrack);
  connect(actSave, &QAction::triggered, this, &CMainWindow::OnSaveTrack);
  connect(actSaveAs, &QAction::triggered, this, &CMainWindow::OnSaveTrackAs);
  connect(actImportMangled, &QAction::triggered, this, &CMainWindow::OnImportMangled);
  connect(actExportMangled, &QAction::triggered, this, &CMainWindow::OnExportMangled);
  connect(actEditSeries, &QAction::triggered, this, &CMainWindow::OnEditSeries);
  connect(actEditData, &QAction::triggered, this, &CMainWindow::OnEditData);
  connect(actGlobalSettings, &QAction::triggered, this, &CMainWindow::OnGlobalSettings);
  connect(actDisplaySettings, &QAction::triggered, this, &CMainWindow::OnDisplaySettings);
  connect(actDebug, &QAction::triggered, this, &CMainWindow::OnDebug);
  connect(actAbout, &QAction::triggered, this, &CMainWindow::OnAbout);

  connect(ckUnmangleTextures, &QCheckBox::toggled, this, &CMainWindow::OnUnmangleTexturesToggled);

  connect(p->m_pEditDataDockWidget, &QDockWidget::visibilityChanged, this, &CMainWindow::OnEditDataVisibilityChanged);
  connect(p->m_pGlobalSettingsDockWidget, &QDockWidget::visibilityChanged, this, &CMainWindow::OnGlobalSettingsVisibilityChanged);
  connect(p->m_pEditSeriesDockWidget, &QDockWidget::visibilityChanged, this, &CMainWindow::OnEditSeriesVisibilityChanged);
  connect(p->m_pDisplaySettingsDockWidget, &QDockWidget::visibilityChanged, this, &CMainWindow::OnDisplaySettingsVisibilityChanged);

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

void CMainWindow::OnEditSeries()
{
  if (!p->m_pEditSeriesDockWidget->isVisible()) {
    p->m_pEditSeriesDockWidget->show();
  } else {
    p->m_pEditSeriesDockWidget->hide();
  }
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnDisplaySettings()
{
  if (!p->m_pDisplaySettingsDockWidget->isVisible()) {
    p->m_pDisplaySettingsDockWidget->show();
  } else {
    p->m_pDisplaySettingsDockWidget->hide();
  }
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditData()
{
  if (!p->m_pEditDataDockWidget->isVisible()) {
    p->m_pEditDataDockWidget->show();
  } else {
    p->m_pEditDataDockWidget->hide();
  }
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnGlobalSettings()
{
  if (!p->m_pGlobalSettingsDockWidget->isVisible()) {
    p->m_pGlobalSettingsDockWidget->show();
  } else {
    p->m_pGlobalSettingsDockWidget->hide();
  }
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

void CMainWindow::OnEditDataVisibilityChanged(bool bVisible)
{
  actEditData->setChecked(bVisible);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnGlobalSettingsVisibilityChanged(bool bVisible)
{
  actGlobalSettings->setChecked(bVisible);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnEditSeriesVisibilityChanged(bool bVisible)
{
  actEditSeries->setChecked(bVisible);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnDisplaySettingsVisibilityChanged(bool bVisible)
{
  actDisplaySettings->setChecked(bVisible);
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

  show();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::SaveSettings()
{
  QSettings settings(m_sSettingsFile, QSettings::IniFormat);
  settings.setValue("track_folder", m_sTrackFilesFolder);

  QByteArray geometry = saveGeometry();
  QByteArray state = saveState();
  settings.setValue("window_geometry", geometry);
  settings.setValue("window_state", state);
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
  for (int i = 0; i < p->m_track.m_chunkAy.size(); ++i) {
    txData->appendPlainText(p->m_track.m_chunkAy[i].sString.c_str());
  }
  
  //stuff data
  txData->appendPlainText("TEX:" + QString(p->m_track.m_sTextureFile.c_str()));
  txData->appendPlainText("BLD:" + QString(p->m_track.m_sBuildingFile.c_str()));
  txData->appendPlainText("\n");

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
  bool bPalLoaded = p->m_palette.LoadPalette(m_sTrackFilesFolder + QDir::separator() + "PALETTE.PAL");
  bool bTexLoaded = p->m_tex.LoadTexture(m_sTrackFilesFolder + QDir::separator() + QString(p->m_track.m_sTextureFile.c_str()), p->m_palette, bMangled);
  bool bBldLoaded = p->m_bld.LoadTexture(m_sTrackFilesFolder + QDir::separator() + QString(p->m_track.m_sBuildingFile.c_str()), p->m_palette, bMangled);
  lblPalletteLoaded->setVisible(!bPalLoaded);
  frmTex->setVisible(bTexLoaded);
  frmBld->setVisible(bBldLoaded);
  lblTextureLoaded->setText(bTexLoaded ? p->m_track.m_sTextureFile.c_str() : "Texture not loaded");
  lblBuildingsLoaded->setText(bBldLoaded ? p->m_track.m_sBuildingFile.c_str() : "Buildings not loaded");

  //add tiles to viewer layouts
  int iTilesPerLine = (twViewer->width() - 256) / (TILE_WIDTH + 6);
  for (int i = 0; i < p->m_tex.m_tileAy.size(); ++i) {
    QLabel *pImageLabel = new QLabel();
    QPixmap pixmap;
    pixmap.convertFromImage(p->m_tex.m_tileAy[i]);
    pImageLabel->setPixmap(pixmap);
    texLayout->addWidget(pImageLabel, i / iTilesPerLine, i % iTilesPerLine);
    p->m_lblAy.push_back(pImageLabel);
  }
  for (int i = 0; i < p->m_bld.m_tileAy.size(); ++i) {
    QLabel *pImageLabel = new QLabel();
    QPixmap pixmap;
    pixmap.convertFromImage(p->m_bld.m_tileAy[i]);
    pImageLabel->setPixmap(pixmap);
    bldLayout->addWidget(pImageLabel, i / iTilesPerLine, i % iTilesPerLine);
    p->m_lblAy.push_back(pImageLabel);
  }
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
}

//-------------------------------------------------------------------------------------------------