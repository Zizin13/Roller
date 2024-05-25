#ifndef _TRACKEDITOR_BACKWIDGET_H
#define _TRACKEDITOR_BACKWIDGET_H
//-------------------------------------------------------------------------------------------------
#include "ui_BackWidget.h"
//-------------------------------------------------------------------------------------------------
class CTexture;
//-------------------------------------------------------------------------------------------------
class CBackWidget : public QWidget, private Ui::BackWidget
{
  Q_OBJECT

public:
  CBackWidget(QWidget *pParent, int iIndex, CTexture *pTexture, int iBack);
  ~CBackWidget();

  int GetBack();

private:
  void UpdateWidget();

  int m_iBack;
  CTexture *m_pTexture;
};

//-------------------------------------------------------------------------------------------------
#endif