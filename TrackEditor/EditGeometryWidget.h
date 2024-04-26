#ifndef _TRACKEDITOR_EDITGEOMETRYWIDGET_H
#define _TRACKEDITOR_EDITGEOMETRYWIDGET_H
//-------------------------------------------------------------------------------------------------
#include "ui_EditGeometryWidget.h"
//-------------------------------------------------------------------------------------------------
class CTrack;
class CEditGeometryWidgetPrivate;
//-------------------------------------------------------------------------------------------------

class CEditGeometryWidget : public QWidget, private Ui::EditGeometryWidget
{
  Q_OBJECT

public:
  CEditGeometryWidget(QWidget *pParent, CTrack *pTrack);
  ~CEditGeometryWidget();

protected slots:
  void UpdateGeometrySelection(int iFrom, int iTo);
  void ValuesChanged();

private:
  CEditGeometryWidgetPrivate *p;
};

//-------------------------------------------------------------------------------------------------
#endif