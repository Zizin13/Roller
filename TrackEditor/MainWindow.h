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

protected slots:
  void OnLogMsg(QString sMsg);
  void OnNewTrack();
  void OnLoadTrack();
  void OnSaveTrack();
  void OnSaveTrackAs();
  void OnImportMangled();
  void OnExportMangled();
  void OnEditSeries();
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
  void OnEditLWall();
  void OnEditRWall();
  void OnEditRoof();
  void OnEditLUOuterWall();
  void OnEditLLOuterWall();
  void OnEditOuterFloor();
  void OnEditRLOuterWall();
  void OnEditRUOuterWall();
  void OnEditEnvirFloor();
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
  void OnLSurfaceLEChanged();
  void OnCSurfaceLEChanged();
  void OnRSurfaceLEChanged();
  void OnLWallLEChanged();
  void OnRWallLEChanged();
  void OnRoofLEChanged();
  void OnLUOuterWallChanged();
  void OnLLOuterWallChanged();
  void OnOuterFloorChanged();
  void OnRLOuterWallChanged();
  void OnRUOuterWallChanged();
  void OnEnvirFloorChanged();
  void OnUnmangleTexturesToggled(bool bChecked);

signals:
  void LogMsgSig(QString sMsg);

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
  bool UpdateSignWithSelectionValue(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, const QString &sValue, CTexture *pTex);
  void UpdateSignButtonDisplay(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, CTexture *pTex);
  void UpdateLEEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, const QString &sValue);
  void UpdateCBEditMode(bool &bEdited, bool &bMixedData, QComboBox *pComboBox, const QString &sValue);
  void UpdateSignEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, QWidget *pWidget, const QString &sValue);
  void RevertGeometry();
  void RevertInfo();
  void UpdateTextures(QLineEdit *pLineEdit, QLabel *pTex1, QLabel *pTex2);

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