#ifndef _TRACKEDITOR_EDITDATAWIDGET_H
#define _TRACKEDITOR_EDITDATAWIDGET_H
//-------------------------------------------------------------------------------------------------
#include "ui_EditDataWidget.h"
//-------------------------------------------------------------------------------------------------
class CTrack;
//-------------------------------------------------------------------------------------------------
class CEditDataWidget : public QWidget, private Ui::EditDataWidget
{
  Q_OBJECT

public:
  CEditDataWidget(QWidget *pParent);
  ~CEditDataWidget();

protected slots:
  void UpdateGeometrySelection(int iFrom, int iTo);

private:
  void UpdateTextures(QLineEdit *pLineEdit, QLabel *pTex1, QLabel *pTex2);
};

//-------------------------------------------------------------------------------------------------
#endif