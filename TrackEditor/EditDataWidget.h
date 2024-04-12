#ifndef _TRACKEDITOR_EDITDATAWIDGET_H
#define _TRACKEDITOR_EDITDATAWIDGET_H
//-------------------------------------------------------------------------------------------------
#include "ui_EditDataWidget.h"
//-------------------------------------------------------------------------------------------------
class CEditDataWidgetPrivate;
class CTrack;
class CTexture;
//-------------------------------------------------------------------------------------------------
class CEditDataWidget : public QWidget, private Ui::EditDataWidget
{
  Q_OBJECT

public:
  CEditDataWidget(QWidget *pParent, CTrack *pTrack, CTexture *pTex, CTexture *pBld);
  ~CEditDataWidget();

protected slots:
  void OnResetSelection();
  void OnUpdateWindow();
  void OnInsertBeforeClicked();
  void OnInsertAfterClicked();
  void OnSelChunksFromChanged(int iValue);
  void OnSelChunksToChanged(int iValue);
  void OnToChecked(bool bChecked);
  void OnApplyClicked();
  void OnCancelClicked();
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

private:
  void UpdateGeometrySelection();
  bool UpdateLEWithSelectionValue(QLineEdit *pLineEdit, const QString &sValue);
  bool UpdateCBWithSelectionValue(QComboBox *pComboBox, const QString &sValue);
  bool UpdateSignWithSelectionValue(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, const QString &sValue, CTexture *pTex);
  void UpdateSignButtonDisplay(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, CTexture *pTex);
  void UpdateLEEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, const QString &sValue);
  void UpdateCBEditMode(bool &bEdited, bool &bMixedData, QComboBox *pComboBox, const QString &sValue);
  void UpdateSignEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, QWidget *pWidget, const QString &sValue);
  void RevertGeometry();
  void UpdateTextures(QLineEdit *pLineEdit, QLabel *pTex1, QLabel *pTex2);

  CEditDataWidgetPrivate *p;
};

//-------------------------------------------------------------------------------------------------
#endif