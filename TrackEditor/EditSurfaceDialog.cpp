#include "TrackEditor.h"
#include "EditSurfaceDialog.h"
#include "Texture.h"
#include "Palette.h"
#include "TilePicker.h"
#include "Track.h"
#include "QtHelpers.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CEditSurfaceDialog::CEditSurfaceDialog(QWidget *pParent, CTexture *pTexture, CPalette *pPalette, int iValue, 
                                       bool bShowDisable, const QString &sDisableEffects, bool bShowDisableAttach)
  : QDialog(pParent)
  , m_pTexture(pTexture)
  , m_pPalette(pPalette)
  , m_bShowDisable(bShowDisable)
  , m_bShowDisableAttach(bShowDisableAttach)
{
  m_uiSignedBitValue = CTrack::GetSignedBitValueFromInt(iValue);
  setupUi(this);

  ckDisable->setVisible(bShowDisable);
  lblDisableEffects->setVisible(bShowDisable);
  ckDisableAttach->setVisible(bShowDisableAttach);
  lblDisableEffects->setText(sDisableEffects);

  UpdateDialog();

  connect(ckDisable,            &QCheckBox::toggled, this, &CEditSurfaceDialog::OnDisableChecked);
  connect(ckDisableAttach,      &QCheckBox::toggled, this, &CEditSurfaceDialog::OnDisableAttachChecked);
  connect(ck31Wall,             &QCheckBox::toggled, this, &CEditSurfaceDialog::On31WallChecked);
  connect(ck30Bounce,           &QCheckBox::toggled, this, &CEditSurfaceDialog::On30BounceChecked);
  connect(ck29Echo,             &QCheckBox::toggled, this, &CEditSurfaceDialog::On29EchoChecked);
  connect(ck28,                 &QCheckBox::toggled, this, &CEditSurfaceDialog::On28Checked);
  connect(ck27PairNext,         &QCheckBox::toggled, this, &CEditSurfaceDialog::On27PairNextChecked);
  connect(ck26,                 &QCheckBox::toggled, this, &CEditSurfaceDialog::On26Checked);
  connect(ck25Pit,              &QCheckBox::toggled, this, &CEditSurfaceDialog::On25PitChecked);
  connect(ck24Yellow,           &QCheckBox::toggled, this, &CEditSurfaceDialog::On24YellowMapChecked);
  connect(ck23,                 &QCheckBox::toggled, this, &CEditSurfaceDialog::On23Checked);
  connect(ck22Wall,             &QCheckBox::toggled, this, &CEditSurfaceDialog::On22WallChecked);
  connect(ck21Transparent,      &QCheckBox::toggled, this, &CEditSurfaceDialog::On21TransparentChecked);
  connect(ck20Bounce,           &QCheckBox::toggled, this, &CEditSurfaceDialog::On20BounceChecked);
  connect(ck19NonMagnetic,      &QCheckBox::toggled, this, &CEditSurfaceDialog::On19NonMagneticChecked);
  connect(ck18FlipVertically,   &QCheckBox::toggled, this, &CEditSurfaceDialog::On18FlipVertChecked);
  connect(ck17NonSolid,         &QCheckBox::toggled, this, &CEditSurfaceDialog::On17NonSolidChecked);
  connect(ck16TexturePair,      &QCheckBox::toggled, this, &CEditSurfaceDialog::On16TexturePairChecked);
  connect(ck15Livery,           &QCheckBox::toggled, this, &CEditSurfaceDialog::On15LiveryChecked);
  connect(ck14,                 &QCheckBox::toggled, this, &CEditSurfaceDialog::On14Checked);
  connect(ck13Motion,           &QCheckBox::toggled, this, &CEditSurfaceDialog::On13MotionChecked);
  connect(ck12FlipHorizontally, &QCheckBox::toggled, this, &CEditSurfaceDialog::On12FlipHorizChecked);
  connect(ck11Back,             &QCheckBox::toggled, this, &CEditSurfaceDialog::On11BackChecked);
  connect(ck10PartialTrans,     &QCheckBox::toggled, this, &CEditSurfaceDialog::On10PartialTransChecked);
  connect(ck9AnmsLookup,        &QCheckBox::toggled, this, &CEditSurfaceDialog::On9AnmsLookupChecked);
  connect(ck8ApplyTexture,      &QCheckBox::toggled, this, &CEditSurfaceDialog::On8ApplyTextureChecked);
  connect(pbTexture1, &QPushButton::clicked, this, &CEditSurfaceDialog::OnTextureClicked);

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
  return CTrack::GetIntValueFromSignedBit(m_uiSignedBitValue);
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::OnDisableChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue = CTrack::GetSignedBitValueFromInt(-1);
  else
    m_uiSignedBitValue = 0;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::OnDisableAttachChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue = CTrack::GetSignedBitValueFromInt(-2);
  else
    m_uiSignedBitValue = 0;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On31WallChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_WALL_31;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_WALL_31;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On30BounceChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_BOUNCE_30;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_BOUNCE_30;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On29EchoChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_ECHO;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_ECHO;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On28Checked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_28;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_28;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On27PairNextChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_PAIR_NEXT_TEX;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_PAIR_NEXT_TEX;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On26Checked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_26;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_26;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On25PitChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_PIT;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_PIT;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On24YellowMapChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_YELLOW_MAP;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_YELLOW_MAP;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On23Checked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_23;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_23;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On22WallChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_WALL_22;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_WALL_22;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On21TransparentChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_TRANSPARENT;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_TRANSPARENT;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On20BounceChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_BOUNCE_20;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_BOUNCE_20;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On19NonMagneticChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_NON_MAGNETIC;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_NON_MAGNETIC;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On18FlipVertChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_FLIP_VERT;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_FLIP_VERT;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On17NonSolidChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_NON_SOLID;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_NON_SOLID;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On16TexturePairChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_TEXTURE_PAIR;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_TEXTURE_PAIR;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On15LiveryChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_ANMS_LIVERY;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_ANMS_LIVERY;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On14Checked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_14;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_14;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On13MotionChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_ANMS_MOTION;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_ANMS_MOTION;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On12FlipHorizChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_FLIP_HORIZ;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_FLIP_HORIZ;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On11BackChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_BACK;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_BACK;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On10PartialTransChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_PARTIAL_TRANS;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_PARTIAL_TRANS;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On9AnmsLookupChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_ANMS_LOOKUP;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_ANMS_LOOKUP;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::On8ApplyTextureChecked(bool bChecked)
{
  if (bChecked)
    m_uiSignedBitValue |= SURFACE_FLAG_APPLY_TEXTURE;
  else
    m_uiSignedBitValue &= ~SURFACE_FLAG_APPLY_TEXTURE;
  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::OnTextureClicked()
{
  int iIndex = m_uiSignedBitValue & SURFACE_TEXTURE_INDEX;
  CTilePicker dlg(this, m_pTexture, iIndex, false);
  if (dlg.exec()) {
    iIndex = dlg.GetSelected();
    m_uiSignedBitValue &= ~SURFACE_TEXTURE_INDEX;
    m_uiSignedBitValue |= iIndex;
  }

  UpdateDialog();
}

//-------------------------------------------------------------------------------------------------

void CEditSurfaceDialog::UpdateDialog()
{
  int iValue = CTrack::GetIntValueFromSignedBit(m_uiSignedBitValue);

  BLOCK_SIG_AND_DO(ckDisable            , setChecked(iValue == -1));
  BLOCK_SIG_AND_DO(ckDisableAttach      , setChecked(iValue == -2));
  BLOCK_SIG_AND_DO(ck31Wall             , setChecked(m_uiSignedBitValue & SURFACE_FLAG_WALL_31));
  BLOCK_SIG_AND_DO(ck30Bounce           , setChecked(m_uiSignedBitValue & SURFACE_FLAG_BOUNCE_30));
  BLOCK_SIG_AND_DO(ck29Echo             , setChecked(m_uiSignedBitValue & SURFACE_FLAG_ECHO));
  BLOCK_SIG_AND_DO(ck28                 , setChecked(m_uiSignedBitValue & SURFACE_FLAG_28));
  BLOCK_SIG_AND_DO(ck27PairNext         , setChecked(m_uiSignedBitValue & SURFACE_FLAG_PAIR_NEXT_TEX));
  BLOCK_SIG_AND_DO(ck26                 , setChecked(m_uiSignedBitValue & SURFACE_FLAG_26));
  BLOCK_SIG_AND_DO(ck25Pit              , setChecked(m_uiSignedBitValue & SURFACE_FLAG_PIT));
  BLOCK_SIG_AND_DO(ck24Yellow           , setChecked(m_uiSignedBitValue & SURFACE_FLAG_YELLOW_MAP));
  BLOCK_SIG_AND_DO(ck23                 , setChecked(m_uiSignedBitValue & SURFACE_FLAG_23));
  BLOCK_SIG_AND_DO(ck22Wall             , setChecked(m_uiSignedBitValue & SURFACE_FLAG_WALL_22));
  BLOCK_SIG_AND_DO(ck21Transparent      , setChecked(m_uiSignedBitValue & SURFACE_FLAG_TRANSPARENT));
  BLOCK_SIG_AND_DO(ck20Bounce           , setChecked(m_uiSignedBitValue & SURFACE_FLAG_BOUNCE_20));
  BLOCK_SIG_AND_DO(ck19NonMagnetic      , setChecked(m_uiSignedBitValue & SURFACE_FLAG_NON_MAGNETIC));
  BLOCK_SIG_AND_DO(ck18FlipVertically   , setChecked(m_uiSignedBitValue & SURFACE_FLAG_FLIP_VERT));
  BLOCK_SIG_AND_DO(ck17NonSolid         , setChecked(m_uiSignedBitValue & SURFACE_FLAG_NON_SOLID));
  BLOCK_SIG_AND_DO(ck16TexturePair      , setChecked(m_uiSignedBitValue & SURFACE_FLAG_TEXTURE_PAIR));
  BLOCK_SIG_AND_DO(ck15Livery           , setChecked(m_uiSignedBitValue & SURFACE_FLAG_ANMS_LIVERY));
  BLOCK_SIG_AND_DO(ck14                 , setChecked(m_uiSignedBitValue & SURFACE_FLAG_14));
  BLOCK_SIG_AND_DO(ck13Motion           , setChecked(m_uiSignedBitValue & SURFACE_FLAG_ANMS_MOTION));
  BLOCK_SIG_AND_DO(ck12FlipHorizontally , setChecked(m_uiSignedBitValue & SURFACE_FLAG_FLIP_HORIZ));
  BLOCK_SIG_AND_DO(ck11Back             , setChecked(m_uiSignedBitValue & SURFACE_FLAG_BACK));
  BLOCK_SIG_AND_DO(ck10PartialTrans     , setChecked(m_uiSignedBitValue & SURFACE_FLAG_PARTIAL_TRANS));
  BLOCK_SIG_AND_DO(ck9AnmsLookup        , setChecked(m_uiSignedBitValue & SURFACE_FLAG_ANMS_LOOKUP));
  BLOCK_SIG_AND_DO(ck8ApplyTexture      , setChecked(m_uiSignedBitValue & SURFACE_FLAG_APPLY_TEXTURE));

  bool bEnableSurface = true;
  if (m_bShowDisable)
    bEnableSurface &= (iValue != -1);
  if (m_bShowDisableAttach)
    bEnableSurface &= (iValue != -2);

  ck31Wall             ->setEnabled(bEnableSurface);
  ck30Bounce           ->setEnabled(bEnableSurface);
  ck29Echo             ->setEnabled(bEnableSurface);
  ck28                 ->setEnabled(bEnableSurface);
  ck27PairNext         ->setEnabled(bEnableSurface);
  ck26                 ->setEnabled(bEnableSurface);
  ck25Pit              ->setEnabled(bEnableSurface);
  ck24Yellow           ->setEnabled(bEnableSurface);
  ck23                 ->setEnabled(bEnableSurface);
  ck22Wall             ->setEnabled(bEnableSurface);
  ck21Transparent      ->setEnabled(bEnableSurface);
  ck20Bounce           ->setEnabled(bEnableSurface);
  ck19NonMagnetic      ->setEnabled(bEnableSurface);
  ck18FlipVertically   ->setEnabled(bEnableSurface);
  ck17NonSolid         ->setEnabled(bEnableSurface);
  ck16TexturePair      ->setEnabled(bEnableSurface);
  ck15Livery           ->setEnabled(bEnableSurface);
  ck14                 ->setEnabled(bEnableSurface);
  ck13Motion           ->setEnabled(bEnableSurface);
  ck12FlipHorizontally ->setEnabled(bEnableSurface);
  ck11Back             ->setEnabled(bEnableSurface);
  ck10PartialTrans     ->setEnabled(bEnableSurface);
  ck9AnmsLookup        ->setEnabled(bEnableSurface);
  ck8ApplyTexture      ->setEnabled(bEnableSurface);
  pbTexture1           ->setEnabled(bEnableSurface);

  //qstring::number makes negative values 64-bit for some reason
  char szBuf[128];
  snprintf(szBuf, sizeof(szBuf), " (%#010x)", m_uiSignedBitValue);

  leValue->setFont(QFont("Courier", 8));
  leValue->setText(QString::number(iValue).leftJustified(10, ' ') + szBuf);

  //textures
  if (m_uiSignedBitValue & SURFACE_FLAG_APPLY_TEXTURE) {
    int iIndex = m_uiSignedBitValue & SURFACE_TEXTURE_INDEX;
    if (iIndex < m_pTexture->m_iNumTiles) {
      QPixmap pixmap;
      pixmap.convertFromImage(QtHelpers::GetQImageFromTile(m_pTexture->m_pTileAy[iIndex]));
      pbTexture1->setIcon(pixmap);

      if (m_uiSignedBitValue & SURFACE_FLAG_TEXTURE_PAIR && iIndex > 0) {
        if (m_uiSignedBitValue & SURFACE_FLAG_PAIR_NEXT_TEX) {
          QPixmap pixmap2;
          pixmap2.convertFromImage(QtHelpers::GetQImageFromTile(m_pTexture->m_pTileAy[iIndex + 1]));
          lblTexture2->setPixmap(pixmap2);
        } else {
          lblTexture2->setPixmap(pixmap);
        }
      } else {
        lblTexture2->setPixmap(QPixmap());
      }
    }
  } else {
    int iIndex = m_uiSignedBitValue & SURFACE_TEXTURE_INDEX;
    if (iIndex < (int)m_pPalette->m_paletteAy.size()) {
      QPixmap pixmap;
      pixmap.convertFromImage(QtHelpers::GetQImageFromColor(m_pPalette->m_paletteAy[iIndex]));
      pbTexture1->setIcon(pixmap);
    } else {
      pbTexture1->setIcon(QPixmap());
    }

    lblTexture2->setPixmap(QPixmap());
  }
}

//-------------------------------------------------------------------------------------------------