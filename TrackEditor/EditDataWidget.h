#ifndef _TRACKEDITOR_EDITDATAWIDGET_H
#define _TRACKEDITOR_EDITDATAWIDGET_H
//-------------------------------------------------------------------------------------------------
#include "ui_EditDataWidget.h"
//-------------------------------------------------------------------------------------------------
class CEditDataWidgetPrivate;
class CTrack;
//-------------------------------------------------------------------------------------------------
class CEditDataWidget : public QWidget, private Ui::EditDataWidget
{
  Q_OBJECT

public:
  CEditDataWidget(QWidget *pParent);
  ~CEditDataWidget();

public slots:
  void UpdateGeometryEditMode();
  void OnCancelClicked();

protected slots:
  void OnUpdateWindow();
  void UpdateGeometrySelection(int iFrom, int iTo);
  void OnInsertBeforeClicked();
  void OnInsertAfterClicked();
  void OnApplyClicked();
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
  void RevertGeometry();
  void UpdateTextures(QLineEdit *pLineEdit, QLabel *pTex1, QLabel *pTex2);

  CEditDataWidgetPrivate *p;
};

//-------------------------------------------------------------------------------------------------
#endif