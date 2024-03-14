#include "TrackEditor.h"
#include "LogDialog.h"
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