#include "EditGeometryWidget.h"
#include "Track.h"
#include "Texture.h"
#include "Palette.h"
#include "MainWindow.h"
#include "ChunkEditValues.h"
#include "QtHelpers.h"
#include "EditSurfaceDialog.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

class CEditGeometryWidgetPrivate
{
public:
  CEditGeometryWidgetPrivate() {};
  ~CEditGeometryWidgetPrivate() {};

  CTrack *m_pTrack;
  CTexture *m_pTex;
  CPalette *m_pPal;
};

//-------------------------------------------------------------------------------------------------

CEditGeometryWidget::CEditGeometryWidget(QWidget *pParent, CTrack *pTrack, CTexture *pTex, CPalette *pPal)
  : QWidget(pParent)
{
  p = new CEditGeometryWidgetPrivate;
  p->m_pTrack = pTrack;
  p->m_pTex = pTex;
  p->m_pPal = pPal;

  setupUi(this);

  connect(g_pMainWindow, &CMainWindow::UpdateGeometrySelectionSig, this, &CEditGeometryWidget::UpdateGeometrySelection);

  lblRoofDisabled->setStyleSheet("QLabel { color : red; }");
  lblLLOWallDisabled->setStyleSheet("QLabel { color : red; }");
  lblLUOWallDisabled->setStyleSheet("QLabel { color : red; }");
  lblRLOWallDisabled->setStyleSheet("QLabel { color : red; }");
  lblRUOWallDisabled->setStyleSheet("QLabel { color : red; }");
  
  connect(dsbYaw              , SIGNAL(valueChanged(double)), this, SLOT(YawChanged(double)));
  connect(dsbPitch            , SIGNAL(valueChanged(double)), this, SLOT(PitchChanged(double)));
  connect(dsbRoll             , SIGNAL(valueChanged(double)), this, SLOT(RollChanged(double)));
  connect(sbLength            , SIGNAL(valueChanged(int)), this, SLOT(LengthChanged(int)));
  connect(sbLLaneWidth        , SIGNAL(valueChanged(int)), this, SLOT(LLaneWidthChanged (int)));
  connect(sbRLaneWidth        , SIGNAL(valueChanged(int)), this, SLOT(RLaneWidthChanged (int)));
  connect(sbLShoulderWidth    , SIGNAL(valueChanged(int)), this, SLOT(LShoulderWidthChanged(int)));
  connect(sbRShoulderWidth    , SIGNAL(valueChanged(int)), this, SLOT(RShoulderWidthChanged(int)));
  connect(sbLShoulderHeight   , SIGNAL(valueChanged(int)), this, SLOT(LShoulderHeightChanged(int)));
  connect(sbRShoulderHeight   , SIGNAL(valueChanged(int)), this, SLOT(RShoulderHeightChanged(int)));
  connect(sbWallHeight        , SIGNAL(valueChanged(int)), this, SLOT(WallHeightChanged(int)));
  connect(sbLLOWallHOffset    , SIGNAL(valueChanged(int)), this, SLOT(LLOWallHOffsetChanged(int)));
  connect(sbLLOWallHeight     , SIGNAL(valueChanged(int)), this, SLOT(LLOWallHeightChanged(int)));
  connect(sbLUOWallHOffset    , SIGNAL(valueChanged(int)), this, SLOT(LUOWallHOffsetChanged(int)));
  connect(sbLUOWallHeight     , SIGNAL(valueChanged(int)), this, SLOT(LUOWallHeightChanged(int)));
  connect(sbRLOWallHOffset    , SIGNAL(valueChanged(int)), this, SLOT(RLOWallHOffsetChanged(int)));
  connect(sbRLOWallHeight     , SIGNAL(valueChanged(int)), this, SLOT(RLOWallHeightChanged(int)));
  connect(sbRUOWallHOffset    , SIGNAL(valueChanged(int)), this, SLOT(RUOWallHOffsetChanged(int)));
  connect(sbRUOWallHeight     , SIGNAL(valueChanged(int)), this, SLOT(RUOWallHeightChanged(int)));
  connect(sbLOuterFloorHOffset, SIGNAL(valueChanged(int)), this, SLOT(LOuterFloorHOffsetChanged(int)));
  connect(sbROuterFloorHOffset, SIGNAL(valueChanged(int)), this, SLOT(ROuterFloorHOffsetChanged(int)));
  connect(sbLOuterFloorHeight , SIGNAL(valueChanged(int)), this, SLOT(LOuterFloorHeightChanged(int)));
  connect(sbROuterFloorHeight , SIGNAL(valueChanged(int)), this, SLOT(ROuterFloorHeightChanged(int)));
  connect(sbAILine1           , SIGNAL(valueChanged(int)), this, SLOT(AILine1Changed(int)));
  connect(sbAILine2           , SIGNAL(valueChanged(int)), this, SLOT(AILine2Changed(int)));
  connect(sbAILine3           , SIGNAL(valueChanged(int)), this, SLOT(AILine3Changed(int)));
  connect(sbAILine4           , SIGNAL(valueChanged(int)), this, SLOT(AILine4Changed(int)));
  connect(sbAIAccuracy        , SIGNAL(valueChanged(int)), this, SLOT(AIAccuracyChanged(int)));

  connect(pbEditCenter,     &QPushButton::clicked, this, &CEditGeometryWidget::EditCSurface);
  connect(pbEditLShoulder,  &QPushButton::clicked, this, &CEditGeometryWidget::EditLShoulder);
  connect(pbEditRShoulder,  &QPushButton::clicked, this, &CEditGeometryWidget::EditRShoulder);
  connect(pbEditLWall,      &QPushButton::clicked, this, &CEditGeometryWidget::EditLWall);
  connect(pbEditRWall,      &QPushButton::clicked, this, &CEditGeometryWidget::EditRWall);
  connect(pbEditBack,       &QPushButton::clicked, this, &CEditGeometryWidget::EditBack);
  connect(pbEditRoofTex,    &QPushButton::clicked, this, &CEditGeometryWidget::EditRoof);
  connect(pbEditLUOWall,    &QPushButton::clicked, this, &CEditGeometryWidget::EditLUOWall);
  connect(pbEditLLOWall,    &QPushButton::clicked, this, &CEditGeometryWidget::EditLLOWall);
  connect(pbEditRLOWall,    &QPushButton::clicked, this, &CEditGeometryWidget::EditRLOWall);
  connect(pbEditRUOWall,    &QPushButton::clicked, this, &CEditGeometryWidget::EditRUOWall);
  connect(pbEditOFloor,     &QPushButton::clicked, this, &CEditGeometryWidget::EditOFloor);

  connect(sldCGrip        , SIGNAL(valueChanged(int)), this, SLOT(CGripChanged(int)));
  connect(sldLShoulderGrip, SIGNAL(valueChanged(int)), this, SLOT(LGripChanged(int)));
  connect(sldRShoulderGrip, SIGNAL(valueChanged(int)), this, SLOT(RGripChanged(int)));
  connect(sldAISpeed,       SIGNAL(valueChanged(int)), this, SLOT(AISpeedChanged(int)));
}

