#include "EditDataWidget.h"
#include "Track.h"
#include "MainWindow.h"
#include "ChunkEditValues.h"
#include "EditSurfaceDialog.h"
#include "TilePicker.h"
#include "Texture.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
class CEditDataWidgetPrivate
{
public:
  CEditDataWidgetPrivate()
    : m_pTrack(NULL)
    , m_pTex(NULL)
    , m_pBld(NULL)
  {
  };
  ~CEditDataWidgetPrivate()
  {
  };

  CTrack *m_pTrack;
  CTexture *m_pTex;
  CTexture *m_pBld;

  //selected geometry values
  CChunkEditValues editVals;

  //header values
  QString sFloorDepth;

  //selected texture values
  QString sTex;
  QString sBld;
  QString sBackVal;

  //selected info values
  QString sTrackNumber;
  QString sImpossibleLaps;
  QString sHardLaps;
  QString sTrickyLaps;
  QString sMediumLaps;
  QString sEasyLaps;
  QString sGirlieLaps;
  QString sTrackMapSize;
  QString sTrackMapFidelity;
  QString sInfoUnknown;

  QString signAy[17] = { "TOWER"
                       , "TOWER 2"
                       , "SIGN 1"
                       , "SIGN 2"
                       , "BUILD"
                       , "BUILD 1"
                       , "BUILD 2"
                       , "BUILD 3"
                       , "HEELBAR"
                       , "BALLOON"
                       , "TREE"
                       , "ADVERT"
                       , "ADVERT 2"
                       , "QUAD BLD"
                       , "BLD 0"
                       , "BIG BALL"
                       , "BIG AD" };
  size_t signAySize = sizeof(signAy) / sizeof(signAy[0]);
};
//-------------------------------------------------------------------------------------------------

