#ifndef _TRACKEDITOR_TILEPICKER_H
#define _TRACKEDITOR_TILEPICKER_H
//-------------------------------------------------------------------------------------------------
#include "ui_TilePicker.h"
//-------------------------------------------------------------------------------------------------
class CTexture;
class CPalette;
//-------------------------------------------------------------------------------------------------
class CTilePicker : public QDialog, private Ui::TilePicker
{
  Q_OBJECT

public:
  CTilePicker(QWidget *pParent, int iIndex, CTexture *pTexture);
  CTilePicker(QWidget *pParent, int iIndex, CPalette *pTexture);
  ~CTilePicker();

  int GetSelected();

protected slots:
  void OnButtonClicked();

private:
  int m_iIndex;
};

//-------------------------------------------------------------------------------------------------
#endif