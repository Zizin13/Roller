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

CEditGeometryWidget::CEditGeometryWidget(QWidget *pParent)
  : QWidget(pParent)
{

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
  connect(pbEditEnvirFloor, &QPushButton::clicked, this, &CEditGeometryWidget::EditEnvirFloor);

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
  if (!g_pMainWindow->GetCurrentTrack() || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  tGeometryChunk chunk;
  g_pMainWindow->GetCurrentTrack()->GetChunk(iFrom, chunk);

  BLOCK_SIG_AND_DO(dsbYaw, setValue(               chunk.dYaw));
  BLOCK_SIG_AND_DO(dsbPitch, setValue(             chunk.dPitch));
  BLOCK_SIG_AND_DO(dsbRoll, setValue(              chunk.dRoll));
  BLOCK_SIG_AND_DO(sbLength, setValue(             chunk.iLength));
  BLOCK_SIG_AND_DO(sbLLaneWidth, setValue(         chunk.iLeftLaneWidth));
  BLOCK_SIG_AND_DO(sbRLaneWidth, setValue(         chunk.iRightLaneWidth));
  BLOCK_SIG_AND_DO(sbLShoulderWidth, setValue(     chunk.iLeftShoulderWidth));
  BLOCK_SIG_AND_DO(sbRShoulderWidth, setValue(     chunk.iRightShoulderWidth));
  BLOCK_SIG_AND_DO(sbLShoulderHeight, setValue(    chunk.iLeftShoulderHeight));
  BLOCK_SIG_AND_DO(sbRShoulderHeight, setValue(    chunk.iRightShoulderHeight));
  BLOCK_SIG_AND_DO(sbWallHeight, setValue(         chunk.iRoofHeight));
  BLOCK_SIG_AND_DO(sbLLOWallHOffset, setValue(     chunk.iLLOuterWallHOffset));
  BLOCK_SIG_AND_DO(sbLLOWallHeight, setValue(      chunk.iLLOuterWallHeight));
  BLOCK_SIG_AND_DO(sbLUOWallHOffset, setValue(     chunk.iLUOuterWallHOffset));
  BLOCK_SIG_AND_DO(sbLUOWallHeight, setValue(      chunk.iLUOuterWallHeight));
  BLOCK_SIG_AND_DO(sbRLOWallHOffset, setValue(     chunk.iRLOuterWallHOffset));
  BLOCK_SIG_AND_DO(sbRLOWallHeight, setValue(      chunk.iRLOuterWallHeight));
  BLOCK_SIG_AND_DO(sbRUOWallHOffset, setValue(     chunk.iRUOuterWallHOffset));
  BLOCK_SIG_AND_DO(sbRUOWallHeight, setValue(      chunk.iRUOuterWallHeight));
  BLOCK_SIG_AND_DO(sbLOuterFloorHOffset, setValue( chunk.iLOuterFloorHOffset));
  BLOCK_SIG_AND_DO(sbROuterFloorHOffset, setValue( chunk.iROuterFloorHOffset));
  BLOCK_SIG_AND_DO(sbLOuterFloorHeight, setValue(  chunk.iLOuterFloorHeight));
  BLOCK_SIG_AND_DO(sbROuterFloorHeight, setValue(  chunk.iROuterFloorHeight));
  BLOCK_SIG_AND_DO(sbAILine1, setValue(            chunk.iAILine1));
  BLOCK_SIG_AND_DO(sbAILine2, setValue(            chunk.iAILine2));
  BLOCK_SIG_AND_DO(sbAILine3, setValue(            chunk.iAILine3));
  BLOCK_SIG_AND_DO(sbAILine4, setValue(            chunk.iAILine4));
  BLOCK_SIG_AND_DO(sldCGrip, setValue(             chunk.iTrackGrip));
  BLOCK_SIG_AND_DO(sldLShoulderGrip, setValue(     chunk.iLeftShoulderGrip));
  BLOCK_SIG_AND_DO(sldRShoulderGrip, setValue(     chunk.iRightShoulderGrip);)
  BLOCK_SIG_AND_DO(sldAISpeed, setValue(           chunk.iAIMaxSpeed / 10));
  BLOCK_SIG_AND_DO(sbAIAccuracy, setValue(         chunk.iAIAccuracy));

  lblCGrip->setText(        "(" + QString::number(chunk.iTrackGrip) + ")");
  lblLShoulderGrip->setText("(" + QString::number(chunk.iLeftShoulderGrip) + ")");
  lblRShoulderGrip->setText("(" + QString::number(chunk.iRightShoulderGrip) + ")");
  lblAISpeed->setText(      "(" + QString::number(chunk.iAIMaxSpeed / 10) + "%)");

  QtHelpers::UpdateTextures(lblCTex1, lblCTex2, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iCenterSurfaceType);
  QtHelpers::UpdateTextures(lblLShoulderTex1, lblLShoulderTex2, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iLeftSurfaceType);
  QtHelpers::UpdateTextures(lblRShoulderTex1, lblRShoulderTex2, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iRightSurfaceType);
  QtHelpers::UpdateTextures(lblLWallTex1, lblLWallTex2, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iLeftWallType);
  QtHelpers::UpdateTextures(lblRWallTex1, lblRWallTex2, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iRightWallType);
  QtHelpers::UpdateTextures(lblBackTex1, lblBackTex2, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iBackTexture);
  QtHelpers::UpdateTextures(lblRoofTex1, lblRoofTex2, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iRoofType);
  QtHelpers::UpdateTextures(lblLLOWallTex1, lblLLOWallTex2, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iLLOuterWallType);
  QtHelpers::UpdateTextures(lblLUOWallTex1, lblLUOWallTex2, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iLUOuterWallType);
  QtHelpers::UpdateTextures(lblRLOWallTex1, lblRLOWallTex2, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iRLOuterWallType);
  QtHelpers::UpdateTextures(lblRUOWallTex1, lblRUOWallTex2, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iRUOuterWallType);
  QtHelpers::UpdateTextures(lblOFloorTex1, lblOFloorTex2, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iOuterFloorType);
  QtHelpers::UpdateTextures(lblEnvirFloorTex, NULL, NULL, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iEnvironmentFloorType);

  //update roof disabled text
  lblRoofDisabled->setVisible(!g_pMainWindow->GetCurrentTrack()->ShouldShowChunkSection(iFrom, eShapeSection::ROOF) && CTrackData::ShouldDrawSurfaceType(chunk.iRoofType));

  //update llowall disabled text
  lblLLOWallDisabled->setVisible(!g_pMainWindow->GetCurrentTrack()->ShouldShowChunkSection(iFrom, eShapeSection::LLOWALL) && CTrackData::ShouldDrawSurfaceType(chunk.iLLOuterWallType));
  if (chunk.iOuterFloorType == -1)
    lblLLOWallDisabled->setText("(disabled by floor)");
  else if (chunk.iOuterFloorType == -2)
    lblLLOWallDisabled->setText("(disabled by center)");
  else
    lblLLOWallDisabled->setText("(disabled by other)");

  //update rlowall disabled text
  lblRLOWallDisabled->setVisible(!g_pMainWindow->GetCurrentTrack()->ShouldShowChunkSection(iFrom, eShapeSection::RLOWALL) && CTrackData::ShouldDrawSurfaceType(chunk.iRLOuterWallType));
  if (chunk.iOuterFloorType == -1)
    lblRLOWallDisabled->setText("(disabled by floor)");
  else if (chunk.iOuterFloorType == -2)
    lblRLOWallDisabled->setText("(disabled by center)");
  else
    lblRLOWallDisabled->setText("(disabled by other)");

  //update luowall disabled text
  lblLUOWallDisabled->setVisible(!g_pMainWindow->GetCurrentTrack()->ShouldShowChunkSection(iFrom, eShapeSection::LUOWALL) && CTrackData::ShouldDrawSurfaceType(chunk.iLUOuterWallType));
  if (chunk.iLLOuterWallType == -1)
    lblLUOWallDisabled->setText("(disabled by lower wall)");
  else if (chunk.iOuterFloorType == -1)
    lblLUOWallDisabled->setText("(disabled by floor)");
  else if (chunk.iOuterFloorType == -2)
    lblLUOWallDisabled->setText("(disabled by center)");
  else
    lblLUOWallDisabled->setText("(disabled by other)");

  //update ruowall disabled text
  lblRUOWallDisabled->setVisible(!g_pMainWindow->GetCurrentTrack()->ShouldShowChunkSection(iFrom, eShapeSection::RUOWALL) && CTrackData::ShouldDrawSurfaceType(chunk.iRUOuterWallType));
  if (chunk.iRLOuterWallType == -1)
    lblRUOWallDisabled->setText("(disabled by lower wall)");
  else if (chunk.iOuterFloorType == -1)
    lblRUOWallDisabled->setText("(disabled by floor)");
  else if (chunk.iOuterFloorType == -2)
    lblRUOWallDisabled->setText("(disabled by center)");
  else
    lblRUOWallDisabled->setText("(disabled by other)");
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::YawChanged(double dValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.dYaw = dValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed yaw");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::PitchChanged(double dValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.dPitch = dValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed pitch");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RollChanged(double dValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.dRoll = dValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed roll");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LengthChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iLength = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed length");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LLaneWidthChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iLeftLaneWidth = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed left lane width");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RLaneWidthChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iRightLaneWidth = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed right lane width");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LShoulderWidthChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iLeftShoulderWidth = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed left shoulder width");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RShoulderWidthChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iRightShoulderWidth = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed right shoulder width");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LShoulderHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iLeftShoulderHeight = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed left shoulder height");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RShoulderHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iRightShoulderHeight = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed right shoulder height");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::WallHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iRoofHeight = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed wall height");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LLOWallHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iLLOuterWallHOffset = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed llowall width");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LLOWallHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iLLOuterWallHeight = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed llowall height");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LUOWallHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iLUOuterWallHOffset = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed luowall width");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LUOWallHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iLUOuterWallHeight = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed luowall height");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RLOWallHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iRLOuterWallHOffset = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed rlowall width");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RLOWallHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iRLOuterWallHeight = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed rlowall height");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RUOWallHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iRUOuterWallHOffset = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed ruowall width");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RUOWallHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iRUOuterWallHeight = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed ruowall height");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LOuterFloorHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iLOuterFloorHOffset = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed lofloor horiz offset");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::ROuterFloorHOffsetChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iROuterFloorHOffset = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed rofloor horiz offset");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LOuterFloorHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iLOuterFloorHeight = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed lofloor height");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::ROuterFloorHeightChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iROuterFloorHeight = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed rofloor height");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AILine1Changed(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iAILine1 = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed ai line 1");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AILine2Changed(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iAILine2 = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed ai line 2");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AILine3Changed(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iAILine3 = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed ai line 3");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AILine4Changed(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iAILine4 = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }

  g_pMainWindow->SaveHistory("Changed ai line 4");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditCSurface()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();
  
  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  tGeometryChunk chunk;
  g_pMainWindow->GetCurrentTrack()->GetChunk(iFrom, chunk);
  CEditSurfaceDialog dlg(this, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iCenterSurfaceType, true, "(also disables outer walls if floor is -2)");
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
      chunk.iCenterSurfaceType = dlg.GetValue();
      g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
    }
  }
  g_pMainWindow->SaveHistory("Changed center surface");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditLShoulder()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  tGeometryChunk chunk;
  g_pMainWindow->GetCurrentTrack()->GetChunk(iFrom, chunk);
  CEditSurfaceDialog dlg(this, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iLeftSurfaceType, true, "(also disables outer walls if floor is -2)");
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
      chunk.iLeftSurfaceType = dlg.GetValue();
      g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
    }
  }
  g_pMainWindow->SaveHistory("Changed left shoulder surface");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditRShoulder()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  tGeometryChunk chunk;
  g_pMainWindow->GetCurrentTrack()->GetChunk(iFrom, chunk);
  CEditSurfaceDialog dlg(this, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iRightSurfaceType, true, "(also disables outer walls if floor is -2)");
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
      chunk.iRightSurfaceType = dlg.GetValue();
      g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
    }
  }
  g_pMainWindow->SaveHistory("Changed right shoulder surface");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditLWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  tGeometryChunk chunk;
  g_pMainWindow->GetCurrentTrack()->GetChunk(iFrom, chunk);
  CEditSurfaceDialog dlg(this, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iLeftWallType, true, "(also disables roof)");
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
      chunk.iLeftWallType = dlg.GetValue();
      g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
    }
  }
  g_pMainWindow->SaveHistory("Changed left wall surface");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditRWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  tGeometryChunk chunk;
  g_pMainWindow->GetCurrentTrack()->GetChunk(iFrom, chunk);
  CEditSurfaceDialog dlg(this, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iRightWallType, true, "(also disables roof)");
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
      chunk.iRightWallType = dlg.GetValue();
      g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
    }
  }
  g_pMainWindow->SaveHistory("Changed right wall surface");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditBack()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  tGeometryChunk chunk;
  g_pMainWindow->GetCurrentTrack()->GetChunk(iFrom, chunk);
  CEditSurfaceDialog dlg(this, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iBackTexture);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
      chunk.iBackTexture = dlg.GetValue();
      g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
    }
  }
  g_pMainWindow->SaveHistory("Changed wall back surfaces");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditRoof()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  tGeometryChunk chunk;
  g_pMainWindow->GetCurrentTrack()->GetChunk(iFrom, chunk);
  CEditSurfaceDialog dlg(this, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iRoofType, true);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
      chunk.iRoofType = dlg.GetValue();
      g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
    }
  }
  g_pMainWindow->SaveHistory("Changed roof surface");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditLUOWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  tGeometryChunk chunk;
  g_pMainWindow->GetCurrentTrack()->GetChunk(iFrom, chunk);
  CEditSurfaceDialog dlg(this, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iLUOuterWallType, true);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
      chunk.iLUOuterWallType = dlg.GetValue();
      g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
    }
  }
  g_pMainWindow->SaveHistory("Changed luowall surface");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditLLOWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  tGeometryChunk chunk;
  g_pMainWindow->GetCurrentTrack()->GetChunk(iFrom, chunk);
  CEditSurfaceDialog dlg(this, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iLLOuterWallType, true, "(also disables upper wall)");
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
      chunk.iLLOuterWallType = dlg.GetValue();
      g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
    }
  }
  g_pMainWindow->SaveHistory("Changed llowall surface");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditRLOWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  tGeometryChunk chunk;
  g_pMainWindow->GetCurrentTrack()->GetChunk(iFrom, chunk);
  CEditSurfaceDialog dlg(this, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iRLOuterWallType, true, "(also disables upper wall)");
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
      chunk.iRLOuterWallType = dlg.GetValue();
      g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
    }
  }
  g_pMainWindow->SaveHistory("Changed rlowall surface");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditRUOWall()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  tGeometryChunk chunk;
  g_pMainWindow->GetCurrentTrack()->GetChunk(iFrom, chunk);
  CEditSurfaceDialog dlg(this, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iRUOuterWallType, true);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
      chunk.iRUOuterWallType = dlg.GetValue();
      g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
    }
  }
  g_pMainWindow->SaveHistory("Changed ruowall surface");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditOFloor()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  tGeometryChunk chunk;
  g_pMainWindow->GetCurrentTrack()->GetChunk(iFrom, chunk);
  CEditSurfaceDialog dlg(this, g_pMainWindow->GetCurrentTrack()->m_pTex, g_pMainWindow->GetCurrentTrack()->m_pPal, chunk.iOuterFloorType, true, "(also disables outer walls)", true);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
      chunk.iOuterFloorType = dlg.GetValue();
      g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
    }
  }
  g_pMainWindow->SaveHistory("Changed ofloor surface");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::EditEnvirFloor()
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  tGeometryChunk chunk;
  g_pMainWindow->GetCurrentTrack()->GetChunk(iFrom, chunk);

  CEditSurfaceDialog dlg(this, 
                         NULL, 
                         g_pMainWindow->GetCurrentTrack()->m_pPal, 
                         chunk.iEnvironmentFloorType);
  if (dlg.exec()) {
    for (int i = iFrom; i <= iTo; ++i) {
      g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
      chunk.iEnvironmentFloorType = dlg.GetValue();
      g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
    }
  }
  g_pMainWindow->SaveHistory("Changed environment floor surface");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::CGripChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iTrackGrip = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }
  g_pMainWindow->SaveHistory("Changed center grip");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::LGripChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iLeftShoulderGrip = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }
  g_pMainWindow->SaveHistory("Changed left shoulder grip");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::RGripChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iRightShoulderGrip = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }
  g_pMainWindow->SaveHistory("Changed right shoulder grip");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AISpeedChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iAIMaxSpeed = iValue * 10;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }
  g_pMainWindow->SaveHistory("Changed ai speed");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditGeometryWidget::AIAccuracyChanged(int iValue)
{
  int iFrom = g_pMainWindow->GetSelFrom();
  int iTo = g_pMainWindow->GetSelTo();

  if (!g_pMainWindow->GetCurrentTrack()
      || iFrom >= g_pMainWindow->GetCurrentTrack()->GetChunkCount()
      || iTo >= g_pMainWindow->GetCurrentTrack()->GetChunkCount())
    return;

  for (int i = iFrom; i <= iTo; ++i) {
    tGeometryChunk chunk;
    g_pMainWindow->GetCurrentTrack()->GetChunk(i, chunk);
    chunk.iAIAccuracy = iValue;
    g_pMainWindow->GetCurrentTrack()->SetChunk(i, chunk);
  }
  g_pMainWindow->SaveHistory("Changed ai accuracy");
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------