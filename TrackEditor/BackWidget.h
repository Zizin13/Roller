#ifndef _TRACKEDITOR_BACKWIDGET_H
#define _TRACKEDITOR_BACKWIDGET_H
//-------------------------------------------------------------------------------------------------
#include "ui_BackWidget.h"
//-------------------------------------------------------------------------------------------------
class CTexture;
class CPalette;
//-------------------------------------------------------------------------------------------------
class CBackWidget : public QWidget, private Ui::BackWidget
{
  Q_OBJECT

public:
  CBackWidget(QWidget *pParent, int iIndex, CTexture *pTexture, CPalette *pPal, int iBack);
  ~CBackWidget();

  int GetIndex();
  int GetBack();

protected slots:
  void OnBackClicked();

private:
  void UpdateWidget();

  int m_iIndex;
  int m_iBack;
  CTexture *m_pTexture;
  CPalette *m_pPalette;
};

//-------------------------------------------------------------------------------------------------
#endif