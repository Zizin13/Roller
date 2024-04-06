#include "TrackEditor.h"
#include "EditSeriesDialog.h"
#include "Texture.h"
#include "Track.h"
#include <qmessagebox.h>
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------


CEditSeriesDialog::CEditSeriesDialog(QWidget *pParent, int iTrackSize)
  : QDialog(pParent)
{
  setupUi(this);

  for (int i = 0; i < chunkFieldsSize; ++i) {
    cbField->addItem(chunkFields[i], i);
  }

  sbStartChunk->setRange(0, iTrackSize - 1);
  sbEndChunk->setRange(0, iTrackSize - 1);
  sbInterval->setRange(1, iTrackSize - 1);

  connect(pbCancel, &QPushButton::clicked, this, &CEditSeriesDialog::reject);
  connect(pbApply, &QPushButton::clicked, this, &CEditSeriesDialog::Validate);
}

//-------------------------------------------------------------------------------------------------

CEditSeriesDialog::~CEditSeriesDialog()
{

}

//-------------------------------------------------------------------------------------------------

int CEditSeriesDialog::GetStartChunk()
{
  return m_iStartChunk;
}

//-------------------------------------------------------------------------------------------------

int CEditSeriesDialog::GetEndChunk()
{
  return m_iEndChunk;
}

//-------------------------------------------------------------------------------------------------

int CEditSeriesDialog::GetInterval()
{
  return m_iInterval;
}

//-------------------------------------------------------------------------------------------------

int CEditSeriesDialog::GetField()
{
  return m_iField;
}

//-------------------------------------------------------------------------------------------------

QString CEditSeriesDialog::GetStartValue()
{
  return m_sStartValue;
}

//-------------------------------------------------------------------------------------------------

QString CEditSeriesDialog::GetEndValue()
{
  return m_sEndValue;
}

//-------------------------------------------------------------------------------------------------

QString CEditSeriesDialog::GetIncrement()
{
  return m_sIncrement;
}

//-------------------------------------------------------------------------------------------------

void CEditSeriesDialog::Validate()
{
  m_iStartChunk = ToInt(sbStartChunk->text());
  m_iEndChunk = ToInt(sbEndChunk->text());
  m_iInterval = ToInt(sbInterval->text());
  m_iField = cbField->currentIndex();
  m_sStartValue = leStartValue->text();
  m_sEndValue = leEndValue->text();
  m_sIncrement = leIncrement->text();

  if (m_iEndChunk < m_iStartChunk)
    QMessageBox::warning(this, "You need more practice!", "End chunk must not be before start chunk");
  else if (m_sEndValue.length() > 0 && m_sIncrement.length() > 0 && IsDirectionValid(m_sEndValue.toDouble(), m_sStartValue.toDouble(), m_sIncrement.toDouble()))
    QMessageBox::warning(this, "You need more practice!", "Start value does not approach end value");
  else
    accept();
}

//-------------------------------------------------------------------------------------------------

bool CEditSeriesDialog::IsDirectionValid(double dStart, double dEnd, double dIncrement)
{
  return (dIncrement == 0.0 && dStart != dEnd) || (dIncrement != 0.0 && (dEnd-dStart > 0.0) == (dIncrement > 0.0));
}

//-------------------------------------------------------------------------------------------------

int CEditSeriesDialog::ToInt(QString sText)
{
  return sText.isEmpty() ? 0 : sText.toInt();
}

//-------------------------------------------------------------------------------------------------