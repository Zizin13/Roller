#ifndef _TRACKEDITOR_TILEPICKER_H
#define _TRACKEDITOR_TILEPICKER_H
//-------------------------------------------------------------------------------------------------
#include "ui_TilePicker.h"
//-------------------------------------------------------------------------------------------------
class CTexture;
//-------------------------------------------------------------------------------------------------
class CTilePicker : public QDialog, private Ui::TilePicker
{
  Q_OBJECT

public:
  CTilePicker(QWidget *pParent, CTexture *pTexture, int iIndex);
  ~CTilePicker();

  int GetSelected();

protected slots:
  void OnButtonClicked();

private:
  CTexture *m_pTexture;
  int m_iIndex;
};

//-------------------------------------------------------------------------------------------------
#endif