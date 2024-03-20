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
  void OnToChecked(bool bChecked);
  void OnApplyClicked();
  void OnApplyTextureClicked();
  void OnApplyInfoClicked();
  void OnCancelClicked();
  void OnCancelTextureClicked();
  void OnCancelInfoClicked();
  void OnDeleteChunkClicked();
  void OnEditLSurface();
  void OnEditCSurface();
  void OnEditRSurface();
  void UpdateGeometryEditMode();
  void UpdateTexturesEditMode();
  void UpdateInfoEditMode();
  void OnSignClicked();
  void OnBackClicked();
  void OnApplySignToggled(bool bChecked);
  void OnApplyBackToggled(bool bChecked);

private:
  void LoadSettings();
  void SaveSettings();
  bool SaveChangesAndContinue();
  void LoadTextures();
  void UpdateWindow();
  void UpdateGeometrySelection();
  void UpdateInfoSelection();
  bool UpdateLEWithSelectionValue(QLineEdit *pLineEdit, const QString &sValue);
  bool UpdateCKWithSelectionValue(QCheckBox *pCheckbox, const QString &sValue);
  bool UpdateSignWithSelectionValue(QPushButton *pPushButton, QCheckBox *pCheckbox, QLabel *pLabel, const QString &sValue);
  void UpdateSignButtonDisplay(QPushButton *pPushButton, QCheckBox *pCheckbox, QLabel *pLabel);
  void UpdateLEEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, const QString &sValue);
  void UpdateSignEditMode(bool &bEdited, bool &bMixedData, QPushButton *pPushButton, QWidget *pWidget, const QString &sValue);
  void UpdateCKEditMode(bool &bEdited, bool &bMixedData, QCheckBox *pCheckbox, const QString &sValue);
  void RevertGeometry();
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