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
  void OnDeleteStuntClicked();
  void OnSignTypeChanged(int iIndex);
  void OnSignTypeLEChanged();
  void OnSignLEChanged();
  void OnBackLEChanged();

private:
  void LoadSettings();
  void SaveSettings();
  bool SaveChangesAndContinue();
  void LoadTextures();
  void UpdateWindow();
  void UpdateGeometrySelection();
  void UpdateInfoSelection();
  bool UpdateLEWithSelectionValue(QLineEdit *pLineEdit, const QString &sValue);
  bool UpdateCBWithSelectionValue(QComboBox *pComboBox, const QString &sValue);
  bool UpdateSignWithSelectionValue(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, const QString &sValue);
  void UpdateSignButtonDisplay(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit);
  void UpdateLEEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, const QString &sValue);
  void UpdateCBEditMode(bool &bEdited, bool &bMixedData, QComboBox *pComboBox, const QString &sValue);
  void UpdateSignEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, QWidget *pWidget, const QString &sValue);
  void RevertGeometry();
  void RevertInfo();
  void UpdateTextures();

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