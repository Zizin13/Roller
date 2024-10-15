#ifndef _TRACKEDITOR_EDITSURFACEDLG_H
#define _TRACKEDITOR_EDITSURFACEDLG_H
//-------------------------------------------------------------------------------------------------
#include "ui_EditSurfaceDialog.h"
#include "Types.h"
//-------------------------------------------------------------------------------------------------
enum class eSurfaceField
{
  SURFACE_CENTER = 0,
  SURFACE_LSHOULDER,
  SURFACE_RSHOULDER,
  SURFACE_LWALL,
  SURFACE_RWALL,
  SURFACE_ROOF,
  SURFACE_LUOWALL,
  SURFACE_LLOWALL,
  SURFACE_OFLOOR,
  SURFACE_RLOWALL,
  SURFACE_RUOWALL,
  SURFACE_ENVFLOOR,
  SURFACE_SIGN,
  SURFACE_BACK
};
//-------------------------------------------------------------------------------------------------
class CTrack;
class CTexture;
//-------------------------------------------------------------------------------------------------
class CEditSurfaceDialog : public QDialog, private Ui::EditSurfaceDialog
{
  Q_OBJECT

public:
  CEditSurfaceDialog(QWidget *pParent, eSurfaceField field,
                     bool bShowDisable = false, 
                     const QString &sDisableEffects = "", 
                     bool bShowDisableAttach = false,
                     bool bApplyCancel = false,
                     int iValue = -1);
  ~CEditSurfaceDialog();

  int GetDialogValue();

protected slots:
  void OnDisableChecked(bool bChecked);
  void OnDisableAttachChecked(bool bChecked);
  void On31WallChecked(bool bChecked);
  void On30BounceChecked(bool bChecked);
  void On29EchoChecked(bool bChecked);
  void On28Checked(bool bChecked);
  void On27PairNextChecked(bool bChecked);
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
  void On15LiveryChecked(bool bChecked);
  void On14Checked(bool bChecked);
  void On13MotionChecked(bool bChecked);
  void On12FlipHorizChecked(bool bChecked);
  void On11BackChecked(bool bChecked);
  void On10PartialTransChecked(bool bChecked);
  void On9AnmsLookupChecked(bool bChecked);
  void On8ApplyTextureChecked(bool bChecked);
  void OnTextureClicked();
  void OnTransparencyTypeChanged(int iIndex);

private:
  void UpdateValueHelper(uint32 uiFlag, bool bChecked);
  int &GetValue(int i);
  CTexture *GetTexture();
  void UpdateDialog();

  eSurfaceField m_field;
  unsigned int m_uiSignedBitValue;
  bool m_bShowDisable;
  bool m_bShowDisableAttach;
  bool m_bApplyCancel;
};

//-------------------------------------------------------------------------------------------------
#endif