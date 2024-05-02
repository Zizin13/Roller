#include "TrackEditor.h"
#include "TilePicker.h"
#include "Texture.h"
#include "Palette.h"
#include "qpushbutton.h"
#include "qdesktopwidget.h"
#include "QtHelpers.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CTilePicker::CTilePicker(QWidget *pParent, int iIndex, CTexture *pTexture)
  : QDialog(pParent)
  , m_iIndex(iIndex)
{
  setupUi(this);
  resize(QDesktopWidget().availableGeometry(this).size() * 0.3);
  connect(pbCancel, &QPushButton::clicked, this, &CTilePicker::reject);

  if (pTexture) {
    lblNotLoaded->setVisible(pTexture->m_iNumTiles == 0);
    int iButtonSize = 90;
    int iTilesPerLine = (width() - 256) / (iButtonSize + 6);
    int i = 0;
    for (; i < pTexture->m_iNumTiles; ++i) {
      QPushButton *pButton = new QPushButton(this);
      pButton->setMaximumHeight(iButtonSize);
      pButton->setMaximumWidth(iButtonSize);
      pButton->setMinimumHeight(iButtonSize);
      pButton->setMinimumWidth(iButtonSize);
      pButton->setIconSize(QSize(64, 64));
      pButton->setCheckable(true);
      pButton->setChecked(i == m_iIndex);
      pButton->setProperty("index", i);
      QPixmap pixmap;
      pixmap.convertFromImage(QtHelpers::GetQImageFromTile(pTexture->m_pTileAy[i]));
      pButton->setIcon(pixmap);
      connect(pButton, &QPushButton::clicked, this, &CTilePicker::OnButtonClicked);
      layoutTextures->addWidget(pButton, i / iTilesPerLine, i % iTilesPerLine);
    }
  }
}

//-------------------------------------------------------------------------------------------------

CTilePicker::CTilePicker(QWidget *pParent, int iIndex, CPalette *pPalette)
  : QDialog(pParent)
  , m_iIndex(iIndex)
{
  setupUi(this);
  resize(QDesktopWidget().availableGeometry(this).size() * 0.3);
  connect(pbCancel, &QPushButton::clicked, this, &CTilePicker::reject);

  if (pPalette) {
    lblNotLoaded->setVisible(pPalette->m_paletteAy.empty());
    int iButtonSize = 90;
    int iTilesPerLine = (width() - 256) / (iButtonSize + 6);
    int i = 0;
    for (; i < (int)pPalette->m_paletteAy.size(); ++i) {
      QPushButton *pButton = new QPushButton(this);
      pButton->setMaximumHeight(iButtonSize);
      pButton->setMaximumWidth(iButtonSize);
      pButton->setMinimumHeight(iButtonSize);
      pButton->setMinimumWidth(iButtonSize);
      pButton->setIconSize(QSize(64, 64));
      pButton->setCheckable(true);
      pButton->setChecked(i == m_iIndex);
      pButton->setProperty("index", i);
      QPixmap pixmap;
      pixmap.convertFromImage(QtHelpers::GetQImageFromColor(pPalette->m_paletteAy[i]));
      pButton->setIcon(pixmap);
      connect(pButton, &QPushButton::clicked, this, &CTilePicker::OnButtonClicked);
      layoutTextures->addWidget(pButton, i / iTilesPerLine, i % iTilesPerLine);
    }
  }
}

//-------------------------------------------------------------------------------------------------

CTilePicker::~CTilePicker()
{

}

//-------------------------------------------------------------------------------------------------

int CTilePicker::GetSelected()
{
  return m_iIndex;
}

//-------------------------------------------------------------------------------------------------

void CTilePicker::OnButtonClicked()
{
  QPushButton *pButton = (QPushButton *)sender();
  m_iIndex = pButton->property("index").toInt();
  accept();
}

//-------------------------------------------------------------------------------------------------