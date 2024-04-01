#include "TrackEditor.h"
#include "EditSeriesDialog.h"
#include "Texture.h"
#include "Track.h"
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

  UpdateDialog();

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

int CEditSeriesDialog::GetValue()
{
  return m_iValue;
}

//-------------------------------------------------------------------------------------------------

int CEditSeriesDialog::GetIncrement()
{
  return m_iIncrement;
}

//-------------------------------------------------------------------------------------------------

void CEditSeriesDialog::Validate()
{
  m_iStartChunk = ToInt(sbStartChunk->text());
  m_iEndChunk = ToInt(sbEndChunk->text());
  m_iInterval = ToInt(sbInterval->text());
  m_iField = cbField->currentIndex();
  m_iValue = ToInt(leValue->text());
  m_iIncrement = ToInt(leIncrement->text());

  if (m_iEndChunk >= m_iStartChunk)
    accept();
}

//-------------------------------------------------------------------------------------------------

int CEditSeriesDialog::ToInt(QString sText)
{
  return sText.isEmpty() ? 0 : sText.toInt();
}

//-------------------------------------------------------------------------------------------------

void CEditSeriesDialog::UpdateDialog()
{
}

//-------------------------------------------------------------------------------------------------