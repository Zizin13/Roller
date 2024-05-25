#include "TrackEditor.h"
#include "BackWidget.h"
#include "Texture.h"
#include "Palette.h"
#include "QtHelpers.h"
#include "TrackData.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CBackWidget::CBackWidget(QWidget *pParent, int iIndex, CTexture *pTexture, int iBack)
  : QWidget(pParent)
  , m_iBack(iBack)
  , m_pTexture(pTexture)
{
  setupUi(this);
  lblIndex->setText(QString::number(iIndex));
  
  QPixmap pixmap;
  pixmap.convertFromImage(QtHelpers::GetQImageFromTile(pTexture->m_pTileAy[iIndex]));
  lblTex->setPixmap(pixmap);

  UpdateWidget();
}

//-------------------------------------------------------------------------------------------------

CBackWidget::~CBackWidget()
{

}

//-------------------------------------------------------------------------------------------------

int CBackWidget::GetBack()
{
  return m_iBack;
}

//-------------------------------------------------------------------------------------------------

void CBackWidget::UpdateWidget()
{
  if (m_iBack < 0)
    return;

  uint32 uiBackIndex = CTrackData::GetSignedBitValueFromInt(m_iBack);
  uiBackIndex &= SURFACE_TEXTURE_INDEX;
  if ((int)uiBackIndex < m_pTexture->m_iNumTiles) {
    QPixmap backPixmap;
    backPixmap.convertFromImage(QtHelpers::GetQImageFromTile(m_pTexture->m_pTileAy[uiBackIndex]));
    pbBack->setIcon(backPixmap);
  }
}

//-------------------------------------------------------------------------------------------------