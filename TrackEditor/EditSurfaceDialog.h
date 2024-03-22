#ifndef _TRACKEDITOR_EDITSURFACEDLG_H
#define _TRACKEDITOR_EDITSURFACEDLG_H
//-------------------------------------------------------------------------------------------------
#include "ui_EditSurfaceDialog.h"
//-------------------------------------------------------------------------------------------------
#define SURFACE_FLAG_PIT           0x01000000
#define SURFACE_FLAG_WALL          0x00200000
#define SURFACE_FLAG_TRANSPARENT   0x00100000
#define SURFACE_FLAG_NON_MAGNETIC  0x00080000
#define SURFACE_FLAG_FLIP_VERT     0x00020000
#define SURFACE_FLAG_NON_SOLID     0x00010000
#define SURFACE_FLAG_TEXTURE_PAIR  0x00008000
#define SURFACE_FLAG_FLIP_HORIZ    0x00004000
#define SURFACE_FLAG_APPLY_TEXTURE 0x00000100
#define SURFACE_TEXTURE_INDEX      0x000000FE
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