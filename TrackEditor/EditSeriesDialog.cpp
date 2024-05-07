#include "TrackEditor.h"
#include "EditSeriesDialog.h"
#include "Texture.h"
#include "Track.h"
#include "MainWindow.h"
#include "ChunkEditValues.h"
#include <qmessagebox.h>
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CEditSeriesDialog::CEditSeriesDialog(QWidget *pParent)
  : QWidget(pParent)
{
  setupUi(this);

  for (int i = 0; i < chunkFieldsSize; ++i) {
    cbField->addItem(chunkFields[i], i);
  }

  connect(g_pMainWindow, &CMainWindow::UpdateWindowSig, this, &CEditSeriesDialog::OnUpdateWindow);
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

void CEditSeriesDialog::OnUpdateWindow()
{
  int iTrackSize = (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size();
  sbStartChunk->setRange(0, iTrackSize - 1);
  sbEndChunk->setRange(0, iTrackSize - 1);
  sbInterval->setRange(1, iTrackSize - 1);
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

  if (m_iEndChunk < m_iStartChunk) {
    QMessageBox::warning(this, "You need more practice!", "End chunk must not be before start chunk");
    return;
  } else if (m_sEndValue.length() > 0 && m_sIncrement.length() > 0 && IsDirectionValid(m_sEndValue.toDouble(), m_sStartValue.toDouble(), m_sIncrement.toDouble())) {
    QMessageBox::warning(this, "You need more practice!", "Start value does not approach end value");
    return;
  }

  int iField = GetField();
  if ((iField >= 7 && iField <= 9) || (iField >= 37 && iField <= 39)) {
    double dStartValue = GetStartValue().toDouble();
    double dIncrement = GetIncrement().toDouble();
    double dEndValue = GetEndValue().length() != 0 ? GetEndValue().toDouble() : dIncrement == 0.0 ? dStartValue : dIncrement > 0.0 ? DBL_MAX : DBL_MIN;
    if (GetIncrement().length() == 0 && GetEndValue().length() != 0)
      dIncrement = (dEndValue - dStartValue) / (GetEndChunk() - GetStartChunk());
    ApplySeriesToGeometry(GetStartChunk(), GetEndChunk(), GetInterval(), GetField(), dStartValue, dEndValue, dIncrement);
  } else {
    int iStartValue = GetStartValue().toInt();
    int iIncrement = GetIncrement().toInt();
    int iEndValue = GetEndValue().length() != 0 ? GetEndValue().toInt() : iIncrement == 0 ? iStartValue : iIncrement > 0 ? INT_MAX : INT_MIN;
    if (GetIncrement().length() == 0 && GetEndValue().length() != 0)
      iIncrement = (iEndValue - iStartValue) / (GetEndChunk() - GetStartChunk());
    ApplySeriesToGeometry(GetStartChunk(), GetEndChunk(), GetInterval(), GetField(), iStartValue, iEndValue, iIncrement);
  }
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
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

template <typename T> void CEditSeriesDialog::ApplySeriesToGeometry(int iStartChunk, int iEndChunk, int iInterval, int iField, T tStartValue, T tEndValue, T tIncrement)
{
  T tValue = tStartValue;
  bool bDirection = tIncrement >= 0;
  for (int i = iStartChunk; i <= iEndChunk && (bDirection ? tValue <= tEndValue : tValue >= tEndValue); i += iInterval) {
    CChunkEditValues values;
    values.Set(iField, QString::number(tValue));
    g_pMainWindow->GetCurrentTrack()->ApplyGeometrySettings(i, i, values);
    tValue += tIncrement;
  }
}

//-------------------------------------------------------------------------------------------------