//-------------------------------------------------------------------------------------------------

CEditGeometryWidget::~CEditGeometryWidget()
{

}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::UpdateGeometrySelection(int iFrom, int iTo)
{
  (void)(iTo);
  if (!p->m_pTrack || iFrom >= (int)p->m_pTrack->m_chunkAy.size())
    return;
  BLOCK_SIG_AND_DO(dsbYaw, setValue(               p->m_pTrack->m_chunkAy[iFrom].dYaw));
  BLOCK_SIG_AND_DO(dsbPitch, setValue(             p->m_pTrack->m_chunkAy[iFrom].dPitch));
  BLOCK_SIG_AND_DO(dsbRoll, setValue(              p->m_pTrack->m_chunkAy[iFrom].dRoll));
  BLOCK_SIG_AND_DO(sbLength, setValue(             p->m_pTrack->m_chunkAy[iFrom].iLength));
  BLOCK_SIG_AND_DO(sbLLaneWidth, setValue(         p->m_pTrack->m_chunkAy[iFrom].iLeftLaneWidth));
  BLOCK_SIG_AND_DO(sbRLaneWidth, setValue(         p->m_pTrack->m_chunkAy[iFrom].iRightLaneWidth));
  BLOCK_SIG_AND_DO(sbLShoulderWidth, setValue(     p->m_pTrack->m_chunkAy[iFrom].iLeftShoulderWidth));
  BLOCK_SIG_AND_DO(sbRShoulderWidth, setValue(     p->m_pTrack->m_chunkAy[iFrom].iRightShoulderWidth));
  BLOCK_SIG_AND_DO(sbLShoulderHeight, setValue(    p->m_pTrack->m_chunkAy[iFrom].iLeftShoulderHeight));
  BLOCK_SIG_AND_DO(sbRShoulderHeight, setValue(    p->m_pTrack->m_chunkAy[iFrom].iRightShoulderHeight));
  BLOCK_SIG_AND_DO(sbWallHeight, setValue(         p->m_pTrack->m_chunkAy[iFrom].iRoofHeight));
  BLOCK_SIG_AND_DO(sbLLOWallHOffset, setValue(     p->m_pTrack->m_chunkAy[iFrom].iLLOuterWallHOffset));
  BLOCK_SIG_AND_DO(sbLLOWallHeight, setValue(      p->m_pTrack->m_chunkAy[iFrom].iLLOuterWallHeight));
  BLOCK_SIG_AND_DO(sbLUOWallHOffset, setValue(     p->m_pTrack->m_chunkAy[iFrom].iLUOuterWallHOffset));
  BLOCK_SIG_AND_DO(sbLUOWallHeight, setValue(      p->m_pTrack->m_chunkAy[iFrom].iLUOuterWallHeight));
  BLOCK_SIG_AND_DO(sbRLOWallHOffset, setValue(     p->m_pTrack->m_chunkAy[iFrom].iRLOuterWallHOffset));
  BLOCK_SIG_AND_DO(sbRLOWallHeight, setValue(      p->m_pTrack->m_chunkAy[iFrom].iRLOuterWallHeight));
  BLOCK_SIG_AND_DO(sbRUOWallHOffset, setValue(     p->m_pTrack->m_chunkAy[iFrom].iRUOuterWallHOffset));
  BLOCK_SIG_AND_DO(sbRUOWallHeight, setValue(      p->m_pTrack->m_chunkAy[iFrom].iRUOuterWallHeight));
  BLOCK_SIG_AND_DO(sbLOuterFloorHOffset, setValue( p->m_pTrack->m_chunkAy[iFrom].iLOuterFloorHOffset));
  BLOCK_SIG_AND_DO(sbROuterFloorHOffset, setValue( p->m_pTrack->m_chunkAy[iFrom].iROuterFloorHOffset));
  BLOCK_SIG_AND_DO(sbLOuterFloorHeight, setValue(  p->m_pTrack->m_chunkAy[iFrom].iLOuterFloorHeight));
  BLOCK_SIG_AND_DO(sbROuterFloorHeight, setValue(  p->m_pTrack->m_chunkAy[iFrom].iROuterFloorHeight));
  BLOCK_SIG_AND_DO(sbAILine1, setValue(            p->m_pTrack->m_chunkAy[iFrom].iAILine1));
  BLOCK_SIG_AND_DO(sbAILine2, setValue(            p->m_pTrack->m_chunkAy[iFrom].iAILine2));
  BLOCK_SIG_AND_DO(sbAILine3, setValue(            p->m_pTrack->m_chunkAy[iFrom].iAILine3));
  BLOCK_SIG_AND_DO(sbAILine4, setValue(            p->m_pTrack->m_chunkAy[iFrom].iAILine4));
  BLOCK_SIG_AND_DO(sldCGrip, setValue(             p->m_pTrack->m_chunkAy[iFrom].iTrackGrip));
  BLOCK_SIG_AND_DO(sldLShoulderGrip, setValue(     p->m_pTrack->m_chunkAy[iFrom].iLeftShoulderGrip));
  BLOCK_SIG_AND_DO(sldRShoulderGrip, setValue(     p->m_pTrack->m_chunkAy[iFrom].iRightShoulderGrip);)
  BLOCK_SIG_AND_DO(sldAISpeed, setValue(           p->m_pTrack->m_chunkAy[iFrom].iAIMaxSpeed / 10));
  BLOCK_SIG_AND_DO(sbAIAccuracy, setValue(         p->m_pTrack->m_chunkAy[iFrom].iAIAccuracy));

  lblCGrip->setText(        "(" + QString::number(p->m_pTrack->m_chunkAy[iFrom].iTrackGrip) + ")");
  lblLShoulderGrip->setText("(" + QString::number(p->m_pTrack->m_chunkAy[iFrom].iLeftShoulderGrip) + ")");
  lblRShoulderGrip->setText("(" + QString::number(p->m_pTrack->m_chunkAy[iFrom].iRightShoulderGrip) + ")");
  lblAISpeed->setText(      "(" + QString::number(p->m_pTrack->m_chunkAy[iFrom].iAIMaxSpeed / 10) + "%)");

  QtHelpers::UpdateTextures(lblCTex1, lblCTex2, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iCenterSurfaceType);
  QtHelpers::UpdateTextures(lblLShoulderTex1, lblLShoulderTex2, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iLeftSurfaceType);
  QtHelpers::UpdateTextures(lblRShoulderTex1, lblRShoulderTex2, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iRightSurfaceType);
  QtHelpers::UpdateTextures(lblLWallTex1, lblLWallTex2, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iLeftWallType);
  QtHelpers::UpdateTextures(lblRWallTex1, lblRWallTex2, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iRightWallType);
  QtHelpers::UpdateTextures(lblBackTex1, lblBackTex2, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iBackTexture);
  QtHelpers::UpdateTextures(lblRoofTex1, lblRoofTex2, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iRoofType);
  QtHelpers::UpdateTextures(lblLLOWallTex1, lblLLOWallTex2, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iLLOuterWallType);
  QtHelpers::UpdateTextures(lblLUOWallTex1, lblLUOWallTex2, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iLUOuterWallType);
  QtHelpers::UpdateTextures(lblRLOWallTex1, lblRLOWallTex2, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iRLOuterWallType);
  QtHelpers::UpdateTextures(lblRUOWallTex1, lblRUOWallTex2, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iRUOuterWallType);
  QtHelpers::UpdateTextures(lblOFloorTex1, lblOFloorTex2, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iOuterFloorType);

  //update roof disabled text
  lblRoofDisabled->setVisible(!p->m_pTrack->ShouldShowChunkSection(iFrom, eShapeSection::ROOF));

  //update llowall disabled text
  lblLLOWallDisabled->setVisible(!p->m_pTrack->ShouldShowChunkSection(iFrom, eShapeSection::LLOWALL));
  if (p->m_pTrack->m_chunkAy[iFrom].iOuterFloorType == -1)
    lblLLOWallDisabled->setText("(disabled by floor)");
  else if (p->m_pTrack->m_chunkAy[iFrom].iOuterFloorType == -2)
    lblLLOWallDisabled->setText("(disabled by center)");

  //update rlowall disabled text
  lblRLOWallDisabled->setVisible(!p->m_pTrack->ShouldShowChunkSection(iFrom, eShapeSection::RLOWALL));
  if (p->m_pTrack->m_chunkAy[iFrom].iOuterFloorType == -1)
    lblRLOWallDisabled->setText("(disabled by floor)");
  else if (p->m_pTrack->m_chunkAy[iFrom].iOuterFloorType == -2)
    lblRLOWallDisabled->setText("(disabled by center)");

  //update luowall disabled text
  lblLUOWallDisabled->setVisible(!p->m_pTrack->ShouldShowChunkSection(iFrom, eShapeSection::LUOWALL));
  if (p->m_pTrack->m_chunkAy[iFrom].iLLOuterWallType == -1)
    lblLUOWallDisabled->setText("(disabled by lower wall)");
  else if (p->m_pTrack->m_chunkAy[iFrom].iOuterFloorType == -1)
    lblLUOWallDisabled->setText("(disabled by floor)");
  else if (p->m_pTrack->m_chunkAy[iFrom].iOuterFloorType == -2)
    lblLUOWallDisabled->setText("(disabled by center)");

  //update ruowall disabled text
  lblRUOWallDisabled->setVisible(!p->m_pTrack->ShouldShowChunkSection(iFrom, eShapeSection::RUOWALL));
  if (p->m_pTrack->m_chunkAy[iFrom].iRLOuterWallType == -1)
    lblRUOWallDisabled->setText("(disabled by lower wall)");
  else if (p->m_pTrack->m_chunkAy[iFrom].iOuterFloorType == -1)
    lblRUOWallDisabled->setText("(disabled by floor)");
  else if (p->m_pTrack->m_chunkAy[iFrom].iOuterFloorType == -2)
    lblRUOWallDisabled->setText("(disabled by center)");
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::YawChanged(double dValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].dYaw = dValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::PitchChanged(double dValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].dPitch = dValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RollChanged(double dValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].dRoll = dValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LengthChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLength = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LLaneWidthChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLeftLaneWidth = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RLaneWidthChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRightLaneWidth = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LShoulderWidthChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLeftShoulderWidth = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RShoulderWidthChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRightShoulderWidth = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LShoulderHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLeftShoulderHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RShoulderHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRightShoulderHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::WallHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRoofHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LLOWallHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLLOuterWallHOffset = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LLOWallHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLLOuterWallHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LUOWallHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLUOuterWallHOffset = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LUOWallHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLUOuterWallHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RLOWallHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRLOuterWallHOffset = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RLOWallHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRLOuterWallHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RUOWallHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRUOuterWallHOffset = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RUOWallHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRUOuterWallHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LOuterFloorHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLOuterFloorHOffset = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::ROuterFloorHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iROuterFloorHOffset = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LOuterFloorHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLOuterFloorHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::ROuterFloorHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iROuterFloorHeight = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AILine1Changed(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iAILine1 = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AILine2Changed(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iAILine2 = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AILine3Changed(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iAILine3 = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AILine4Changed(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iAILine4 = iValue;
  }

  p->m_pTrack->UpdateChunkStrings();
  p->m_pTrack->GenerateTrackMath();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditCSurface()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();
  
  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iCenterSurfaceType, true, "(also disables outer walls if floor is -2)");
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iCenterSurfaceType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditLShoulder()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iLeftSurfaceType, true, "(also disables outer walls if floor is -2)");
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iLeftSurfaceType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditRShoulder()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iRightSurfaceType, true, "(also disables outer walls if floor is -2)");
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iRightSurfaceType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditLWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iLeftWallType, true, "(also disables roof)");
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iLeftWallType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditRWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iRightWallType, true, "(also disables roof)");
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iRightWallType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditBack()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iBackTexture);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iBackTexture = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditRoof()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iRoofType, true);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iRoofType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditLUOWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iLUOuterWallType, true);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iLUOuterWallType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditLLOWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iLLOuterWallType, true, "(also disables upper wall)");
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iLLOuterWallType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditRLOWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iRLOuterWallType, true, "(also disables upper wall)");
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iRLOuterWallType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditRUOWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iRUOuterWallType, true);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iRUOuterWallType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditOFloor()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  CEditSurfaceDialog dlg(this, p->m_pTex, p->m_pPal, p->m_pTrack->m_chunkAy[iFrom].iOuterFloorType, true, "(also disables outer walls)", true);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      p->m_pTrack->m_chunkAy[i].iOuterFloorType = dlg.GetValue();
    }
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::CGripChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iTrackGrip = iValue;
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LGripChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iLeftShoulderGrip = iValue;
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RGripChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iRightShoulderGrip = iValue;
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AISpeedChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iAIMaxSpeed = iValue * 10;
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AIAccuracyChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!p->m_pTrack
      || iFrom >= (int)p->m_pTrack->m_chunkAy.size()
      || iTo >= (int)p->m_pTrack->m_chunkAy.size())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    p->m_pTrack->m_chunkAy[i].iAIAccuracy = iValue;
  }
  p->m_pTrack->UpdateChunkStrings();
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------