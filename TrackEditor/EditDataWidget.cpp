#include "EditDataWidget.h"
#include "Track.h"
#include "MainWindow.h"
#include "ChunkEditValues.h"
#include "EditSurfaceDialog.h"
#include "TilePicker.h"
#include "Texture.h"
#include "QtHelpers.h"
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
  {
  };
  ~CEditDataWidgetPrivate()
  {
  };

  CTrack *m_pTrack;

  //selected geometry values
  CChunkEditValues editVals;

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

CEditDataWidget::CEditDataWidget(QWidget *pParent, CTrack *pTrack)
  : QWidget(pParent)
{
  p = new CEditDataWidgetPrivate;
  p->m_pTrack = pTrack;
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
  connect(pbCancel, &QPushButton::clicked, this, &CEditDataWidget::OnCancelClicked);
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
  connect(leAIMaxAccel, &QLineEdit::textChanged, this, &CEditDataWidget::UpdateGeometryEditMode);
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
    , leAIMaxAccel->text(), leUnk05->text(), leAudioAboveTrigger->text(), leAudioTriggerSpeed->text(), leAudioBelowTrigger->text()
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
    , leAIMaxAccel->text(), leUnk05->text(), leAudioAboveTrigger->text(), leAudioTriggerSpeed->text(), leAudioBelowTrigger->text()
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
    , leAIMaxAccel->text(), leUnk05->text(), leAudioAboveTrigger->text(), leAudioTriggerSpeed->text(), leAudioBelowTrigger->text()
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

void CEditDataWidget::OnCancelClicked()
{
  RevertGeometry();
  UpdateGeometryEditMode();
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

  CEditSurfaceDialog dlg(this, &p->m_pTrack->m_tex, iValue);
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

  CEditSurfaceDialog dlg(this, &p->m_pTrack->m_tex, iValue);
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

  CEditSurfaceDialog dlg(this, &p->m_pTrack->m_tex, iValue);
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

  CEditSurfaceDialog dlg(this, &p->m_pTrack->m_tex, iValue);
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

  CEditSurfaceDialog dlg(this, &p->m_pTrack->m_tex, iValue);
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

  CEditSurfaceDialog dlg(this, &p->m_pTrack->m_tex, iValue);
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

  CEditSurfaceDialog dlg(this, &p->m_pTrack->m_tex, iValue);
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

  CEditSurfaceDialog dlg(this, &p->m_pTrack->m_tex, iValue);
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

  CEditSurfaceDialog dlg(this, &p->m_pTrack->m_tex, iValue);
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

  CEditSurfaceDialog dlg(this, &p->m_pTrack->m_tex, iValue);
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

  CEditSurfaceDialog dlg(this, &p->m_pTrack->m_tex, iValue);
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

  CEditSurfaceDialog dlg(this, &p->m_pTrack->m_tex, iValue);
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
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leLShoulderWidth, p->editVals.sLeftShoulderWidth);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leLLaneWidth, p->editVals.sLeftLaneWidth);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leRLaneWidth, p->editVals.sRightLaneWidth);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leRShoulderWidth, p->editVals.sRightShoulderWidth);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leLShoulderHeight, p->editVals.sLShoulderHeight);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leRShoulderHeight, p->editVals.sRShoulderHeight);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leLength, p->editVals.sLength);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leYaw, p->editVals.sYaw);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, lePitch, p->editVals.sPitch);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leRoll, p->editVals.sRoll);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leAILine1, p->editVals.sAILine1);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leAILine2, p->editVals.sAILine2);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leAILine3, p->editVals.sAILine3);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leAILine4, p->editVals.sAILine4);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leTrackGrip, p->editVals.sTrackGrip);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leLeftShoulderGrip, p->editVals.sLeftShoulderGrip);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leRShoulderGrip, p->editVals.sRightShoulderGrip);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leAIMaxAccel, p->editVals.sAIMaxAccel);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leUnk05, p->editVals.sUnk05);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leAudioAboveTrigger, p->editVals.sAudioAboveTrigger);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leAudioTriggerSpeed, p->editVals.sAudioTriggerSpeed);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leAudioBelowTrigger, p->editVals.sAudioBelowTrigger);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leLeftSurfaceType, p->editVals.sLeftSurfaceType);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leCenterSurfaceType, p->editVals.sCenterSurfaceType);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leRightSurfaceType, p->editVals.sRightSurfaceType);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leLWallType, p->editVals.sLWallType);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leRWallType, p->editVals.sRWallType);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leRoofType, p->editVals.sRoofType);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leLUOuterWallType, p->editVals.sLUOuterWallType);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leLLOuterWallType, p->editVals.sLLOuterWallType);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leOuterFloorType, p->editVals.sOuterFloorType);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leRLOuterWallType, p->editVals.sRLOuterWallType);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leRUOuterWallType, p->editVals.sRUOuterWallType);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leEnvironmentFloorType, p->editVals.sEnvironmentFloorType);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leSignType, p->editVals.sSignType);
  QtHelpers::UpdateCBEditMode(bEditMode, bMixedData, cbSignType, p->editVals.sSignType);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leSignHorizOffset, p->editVals.sSignHorizOffset);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leSignVertOffset, p->editVals.sSignVertOffset);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leSignYaw, p->editVals.sSignYaw);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leSignPitch, p->editVals.sSignPitch);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leSignRoll, p->editVals.sSignRoll);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leLUOuterWallHOffset, p->editVals.sLUOuterWallHOffset);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leLLOuterWallHOffset, p->editVals.sLLOuterWallHOffset);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leLOuterFloorHOffset, p->editVals.sLOuterFloorHOffset);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leROuterFloorHOffset, p->editVals.sROuterFloorHOffset);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leRLOuterWallHOffset, p->editVals.sRLOuterWallHOffset);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leRUOuterWallHOffset, p->editVals.sRUOuterWallHOffset);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leLUOuterWallHeight, p->editVals.sLUOuterWallHeight);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leLLOuterWallHeight, p->editVals.sLLOuterWallHeight);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leLOuterFloorHeight, p->editVals.sLOuterFloorHeight);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leROuterFloorHeight, p->editVals.sROuterFloorHeight);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leRLOuterWallHeight, p->editVals.sRLOuterWallHeight);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leRUOuterWallHeight, p->editVals.sRUOuterWallHeight);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leRoofHeight, p->editVals.sRoofHeight);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leDrawOrder1, p->editVals.sDrawOrder1);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leDrawOrder2, p->editVals.sDrawOrder2);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leDrawOrder3, p->editVals.sDrawOrder3);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leUnk37, p->editVals.sUnk37);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leUnk38, p->editVals.sUnk38);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leUnk39, p->editVals.sUnk39);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leUnk40, p->editVals.sUnk40);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leUnk41, p->editVals.sUnk41);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leUnk42, p->editVals.sUnk42);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leUnk43, p->editVals.sUnk43);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leUnk44, p->editVals.sUnk44);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leUnk45, p->editVals.sUnk45);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leUnk46, p->editVals.sUnk46);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leUnk47, p->editVals.sUnk47);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leUnk48, p->editVals.sUnk48);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leUnk49, p->editVals.sUnk49);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leUnk50, p->editVals.sUnk50);
  QtHelpers::UpdateSignEditMode(bEditMode, bMixedData, leSign, widgetSign, p->editVals.sSignTexture);
  QtHelpers::UpdateSignEditMode(bEditMode, bMixedData, leBack, widgetBack, p->editVals.sBackTexture);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leStuntScaleFact, p->editVals.sStuntScaleFactor);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leStuntAngle, p->editVals.sStuntAngle);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leStuntUnk, p->editVals.sStuntUnknown);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leStuntTimingGroup, p->editVals.sStuntTimingGroup);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leStuntHeight, p->editVals.sStuntHeight);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leStuntTimeBulging, p->editVals.sStuntTimeBulging);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leStuntTimeFlat, p->editVals.sStuntTimeFlat);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leStuntExpandContract, p->editVals.sStuntExpandsContracts);
  QtHelpers::UpdateLEEditMode(bEditMode, bMixedData, leStuntBulge, p->editVals.sStuntBulge);

  pbApply->setEnabled(bEditMode);
  pbCancel->setEnabled(bEditMode);
  pbInsertAfter->setEnabled(!bMixedData);
  pbInsertBefore->setEnabled(!bMixedData);
  sbSelChunksFrom->setEnabled(!bEditMode);
  ckTo->setEnabled(!bEditMode);
  sbSelChunksTo->setEnabled(!bEditMode && ckTo->isChecked());
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnSignClicked()
{
  QString sValue = leSign->text();
  int iValue = sValue.toInt();
  unsigned int uiSignedBitVal = CTrack::GetSignedBitValueFromInt(iValue);
  int iBldIndex = uiSignedBitVal & SURFACE_TEXTURE_INDEX;

  CTilePicker dlg(this, &p->m_pTrack->m_bld, iBldIndex, true);
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
  QtHelpers::UpdateSignButtonDisplay(pbSign, ckApplySign, leSign, &p->m_pTrack->m_bld);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnBackClicked()
{
  QString sValue = leBack->text();
  int iValue = sValue.toInt();
  unsigned int uiSignedBitVal = CTrack::GetSignedBitValueFromInt(iValue);
  int iBldIndex = uiSignedBitVal & SURFACE_TEXTURE_INDEX;

  CTilePicker dlg(this, &p->m_pTrack->m_tex, iBldIndex, true);
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
  QtHelpers::UpdateSignButtonDisplay(pbBack, ckApplyBack, leBack, &p->m_pTrack->m_tex);
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
  QtHelpers::UpdateSignButtonDisplay(pbSign, ckApplySign, leSign, &p->m_pTrack->m_bld);
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
  QtHelpers::UpdateSignButtonDisplay(pbBack, ckApplyBack, leBack, &p->m_pTrack->m_tex);
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
  QtHelpers::UpdateSignButtonDisplay(pbSign, ckApplySign, leSign, &p->m_pTrack->m_bld);
  UpdateGeometryEditMode();
}

//-------------------------------------------------------------------------------------------------

void CEditDataWidget::OnBackLEChanged()
{
  QtHelpers::UpdateSignButtonDisplay(pbBack, ckApplyBack, leBack, &p->m_pTrack->m_tex);
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

void CEditDataWidget::RevertGeometry()
{
  pbDelete->setEnabled(!p->m_pTrack->m_chunkAy.empty());

  bool bMixedData = false;
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLShoulderWidth, p->editVals.sLeftShoulderWidth);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLLaneWidth, p->editVals.sLeftLaneWidth);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRLaneWidth, p->editVals.sRightLaneWidth);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRShoulderWidth, p->editVals.sRightShoulderWidth);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLShoulderHeight, p->editVals.sLShoulderHeight);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRShoulderHeight, p->editVals.sRShoulderHeight);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLength, p->editVals.sLength);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leYaw, p->editVals.sYaw);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(lePitch, p->editVals.sPitch);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRoll, p->editVals.sRoll);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAILine1, p->editVals.sAILine1);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAILine2, p->editVals.sAILine2);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAILine3, p->editVals.sAILine3);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAILine4, p->editVals.sAILine4);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leTrackGrip, p->editVals.sTrackGrip);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLeftShoulderGrip, p->editVals.sLeftShoulderGrip);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRShoulderGrip, p->editVals.sRightShoulderGrip);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAIMaxAccel, p->editVals.sAIMaxAccel);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk05, p->editVals.sUnk05);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAudioAboveTrigger, p->editVals.sAudioAboveTrigger);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAudioTriggerSpeed, p->editVals.sAudioTriggerSpeed);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leAudioBelowTrigger, p->editVals.sAudioBelowTrigger);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLeftSurfaceType, p->editVals.sLeftSurfaceType);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leCenterSurfaceType, p->editVals.sCenterSurfaceType);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRightSurfaceType, p->editVals.sRightSurfaceType);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLWallType, p->editVals.sLWallType);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRWallType, p->editVals.sRWallType);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRoofType, p->editVals.sRoofType);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLUOuterWallType, p->editVals.sLUOuterWallType);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLLOuterWallType, p->editVals.sLLOuterWallType);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leOuterFloorType, p->editVals.sOuterFloorType);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRLOuterWallType, p->editVals.sRLOuterWallType);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRUOuterWallType, p->editVals.sRUOuterWallType);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leEnvironmentFloorType, p->editVals.sEnvironmentFloorType);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leSignType, p->editVals.sSignType);
  bMixedData |= QtHelpers::UpdateCBWithSelectionValue(cbSignType, p->editVals.sSignType);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leSignHorizOffset, p->editVals.sSignHorizOffset);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leSignVertOffset, p->editVals.sSignVertOffset);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leSignYaw, p->editVals.sSignYaw);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leSignPitch, p->editVals.sSignPitch);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leSignRoll, p->editVals.sSignRoll);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLUOuterWallHOffset, p->editVals.sLUOuterWallHOffset);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLLOuterWallHOffset, p->editVals.sLLOuterWallHOffset);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLOuterFloorHOffset, p->editVals.sLOuterFloorHOffset);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leROuterFloorHOffset, p->editVals.sROuterFloorHOffset);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRLOuterWallHOffset, p->editVals.sRLOuterWallHOffset);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRUOuterWallHOffset, p->editVals.sRUOuterWallHOffset);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLUOuterWallHeight, p->editVals.sLUOuterWallHeight);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLLOuterWallHeight, p->editVals.sLLOuterWallHeight);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leLOuterFloorHeight, p->editVals.sLOuterFloorHeight);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leROuterFloorHeight, p->editVals.sROuterFloorHeight);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRLOuterWallHeight, p->editVals.sRLOuterWallHeight);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRUOuterWallHeight, p->editVals.sRUOuterWallHeight);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leRoofHeight, p->editVals.sRoofHeight);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leDrawOrder1, p->editVals.sDrawOrder1);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leDrawOrder2, p->editVals.sDrawOrder2);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leDrawOrder3, p->editVals.sDrawOrder3);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk37, p->editVals.sUnk37);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk38, p->editVals.sUnk38);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk39, p->editVals.sUnk39);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk40, p->editVals.sUnk40);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk41, p->editVals.sUnk41);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk42, p->editVals.sUnk42);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk43, p->editVals.sUnk43);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk44, p->editVals.sUnk44);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk45, p->editVals.sUnk45);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk46, p->editVals.sUnk46);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk47, p->editVals.sUnk47);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk48, p->editVals.sUnk48);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk49, p->editVals.sUnk49);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leUnk50, p->editVals.sUnk50);
  bMixedData |= QtHelpers::UpdateSignWithSelectionValue(pbSign, ckApplySign, leSign, p->editVals.sSignTexture, &p->m_pTrack->m_bld);
  bMixedData |= QtHelpers::UpdateSignWithSelectionValue(pbBack, ckApplyBack, leBack, p->editVals.sBackTexture, &p->m_pTrack->m_tex);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leStuntScaleFact, p->editVals.sStuntScaleFactor);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leStuntAngle, p->editVals.sStuntAngle);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leStuntUnk, p->editVals.sStuntUnknown);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leStuntTimingGroup, p->editVals.sStuntTimingGroup);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leStuntHeight, p->editVals.sStuntHeight);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leStuntTimeBulging, p->editVals.sStuntTimeBulging);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leStuntTimeFlat, p->editVals.sStuntTimeFlat);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leStuntExpandContract, p->editVals.sStuntExpandsContracts);
  bMixedData |= QtHelpers::UpdateLEWithSelectionValue(leStuntBulge, p->editVals.sStuntBulge);

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
    if (iIndex < p->m_pTrack->m_tex.m_iNumTiles) {
      pixmap.convertFromImage(QtHelpers::GetQImageFromTile(p->m_pTrack->m_tex.m_pTileAy[iIndex]));
      pTex1->setPixmap(pixmap);

      if (uiSignedBitVal & SURFACE_FLAG_TEXTURE_PAIR && iIndex > 0) {
        if (uiSignedBitVal & SURFACE_FLAG_PAIR_NEXT_TEX)
          pixmap.convertFromImage(QtHelpers::GetQImageFromTile(p->m_pTrack->m_tex.m_pTileAy[iIndex + 1]));
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