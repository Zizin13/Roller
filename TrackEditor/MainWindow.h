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
  CMainWindow(const QString &sAppPath, float fDesktopScale);
  ~CMainWindow();

  void LogMessage(const QString &sMsg);
  void SetUnsavedChanges(bool bUnsavedChanges);
  void UpdateWindow();
  void LoadTextures();
  void InsertUIUpdate(int iInsertVal);
  const QString &GetTrackFilesFolder();
  int GetSelFrom();
  int GetSelTo();
  float GetDesktopScale() { return m_fDesktopScale; };

protected:
  void closeEvent(QCloseEvent *pEvent);

protected slots:
  void OnLogMsg(QString sMsg);
  void OnNewTrack();
  void OnLoadTrack();
  void OnSaveTrack();
  void OnSaveTrackAs();
  void OnDebug();
  void OnAbout();
  void OnSelChunksFromChanged(int iValue);
  void OnSelChunksToChanged(int iValue);
  void OnToChecked(bool bChecked);
  void OnDeleteChunkClicked();
  void OnAddChunkClicked();

signals:
  void LogMsgSig(QString sMsg);
  void UpdateWindowSig();
  void UpdateGeometrySelectionSig(int iFrom, int iTo);

private:
  void LoadSettings();
  void SaveSettings();
  bool SaveChangesAndContinue();
  void UpdateGeometrySelection();

  CMainWindowPrivate *p;
  bool m_bUnsavedChanges;
  bool m_bAlreadySaved;
  QString m_sAppPath;
  QString m_sSettingsFile;
  QString m_sTrackFile;
  QString m_sTrackFilesFolder;
  float m_fDesktopScale;
};

//-------------------------------------------------------------------------------------------------

extern CMainWindow *g_pMainWindow;

//-------------------------------------------------------------------------------------------------
#endif