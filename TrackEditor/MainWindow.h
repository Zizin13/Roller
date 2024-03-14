#ifndef _TRACKEDITOR_MAINWINDOW_H
#define _TRACKEDITOR_MAINWINDOW_H
//-------------------------------------------------------------------------------------------------
#include "ui_MainWindow.h"
//-------------------------------------------------------------------------------------------------
class CMainWindowPrivate;
//-------------------------------------------------------------------------------------------------

class CMainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

public:
  CMainWindow(const QString &sAppPath);
  ~CMainWindow();

  void LogMessage(const QString &sMsg);

protected slots:
  void OnNewTrack();
  void OnLoadTrack();
  void OnSaveTrack();
  void OnSaveTrackAs();
  void OnImportMangled();
  void OnExportMangled();
  void OnDebug();
  void OnAbout();
  void OnEditTabChanged(int iIndex);
  void OnSelChunksFromChanged(int iValue);
  void OnSelChunksToChanged(int iValue);
  void OnSelectedTupleChanged(int iValue);
  void OnToChecked(bool bChecked);
  void OnApplyClicked();
  void OnApplyTupleClicked();
  void OnCancelClicked();
  void OnCancelTupleClicked();
  void OnEditLSurface();
  void OnEditCSurface();
  void OnEditRSurface();
  void UpdateGeometryEditMode();
  void UpdateTuplesEditMode();

private:
  void LoadSettings();
  void SaveSettings();
  bool SaveChangesAndContinue();
  void UpdateWindow();
  void UpdateGeometrySelection();
  void UpdateTupleSelection();
  void UpdateLEWithSelectionValue(QLineEdit *pLineEdit, const QString &sValue);
  bool UpdateLEEditMode(QLineEdit *pLineEdit, const QString &sValue);
  void RevertGeometry();
  void RevertTuples();

  CMainWindowPrivate *p;
  bool m_bUnsavedChanges;
  QString m_sAppPath;
  QString m_sSettingsFile;
  QString m_sTrackFile;
  QString m_sTrackFilesFolder;
};

//-------------------------------------------------------------------------------------------------

extern CMainWindow *g_pMainWindow;

//-------------------------------------------------------------------------------------------------
#endif