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
  void OnInsertBeforeClicked();
  void OnInsertAfterClicked();
  void OnSelChunksFromChanged(int iValue);
  void OnSelChunksToChanged(int iValue);
  void OnSelectedTupleChanged(int iValue);
  void OnToChecked(bool bChecked);
  void OnApplyClicked();
  void OnApplyTupleClicked();
  void OnApplyStuntClicked();
  void OnApplyTextureClicked();
  void OnApplyInfoClicked();
  void OnCancelClicked();
  void OnCancelTupleClicked();
  void OnCancelStuntClicked();
  void OnCancelTextureClicked();
  void OnCancelInfoClicked();
  void OnDeleteChunkClicked();
  void OnDeleteTuplesClicked();
  void OnDeleteStuntClicked();
  void OnDeleteBackClicked();
  void OnEditLSurface();
  void OnEditCSurface();
  void OnEditRSurface();
  void OnTupleLValChanged();
  void UpdateGeometryEditMode();
  void UpdateTuplesEditMode();
  void OnStuntIndexChanged();
  void UpdateStuntsEditMode();
  void OnBackIndexChanged();
  void UpdateTexturesEditMode();
  void UpdateInfoEditMode();

private:
  void LoadSettings();
  void SaveSettings();
  bool SaveChangesAndContinue();
  void UpdateWindow();
  void UpdateGeometrySelection();
  void UpdateTupleSelection();
  void UpdateStuntSelection();
  void UpdateBackSelection();
  void UpdateInfoSelection();
  bool UpdateLEWithSelectionValue(QLineEdit *pLineEdit, const QString &sValue);
  void UpdateLEEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, const QString &sValue, bool bNew = false);
  void RevertGeometry();
  void RevertTuples();
  void RevertStunts();
  void RevertBacks();
  void RevertInfo();

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