#include "TrackEditor.h"
#include "EditSurfaceDialog.h"
#include "Texture.h"
#include "TilePicker.h"
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

CEditSurfaceDialog::CEditSurfaceDialog(QWidget *pParent, CTexture *pTexture, int iValue)
  : QDialog(pParent)
  , m_pTexture(pTexture)
  , m_iValue(iValue)
{
  setupUi(this);

  UpdateDialog();
  ckPit->setChecked(iValue & SURFACE_FLAG_PIT);
  ckWall->setChecked(iValue & SURFACE_FLAG_WALL);
  ckTransparent->setChecked(iValue & SURFACE_FLAG_TRANSPARENT);
  ckNonMagnetic->setChecked(iValue & SURFACE_FLAG_NON_MAGNETIC);
  ckFlipVertically->setChecked(iValue & SURFACE_FLAG_FLIP_VERT);
  ckcNonSolid->setChecked(iValue & SURFACE_FLAG_NON_SOLID);
  ckTexturePair->setChecked(iValue & SURFACE_FLAG_TEXTURE_PAIR);
  ckFlipHorizontally->setChecked(iValue & SURFACE_FLAG_FLIP_HORIZ);
  ckApplyTexture->setChecked(iValue & SURFACE_FLAG_APPLY_TEXTURE);

  connect(ckPit,              &QCheckBox::toggled, this, &CEditSurfaceDialog::OnPitChecked);
  connect(ckWall,             &QCheckBox::toggled, this, &CEditSurfaceDialog::OnWallChecked);
  connect(ckTransparent,      &QCheckBox::toggled, this, &CEditSurfaceDialog::OnTransparentChecked);
  connect(ckNonMagnetic,      &QCheckBox::toggled, this, &CEditSurfaceDialog::OnNonMagneticChecked);
  connect(ckFlipVertically,   &QCheckBox::toggled, this, &CEditSurfaceDialog::OnFlipVertChecked);
  connect(ckcNonSolid,        &QCheckBox::toggled, this, &CEditSurfaceDialog::OnNonSolidChecked);
  connect(ckTexturePair,      &QCheckBox::toggled, this, &CEditSurfaceDialog::OnTexturePairChecked);
  connect(ckFlipHorizontally, &QCheckBox::toggled, this, &CEditSurfaceDialog::OnFlipHorizChecked);
  connect(ckApplyTexture,     &QCheckBox::toggled, this, &CEditSurfaceDialog::OnApplyTextureChecked);
  connect(pbTexture1,         &QPushButton::clicked, this, &CEditSurfaceDialog::OnTextureClicked);

  connect(pbCancel, &QPushButton::clicked, this, &CEditSurfaceDialog::reject);
  connect(pbApply, &QPushButton::clicked, this, &CEditSurfaceDialog::accept);
}

//-------------------------------------------------------------------------------------------------

CEditSurfaceDialog::~CEditSurfaceDialog()
{

}

//-------------------------------------------------------------------------------------------------

int CEditSurfaceDialog::GetValue()
{
  return m_iValue;
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::OnPitChecked(bool bChecked)
{
  if (bChecked)
    m_iValue |= SURFACE_FLAG_PIT;
  else
    m_iValue &= ~SURFACE_FLAG_PIT;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::OnWallChecked(bool bChecked)
{
  if (bChecked)
    m_iValue |= SURFACE_FLAG_WALL;
  else
    m_iValue &= ~SURFACE_FLAG_WALL;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::OnTransparentChecked(bool bChecked)
{
  if (bChecked)
    m_iValue |= SURFACE_FLAG_TRANSPARENT;
  else
    m_iValue &= ~SURFACE_FLAG_TRANSPARENT;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::OnNonMagneticChecked(bool bChecked)
{
  if (bChecked)
    m_iValue |= SURFACE_FLAG_NON_MAGNETIC;
  else
    m_iValue &= ~SURFACE_FLAG_NON_MAGNETIC;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::OnFlipVertChecked(bool bChecked)
{
  if (bChecked)
    m_iValue |= SURFACE_FLAG_FLIP_VERT;
  else
    m_iValue &= ~SURFACE_FLAG_FLIP_VERT;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::OnNonSolidChecked(bool bChecked)
{
  if (bChecked)
    m_iValue |= SURFACE_FLAG_NON_SOLID;
  else
    m_iValue &= ~SURFACE_FLAG_NON_SOLID;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::OnTexturePairChecked(bool bChecked)
{
  if (bChecked)
    m_iValue |= SURFACE_FLAG_TEXTURE_PAIR;
  else
    m_iValue &= ~SURFACE_FLAG_TEXTURE_PAIR;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::OnFlipHorizChecked(bool bChecked)
{
  if (bChecked)
    m_iValue |= SURFACE_FLAG_FLIP_HORIZ;
  else
    m_iValue &= ~SURFACE_FLAG_FLIP_HORIZ;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::OnApplyTextureChecked(bool bChecked)
{
  if (bChecked)
    m_iValue |= SURFACE_FLAG_APPLY_TEXTURE;
  else
    m_iValue &= ~SURFACE_FLAG_APPLY_TEXTURE;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::OnTextureClicked()
{
  int iIndex = m_iValue & SURFACE_TEXTURE_INDEX;
  iIndex = iIndex >> 1;
  CTilePicker dlg(this, m_pTexture, iIndex, false);
  if (dlg.exec()) {
    int iIndex = dlg.GetSelected();
    iIndex = iIndex << 1;
    m_iValue &= ~SURFACE_TEXTURE_INDEX;
    m_iValue |= iIndex;
  }

  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::UpdateDialog()
{
  //qstring::number makes negative values 64-bit for some reason
  char szBuf[128];
  snprintf(szBuf, sizeof(szBuf), " (%#010x)", m_iValue);

  leValue->setFont(QFont("Courier", 8));
  leValue->setText(QString::number(m_iValue).leftJustified(10, ' ') + szBuf);

  //textures
  int iIndex = m_iValue & SURFACE_TEXTURE_INDEX;
  iIndex = iIndex >> 1;
  if (iIndex < m_pTexture->m_tileAy.size()) {
    QPixmap pixmap;
    pixmap.convertFromImage(m_pTexture->m_tileAy[iIndex]);
    pbTexture1->setIcon(pixmap);

    if (m_iValue & SURFACE_FLAG_TEXTURE_PAIR && iIndex > 0) {
      QPixmap pixmap2;
      pixmap2.convertFromImage(m_pTexture->m_tileAy[iIndex - 1]);
      lblTexture2->setPixmap(pixmap2);
    } else {
      lblTexture2->setPixmap(QPixmap());
    }
  }
}

//-------------------------------------------------------------------------------------------------