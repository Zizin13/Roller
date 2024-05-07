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
#include "EditSignWidget.h"
#include "EditAudioWidget.h"
#include "EditStuntWidget.h"
#include "qtextstream.h"
#include "QtHelpers.h"
#include "Logging.h"
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

static void LogMessageCbStatic(const char *szMsg, int iLen)
{
  (void)(iLen);
  g_pMainWindow->LogMessage(szMsg);
}

//-------------------------------------------------------------------------------------------------

class CMainWindowPrivate
{
public:
  CMainWindowPrivate(CMainWindow *pMainWindow)
    : m_logDialog(pMainWindow)
  {};
  ~CMainWindowPrivate() 
  {};
  
  CLogDialog m_logDialog;

  QDockWidget *m_pEditDataDockWidget;
  CEditDataWidget *m_pEditData;
  QDockWidget *m_pGlobalSettingsDockWidget;
  QDockWidget *m_pEditSeriesDockWidget;
  QDockWidget *m_pDisplaySettingsDockWidget;
  QDockWidget *m_pEditGeometryDockWidget;
  QDockWidget *m_pEditSignDockWidget;
  QDockWidget *m_pEditAudioDockWidget;
  QDockWidget *m_pEditStuntDockWidget;
  CDisplaySettings *m_pDisplaySettings;
  QAction *m_pDebugAction;
  std::vector<CTrackPreview *> m_previewAy;
};

//-------------------------------------------------------------------------------------------------

