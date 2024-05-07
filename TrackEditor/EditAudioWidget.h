#ifndef _TRACKEDITOR_EDITAUDIOWIDGET_H
#define _TRACKEDITOR_EDITAUDIOWIDGET_H
//-------------------------------------------------------------------------------------------------
#include "ui_EditAudioWidget.h"
//-------------------------------------------------------------------------------------------------
class CEditAudioWidgetPrivate;
//-------------------------------------------------------------------------------------------------

class CEditAudioWidget : public QWidget, private Ui::EditAudioWidget
{
  Q_OBJECT

public:
  CEditAudioWidget(QWidget *pParent);
  ~CEditAudioWidget();

protected slots:
  void UpdateGeometrySelection(int iFrom, int iTo);
  void SpeedChanged(int iVal);
  void BelowChanged(int iIndex);
  void AboveChanged(int iIndex);
  void AudioClicked();

private:
  CEditAudioWidgetPrivate *p;
};

//-------------------------------------------------------------------------------------------------
#endif