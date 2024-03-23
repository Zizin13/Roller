#include "TrackEditor.h"
#include "LogDialog.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CLogDialog::CLogDialog(QWidget *pParent)
  : QDialog(pParent)
{
  setupUi(this);
  connect(pbClose, &QPushButton::clicked, this, &CLogDialog::reject);
}

//-------------------------------------------------------------------------------------------------

CLogDialog::~CLogDialog()
{

}

//-------------------------------------------------------------------------------------------------

void CLogDialog::LogMessage(const QString &sMsg)
{
  txDebug->appendPlainText(sMsg);
}

//-------------------------------------------------------------------------------------------------