CMainWindow::CMainWindow(const QString &sAppPath, float fDesktopScale)
  : QMainWindow(NULL)
  , m_sAppPath(sAppPath)
  , m_sLastTrackFilesFolder("")
  , m_fDesktopScale(fDesktopScale)
{
  //init
  Logging::SetWhipLibLoggingCallback(LogMessageCbStatic);
  p = new CMainWindowPrivate(this);
  g_pMainWindow = this;
  m_sSettingsFile = m_sAppPath + "/TrackEditor.ini";
  m_sSettingsFile = QDir::toNativeSeparators(m_sSettingsFile);
  setupUi(this);
  p->m_logDialog.hide();
  twViewer->setTabsClosable(true);

  //setup dock widgets
  p->m_pEditDataDockWidget = new QDockWidget("Debug Chunk Data", this);
  p->m_pEditDataDockWidget->setObjectName("EditChunkData");
  p->m_pEditDataDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pEditData = new CEditDataWidget(p->m_pEditDataDockWidget);
  p->m_pEditDataDockWidget->setWidget(p->m_pEditData);

  p->m_pGlobalSettingsDockWidget = new QDockWidget("Global Track Settings", this);
  p->m_pGlobalSettingsDockWidget->setObjectName("GlobalTrackSettings");
  p->m_pGlobalSettingsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pGlobalSettingsDockWidget->setWidget(new CGlobalTrackSettings(p->m_pGlobalSettingsDockWidget));

  p->m_pEditSeriesDockWidget = new QDockWidget("Edit Series...", this);
  p->m_pEditSeriesDockWidget->setObjectName("EditSeries");
  p->m_pEditSeriesDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pEditSeriesDockWidget->setWidget(new CEditSeriesDialog(p->m_pEditSeriesDockWidget));

  p->m_pDisplaySettingsDockWidget = new QDockWidget("Display Settings", this);
  p->m_pDisplaySettingsDockWidget->setObjectName("DisplaySettings");
  p->m_pDisplaySettingsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pDisplaySettings = new CDisplaySettings(p->m_pDisplaySettingsDockWidget);
  p->m_pDisplaySettingsDockWidget->setWidget(p->m_pDisplaySettings);

  p->m_pEditGeometryDockWidget = new QDockWidget("Edit Chunk Data", this);
  p->m_pEditGeometryDockWidget->setObjectName("EditGeometry");
  p->m_pEditGeometryDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pEditGeometryDockWidget->setWidget(new CEditGeometryWidget(p->m_pEditGeometryDockWidget));

  p->m_pEditSignDockWidget = new QDockWidget("Edit Signs", this);
  p->m_pEditSignDockWidget->setObjectName("EditSigns");
  p->m_pEditSignDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pEditSignDockWidget->setWidget(new CEditSignWidget(p->m_pEditSignDockWidget));

  p->m_pEditAudioDockWidget = new QDockWidget("Edit Audio", this);
  p->m_pEditAudioDockWidget->setObjectName("EditAudio");
  p->m_pEditAudioDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pEditAudioDockWidget->setWidget(new CEditAudioWidget(p->m_pEditAudioDockWidget));

  p->m_pEditStuntDockWidget = new QDockWidget("Edit Stunts", this);
  p->m_pEditStuntDockWidget->setObjectName("EditStunts");
  p->m_pEditStuntDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  p->m_pEditStuntDockWidget->setWidget(new CEditStuntWidget(p->m_pEditStuntDockWidget));

  //setup view menu
  menuView->addAction(p->m_pEditGeometryDockWidget->toggleViewAction());
  menuView->addAction(p->m_pEditSignDockWidget->toggleViewAction());
  menuView->addAction(p->m_pEditAudioDockWidget->toggleViewAction());
  menuView->addAction(p->m_pEditStuntDockWidget->toggleViewAction());
  menuView->addAction(p->m_pEditSeriesDockWidget->toggleViewAction());
  menuView->addAction(p->m_pGlobalSettingsDockWidget->toggleViewAction());
  menuView->addAction(p->m_pDisplaySettingsDockWidget->toggleViewAction());
  menuView->addSeparator();
  menuView->addAction(p->m_pEditDataDockWidget->toggleViewAction());
  p->m_pDebugAction = new QAction("Debug Log", menuView);
  p->m_pDebugAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
  menuView->addAction(p->m_pDebugAction);

  //signals
  connect(this, &CMainWindow::LogMsgSig, this, &CMainWindow::OnLogMsg, Qt::QueuedConnection);
  connect(actNew, &QAction::triggered, this, &CMainWindow::OnNewTrack);
  connect(actLoad, &QAction::triggered, this, &CMainWindow::OnLoadTrack);
  connect(actSave, &QAction::triggered, this, &CMainWindow::OnSaveTrack);
  connect(actSaveAs, &QAction::triggered, this, &CMainWindow::OnSaveTrackAs);
  connect(p->m_pDebugAction, &QAction::triggered, this, &CMainWindow::OnDebug);
  connect(actAbout, &QAction::triggered, this, &CMainWindow::OnAbout);
  connect(twViewer, &QTabWidget::tabCloseRequested, this, &CMainWindow::OnTabCloseRequested);
  connect(twViewer, &QTabWidget::currentChanged, this, &CMainWindow::OnTabChanged);

  connect(sbSelChunksFrom, SIGNAL(valueChanged(int)), this, SLOT(OnSelChunksFromChanged(int)));
  connect(sbSelChunksTo, SIGNAL(valueChanged(int)), this, SLOT(OnSelChunksToChanged(int)));
  connect(ckTo, &QCheckBox::toggled, this, &CMainWindow::OnToChecked);
  connect(pbDelete, &QPushButton::clicked, this, &CMainWindow::OnDeleteChunkClicked);
  connect(pbAddChunk, &QPushButton::clicked, this, &CMainWindow::OnAddChunkClicked);

  connect(p->m_pDisplaySettings, &CDisplaySettings::UpdatePreviewSig, this, &CMainWindow::OnUpdatePreview);
  connect(p->m_pDisplaySettings, &CDisplaySettings::SetScaleSig, this, &CMainWindow::OnSetScale);
  connect(p->m_pDisplaySettings, &CDisplaySettings::AttachLastCheckedSig, this, &CMainWindow::OnAttachLast);

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
  if (!SaveChangesAndContinue()) {
    pEvent->ignore();
    return;
  }
  pEvent->accept();

  SaveSettings();

  //cleanup
  twViewer->blockSignals(true);
  for (int i = 0; i < (int)p->m_previewAy.size(); ++i) {
    p->m_previewAy[i]->makeCurrent();
    delete p->m_previewAy[i];
  }
  p->m_previewAy.clear();
  twViewer->blockSignals(false);

  QMainWindow::closeEvent(pEvent);

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
  if (GetCurrentPreview())
    GetCurrentPreview()->m_bUnsavedChanges = bUnsavedChanges;
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnLogMsg(QString sMsg)
{
  if (p)
    p->m_logDialog.LogMessage(sMsg);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnNewTrack()
{
  //sbSelChunksFrom->setValue(0);
  //sbSelChunksTo->setValue(0);
  //p->m_trackAy[0]->ClearData();
  //m_sTrackFile = "";
  //m_bUnsavedChanges = false;
  //m_bAlreadySaved = false;
  //LoadTextures();
  //UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnLoadTrack()
{
  //load track
  QString sFilename = QDir::toNativeSeparators(QFileDialog::getOpenFileName(
    this, "Load Track", m_sLastTrackFilesFolder, QString("Track Files (*.TRK)")));
  if (sFilename.isEmpty())
    return;

  for (int i = 0; i < (int)p->m_previewAy.size(); ++i) {
    if (p->m_previewAy[i]->GetFilename().compare(sFilename) == 0) {
      twViewer->setCurrentIndex(i);
      return;
    }
  }

  CTrackPreview *pPreview = new CTrackPreview(this);
  if (!pPreview->LoadTrack(sFilename)) {
    //load failed
    delete pPreview;
  } else { //load successful    
    //update variables
    m_sLastTrackFilesFolder = sFilename.left(sFilename.lastIndexOf(QDir::separator()));

    //add to array and create preview window
    p->m_previewAy.push_back(pPreview);
    twViewer->addTab(pPreview, "Preview");

    OnUpdatePreview();
    OnSetScale(p->m_pDisplaySettings->GetScale());
    OnAttachLast(p->m_pDisplaySettings->GetAttachLast());
    twViewer->setCurrentIndex((int)p->m_previewAy.size() - 1);
  }
  //update app
  UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnSaveTrack()
{
  if (GetCurrentPreview())
    GetCurrentPreview()->SaveTrack();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnSaveTrackAs()
{
  if (GetCurrentPreview())
    GetCurrentPreview()->SaveTrackAs();
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

void CMainWindow::OnTabCloseRequested(int iIndex)
{
  if (iIndex > p->m_previewAy.size()) return;

  bool bChangeCurrentIndex = iIndex == twViewer->currentIndex();

  twViewer->blockSignals(true);
  if (p->m_previewAy[iIndex]->SaveChangesAndContinue()) {
    p->m_previewAy[iIndex]->makeCurrent();
    delete p->m_previewAy[iIndex];
    p->m_previewAy.erase(p->m_previewAy.begin() + iIndex);
  }
  twViewer->blockSignals(false);

  if (bChangeCurrentIndex) {
    OnTabChanged(twViewer->currentIndex());
  }
  UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnTabChanged(int iIndex)
{
  if (iIndex > p->m_previewAy.size()) return;

  eWhipModel carModel;
  eShapeSection aiLine;
  bool bMillionPlus;
  uint32 uiShowModels = p->m_pDisplaySettings->GetDisplaySettings(carModel, aiLine, bMillionPlus);
  p->m_previewAy[iIndex]->makeCurrent();
  p->m_previewAy[iIndex]->ShowModels(uiShowModels);
  p->m_previewAy[iIndex]->UpdateCar(carModel, aiLine, bMillionPlus);
  p->m_previewAy[iIndex]->AttachLast(p->m_pDisplaySettings->GetAttachLast());
  p->m_previewAy[iIndex]->SetScale(p->m_pDisplaySettings->GetScale());
  BLOCK_SIG_AND_DO(sbSelChunksFrom, setValue(p->m_previewAy[iIndex]->m_iSelFrom));
  BLOCK_SIG_AND_DO(sbSelChunksTo, setValue(p->m_previewAy[iIndex]->m_iSelTo));
  BLOCK_SIG_AND_DO(ckTo, setChecked(p->m_previewAy[iIndex]->m_bToChecked));

  UpdateWindow();
  UpdateGeometrySelection();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnSelChunksFromChanged(int iValue)
{
  if (!ckTo->isChecked() || sbSelChunksTo->value() < iValue) {
    BLOCK_SIG_AND_DO(sbSelChunksTo, setValue(iValue));
  }
  UpdateGeometrySelection();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnSelChunksToChanged(int iValue)
{
  if (sbSelChunksFrom->value() > iValue) {
    BLOCK_SIG_AND_DO(sbSelChunksFrom, setValue(iValue));
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
  if (!GetCurrentTrack() || GetCurrentTrack()->m_chunkAy.empty()) return;
  if (sbSelChunksFrom->value() > sbSelChunksTo->value() || sbSelChunksTo->value() > GetCurrentTrack()->m_chunkAy.size()) {
    assert(0);
    return;
  }

  GetCurrentTrack()->m_chunkAy.erase(
    GetCurrentTrack()->m_chunkAy.begin() + sbSelChunksFrom->value(),
    GetCurrentTrack()->m_chunkAy.begin() + sbSelChunksTo->value() + 1);

  GetCurrentPreview()->m_bUnsavedChanges = true;
  g_pMainWindow->LogMessage("Deleted geometry chunk");
  g_pMainWindow->UpdateWindow();
  BLOCK_SIG_AND_DO(sbSelChunksTo, setValue(sbSelChunksFrom->value()));
  p->m_pEditData->UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnAddChunkClicked()
{
  if (!GetCurrentTrack()) return;

  CChunkEditValues editVals;
  int iLastPos = (int)GetCurrentTrack()->m_chunkAy.size() - 1;
  GetCurrentTrack()->GetGeometryValuesFromSelection(iLastPos, iLastPos, editVals);
  GetCurrentTrack()->InsertGeometryChunk(iLastPos, 1, editVals);

  GetCurrentPreview()->m_bUnsavedChanges = true;
  g_pMainWindow->LogMessage("Added geometry chunk");
  g_pMainWindow->UpdateWindow();
  BLOCK_SIG_AND_DO(sbSelChunksTo, setValue(iLastPos + 1));
  BLOCK_SIG_AND_DO(sbSelChunksFrom, setValue(iLastPos + 1));
  p->m_pEditData->UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnAttachLast(bool bChecked)
{
  if (!GetCurrentPreview()) return;

  GetCurrentPreview()->AttachLast(bChecked);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnSetScale(int iValue)
{
  if (!GetCurrentPreview()) return;

  GetCurrentPreview()->SetScale(iValue);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::OnUpdatePreview()
{
  if (!GetCurrentPreview()) return;

  eWhipModel carModel;
  eShapeSection aiLine;
  bool bMillionPlus;
  uint32 uiShowModels = p->m_pDisplaySettings->GetDisplaySettings(carModel, aiLine, bMillionPlus);
  GetCurrentPreview()->ShowModels(uiShowModels);
  GetCurrentPreview()->UpdateCar(carModel, aiLine, bMillionPlus);
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::LoadSettings()
{
  QSettings settings(m_sSettingsFile, QSettings::IniFormat);
  m_sLastTrackFilesFolder = settings.value("track_folder", m_sLastTrackFilesFolder).toString();

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
      && settings.contains("show_edit_geometry")
      && settings.contains("show_edit_sign")
      && settings.contains("show_edit_audio")
      && settings.contains("show_edit_stunt")) {
    bool bShowEditData = false;
    bool bShowGlobalSettings = false;
    bool bShowEditSeries = false;
    bool bShowDisplaySettings = false;
    bool bShowEditGeometry = false;
    bool bShowEditSign = false;
    bool bShowEditAudio = false;
    bool bShowEditStunt = false;
    bShowEditData = settings.value("show_edit_data", bShowEditData).toBool();
    bShowGlobalSettings = settings.value("show_global_settings", bShowGlobalSettings).toBool();
    bShowEditSeries = settings.value("show_edit_series", bShowEditSeries).toBool();
    bShowDisplaySettings = settings.value("show_display_settings", bShowDisplaySettings).toBool();
    bShowEditGeometry = settings.value("show_edit_geometry", bShowEditGeometry).toBool();
    bShowEditSign = settings.value("show_edit_sign", bShowEditSign).toBool();
    bShowEditAudio = settings.value("show_edit_audio", bShowEditAudio).toBool();
    bShowEditStunt = settings.value("show_edit_stunt", bShowEditStunt).toBool();
    p->m_pEditDataDockWidget->setVisible(bShowEditData);
    p->m_pGlobalSettingsDockWidget->setVisible(bShowGlobalSettings);
    p->m_pEditSeriesDockWidget->setVisible(bShowEditSeries);
    p->m_pDisplaySettingsDockWidget->setVisible(bShowDisplaySettings);
    p->m_pEditGeometryDockWidget->setVisible(bShowEditGeometry);
    p->m_pEditSignDockWidget->setVisible(bShowEditSign);
    p->m_pEditAudioDockWidget->setVisible(bShowEditAudio);
    p->m_pEditStuntDockWidget->setVisible(bShowEditStunt);
    restoreDockWidget(p->m_pEditDataDockWidget);
    restoreDockWidget(p->m_pGlobalSettingsDockWidget);
    restoreDockWidget(p->m_pEditSeriesDockWidget);
    restoreDockWidget(p->m_pDisplaySettingsDockWidget);
    restoreDockWidget(p->m_pEditGeometryDockWidget);
    restoreDockWidget(p->m_pEditSignDockWidget);
    restoreDockWidget(p->m_pEditAudioDockWidget);
    restoreDockWidget(p->m_pEditStuntDockWidget);
  } else {
    addDockWidget(Qt::LeftDockWidgetArea, p->m_pEditDataDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, p->m_pGlobalSettingsDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, p->m_pEditSeriesDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, p->m_pDisplaySettingsDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, p->m_pEditGeometryDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, p->m_pEditSignDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, p->m_pEditAudioDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, p->m_pEditStuntDockWidget);
    p->m_pEditDataDockWidget->setVisible(false);
    p->m_pGlobalSettingsDockWidget->setVisible(false);
    p->m_pEditSeriesDockWidget->setVisible(false);
    p->m_pDisplaySettingsDockWidget->setVisible(false);
    p->m_pEditGeometryDockWidget->setVisible(false);
    p->m_pEditSignDockWidget->setVisible(false);
    p->m_pEditAudioDockWidget->setVisible(false);
    p->m_pEditStuntDockWidget->setVisible(false);
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
  if (settings.contains("scale")) {
    int iScale = p->m_pDisplaySettings->GetScale();
    iScale = settings.value("scale", iScale).toInt();
    p->m_pDisplaySettings->SetScale(iScale);
  }

  show();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::SaveSettings()
{
  QSettings settings(m_sSettingsFile, QSettings::IniFormat);
  settings.setValue("track_folder", m_sLastTrackFilesFolder);

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
  settings.setValue("show_edit_sign", p->m_pEditSignDockWidget->isVisible());
  settings.setValue("show_edit_audio", p->m_pEditAudioDockWidget->isVisible());
  settings.setValue("show_edit_stunt", p->m_pEditStuntDockWidget->isVisible());
  settings.setValue("show_models", p->m_pDisplaySettings->GetDisplaySettings(carModel, aiLine, bMillionPlus));
  settings.setValue("car_model", (int)carModel);
  settings.setValue("car_pos", (int)aiLine);
  settings.setValue("wrong_way", bMillionPlus);
  settings.setValue("attach_last", p->m_pDisplaySettings->GetAttachLast());
  settings.setValue("scale", p->m_pDisplaySettings->GetScale());
}

//-------------------------------------------------------------------------------------------------

bool CMainWindow::SaveChangesAndContinue()
{
  bool bRet = true;
  for (int i = 0; i < p->m_previewAy.size(); ++i)
    bRet &= p->m_previewAy[i]->SaveChangesAndContinue();

  UpdateWindow();
  return bRet;
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateWindow()
{
  //update title bar
  QString sTitle = "Track Editor";
  QString sCurrentTab = "";
  for (int i = 0; i < (int)p->m_previewAy.size(); ++i) {
    if (i == twViewer->currentIndex())
      sCurrentTab = p->m_previewAy[i]->GetTitle(true) + " - ";
    twViewer->setTabText(i, p->m_previewAy[i]->GetTitle(false));
  }
  setWindowTitle(sCurrentTab + sTitle);

  if (GetCurrentPreview()) {
    GetCurrentPreview()->UpdateTrack();

    BLOCK_SIG_AND_DO(sbSelChunksFrom, setRange(0, (int)GetCurrentTrack()->m_chunkAy.size() - 1));
    BLOCK_SIG_AND_DO(sbSelChunksTo, setRange(0, (int)GetCurrentTrack()->m_chunkAy.size() - 1));
    leChunkCount->setText(QString::number(GetCurrentTrack()->m_chunkAy.size()));
  }
  UpdateGeometrySelection();
  emit UpdateWindowSig();
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::UpdateGeometrySelection()
{
  if (GetCurrentPreview()) {
    GetCurrentPreview()->m_iSelFrom = sbSelChunksFrom->value();
    GetCurrentPreview()->m_iSelTo = sbSelChunksTo->value();
    GetCurrentPreview()->m_bToChecked = ckTo->isChecked();
    GetCurrentPreview()->UpdateGeometrySelection();
  }
  emit UpdateGeometrySelectionSig(sbSelChunksFrom->value(), sbSelChunksTo->value());
}

//-------------------------------------------------------------------------------------------------

void CMainWindow::InsertUIUpdate(int iInsertVal)
{
  if (GetCurrentTrack()) {
    BLOCK_SIG_AND_DO(sbSelChunksFrom, setRange(0, (int)GetCurrentTrack()->m_chunkAy.size() - 1));
    BLOCK_SIG_AND_DO(sbSelChunksTo, setRange(0, (int)GetCurrentTrack()->m_chunkAy.size() - 1));
  }
  BLOCK_SIG_AND_DO(sbSelChunksTo, setValue(sbSelChunksFrom->value() + iInsertVal - 1));
  BLOCK_SIG_AND_DO(ckTo, setChecked(iInsertVal > 1));
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

CTrack *CMainWindow::GetCurrentTrack()
{
  if (!GetCurrentPreview()) return NULL;
  return GetCurrentPreview()->GetTrack();
}

//-------------------------------------------------------------------------------------------------

CTrackPreview *CMainWindow::GetCurrentPreview()
{
  if (p->m_previewAy.empty() || twViewer->currentIndex() >= (int)p->m_previewAy.size()) return NULL;
  return p->m_previewAy[twViewer->currentIndex()];
}

//-------------------------------------------------------------------------------------------------