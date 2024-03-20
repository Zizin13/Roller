#include "TrackEditor.h"
#include "TilePicker.h"
#include "Texture.h"
#include "qpushbutton.h"
#include "qdesktopwidget.h"
//-------------------------------------------------------------------------------------------------

CTilePicker::CTilePicker(QWidget *pParent, CTexture *pTexture, int iIndex)
  : QDialog(pParent)
  , m_pTexture(pTexture)
  , m_iIndex(iIndex)
{
  setupUi(this);
  resize(QDesktopWidget().availableGeometry(this).size() * 0.3);
  lblNotLoaded->setVisible(pTexture->m_tileAy.empty());

  int iButtonSize = 90;
  int iTilesPerLine = (width() - 256) / (iButtonSize + 6);
  int i = 0;
  for (; i < m_pTexture->m_tileAy.size(); ++i) {
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
    pixmap.convertFromImage(m_pTexture->m_tileAy[i]);
    pButton->setIcon(pixmap);
    connect(pButton, &QPushButton::clicked, this, &CTilePicker::OnButtonClicked);
    layoutTextures->addWidget(pButton, i / iTilesPerLine, i % iTilesPerLine);
  }

  //add none option
  QPushButton *pButton = new QPushButton(this);
  pButton->setMaximumHeight(iButtonSize);
  pButton->setMaximumWidth(iButtonSize);
  pButton->setMinimumHeight(iButtonSize);
  pButton->setMinimumWidth(iButtonSize);
  pButton->setCheckable(true);
  pButton->setChecked(i == m_iIndex);
  pButton->setProperty("index", -1);
  pButton->setText("<none>");
  connect(pButton, &QPushButton::clicked, this, &CTilePicker::OnButtonClicked);
  layoutTextures->addWidget(pButton, i / iTilesPerLine, i %iTilesPerLine);

  connect(pbCancel, &QPushButton::clicked, this, &CTilePicker::reject);
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