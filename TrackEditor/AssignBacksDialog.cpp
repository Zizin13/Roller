#include "TrackEditor.h"
#include "AssignBacksDialog.h"
#include "qdesktopwidget.h"
#include "Track.h"
#include "Texture.h"
#include "BackWidget.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CAssignBacksDialog::CAssignBacksDialog(QWidget *pParent, CTrack *pTrack)
  : QDialog(pParent)
{
  setupUi(this);
  resize(QDesktopWidget().availableGeometry(this).size().width() * 0.1, QDesktopWidget().availableGeometry(this).size().height() * 0.8);
  connect(pbCancel, &QPushButton::clicked, this, &CAssignBacksDialog::reject);
  
  if (pTrack && pTrack->m_pTex) {
    lblNotLoaded->setVisible(pTrack->m_pTex->m_iNumTiles == 0);
    int iButtonSize = 90;
    int iTilesPerLine = (width() - 256) / (iButtonSize + 6);
    int i = 0;
    for (; i < pTrack->m_pTex->m_iNumTiles; ++i) {
      int iBack = -1;
      CSignMap::iterator it = pTrack->m_backsMap.find(i);
      if (it != pTrack->m_backsMap.end())
        iBack = it->second;

      CBackWidget *pBackWidget = new CBackWidget(this, i, pTrack->m_pTex, iBack);
      backsLayout->addWidget(pBackWidget);
    }
  }
}

//-------------------------------------------------------------------------------------------------

CAssignBacksDialog::~CAssignBacksDialog()
{

}

//-------------------------------------------------------------------------------------------------