#include "TrackEditor.h"
#include "EditSeriesDialog.h"
#include "Texture.h"
#include "Track.h"
#include "MainWindow.h"
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
  if (!g_pMainWindow->GetCurrentTrack())
    return;

  int iTrackSize = (int)g_pMainWindow->GetCurrentTrack()->m_chunkAy.size();
  sbInterval->setRange(1, iTrackSize - 1);
}

//-------------------------------------------------------------------------------------------------

void CEditSeriesDialog::Validate()
{
  m_iStartChunk = g_pMainWindow->GetSelFrom();
  m_iEndChunk = g_pMainWindow->GetSelTo();
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
  g_pMainWindow->SaveHistory("Edited geometry chunks in series");
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
  if (!g_pMainWindow->GetCurrentTrack())
    return;

  T tValue = tStartValue;
  bool bDirection = tIncrement >= 0;
  for (int i = iStartChunk; i <= iEndChunk && (bDirection ? tValue <= tEndValue : tValue >= tEndValue); i += iInterval) {
    switch (iField) {
      case 0: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iLeftShoulderWidth = tValue; break;
      case 1: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iLeftLaneWidth = tValue; break;
      case 2: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iRightLaneWidth = tValue; break;
      case 3: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iRightShoulderWidth = tValue; break;
      case 4: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iLeftShoulderHeight = tValue; break;
      case 5: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iRightShoulderHeight = tValue; break;
      case 6: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iLength = tValue; break;
      case 7: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].dYaw = tValue; break;
      case 8: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].dPitch = tValue; break;
      case 9: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].dRoll = tValue; break;
      case 10: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iAILine1 = tValue; break;
      case 11: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iAILine2 = tValue; break;
      case 12: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iAILine3 = tValue; break;
      case 13: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iAILine4 = tValue; break;
      case 14: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iTrackGrip = tValue; break;
      case 15: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iLeftShoulderGrip = tValue; break;
      case 16: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iRightShoulderGrip = tValue; break;
      case 17: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iAIMaxSpeed = tValue; break;
      case 18: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iAIAccuracy = tValue; break;
      case 19: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iAudioAboveTrigger = tValue; break;
      case 20: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iAudioTriggerSpeed = tValue; break;
      case 21: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iAudioBelowTrigger = tValue; break;
      case 22: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iLeftSurfaceType = tValue; break;
      case 23: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iCenterSurfaceType = tValue; break;
      case 24: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iRightSurfaceType = tValue; break;
      case 25: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iLeftWallType = tValue; break;
      case 26: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iRightWallType = tValue; break;
      case 27: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iRoofType = tValue; break;
      case 28: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iLUOuterWallType = tValue; break;
      case 29: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iLLOuterWallType = tValue; break;
      case 30: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iOuterFloorType = tValue; break;
      case 31: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iRLOuterWallType = tValue; break;
      case 32: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iRUOuterWallType = tValue; break;
      case 33: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iEnvironmentFloorType = tValue; break;
      case 34: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iSignType = tValue; break;
      case 35: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iSignHorizOffset = tValue; break;
      case 36: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iSignVertOffset = tValue; break;
      case 37: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].dSignYaw = tValue; break;
      case 38: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].dSignPitch = tValue; break;
      case 39: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].dSignRoll = tValue; break;
      case 40: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iLUOuterWallHOffset = tValue; break;
      case 41: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iLLOuterWallHOffset = tValue; break;
      case 42: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iLOuterFloorHOffset = tValue; break;
      case 43: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iROuterFloorHOffset = tValue; break;
      case 44: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iRLOuterWallHOffset = tValue; break;
      case 45: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iRUOuterWallHOffset = tValue; break;
      case 46: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iLUOuterWallHeight = tValue; break;
      case 47: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iLLOuterWallHeight = tValue; break;
      case 48: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iLOuterFloorHeight = tValue; break;
      case 49: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iROuterFloorHeight = tValue; break;
      case 50: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iRLOuterWallHeight = tValue; break;
      case 51: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iRUOuterWallHeight = tValue; break;
      case 52: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iRoofHeight = tValue; break;
      case 53: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iDrawOrder1 = tValue; break;
      case 54: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iDrawOrder2 = tValue; break;
      case 55: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iDrawOrder3 = tValue; break;
      case 56: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iUnk37 = tValue; break;
      case 57: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iUnk38 = tValue; break;
      case 58: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iUnk39 = tValue; break;
      case 59: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iUnk40 = tValue; break;
      case 60: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iUnk41 = tValue; break;
      case 61: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iUnk42 = tValue; break;
      case 62: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iUnk43 = tValue; break;
      case 63: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iUnk44 = tValue; break;
      case 64: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iUnk45 = tValue; break;
      case 65: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iUnk46 = tValue; break;
      case 66: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iUnk47 = tValue; break;
      case 67: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iUnk48 = tValue; break;
      case 68: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iUnk49 = tValue; break;
      case 69: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iUnk50 = tValue; break;
      case 70: g_pMainWindow->GetCurrentTrack()->m_chunkAy[i].iSignTexture = tValue; break;
    }
    tValue += tIncrement;
  }
}

//-------------------------------------------------------------------------------------------------