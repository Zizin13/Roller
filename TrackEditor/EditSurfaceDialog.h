#ifndef _TRACKEDITOR_EDITSURFACEDLG_H
#define _TRACKEDITOR_EDITSURFACEDLG_H
//-------------------------------------------------------------------------------------------------
#include "ui_EditSurfaceDialog.h"
//-------------------------------------------------------------------------------------------------
class CTexture;
//-------------------------------------------------------------------------------------------------
class CEditSurfaceDialog : public QDialog, private Ui::EditSurfaceDialog
{
  Q_OBJECT

public:
  CEditSurfaceDialog(QWidget *pParent, CTexture *pTexture, int iValue);
  ~CEditSurfaceDialog();

  int GetValue();

protected slots:
  void OnPitChecked(bool bChecked);
  void OnWallChecked(bool bChecked);
  void OnTransparentChecked(bool bChecked);
  void OnNonMagneticChecked(bool bChecked);
  void OnFlipVertChecked(bool bChecked);
  void OnNonSolidChecked(bool bChecked);
  void OnTexturePairChecked(bool bChecked);
  void OnFlipHorizChecked(bool bChecked);
  void OnApplyTextureChecked(bool bChecked);
  void OnTextureClicked();

private:
  void UpdateDialog();

  CTexture *m_pTexture;
  int m_iValue;
};

//-------------------------------------------------------------------------------------------------
#endif