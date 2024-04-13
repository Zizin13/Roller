#ifndef _TRACKEDITOR_MAINWINDOW_H
#define _TRACKEDITOR_MAINWINDOW_H
//-------------------------------------------------------------------------------------------------
#include "ui_MainWindow.h"
//-------------------------------------------------------------------------------------------------
class CMainWindowPrivate;
class CTexture;
//-------------------------------------------------------------------------------------------------

class CMainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

public:
  CMainWindow(const QString &sAppPath);
  ~CMainWindow();

  void LogMessage(const QString &sMsg);
  void SetUnsavedChanges(bool bUnsavedChanges);
  void UpdateWindow();
  void LoadTextures();
  void UpdateGeometrySelection(int iFrom, int iTo);

protected slots:
  void OnLogMsg(QString sMsg);
  void OnNewTrack();
  void OnLoadTrack();
  void OnSaveTrack();
  void OnSaveTrackAs();
  void OnImportMangled();
  void OnExportMangled();
  void OnEditSeries();
  void OnDisplaySettings();
  void OnEditData();
  void OnGlobalSettings();
  void OnDebug();
  void OnAbout();
  void OnUnmangleTexturesToggled(bool bChecked);
  void OnEditDataVisibilityChanged(bool bVisible);
  void OnGlobalSettingsVisibilityChanged(bool bVisible);
  void OnEditSeriesVisibilityChanged(bool bVisible);
  void OnDisplaySettingsVisibilityChanged(bool bVisible);

signals:
  void LogMsgSig(QString sMsg);
  void ResetSelectionSig();
  void UpdateWindowSig();

private:
  void LoadSettings();
  void SaveSettings();
  bool SaveChangesAndContinue();

  CMainWindowPrivate *p;
  bool m_bUnsavedChanges;
  bool m_bAlreadySaved;
  QString m_sAppPath;
  QString m_sSettingsFile;
  QString m_sTrackFile;
  QString m_sTrackFilesFolder;
};

//-------------------------------------------------------------------------------------------------

extern CMainWindow *g_pMainWindow;

//-------------------------------------------------------------------------------------------------
#endif