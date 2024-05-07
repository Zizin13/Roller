#ifndef _TRACKEDITOR_EDITSIGNWIDGET_H
#define _TRACKEDITOR_EDITSIGNWIDGET_H
//-------------------------------------------------------------------------------------------------
#include "ui_EditSignWidget.h"
//-------------------------------------------------------------------------------------------------
class CTrack;
class CEditSignWidgetPrivate;
//-------------------------------------------------------------------------------------------------

class CEditSignWidget : public QWidget, private Ui::EditSignWidget
{
  Q_OBJECT

public:
  CEditSignWidget(QWidget *pParent, CTrack *pTrack);
  ~CEditSignWidget();

protected slots:
  void UpdateGeometrySelection(int iFrom, int iTo);
  void YawChanged(double dVal);
  void PitchChanged(double dVal);
  void RollChanged(double dVal);
  void HOffsetChanged(int iVal);
  void VOffsetChanged(int iVal);
  void TypeChanged(int iIndex);
  void EditClicked();
  void SignClicked();
  void UnkChanged(const QString &sText);

private:
  CEditSignWidgetPrivate *p;
};

//-------------------------------------------------------------------------------------------------
#endif