#ifndef _TRACKEDITOR_EDITSTUNTWIDGET_H
#define _TRACKEDITOR_EDITSTUNTWIDGET_H
//-------------------------------------------------------------------------------------------------
#include "ui_EditStuntWidget.h"
//-------------------------------------------------------------------------------------------------
class CTrack;
class CEditStuntWidgetPrivate;
//-------------------------------------------------------------------------------------------------

class CEditStuntWidget : public QWidget, private Ui::EditStuntWidget
{
  Q_OBJECT

public:
  CEditStuntWidget(QWidget *pParent, CTrack *pTrack);
  ~CEditStuntWidget();

protected slots:
  void UpdateGeometrySelection(int iFrom, int iTo);
  void ScaleFactChanged(int iVal);
  void AngleChanged(int iVal);
  void UnknownChanged(int iVal);
  void TimingGroupChanged(int iVal);
  void HeightChanged(int iVal);
  void TimeBulgingChanged(int iVal);
  void TimeFlatChanged(int iVal);
  void ExpandContractChanged(int iVal);
  void BulgeChanged(int iVal);
  void StuntClicked();

private:
  CEditStuntWidgetPrivate *p;
};

//-------------------------------------------------------------------------------------------------
#endif