CEditDataWidget::CEditDataWidget(QWidget *pParent, CTrack *pTrack, CTexture *pTex, CTexture *pBld)
  : QWidget(pParent)
{
  p = new CEditDataWidgetPrivate;
  p->m_pTrack = pTrack;
  p->m_pTex = pTex;
  p->m_pBld = pBld;
  setupUi(this);

  cbSignType->addItem("<none>", -1);
  for (int i = 0; i < p->signAySize; ++i) {
    cbSignType->addItem(p->signAy[i], i);
  }


  connect(g_pMainWindow, &CMainWindow::ResetSelectionSig, this, &CEditDataWidget::OnResetSelection);
  connect(g_pMainWindow, &CMainWindow::UpdateWindowSig, this, &CEditDataWidget::OnUpdateWindow);

  connect(pbInsertBefore, &QPushButton::clicked, this, &CEditDataWidget::OnInsertBeforeClicked);
  connect(pbInsertAfter, &QPushButton::clicked, this, &CEditDataWidget::OnInsertAfterClicked);
  connect(sbSelChunksFrom, SIGNAL(valueChanged(int)), this, SLOT(OnSelChunksFromChanged(int)));
  connect(sbSelChunksTo, SIGNAL(valueChanged(int)), this, SLOT(OnSelChunksToChanged(int)));
  connect(ckTo, &QCheckBox::toggled, this, &CEditDataWidget::OnToChecked);
  connect(pbApply, &QPushButton::clicked, this, &CEditDataWidget::OnApplyClicked);
  connect(pbApplyInfo, &QPushButton::clicked, this, &CEditDataWidget::OnApplyInfoClicked);
  connect(pbCancel, &QPushButton::clicked, this, &CEditDataWidget::OnCancelClicked);
  connect(pbRevertInfo, &QPushButton::clicked, this, &CEditDataWidget::OnCancelInfoClicked);
  connect(pbDelete, &QPushButton::clicked, this, &CEditDataWidget::OnDeleteChunkClicked);
  connect(pbEditLSurface, &QPushButton::clicked, this, &CEditDataWidget::OnEditLSurface);
  connect(pbEditCSurface, &QPushButton::clicked, this, &CEditDataWidget::OnEditCSurface);
  connect(pbEditRSurface, &QPushButton::clicked, this, &CEditDataWidget::OnEditRSurface);
  connect(pbEditLWall, &QPushButton::clicked, this, &CEditDataWidget::OnEditLWall);
  connect(pbEditRWall, &QPushButton::clicked, this, &CEditDataWidget::OnEditRWall);
  connect(pbEditRoof, &QPushButton::clicked, this, &CEditDataWidget::OnEditRoof);
  connect(pbEditLUOuterWall, &QPushButton::clicked, this, &CEditDataWidget::OnEditLUOuterWall);
  connect(pbEditLLOuterWall, &QPushButton::clicked, this, &CEditDataWidget::OnEditLLOuterWall);
  connect(pbEditOuterFloor, &QPushButton::clicked, this, &CEditDataWidget::OnEditOuterFloor);
  connect(pbEditRLOuterWall, &QPushButton::clicked, this, &CEditDataWidget::OnEditRLOuterWall);
  connect(pbEditRUOuterWall, &QPushButton::clicked, this, &CEditDataWidget::OnEditRUOuterWall);
  connect(pbEditEnvirFloor, &QPushButton::clicked, this, &CEditDataWidget::OnEditEnvirFloor);

  connect(leLShoulderWidth, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leLLaneWidth, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leRLaneWidth, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leRShoulderWidth, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leLShoulderHeight, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leRShoulderHeight, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leLength, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leYaw, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(lePitch, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leRoll , &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leAILine1, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leAILine2, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leAILine3, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leAILine4, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leTrackGrip, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leLeftShoulderGrip, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leRShoulderGrip, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk04, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk05, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leAudioAboveTrigger, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leAudioTriggerSpeed, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leAudioBelowTrigger, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leLeftSurfaceType, &QLineEdit::textChanged, this, &CEditDataWidget::OnLSurfaceLEChanged);
  connect(leCenterSurfaceType, &QLineEdit::textChanged, this, &CEditDataWidget::OnCSurfaceLEChanged);
  connect(leRightSurfaceType, &QLineEdit::textChanged, this, &CEditDataWidget::OnRSurfaceLEChanged);
  connect(leLWallType, &QLineEdit::textChanged, this, &CEditDataWidget::OnLWallLEChanged);
  connect(leRWallType, &QLineEdit::textChanged, this, &CEditDataWidget::OnRWallLEChanged);
  connect(leRoofType, &QLineEdit::textChanged, this, &CEditDataWidget::OnRoofLEChanged);
  connect(leLUOuterWallType, &QLineEdit::textChanged, this, &CEditDataWidget::OnLUOuterWallChanged);
  connect(leLLOuterWallType, &QLineEdit::textChanged, this, &CEditDataWidget::OnLLOuterWallChanged);
  connect(leOuterFloorType, &QLineEdit::textChanged, this, &CEditDataWidget::OnOuterFloorChanged);
  connect(leRLOuterWallType, &QLineEdit::textChanged, this, &CEditDataWidget::OnRLOuterWallChanged);
  connect(leRUOuterWallType, &QLineEdit::textChanged, this, &CEditDataWidget::OnRUOuterWallChanged);
  connect(leEnvironmentFloorType, &QLineEdit::textChanged, this, &CEditDataWidget::OnEnvirFloorChanged);
  connect(leSignType, &QLineEdit::textChanged, this, &CEditDataWidget::OnSignTypeLEChanged);
  connect(cbSignType, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSignTypeChanged(int)));
  connect(leSignHorizOffset, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leSignVertOffset, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leSignYaw, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leSignPitch, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leSignRoll, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leLUOuterWallHOffset, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leLLOuterWallHOffset, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leLOuterFloorHOffset, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leROuterFloorHOffset, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leRLOuterWallHOffset, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leRUOuterWallHOffset, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leLUOuterWallHeight, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leLLOuterWallHeight, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leLOuterFloorHeight, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leROuterFloorHeight, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leRLOuterWallHeight, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leRUOuterWallHeight, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leRoofHeight, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leDrawOrder1, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leDrawOrder2, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leDrawOrder3, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk37, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk38, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk39, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk40, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk41, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk42, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk43, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk44, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk45, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk46, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk47, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk48, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk49, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leUnk50, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leSign, &QLineEdit::textChanged, this, &CEditDataWidget::OnSignLEChanged);
  connect(leBack, &QLineEdit::textChanged, this, &CEditDataWidget::OnBackLEChanged);
  connect(pbSign, &QPushButton::clicked, this, &CEditDataWidget::OnSignClicked);
  connect(pbBack, &QPushButton::clicked, this, &CEditDataWidget::OnBackClicked);
  connect(ckApplySign, &QCheckBox::toggled, this, &CEditDataWidget::OnApplySignToggled);
  connect(ckApplyBack, &QCheckBox::toggled, this, &CEditDataWidget::OnApplyBackToggled);

  connect(leStuntScaleFact, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leStuntAngle, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leStuntUnk, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leStuntTimingGroup, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leStuntHeight, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leStuntTimeBulging, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leStuntTimeFlat, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leStuntExpandContract, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(leStuntBulge, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
  connect(pbDeleteStunt, &QPushButton::clicked, this, &CEditDataWidget::OnDeleteStuntClicked);

  connect(leFloorDepth, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateInfoEditMode);
  connect(leTex, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateInfoEditMode);
  connect(leBld, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateInfoEditMode);
  connect(leTrackNum, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateInfoEditMode);
  connect(leImpossibleLaps, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateInfoEditMode);
  connect(leHardLaps, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateInfoEditMode);
  connect(leTrickyLaps, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateInfoEditMode);
  connect(leMediumLaps, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateInfoEditMode);
  connect(leEasyLaps, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateInfoEditMode);
  connect(leGirlieLaps, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateInfoEditMode);
  connect(leMapSize, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateInfoEditMode);
  connect(leMapFidelity, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateInfoEditMode);
  connect(leInfoUnknown, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateInfoEditMode);
}

//-------------------------------------------------------------------------------------------------

CEditDataWidget::~CEditDataWidget()
{
  if (p) {
    delete p;
    p = NULL;
  }
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnResetSelection()
{
  sbSelChunksFrom->setValue(0);
  sbSelChunksTo->setValue(0);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnUpdateWindow()
{
  if (!p->m_pTrack)
    return;

  leChunkCount->setText(QString::number(p->m_pTrack->m_chunkAy.size()));
  sbSelChunksFrom->blockSignals(true);
  sbSelChunksTo->blockSignals(true);
  sbInsert->setRange(1, 65535 - (int)p->m_pTrack->m_chunkAy.size());
  sbSelChunksFrom->setRange(0, (int)p->m_pTrack->m_chunkAy.size() - 1);
  sbSelChunksTo->setRange(0, (int)p->m_pTrack->m_chunkAy.size() - 1);
  sbSelChunksFrom->blockSignals(false);
  sbSelChunksTo->blockSignals(false);
  UpdateGeometrySelection();
  UpdateGeometryEditMode();
  UpdateInfoSelection();
  UpdateInfoEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnInsertBeforeClicked()
{
  if (!p->m_pTrack)
    return;

  CChunkEditValues editVals(leLShoulderWidth->text(), leLLaneWidth->text(), leRLaneWidth->text(), leRShoulderWidth->text()
    , leLShoulderHeight->text(), leRShoulderHeight->text(), leLength->text()
    , leYaw->text(), lePitch->text(), leRoll->text()
    , leAILine1->text(), leAILine2->text(), leAILine3->text(), leAILine4->text()
    , leTrackGrip->text(), leLeftShoulderGrip->text(), leRShoulderGrip->text()
    , leUnk04->text(), leUnk05->text(), leAudioAboveTrigger->text(), leAudioTriggerSpeed->text(), leAudioBelowTrigger->text()
    , leLeftSurfaceType->text(), leCenterSurfaceType->text(), leRightSurfaceType->text()
    , leLWallType->text(), leRWallType->text(), leRoofType->text(), leLUOuterWallType->text(), leLLOuterWallType->text(), leOuterFloorType->text()
    , leRLOuterWallType->text(), leRUOuterWallType->text(), leEnvironmentFloorType->text(), leSignType->text(), leSignHorizOffset->text(), leSignVertOffset->text()
    , leSignYaw->text(), leSignPitch->text(), leSignRoll->text()
    , leLUOuterWallHOffset->text(), leLLOuterWallHOffset->text(), leLOuterFloorHOffset->text(), leROuterFloorHOffset->text(), leRLOuterWallHOffset->text(), leRUOuterWallHOffset->text()
    , leLUOuterWallHeight->text(), leLLOuterWallHeight->text(), leLOuterFloorHeight->text(), leROuterFloorHeight->text(), leRLOuterWallHeight->text(), leRUOuterWallHeight->text()
    , leRoofHeight->text(), leDrawOrder1->text(), leDrawOrder2->text(), leDrawOrder3->text(), leUnk37->text(), leUnk38->text()
    , leUnk39->text(), leUnk40->text(), leUnk41->text(), leUnk42->text(), leUnk43->text(), leUnk44->text()
    , leUnk45->text(), leUnk46->text(), leUnk47->text(), leUnk48->text(), leUnk49->text(), leUnk50->text()
    , pbSign->property("value").toString(), pbBack->property("value").toString()
    , leStuntScaleFact->text(), leStuntAngle->text(), leStuntUnk->text(), leStuntTimingGroup->text(), leStuntHeight->text(), leStuntTimeBulging->text()
    , leStuntTimeFlat->text(), leStuntExpandContract->text(), leStuntBulge->text());

  p->m_pTrack->InsertGeometryChunk(sbSelChunksFrom->value(), sbInsert->value(), editVals);

  g_pMainWindow->SetUnsavedChanges(true);
  sbSelChunksFrom->blockSignals(true);
  sbSelChunksTo->blockSignals(true);
  ckTo->blockSignals(true);
  sbSelChunksFrom->setRange(0, (int)p->m_pTrack->m_chunkAy.size() - 1);
  sbSelChunksTo->setRange(0, (int)p->m_pTrack->m_chunkAy.size() - 1);
  sbSelChunksTo->setValue(sbSelChunksFrom->value() + sbInsert->value() - 1);
  ckTo->setChecked(sbInsert->value() > 1);
  sbSelChunksFrom->blockSignals(false);
  sbSelChunksTo->blockSignals(false);
  ckTo->blockSignals(false);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnInsertAfterClicked()
{
  if (!p->m_pTrack)
    return;

  CChunkEditValues editVals(leLShoulderWidth->text(), leLLaneWidth->text(), leRLaneWidth->text(), leRShoulderWidth->text()
    , leLShoulderHeight->text(), leRShoulderHeight->text(), leLength->text()
    , leYaw->text(), lePitch->text(), leRoll->text()
    , leAILine1->text(), leAILine2->text(), leAILine3->text(), leAILine4->text()
    , leTrackGrip->text(), leLeftShoulderGrip->text(), leRShoulderGrip->text()
    , leUnk04->text(), leUnk05->text(), leAudioAboveTrigger->text(), leAudioTriggerSpeed->text(), leAudioBelowTrigger->text()
    , leLeftSurfaceType->text(), leCenterSurfaceType->text(), leRightSurfaceType->text()
    , leLWallType->text(), leRWallType->text(), leRoofType->text(), leLUOuterWallType->text(), leLLOuterWallType->text(), leOuterFloorType->text()
    , leRLOuterWallType->text(), leRUOuterWallType->text(), leEnvironmentFloorType->text(), leSignType->text(), leSignHorizOffset->text(), leSignVertOffset->text()
    , leSignYaw->text(), leSignPitch->text(), leSignRoll->text()
    , leLUOuterWallHOffset->text(), leLLOuterWallHOffset->text(), leLOuterFloorHOffset->text(), leROuterFloorHOffset->text(), leRLOuterWallHOffset->text(), leRUOuterWallHOffset->text()
    , leLUOuterWallHeight->text(), leLLOuterWallHeight->text(), leLOuterFloorHeight->text(), leROuterFloorHeight->text(), leRLOuterWallHeight->text(), leRUOuterWallHeight->text()
    , leRoofHeight->text(), leDrawOrder1->text(), leDrawOrder2->text(), leDrawOrder3->text(), leUnk37->text(), leUnk38->text()
    , leUnk39->text(), leUnk40->text(), leUnk41->text(), leUnk42->text(), leUnk43->text(), leUnk44->text()
    , leUnk45->text(), leUnk46->text(), leUnk47->text(), leUnk48->text(), leUnk49->text(), leUnk50->text()
    , leSign->text(), leBack->text()
    , leStuntScaleFact->text(), leStuntAngle->text(), leStuntUnk->text(), leStuntTimingGroup->text(), leStuntHeight->text(), leStuntTimeBulging->text()
    , leStuntTimeFlat->text(), leStuntExpandContract->text(), leStuntBulge->text());

  p->m_pTrack->InsertGeometryChunk(sbSelChunksTo->value() + 1, sbInsert->value(), editVals);

  g_pMainWindow->SetUnsavedChanges(true);
  sbSelChunksFrom->blockSignals(true);
  sbSelChunksTo->blockSignals(true);
  ckTo->blockSignals(true);
  sbSelChunksFrom->setRange(0, (int)p->m_pTrack->m_chunkAy.size() - 1);
  sbSelChunksTo->setRange(0, (int)p->m_pTrack->m_chunkAy.size() - 1);
  sbSelChunksFrom->setValue(sbSelChunksTo->value() + 1);
  sbSelChunksTo->setValue(sbSelChunksFrom->value() + sbInsert->value() - 1);
  ckTo->setChecked(sbInsert->value() > 1);
  sbSelChunksFrom->blockSignals(false);
  sbSelChunksTo->blockSignals(false);
  ckTo->blockSignals(false);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnSelChunksFromChanged(int iValue)
{
  if (!ckTo->isChecked() || sbSelChunksTo->value() < iValue) {
    sbSelChunksTo->blockSignals(true);
    sbSelChunksTo->setValue(iValue);
    sbSelChunksTo->blockSignals(false);
  }
  UpdateGeometrySelection();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnSelChunksToChanged(int iValue)
{
  if (sbSelChunksFrom->value() > iValue) {
    sbSelChunksFrom->blockSignals(true);
    sbSelChunksFrom->setValue(iValue);
    sbSelChunksFrom->blockSignals(false);
  }
  UpdateGeometrySelection();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnToChecked(bool bChecked)
{
  sbSelChunksTo->setEnabled(bChecked && !pbApply->isEnabled());
  if (!bChecked) {
    sbSelChunksTo->setValue(sbSelChunksFrom->value());
  }
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnApplyClicked()
{
  if (!p->m_pTrack)
    return;

  CChunkEditValues editVals(leLShoulderWidth->text(), leLLaneWidth->text(), leRLaneWidth->text(), leRShoulderWidth->text()
    , leLShoulderHeight->text(), leRShoulderHeight->text(), leLength->text()
    , leYaw->text(), lePitch->text(), leRoll->text()
    , leAILine1->text(), leAILine2->text(), leAILine3->text(), leAILine4->text()
    , leTrackGrip->text(), leLeftShoulderGrip->text(), leRShoulderGrip->text()
    , leUnk04->text(), leUnk05->text(), leAudioAboveTrigger->text(), leAudioTriggerSpeed->text(), leAudioBelowTrigger->text()
    , leLeftSurfaceType->text(), leCenterSurfaceType->text(), leRightSurfaceType->text()
    , leLWallType->text(), leRWallType->text(), leRoofType->text(), leLUOuterWallType->text(), leLLOuterWallType->text(), leOuterFloorType->text()
    , leRLOuterWallType->text(), leRUOuterWallType->text(), leEnvironmentFloorType->text(), leSignType->text(), leSignHorizOffset->text(), leSignVertOffset->text()
    , leSignYaw->text(), leSignPitch->text(), leSignRoll->text()
    , leLUOuterWallHOffset->text(), leLLOuterWallHOffset->text(), leLOuterFloorHOffset->text(), leROuterFloorHOffset->text(), leRLOuterWallHOffset->text(), leRUOuterWallHOffset->text()
    , leLUOuterWallHeight->text(), leLLOuterWallHeight->text(), leLOuterFloorHeight->text(), leROuterFloorHeight->text(), leRLOuterWallHeight->text(), leRUOuterWallHeight->text()
    , leRoofHeight->text(), leDrawOrder1->text(), leDrawOrder2->text(), leDrawOrder3->text(), leUnk37->text(), leUnk38->text()
    , leUnk39->text(), leUnk40->text(), leUnk41->text(), leUnk42->text(), leUnk43->text(), leUnk44->text()
    , leUnk45->text(), leUnk46->text(), leUnk47->text(), leUnk48->text(), leUnk49->text(), leUnk50->text()
    , leSign->text(), leBack->text()
    , leStuntScaleFact->text(), leStuntAngle->text(), leStuntUnk->text(), leStuntTimingGroup->text(), leStuntHeight->text(), leStuntTimeBulging->text()
    , leStuntTimeFlat->text(), leStuntExpandContract->text(), leStuntBulge->text());

  p->m_pTrack->ApplyGeometrySettings(sbSelChunksFrom->value(), sbSelChunksTo->value(), editVals);
  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->UpdateWindow();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnApplyInfoClicked()
{
  if (!p->m_pTrack)
    return;

  p->m_pTrack->m_raceInfo.iTrackNumber = leTrackNum->text().toInt();
  p->m_pTrack->m_raceInfo.iImpossibleLaps = leImpossibleLaps->text().toInt();
  p->m_pTrack->m_raceInfo.iHardLaps = leHardLaps->text().toInt();
  p->m_pTrack->m_raceInfo.iTrickyLaps = leTrickyLaps->text().toInt();
  p->m_pTrack->m_raceInfo.iMediumLaps = leMediumLaps->text().toInt();
  p->m_pTrack->m_raceInfo.iEasyLaps = leEasyLaps->text().toInt();
  p->m_pTrack->m_raceInfo.iGirlieLaps = leGirlieLaps->text().toInt();
  p->m_pTrack->m_raceInfo.dTrackMapSize = leMapSize->text().toDouble();
  p->m_pTrack->m_raceInfo.iTrackMapFidelity = leMapFidelity->text().toInt();
  p->m_pTrack->m_raceInfo.dUnknown = leInfoUnknown->text().toDouble();
  p->m_pTrack->m_sTextureFile = leTex->text().toLatin1().constData();
  p->m_pTrack->m_sBuildingFile = leBld->text().toLatin1().constData();
  p->m_pTrack->m_header.iFloorDepth = leFloorDepth->text().toInt();

  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->LogMessage("Applied global track settings");
  g_pMainWindow->LoadTextures();
  g_pMainWindow->UpdateWindow();
}
//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnCancelClicked()
{
  RevertGeometry();
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnCancelInfoClicked()
{
  RevertInfo();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnDeleteChunkClicked()
{
  if (!p->m_pTrack)
    return;

  if (p->m_pTrack->m_chunkAy.empty()) return;
  if (sbSelChunksFrom->value() > sbSelChunksTo->value() || sbSelChunksTo->value() > p->m_pTrack->m_chunkAy.size()) {
    assert(0);
    return;
  }
  p->m_pTrack->m_chunkAy.erase(
    p->m_pTrack->m_chunkAy.begin() + sbSelChunksFrom->value(),
    p->m_pTrack->m_chunkAy.begin() + sbSelChunksTo->value() + 1);

  g_pMainWindow->SetUnsavedChanges(true);
  g_pMainWindow->LogMessage("Deleted geometry chunk");
  sbSelChunksFrom->blockSignals(true);
  sbSelChunksTo->blockSignals(true);
  g_pMainWindow->UpdateWindow();
  sbSelChunksTo->setValue(sbSelChunksFrom->value());
  sbSelChunksFrom->blockSignals(false);
  sbSelChunksTo->blockSignals(false);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnEditLSurface()
{
  int iValue = leLeftSurfaceType->text().toInt();

  CEditSurfaceDialog dlg(this, p->m_pTex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leLeftSurfaceType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leLeftSurfaceType, lblLSurfaceTex1, lblLSurfaceTex2);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnEditCSurface()
{
  int iValue = leCenterSurfaceType->text().toInt();

  CEditSurfaceDialog dlg(this, p->m_pTex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leCenterSurfaceType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leCenterSurfaceType, lblCSurfaceTex1, lblCSurfaceTex2);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnEditRSurface()
{
  int iValue = leRightSurfaceType->text().toInt();

  CEditSurfaceDialog dlg(this, p->m_pTex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leRightSurfaceType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leRightSurfaceType, lblRSurfaceTex1, lblRSurfaceTex2);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnEditLWall()
{
  int iValue = leLWallType->text().toInt();

  CEditSurfaceDialog dlg(this, p->m_pTex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leLWallType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leLWallType, lblLWallTex1, lblLWallTex2);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnEditRWall()
{
  int iValue = leRWallType->text().toInt();

  CEditSurfaceDialog dlg(this, p->m_pTex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leRWallType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leRWallType, lblRWallTex1, lblRWallTex2);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnEditRoof()
{
  int iValue = leRoofType->text().toInt();

  CEditSurfaceDialog dlg(this, p->m_pTex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leRoofType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leRoofType, lblRoofTex1, lblRoofTex2);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnEditLUOuterWall()
{
  int iValue = leLUOuterWallType->text().toInt();

  CEditSurfaceDialog dlg(this, p->m_pTex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leLUOuterWallType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leLUOuterWallType, lblLUOuterWallTex1, lblLUOuterWallTex2);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnEditLLOuterWall()
{
  int iValue = leLLOuterWallType->text().toInt();

  CEditSurfaceDialog dlg(this, p->m_pTex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leLLOuterWallType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leLLOuterWallType, lblLLOuterWallTex1, lblLLOuterWallTex2);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnEditOuterFloor()
{
  int iValue = leOuterFloorType->text().toInt();

  CEditSurfaceDialog dlg(this, p->m_pTex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leOuterFloorType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leOuterFloorType, lblOuterFloorTex1, lblOuterFloorTex2);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnEditRLOuterWall()
{
  int iValue = leRLOuterWallType->text().toInt();

  CEditSurfaceDialog dlg(this, p->m_pTex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leRLOuterWallType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leRLOuterWallType, lblRLOuterWallTex1, lblRLOuterWallTex2);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnEditRUOuterWall()
{
  int iValue = leRUOuterWallType->text().toInt();

  CEditSurfaceDialog dlg(this, p->m_pTex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leRUOuterWallType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leRUOuterWallType, lblRUOuterWallTex1, lblRUOuterWallTex2);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnEditEnvirFloor()
{
  int iValue = leEnvironmentFloorType->text().toInt();

  CEditSurfaceDialog dlg(this, p->m_pTex, iValue);
  if (dlg.exec()) {
    iValue = dlg.GetValue();
    leEnvironmentFloorType->setText(QString::number(iValue));
  }

  UpdateGeometryEditMode();
  UpdateTextures(leEnvironmentFloorType, lblEnvirFloorTex1, lblEnvirFloorTex2);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::UpdateGeometryEditMode()
{
  bool bEditMode = false;
  bool bMixedData = false;
  UpdateLEEditMode(bEditMode, bMixedData, leLShoulderWidth, p->editVals.sLeftShoulderWidth);
  UpdateLEEditMode(bEditMode, bMixedData, leLLaneWidth, p->editVals.sLeftLaneWidth);
  UpdateLEEditMode(bEditMode, bMixedData, leRLaneWidth, p->editVals.sRightLaneWidth);
  UpdateLEEditMode(bEditMode, bMixedData, leRShoulderWidth, p->editVals.sRightShoulderWidth);
  UpdateLEEditMode(bEditMode, bMixedData, leLShoulderHeight, p->editVals.sLShoulderHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leRShoulderHeight, p->editVals.sRShoulderHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leLength, p->editVals.sLength);
  UpdateLEEditMode(bEditMode, bMixedData, leYaw, p->editVals.sYaw);
  UpdateLEEditMode(bEditMode, bMixedData, lePitch, p->editVals.sPitch);
  UpdateLEEditMode(bEditMode, bMixedData, leRoll, p->editVals.sRoll);
  UpdateLEEditMode(bEditMode, bMixedData, leAILine1, p->editVals.sAILine1);
  UpdateLEEditMode(bEditMode, bMixedData, leAILine2, p->editVals.sAILine2);
  UpdateLEEditMode(bEditMode, bMixedData, leAILine3, p->editVals.sAILine3);
  UpdateLEEditMode(bEditMode, bMixedData, leAILine4, p->editVals.sAILine4);
  UpdateLEEditMode(bEditMode, bMixedData, leTrackGrip, p->editVals.sTrackGrip);
  UpdateLEEditMode(bEditMode, bMixedData, leLeftShoulderGrip, p->editVals.sLeftShoulderGrip);
  UpdateLEEditMode(bEditMode, bMixedData, leRShoulderGrip, p->editVals.sRightShoulderGrip);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk04, p->editVals.sUnk04);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk05, p->editVals.sUnk05);
  UpdateLEEditMode(bEditMode, bMixedData, leAudioAboveTrigger, p->editVals.sAudioAboveTrigger);
  UpdateLEEditMode(bEditMode, bMixedData, leAudioTriggerSpeed, p->editVals.sAudioTriggerSpeed);
  UpdateLEEditMode(bEditMode, bMixedData, leAudioBelowTrigger, p->editVals.sAudioBelowTrigger);
  UpdateLEEditMode(bEditMode, bMixedData, leLeftSurfaceType, p->editVals.sLeftSurfaceType);
  UpdateLEEditMode(bEditMode, bMixedData, leCenterSurfaceType, p->editVals.sCenterSurfaceType);
  UpdateLEEditMode(bEditMode, bMixedData, leRightSurfaceType, p->editVals.sRightSurfaceType);
  UpdateLEEditMode(bEditMode, bMixedData, leLWallType, p->editVals.sLWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leRWallType, p->editVals.sRWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leRoofType, p->editVals.sRoofType);
  UpdateLEEditMode(bEditMode, bMixedData, leLUOuterWallType, p->editVals.sLUOuterWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leLLOuterWallType, p->editVals.sLLOuterWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leOuterFloorType, p->editVals.sOuterFloorType);
  UpdateLEEditMode(bEditMode, bMixedData, leRLOuterWallType, p->editVals.sRLOuterWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leRUOuterWallType, p->editVals.sRUOuterWallType);
  UpdateLEEditMode(bEditMode, bMixedData, leEnvironmentFloorType, p->editVals.sEnvironmentFloorType);
  UpdateLEEditMode(bEditMode, bMixedData, leSignType, p->editVals.sSignType);
  UpdateCBEditMode(bEditMode, bMixedData, cbSignType, p->editVals.sSignType);
  UpdateLEEditMode(bEditMode, bMixedData, leSignHorizOffset, p->editVals.sSignHorizOffset);
  UpdateLEEditMode(bEditMode, bMixedData, leSignVertOffset, p->editVals.sSignVertOffset);
  UpdateLEEditMode(bEditMode, bMixedData, leSignYaw, p->editVals.sSignYaw);
  UpdateLEEditMode(bEditMode, bMixedData, leSignPitch, p->editVals.sSignPitch);
  UpdateLEEditMode(bEditMode, bMixedData, leSignRoll, p->editVals.sSignRoll);
  UpdateLEEditMode(bEditMode, bMixedData, leLUOuterWallHOffset, p->editVals.sLUOuterWallHOffset);
  UpdateLEEditMode(bEditMode, bMixedData, leLLOuterWallHOffset, p->editVals.sLLOuterWallHOffset);
  UpdateLEEditMode(bEditMode, bMixedData, leLOuterFloorHOffset, p->editVals.sLOuterFloorHOffset);
  UpdateLEEditMode(bEditMode, bMixedData, leROuterFloorHOffset, p->editVals.sROuterFloorHOffset);
  UpdateLEEditMode(bEditMode, bMixedData, leRLOuterWallHOffset, p->editVals.sRLOuterWallHOffset);
  UpdateLEEditMode(bEditMode, bMixedData, leRUOuterWallHOffset, p->editVals.sRUOuterWallHOffset);
  UpdateLEEditMode(bEditMode, bMixedData, leLUOuterWallHeight, p->editVals.sLUOuterWallHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leLLOuterWallHeight, p->editVals.sLLOuterWallHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leLOuterFloorHeight, p->editVals.sLOuterFloorHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leROuterFloorHeight, p->editVals.sROuterFloorHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leRLOuterWallHeight, p->editVals.sRLOuterWallHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leRUOuterWallHeight, p->editVals.sRUOuterWallHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leRoofHeight, p->editVals.sRoofHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leDrawOrder1, p->editVals.sDrawOrder1);
  UpdateLEEditMode(bEditMode, bMixedData, leDrawOrder2, p->editVals.sDrawOrder2);
  UpdateLEEditMode(bEditMode, bMixedData, leDrawOrder3, p->editVals.sDrawOrder3);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk37, p->editVals.sUnk37);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk38, p->editVals.sUnk38);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk39, p->editVals.sUnk39);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk40, p->editVals.sUnk40);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk41, p->editVals.sUnk41);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk42, p->editVals.sUnk42);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk43, p->editVals.sUnk43);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk44, p->editVals.sUnk44);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk45, p->editVals.sUnk45);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk46, p->editVals.sUnk46);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk47, p->editVals.sUnk47);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk48, p->editVals.sUnk48);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk49, p->editVals.sUnk49);
  UpdateLEEditMode(bEditMode, bMixedData, leUnk50, p->editVals.sUnk50);
  UpdateSignEditMode(bEditMode, bMixedData, leSign, widgetSign, p->editVals.sSignTexture);
  UpdateSignEditMode(bEditMode, bMixedData, leBack, widgetBack, p->editVals.sBackTexture);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntScaleFact, p->editVals.sStuntScaleFactor);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntAngle, p->editVals.sStuntAngle);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntUnk, p->editVals.sStuntUnknown);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntTimingGroup, p->editVals.sStuntTimingGroup);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntHeight, p->editVals.sStuntHeight);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntTimeBulging, p->editVals.sStuntTimeBulging);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntTimeFlat, p->editVals.sStuntTimeFlat);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntExpandContract, p->editVals.sStuntExpandsContracts);
  UpdateLEEditMode(bEditMode, bMixedData, leStuntBulge, p->editVals.sStuntBulge);

  pbApply->setEnabled(bEditMode);
  pbCancel->setEnabled(bEditMode);
  pbInsertAfter->setEnabled(!bMixedData);
  pbInsertBefore->setEnabled(!bMixedData);
  sbSelChunksFrom->setEnabled(!bEditMode);
  ckTo->setEnabled(!bEditMode);
  sbSelChunksTo->setEnabled(!bEditMode && ckTo->isChecked());
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::UpdateInfoEditMode()
{
  bool bEditMode = false;
  bool bMixedData = false;
  UpdateLEEditMode(bEditMode, bMixedData, leFloorDepth, p->sFloorDepth);
  UpdateLEEditMode(bEditMode, bMixedData, leTex, p->sTex);
  UpdateLEEditMode(bEditMode, bMixedData, leBld, p->sBld);
  UpdateLEEditMode(bEditMode, bMixedData, leTrackNum, p->sTrackNumber);
  UpdateLEEditMode(bEditMode, bMixedData, leImpossibleLaps, p->sImpossibleLaps);
  UpdateLEEditMode(bEditMode, bMixedData, leHardLaps, p->sHardLaps);
  UpdateLEEditMode(bEditMode, bMixedData, leTrickyLaps, p->sTrickyLaps);
  UpdateLEEditMode(bEditMode, bMixedData, leMediumLaps, p->sMediumLaps);
  UpdateLEEditMode(bEditMode, bMixedData, leEasyLaps, p->sEasyLaps);
  UpdateLEEditMode(bEditMode, bMixedData, leGirlieLaps, p->sGirlieLaps);
  UpdateLEEditMode(bEditMode, bMixedData, leMapFidelity, p->sTrackMapFidelity);
  UpdateLEEditMode(bEditMode, bMixedData, leMapSize, p->sTrackMapSize);
  UpdateLEEditMode(bEditMode, bMixedData, leInfoUnknown, p->sInfoUnknown);

  pbApplyInfo->setEnabled(bEditMode);
  pbRevertInfo->setEnabled(bEditMode);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnSignClicked()
{
  QString sValue = leSign->text();
  int iValue = sValue.toInt();
  unsigned int uiSignedBitVal = CTrack::GetSignedBitValueFromInt(iValue);
  int iBldIndex = uiSignedBitVal & SURFACE_TEXTURE_INDEX;

  CTilePicker dlg(this, p->m_pBld, iBldIndex, true);
  if (dlg.exec()) {
    int iIndex = dlg.GetSelected();
    if (iIndex >= 0) {
      iBldIndex = iIndex;
      uiSignedBitVal = iBldIndex;
      if (ckApplySign->isChecked()) {
        uiSignedBitVal |= SURFACE_FLAG_APPLY_TEXTURE;
      } else {
        uiSignedBitVal &= ~SURFACE_FLAG_APPLY_TEXTURE;
      }
      iValue = CTrack::GetIntValueFromSignedBit(uiSignedBitVal);
      sValue = QString::number(iValue);
    }
  }

  leSign->setText(sValue);
  UpdateSignButtonDisplay(pbSign, ckApplySign, leSign, p->m_pBld);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnBackClicked()
{
  QString sValue = leBack->text();
  int iValue = sValue.toInt();
  unsigned int uiSignedBitVal = CTrack::GetSignedBitValueFromInt(iValue);
  int iBldIndex = uiSignedBitVal & SURFACE_TEXTURE_INDEX;

  CTilePicker dlg(this, p->m_pTex, iBldIndex, true);
  if (dlg.exec()) {
    int iIndex = dlg.GetSelected();
    if (iIndex >= 0) {
      iBldIndex = iIndex;
      uiSignedBitVal = iBldIndex;
      if (ckApplyBack->isChecked()) {
        uiSignedBitVal |= SURFACE_FLAG_APPLY_TEXTURE;
      } else {
        uiSignedBitVal &= ~SURFACE_FLAG_APPLY_TEXTURE;
      }
    }
    iValue = CTrack::GetIntValueFromSignedBit(uiSignedBitVal);
    sValue = QString::number(iValue);
  }

  leBack->setText(sValue);
  UpdateSignButtonDisplay(pbBack, ckApplyBack, leBack, p->m_pTex);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnApplySignToggled(bool bChecked)
{
  int iValue = leSign->text().toInt();
  unsigned int uiSignedBitVal = CTrack::GetSignedBitValueFromInt(iValue);
  if (bChecked) {
    uiSignedBitVal |= SURFACE_FLAG_APPLY_TEXTURE;
  } else {
    uiSignedBitVal &= ~SURFACE_FLAG_APPLY_TEXTURE;
  }
  iValue = CTrack::GetIntValueFromSignedBit(uiSignedBitVal);
  QString sNewValue = QString::number(iValue);
  leSign->setText(sNewValue);
  UpdateSignButtonDisplay(pbSign, ckApplySign, leSign, p->m_pBld);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnApplyBackToggled(bool bChecked)
{
  int iValue = leBack->text().toInt();
  unsigned int uiSignedBitVal = CTrack::GetSignedBitValueFromInt(iValue);
  if (bChecked) {
    uiSignedBitVal |= SURFACE_FLAG_APPLY_TEXTURE;
  } else {
    uiSignedBitVal &= ~SURFACE_FLAG_APPLY_TEXTURE;
  }
  iValue = CTrack::GetIntValueFromSignedBit(uiSignedBitVal);
  QString sNewValue = QString::number(iValue);
  leBack->setText(sNewValue);
  UpdateSignButtonDisplay(pbBack, ckApplyBack, leBack, p->m_pTex);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnDeleteStuntClicked()
{
  leStuntScaleFact->blockSignals(true);
  leStuntAngle->blockSignals(true);
  leStuntUnk->blockSignals(true);
  leStuntTimingGroup->blockSignals(true);
  leStuntHeight->blockSignals(true);
  leStuntTimeBulging->blockSignals(true);
  leStuntTimeFlat->blockSignals(true);
  leStuntExpandContract->blockSignals(true);
  leStuntBulge->blockSignals(true);

  leStuntScaleFact->setText("0");
  leStuntAngle->setText("0");
  leStuntUnk->setText("0");
  leStuntTimingGroup->setText("0");
  leStuntHeight->setText("0");
  leStuntTimeBulging->setText("0");
  leStuntTimeFlat->setText("0");
  leStuntExpandContract->setText("0");
  leStuntBulge->setText("0");

  leStuntScaleFact->blockSignals(false);
  leStuntAngle->blockSignals(false);
  leStuntUnk->blockSignals(false);
  leStuntTimingGroup->blockSignals(false);
  leStuntHeight->blockSignals(false);
  leStuntTimeBulging->blockSignals(false);
  leStuntTimeFlat->blockSignals(false);
  leStuntExpandContract->blockSignals(false);
  leStuntBulge->blockSignals(false);

  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnSignTypeChanged(int iIndex)
{
  int iData = cbSignType->itemData(iIndex).toInt();
  leSignType->blockSignals(true);
  leSignType->setText(QString::number(iData));
  leSignType->blockSignals(false);

  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnSignTypeLEChanged()
{
  cbSignType->blockSignals(true);
  cbSignType->setCurrentIndex(cbSignType->findData(leSignType->text().toInt()));
  cbSignType->blockSignals(false);

  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnSignLEChanged()
{
  UpdateSignButtonDisplay(pbSign, ckApplySign, leSign, p->m_pBld);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnBackLEChanged()
{
  UpdateSignButtonDisplay(pbBack, ckApplyBack, leBack, p->m_pTex);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnLSurfaceLEChanged()
{
  UpdateTextures(leLeftSurfaceType, lblLSurfaceTex1, lblLSurfaceTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnCSurfaceLEChanged()
{
  UpdateTextures(leCenterSurfaceType, lblCSurfaceTex1, lblCSurfaceTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnRSurfaceLEChanged()
{
  UpdateTextures(leRightSurfaceType, lblRSurfaceTex1, lblRSurfaceTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnLWallLEChanged()
{
  UpdateTextures(leLWallType, lblLWallTex1, lblLWallTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnRWallLEChanged()
{
  UpdateTextures(leRWallType, lblRWallTex1, lblRWallTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnRoofLEChanged()
{
  UpdateTextures(leRoofType, lblRoofTex1, lblRoofTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnLUOuterWallChanged()
{
  UpdateTextures(leLUOuterWallType, lblLUOuterWallTex1, lblLUOuterWallTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnLLOuterWallChanged()
{
  UpdateTextures(leLLOuterWallType, lblLLOuterWallTex1, lblLLOuterWallTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnOuterFloorChanged()
{
  UpdateTextures(leOuterFloorType, lblOuterFloorTex1, lblOuterFloorTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnRLOuterWallChanged()
{
  UpdateTextures(leRLOuterWallType, lblRLOuterWallTex1, lblRLOuterWallTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnRUOuterWallChanged()
{
  UpdateTextures(leRUOuterWallType, lblRUOuterWallTex1, lblRUOuterWallTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnEnvirFloorChanged()
{
  UpdateTextures(leEnvironmentFloorType, lblEnvirFloorTex1, lblEnvirFloorTex2);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::UpdateGeometrySelection()
{
  //update view window selection
  g_pMainWindow->UpdateGeometrySelection(sbSelChunksFrom->value(), sbSelChunksTo->value());

  //update values in edit window
  p->m_pTrack->GetGeometryValuesFromSelection(sbSelChunksFrom->value(), sbSelChunksTo->value(), p->editVals);

  RevertGeometry();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::UpdateInfoSelection()
{
  p->sFloorDepth = QString::number(p->m_pTrack->m_header.iFloorDepth);
  p->sTex = p->m_pTrack->m_sTextureFile.c_str();
  p->sBld = p->m_pTrack->m_sBuildingFile.c_str();
  p->sTrackNumber = QString::number(p->m_pTrack->m_raceInfo.iTrackNumber);
  p->sImpossibleLaps = QString::number(p->m_pTrack->m_raceInfo.iImpossibleLaps);
  p->sHardLaps = QString::number(p->m_pTrack->m_raceInfo.iHardLaps);
  p->sTrickyLaps = QString::number(p->m_pTrack->m_raceInfo.iTrickyLaps);
  p->sMediumLaps = QString::number(p->m_pTrack->m_raceInfo.iMediumLaps);
  p->sEasyLaps = QString::number(p->m_pTrack->m_raceInfo.iEasyLaps);
  p->sGirlieLaps = QString::number(p->m_pTrack->m_raceInfo.iGirlieLaps);
  p->sTrackMapSize = QString::number(p->m_pTrack->m_raceInfo.dTrackMapSize, 'f', 2);
  p->sTrackMapFidelity = QString::number(p->m_pTrack->m_raceInfo.iTrackMapFidelity);
  p->sInfoUnknown = QString::number(p->m_pTrack->m_raceInfo.dUnknown, 'f', 2);

  RevertInfo();
}

//-------------------------------------------------------------------------------------------------

bool CEditDataWidget::UpdateLEWithSelectionValue(QLineEdit *pLineEdit, const QString &sValue)
{
  pLineEdit->blockSignals(true);
  if (sValue.compare(MIXED_DATA) == 0) {
    pLineEdit->setText("");
    pLineEdit->setPlaceholderText(sValue);
    //pLineEdit->setStyleSheet("background-color: rgb(255,0,0)");
  } else {
    pLineEdit->setPlaceholderText("");
    pLineEdit->setText(sValue);
    pLineEdit->setStyleSheet("");
  }
  pLineEdit->blockSignals(false);
  return (sValue.compare(MIXED_DATA) == 0) && pLineEdit->text().isEmpty();
}

//-------------------------------------------------------------------------------------------------

bool CEditDataWidget::UpdateCBWithSelectionValue(QComboBox *pComboBox, const QString &sValue)
{
  std::string sVal = sValue.toLatin1().constData();
  pComboBox->blockSignals(true);
  if (sValue.compare(MIXED_DATA) == 0) {
    if (pComboBox->findData(MIXED_DATA) == -1)
      pComboBox->addItem(MIXED_DATA, MIXED_DATA);
    pComboBox->setCurrentIndex(pComboBox->findData(MIXED_DATA));
  } else {
    pComboBox->removeItem(pComboBox->findData(MIXED_DATA));
    pComboBox->setCurrentIndex(pComboBox->findData(sValue.toInt()));
  }
  pComboBox->blockSignals(false);
  return (sValue.compare(MIXED_DATA) == 0);
}

//-------------------------------------------------------------------------------------------------

bool CEditDataWidget::UpdateSignWithSelectionValue(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, const QString &sValue, CTexture *pTex)
{
  pLineEdit->blockSignals(true);
  if (sValue.compare(MIXED_DATA) == 0) {
    pLineEdit->setText("");
    pLineEdit->setPlaceholderText(sValue);
    //pLineEdit->setStyleSheet("background-color: rgb(255,0,0)");
  } else {
    pLineEdit->setPlaceholderText("");
    pLineEdit->setText(sValue);
    pLineEdit->setStyleSheet("");
  }
  pLineEdit->blockSignals(false);
  UpdateSignButtonDisplay(pPushButton, pCheckbox, pLineEdit, pTex);
  return (sValue.compare(MIXED_DATA) == 0) && pLineEdit->text().isEmpty();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::UpdateSignButtonDisplay(QPushButton *pPushButton, QCheckBox *pCheckbox, QLineEdit *pLineEdit, CTexture *pTex)
{
  pCheckbox->blockSignals(true);
  if (pLineEdit->text().compare("-1") == 0) {
    pPushButton->setIcon(QIcon());
    pPushButton->setText("<none>");
    pCheckbox->setEnabled(false);
    pCheckbox->setChecked(false);
  } else if (pLineEdit->text().isEmpty() && pLineEdit->placeholderText().compare(MIXED_DATA) == 0) {
    pPushButton->setIcon(QIcon());
    pPushButton->setText(MIXED_DATA);
    pCheckbox->setEnabled(false);
    pCheckbox->setChecked(false);
  } else {
    int iValue = pLineEdit->text().toInt();
    unsigned int uiSignedBitVal = CTrack::GetSignedBitValueFromInt(iValue);
    bool bChecked = uiSignedBitVal & SURFACE_FLAG_APPLY_TEXTURE;
    int iBldIndex = CTrack::GetIntValueFromSignedBit(uiSignedBitVal & SURFACE_TEXTURE_INDEX);
    if (iBldIndex < pTex->m_tileAy.size()) {
      QPixmap pixmap;
      pixmap.convertFromImage(pTex->m_tileAy[iBldIndex]);
      pPushButton->setIcon(pixmap);
      pPushButton->setText("");
    }
    pCheckbox->setEnabled(true);
    pCheckbox->setChecked(bChecked);
  }
  pCheckbox->blockSignals(false);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::UpdateLEEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, const QString &sValue)
{
  QString sLineEditVal = pLineEdit->text();
  if (sLineEditVal.isEmpty())
    sLineEditVal = "0";

  if (sLineEditVal.compare(sValue) != 0) {
    if (pLineEdit->text().isEmpty() && pLineEdit->placeholderText().compare(MIXED_DATA) == 0) {
      bMixedData = true;
      pLineEdit->setStyleSheet("");
    } else {
      bEdited = true;
      pLineEdit->setStyleSheet("background-color: rgb(255,255,0)");
    }
  } else {
    pLineEdit->setStyleSheet("");
  }
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::UpdateCBEditMode(bool &bEdited, bool &bMixedData, QComboBox *pComboBox, const QString &sValue)
{
  QString sCbxVal = pComboBox->currentData().toString();
  if (sCbxVal.compare(sValue) != 0 && pComboBox->currentIndex() != -1) {
    if (sCbxVal.compare(MIXED_DATA) == 0) {
      bMixedData = true;
      pComboBox->setStyleSheet("");
    } else {
      bEdited = true;
      pComboBox->setStyleSheet("background-color: rgb(255,255,0)");
    }
  } else {
    pComboBox->setStyleSheet("");
  }
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::UpdateSignEditMode(bool &bEdited, bool &bMixedData, QLineEdit *pLineEdit, QWidget *pWidget, const QString &sValue)
{
  QString sLineEditVal = pLineEdit->text();
  if (sLineEditVal.isEmpty())
    sLineEditVal = "0";

  if (sLineEditVal.compare(sValue) != 0) {
    if (pLineEdit->text().isEmpty() && pLineEdit->placeholderText().compare(MIXED_DATA) == 0) {
      bMixedData = true;
      pWidget->setStyleSheet("");
    } else {
      bEdited = true;
      pWidget->setStyleSheet("background-color: rgb(255,255,0)");
    }
  } else {
    pWidget->setStyleSheet("");
  }
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::RevertGeometry()
{
  pbDelete->setEnabled(!p->m_pTrack->m_chunkAy.empty());

  bool bMixedData = false;
  bMixedData |= UpdateLEWithSelectionValue(leLShoulderWidth, p->editVals.sLeftShoulderWidth);
  bMixedData |= UpdateLEWithSelectionValue(leLLaneWidth, p->editVals.sLeftLaneWidth);
  bMixedData |= UpdateLEWithSelectionValue(leRLaneWidth, p->editVals.sRightLaneWidth);
  bMixedData |= UpdateLEWithSelectionValue(leRShoulderWidth, p->editVals.sRightShoulderWidth);
  bMixedData |= UpdateLEWithSelectionValue(leLShoulderHeight, p->editVals.sLShoulderHeight);
  bMixedData |= UpdateLEWithSelectionValue(leRShoulderHeight, p->editVals.sRShoulderHeight);
  bMixedData |= UpdateLEWithSelectionValue(leLength, p->editVals.sLength);
  bMixedData |= UpdateLEWithSelectionValue(leYaw, p->editVals.sYaw);
  bMixedData |= UpdateLEWithSelectionValue(lePitch, p->editVals.sPitch);
  bMixedData |= UpdateLEWithSelectionValue(leRoll, p->editVals.sRoll);
  bMixedData |= UpdateLEWithSelectionValue(leAILine1, p->editVals.sAILine1);
  bMixedData |= UpdateLEWithSelectionValue(leAILine2, p->editVals.sAILine2);
  bMixedData |= UpdateLEWithSelectionValue(leAILine3, p->editVals.sAILine3);
  bMixedData |= UpdateLEWithSelectionValue(leAILine4, p->editVals.sAILine4);
  bMixedData |= UpdateLEWithSelectionValue(leTrackGrip, p->editVals.sTrackGrip);
  bMixedData |= UpdateLEWithSelectionValue(leLeftShoulderGrip, p->editVals.sLeftShoulderGrip);
  bMixedData |= UpdateLEWithSelectionValue(leRShoulderGrip, p->editVals.sRightShoulderGrip);
  bMixedData |= UpdateLEWithSelectionValue(leUnk04, p->editVals.sUnk04);
  bMixedData |= UpdateLEWithSelectionValue(leUnk05, p->editVals.sUnk05);
  bMixedData |= UpdateLEWithSelectionValue(leAudioAboveTrigger, p->editVals.sAudioAboveTrigger);
  bMixedData |= UpdateLEWithSelectionValue(leAudioTriggerSpeed, p->editVals.sAudioTriggerSpeed);
  bMixedData |= UpdateLEWithSelectionValue(leAudioBelowTrigger, p->editVals.sAudioBelowTrigger);
  bMixedData |= UpdateLEWithSelectionValue(leLeftSurfaceType, p->editVals.sLeftSurfaceType);
  bMixedData |= UpdateLEWithSelectionValue(leCenterSurfaceType, p->editVals.sCenterSurfaceType);
  bMixedData |= UpdateLEWithSelectionValue(leRightSurfaceType, p->editVals.sRightSurfaceType);
  bMixedData |= UpdateLEWithSelectionValue(leLWallType, p->editVals.sLWallType);
  bMixedData |= UpdateLEWithSelectionValue(leRWallType, p->editVals.sRWallType);
  bMixedData |= UpdateLEWithSelectionValue(leRoofType, p->editVals.sRoofType);
  bMixedData |= UpdateLEWithSelectionValue(leLUOuterWallType, p->editVals.sLUOuterWallType);
  bMixedData |= UpdateLEWithSelectionValue(leLLOuterWallType, p->editVals.sLLOuterWallType);
  bMixedData |= UpdateLEWithSelectionValue(leOuterFloorType, p->editVals.sOuterFloorType);
  bMixedData |= UpdateLEWithSelectionValue(leRLOuterWallType, p->editVals.sRLOuterWallType);
  bMixedData |= UpdateLEWithSelectionValue(leRUOuterWallType, p->editVals.sRUOuterWallType);
  bMixedData |= UpdateLEWithSelectionValue(leEnvironmentFloorType, p->editVals.sEnvironmentFloorType);
  bMixedData |= UpdateLEWithSelectionValue(leSignType, p->editVals.sSignType);
  bMixedData |= UpdateCBWithSelectionValue(cbSignType, p->editVals.sSignType);
  bMixedData |= UpdateLEWithSelectionValue(leSignHorizOffset, p->editVals.sSignHorizOffset);
  bMixedData |= UpdateLEWithSelectionValue(leSignVertOffset, p->editVals.sSignVertOffset);
  bMixedData |= UpdateLEWithSelectionValue(leSignYaw, p->editVals.sSignYaw);
  bMixedData |= UpdateLEWithSelectionValue(leSignPitch, p->editVals.sSignPitch);
  bMixedData |= UpdateLEWithSelectionValue(leSignRoll, p->editVals.sSignRoll);
  bMixedData |= UpdateLEWithSelectionValue(leLUOuterWallHOffset, p->editVals.sLUOuterWallHOffset);
  bMixedData |= UpdateLEWithSelectionValue(leLLOuterWallHOffset, p->editVals.sLLOuterWallHOffset);
  bMixedData |= UpdateLEWithSelectionValue(leLOuterFloorHOffset, p->editVals.sLOuterFloorHOffset);
  bMixedData |= UpdateLEWithSelectionValue(leROuterFloorHOffset, p->editVals.sROuterFloorHOffset);
  bMixedData |= UpdateLEWithSelectionValue(leRLOuterWallHOffset, p->editVals.sRLOuterWallHOffset);
  bMixedData |= UpdateLEWithSelectionValue(leRUOuterWallHOffset, p->editVals.sRUOuterWallHOffset);
  bMixedData |= UpdateLEWithSelectionValue(leLUOuterWallHeight, p->editVals.sLUOuterWallHeight);
  bMixedData |= UpdateLEWithSelectionValue(leLLOuterWallHeight, p->editVals.sLLOuterWallHeight);
  bMixedData |= UpdateLEWithSelectionValue(leLOuterFloorHeight, p->editVals.sLOuterFloorHeight);
  bMixedData |= UpdateLEWithSelectionValue(leROuterFloorHeight, p->editVals.sROuterFloorHeight);
  bMixedData |= UpdateLEWithSelectionValue(leRLOuterWallHeight, p->editVals.sRLOuterWallHeight);
  bMixedData |= UpdateLEWithSelectionValue(leRUOuterWallHeight, p->editVals.sRUOuterWallHeight);
  bMixedData |= UpdateLEWithSelectionValue(leRoofHeight, p->editVals.sRoofHeight);
  bMixedData |= UpdateLEWithSelectionValue(leDrawOrder1, p->editVals.sDrawOrder1);
  bMixedData |= UpdateLEWithSelectionValue(leDrawOrder2, p->editVals.sDrawOrder2);
  bMixedData |= UpdateLEWithSelectionValue(leDrawOrder3, p->editVals.sDrawOrder3);
  bMixedData |= UpdateLEWithSelectionValue(leUnk37, p->editVals.sUnk37);
  bMixedData |= UpdateLEWithSelectionValue(leUnk38, p->editVals.sUnk38);
  bMixedData |= UpdateLEWithSelectionValue(leUnk39, p->editVals.sUnk39);
  bMixedData |= UpdateLEWithSelectionValue(leUnk40, p->editVals.sUnk40);
  bMixedData |= UpdateLEWithSelectionValue(leUnk41, p->editVals.sUnk41);
  bMixedData |= UpdateLEWithSelectionValue(leUnk42, p->editVals.sUnk42);
  bMixedData |= UpdateLEWithSelectionValue(leUnk43, p->editVals.sUnk43);
  bMixedData |= UpdateLEWithSelectionValue(leUnk44, p->editVals.sUnk44);
  bMixedData |= UpdateLEWithSelectionValue(leUnk45, p->editVals.sUnk45);
  bMixedData |= UpdateLEWithSelectionValue(leUnk46, p->editVals.sUnk46);
  bMixedData |= UpdateLEWithSelectionValue(leUnk47, p->editVals.sUnk47);
  bMixedData |= UpdateLEWithSelectionValue(leUnk48, p->editVals.sUnk48);
  bMixedData |= UpdateLEWithSelectionValue(leUnk49, p->editVals.sUnk49);
  bMixedData |= UpdateLEWithSelectionValue(leUnk50, p->editVals.sUnk50);
  bMixedData |= UpdateSignWithSelectionValue(pbSign, ckApplySign, leSign, p->editVals.sSignTexture, p->m_pBld);
  bMixedData |= UpdateSignWithSelectionValue(pbBack, ckApplyBack, leBack, p->editVals.sBackTexture, p->m_pTex);
  bMixedData |= UpdateLEWithSelectionValue(leStuntScaleFact, p->editVals.sStuntScaleFactor);
  bMixedData |= UpdateLEWithSelectionValue(leStuntAngle, p->editVals.sStuntAngle);
  bMixedData |= UpdateLEWithSelectionValue(leStuntUnk, p->editVals.sStuntUnknown);
  bMixedData |= UpdateLEWithSelectionValue(leStuntTimingGroup, p->editVals.sStuntTimingGroup);
  bMixedData |= UpdateLEWithSelectionValue(leStuntHeight, p->editVals.sStuntHeight);
  bMixedData |= UpdateLEWithSelectionValue(leStuntTimeBulging, p->editVals.sStuntTimeBulging);
  bMixedData |= UpdateLEWithSelectionValue(leStuntTimeFlat, p->editVals.sStuntTimeFlat);
  bMixedData |= UpdateLEWithSelectionValue(leStuntExpandContract, p->editVals.sStuntExpandsContracts);
  bMixedData |= UpdateLEWithSelectionValue(leStuntBulge, p->editVals.sStuntBulge);

  UpdateTextures(leLeftSurfaceType, lblLSurfaceTex1, lblLSurfaceTex2);
  UpdateTextures(leCenterSurfaceType, lblCSurfaceTex1, lblCSurfaceTex2);
  UpdateTextures(leRightSurfaceType, lblRSurfaceTex1, lblRSurfaceTex2);
  UpdateTextures(leLWallType, lblLWallTex1, lblLWallTex2);
  UpdateTextures(leRWallType, lblRWallTex1, lblRWallTex2);
  UpdateTextures(leRoofType, lblRoofTex1, lblRoofTex2);
  UpdateTextures(leLUOuterWallType, lblLUOuterWallTex1, lblLUOuterWallTex2);
  UpdateTextures(leLLOuterWallType, lblLLOuterWallTex1, lblLLOuterWallTex2);
  UpdateTextures(leOuterFloorType, lblOuterFloorTex1, lblOuterFloorTex2);
  UpdateTextures(leRLOuterWallType, lblRLOuterWallTex1, lblRLOuterWallTex2);
  UpdateTextures(leRUOuterWallType, lblRUOuterWallTex1, lblRUOuterWallTex2);
  UpdateTextures(leEnvironmentFloorType, lblEnvirFloorTex1, lblEnvirFloorTex2);

  pbInsertAfter->setEnabled(!bMixedData);
  pbInsertBefore->setEnabled(!bMixedData);
  pbApply->setEnabled(false);
  pbCancel->setEnabled(false);
  sbSelChunksFrom->setEnabled(true);
  ckTo->setEnabled(true);
  sbSelChunksTo->setEnabled(ckTo->isChecked());
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::RevertInfo()
{
  UpdateLEWithSelectionValue(leFloorDepth, p->sFloorDepth);
  UpdateLEWithSelectionValue(leTex, p->sTex);
  UpdateLEWithSelectionValue(leBld, p->sBld);
  UpdateLEWithSelectionValue(leTrackNum, p->sTrackNumber);
  UpdateLEWithSelectionValue(leImpossibleLaps, p->sImpossibleLaps);
  UpdateLEWithSelectionValue(leHardLaps, p->sHardLaps);
  UpdateLEWithSelectionValue(leTrickyLaps, p->sTrickyLaps);
  UpdateLEWithSelectionValue(leMediumLaps, p->sMediumLaps);
  UpdateLEWithSelectionValue(leEasyLaps, p->sEasyLaps);
  UpdateLEWithSelectionValue(leGirlieLaps, p->sGirlieLaps);
  UpdateLEWithSelectionValue(leMapSize, p->sTrackMapSize);
  UpdateLEWithSelectionValue(leMapFidelity, p->sTrackMapFidelity);
  UpdateLEWithSelectionValue(leInfoUnknown, p->sInfoUnknown);

  pbApplyInfo->setEnabled(false);
  pbRevertInfo->setEnabled(false);
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::UpdateTextures(QLineEdit *pLineEdit, QLabel *pTex1, QLabel *pTex2)
{
  //textures
  QPixmap pixmap;
  int iIndex;
  if (pLineEdit->text().compare("-1") == 0) {
    pTex1->setPixmap(QPixmap());
    pTex2->setPixmap(QPixmap());
  } else if (!pLineEdit->text().isEmpty() && pLineEdit->placeholderText().compare(MIXED_DATA) != 0) {
    int iValue = pLineEdit->text().toInt();
    unsigned int uiSignedBitVal = CTrack::GetSignedBitValueFromInt(iValue);
    iIndex = CTrack::GetIntValueFromSignedBit(uiSignedBitVal & SURFACE_TEXTURE_INDEX);
    if (iIndex < p->m_pTex->m_tileAy.size()) {
      pixmap.convertFromImage(p->m_pTex->m_tileAy[iIndex]);
      pTex1->setPixmap(pixmap);

      if (uiSignedBitVal & SURFACE_FLAG_TEXTURE_PAIR && iIndex > 0) {
        pixmap.convertFromImage(p->m_pTex->m_tileAy[iIndex + 1]);
        pTex2->setPixmap(pixmap);
      } else {
        pTex2->setPixmap(QPixmap());
      }
    }
  } else {
    pTex1->setPixmap(QPixmap());
    pTex2->setPixmap(QPixmap());
  }
}

//-------------------------------------------------------------------------------------------------