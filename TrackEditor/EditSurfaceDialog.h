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
  void On11BackChecked(bool bChecked);
  void On10PartialTransChecked(bool bChecked);
  void On9Checked(bool bChecked);
  void On8ApplyTextureChecked(bool bChecked);
  void OnTextureClicked();

private:
  void UpdateDialog();

  CTexture *m_pTexture;
  unsigned int m_uiSignedBitValue;
};

//-------------------------------------------------------------------------------------------------
#endif