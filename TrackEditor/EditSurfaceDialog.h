#ifndef _TRACKEDITOR_EDITSURFACEDLG_H
#define _TRACKEDITOR_EDITSURFACEDLG_H
//-------------------------------------------------------------------------------------------------
#include "ui_EditSurfaceDialog.h"
//-------------------------------------------------------------------------------------------------
#define SURFACE_FLAG_WALL_31       0x80000000
#define SURFACE_FLAG_BOUNCE_30     0x40000000
#define SURFACE_FLAG_ECHO          0x20000000
#define SURFACE_FLAG_28            0x10000000
#define SURFACE_FLAG_27            0x08000000
#define SURFACE_FLAG_26            0x04000000
#define SURFACE_FLAG_PIT           0x02000000
#define SURFACE_FLAG_YELLOW_MAP    0x01000000
#define SURFACE_FLAG_23            0x00800000
#define SURFACE_FLAG_WALL_22       0x00400000
#define SURFACE_FLAG_TRANSPARENT   0x00200000
#define SURFACE_FLAG_BOUNCE_20     0x00100000
#define SURFACE_FLAG_NON_MAGNETIC  0x00080000
#define SURFACE_FLAG_FLIP_VERT     0x00040000
#define SURFACE_FLAG_NON_SOLID     0x00020000
#define SURFACE_FLAG_TEXTURE_PAIR  0x00010000
#define SURFACE_FLAG_15            0x00008000
#define SURFACE_FLAG_14            0x00004000
#define SURFACE_FLAG_13            0x00002000
#define SURFACE_FLAG_FLIP_HORIZ    0x00001000
#define SURFACE_FLAG_11            0x00000800
#define SURFACE_FLAG_10            0x00000400
#define SURFACE_FLAG_9             0x00000200
#define SURFACE_FLAG_APPLY_TEXTURE 0x00000100
#define SURFACE_TEXTURE_INDEX      0x000000FF
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
  void On31WallChecked(bool bChecked);
  void On30BounceChecked(bool bChecked);
  void On29EchoChecked(bool bChecked);
  void On28Checked(bool bChecked);
  void On27Checked(bool bChecked);
  void On26Checked(bool bChecked);
  void On25PitChecked(bool bChecked);
  void On24YellowMapChecked(bool bChecked);
  void On23Checked(bool bChecked);
  void On22WallChecked(bool bChecked);
  void On21TransparentChecked(bool bChecked);
  void On20BounceChecked(bool bChecked);
  void On19NonMagneticChecked(bool bChecked);
  void On18FlipVertChecked(bool bChecked);
  void On17NonSolidChecked(bool bChecked);
  void On16TexturePairChecked(bool bChecked);
  void On15Checked(bool bChecked);
  void On14Checked(bool bChecked);
  void On13Checked(bool bChecked);
  void On12FlipHorizChecked(bool bChecked);
  void On11Checked(bool bChecked);
  void On10Checked(bool bChecked);
  void On9Checked(bool bChecked);
  void On8ApplyTextureChecked(bool bChecked);
  void OnTextureClicked();

private:
  void UpdateDialog();

  CTexture *m_pTexture;
  int m_iValue;
};

//-------------------------------------------------------------------------------------------------
#endif