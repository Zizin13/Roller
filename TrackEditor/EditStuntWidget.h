#ifndef _TRACKEDITOR_EDITSTUNTWIDGET_H
#define _TRACKEDITOR_EDITSTUNTWIDGET_H
//-------------------------------------------------------------------------------------------------
#include "ui_EditStuntWidget.h"
//-------------------------------------------------------------------------------------------------

class CEditStuntWidget : public QWidget, private Ui::EditStuntWidget
{
  Q_OBJECT

public:
  CEditStuntWidget(QWidget *pParent);
  ~CEditStuntWidget();

protected slots:
  void UpdateGeometrySelection(int iFrom, int iTo);
  void ChunkCountChanged(int iVal);
  void NumTicksChanged(int iVal);
  void TickStartIdxChanged(int iVal);
  void TimingGroupChanged(int iVal);
  void HeightChanged(int iVal);
  void TimeBulgingChanged(int iVal);
  void TimeFlatChanged(int iVal);
  void LengthChanged(int iVal);
  void FlagsChanged();
  void StuntClicked();

private:
  void UpdateDialog();
};

//-------------------------------------------------------------------------------------------------
#endif