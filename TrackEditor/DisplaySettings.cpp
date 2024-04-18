#include "TrackEditor.h"
#include "DisplaySettings.h"
#include "TrackPreview.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CDisplaySettings::CDisplaySettings(QWidget *pParent, CTrackPreview *pTrackPreview)
  : QWidget(pParent)
  , m_pTrackPreview(pTrackPreview)
{
  setupUi(this);

  ckAllSurface->setChecked(true);

  ckHighlightSelection->hide();
  line->hide();
  lblRoof->hide();
  ckRoofSurface->hide();
  ckRoofWireframe->hide();
  lblLUOWall->hide();
  ckLUOWallSurface->hide();
  ckLUOWallWireframe->hide();
  lblLLOWall->hide();
  ckLLOWallSurface->hide();
  ckLLOWallWireframe->hide();
  lblRUOWall->hide();
  ckRUOWallSurface->hide();
  ckRUOWallWireframe->hide();
  lblRLOWall->hide();
  ckRLOWallSurface->hide();
  ckRLOWallWireframe->hide();
  lblOWallFloor->hide();
  ckOWallFloorSurface->hide();
  ckOWallFloorWireframe->hide();
  lblEnvirFloor->hide();
  ckEnvirFloorSurface->hide();
  ckEnvirFloorWireframe->hide();

  connect(ckAllSurface, &QCheckBox::toggled, this, &CDisplaySettings::UpdateAll);
  connect(ckAllWireframe, &QCheckBox::toggled, this, &CDisplaySettings::UpdateAll);
  connect(ckLLaneSurface, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckLLaneWireframe, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckRLaneSurface, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckRLaneWireframe, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckLShoulderSurface, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckLShoulderWireframe, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckRShoulderSurface, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckRShoulderWireframe, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckLWallSurface, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckLWallWireframe, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckRWallSurface, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckRWallWireframe, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckRoofSurface, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckRoofWireframe, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckLUOWallSurface, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckLUOWallWireframe, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckLLOWallSurface, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckLLOWallWireframe, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckRUOWallSurface, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckRUOWallWireframe, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckRLOWallSurface, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckRLOWallWireframe, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckOWallFloorSurface, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckOWallFloorWireframe, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckEnvirFloorSurface, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckEnvirFloorWireframe, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);

  UpdateAll();
}

//-------------------------------------------------------------------------------------------------

CDisplaySettings::~CDisplaySettings()
{
}

//-------------------------------------------------------------------------------------------------

uint32 CDisplaySettings::GetDisplaySettings()
{
  uint32 uiShowModels = 0;
  if (ckLLaneSurface->isChecked())        uiShowModels |= SHOW_LLANE_SURF_MODEL;
  if (ckLLaneWireframe->isChecked())      uiShowModels |= SHOW_LLANE_WIRE_MODEL;
  if (ckRLaneSurface->isChecked())        uiShowModels |= SHOW_RLANE_SURF_MODEL;
  if (ckRLaneWireframe->isChecked())      uiShowModels |= SHOW_RLANE_WIRE_MODEL;
  if (ckLShoulderSurface->isChecked())    uiShowModels |= SHOW_LSHOULDER_SURF_MODEL;
  if (ckLShoulderWireframe->isChecked())  uiShowModels |= SHOW_LSHOULDER_WIRE_MODEL;
  if (ckRShoulderSurface->isChecked())    uiShowModels |= SHOW_RSHOULDER_SURF_MODEL;
  if (ckRShoulderWireframe->isChecked())  uiShowModels |= SHOW_RSHOULDER_WIRE_MODEL;
  if (ckLWallSurface->isChecked())        uiShowModels |= SHOW_LWALL_SURF_MODEL;
  if (ckLWallWireframe->isChecked())      uiShowModels |= SHOW_LWALL_WIRE_MODEL;
  if (ckRWallSurface->isChecked())        uiShowModels |= SHOW_RWALL_SURF_MODEL;
  if (ckRWallWireframe->isChecked())      uiShowModels |= SHOW_RWALL_WIRE_MODEL;
  return uiShowModels;
}

//-------------------------------------------------------------------------------------------------

void CDisplaySettings::SetDisplaySettings(uint32 uiShowModels)
{
  ckLLaneSurface->blockSignals(true);
  ckLLaneWireframe->blockSignals(true);
  ckRLaneSurface->blockSignals(true);
  ckRLaneWireframe->blockSignals(true);
  ckLShoulderSurface->blockSignals(true);
  ckLShoulderWireframe->blockSignals(true);
  ckRShoulderSurface->blockSignals(true);
  ckRShoulderWireframe->blockSignals(true);
  ckLWallSurface->blockSignals(true);
  ckLWallWireframe->blockSignals(true);
  ckRWallSurface->blockSignals(true);
  ckRWallWireframe->blockSignals(true);
  ckRoofSurface->blockSignals(true);
  ckRoofWireframe->blockSignals(true);
  ckLUOWallSurface->blockSignals(true);
  ckLUOWallWireframe->blockSignals(true);
  ckLLOWallSurface->blockSignals(true);
  ckLLOWallWireframe->blockSignals(true);
  ckRUOWallSurface->blockSignals(true);
  ckRUOWallWireframe->blockSignals(true);
  ckRLOWallSurface->blockSignals(true);
  ckRLOWallWireframe->blockSignals(true);
  ckOWallFloorSurface->blockSignals(true);
  ckOWallFloorWireframe->blockSignals(true);
  ckEnvirFloorSurface->blockSignals(true);
  ckEnvirFloorWireframe->blockSignals(true);

  ckLLaneSurface->setChecked(       uiShowModels & SHOW_LLANE_SURF_MODEL);
  ckLLaneWireframe->setChecked(     uiShowModels & SHOW_LLANE_WIRE_MODEL);
  ckRLaneSurface->setChecked(       uiShowModels & SHOW_RLANE_SURF_MODEL);
  ckRLaneWireframe->setChecked(     uiShowModels & SHOW_RLANE_WIRE_MODEL);
  ckLShoulderSurface->setChecked(   uiShowModels & SHOW_LSHOULDER_SURF_MODEL);
  ckLShoulderWireframe->setChecked( uiShowModels & SHOW_LSHOULDER_WIRE_MODEL);
  ckRShoulderSurface->setChecked(   uiShowModels & SHOW_RSHOULDER_SURF_MODEL);
  ckRShoulderWireframe->setChecked( uiShowModels & SHOW_RSHOULDER_WIRE_MODEL);
  ckLWallSurface->setChecked(       uiShowModels & SHOW_LWALL_SURF_MODEL);
  ckLWallWireframe->setChecked(     uiShowModels & SHOW_LWALL_WIRE_MODEL);
  ckRWallSurface->setChecked(       uiShowModels & SHOW_RWALL_SURF_MODEL);
  ckRWallWireframe->setChecked(     uiShowModels & SHOW_RWALL_WIRE_MODEL);

  ckLLaneSurface->blockSignals(false);
  ckLLaneWireframe->blockSignals(false);
  ckRLaneSurface->blockSignals(false);
  ckRLaneWireframe->blockSignals(false);
  ckLShoulderSurface->blockSignals(false);
  ckLShoulderWireframe->blockSignals(false);
  ckRShoulderSurface->blockSignals(false);
  ckRShoulderWireframe->blockSignals(false);
  ckLWallSurface->blockSignals(false);
  ckLWallWireframe->blockSignals(false);
  ckRWallSurface->blockSignals(false);
  ckRWallWireframe->blockSignals(false);
  ckRoofSurface->blockSignals(false);
  ckRoofWireframe->blockSignals(false);
  ckLUOWallSurface->blockSignals(false);
  ckLUOWallWireframe->blockSignals(false);
  ckLLOWallSurface->blockSignals(false);
  ckLLOWallWireframe->blockSignals(false);
  ckRUOWallSurface->blockSignals(false);
  ckRUOWallWireframe->blockSignals(false);
  ckRLOWallSurface->blockSignals(false);
  ckRLOWallWireframe->blockSignals(false);
  ckOWallFloorSurface->blockSignals(false);
  ckOWallFloorWireframe->blockSignals(false);
  ckEnvirFloorSurface->blockSignals(false);
  ckEnvirFloorWireframe->blockSignals(false);

  UpdatePreviewSelection();
}

//-------------------------------------------------------------------------------------------------

void CDisplaySettings::UpdateAll()
{
  ckLLaneSurface->blockSignals(true);
  ckLLaneWireframe->blockSignals(true);
  ckRLaneSurface->blockSignals(true);
  ckRLaneWireframe->blockSignals(true);
  ckLShoulderSurface->blockSignals(true);
  ckLShoulderWireframe->blockSignals(true);
  ckRShoulderSurface->blockSignals(true);
  ckRShoulderWireframe->blockSignals(true);
  ckLWallSurface->blockSignals(true);
  ckLWallWireframe->blockSignals(true);
  ckRWallSurface->blockSignals(true);
  ckRWallWireframe->blockSignals(true);
  ckRoofSurface->blockSignals(true);
  ckRoofWireframe->blockSignals(true);
  ckLUOWallSurface->blockSignals(true);
  ckLUOWallWireframe->blockSignals(true);
  ckLLOWallSurface->blockSignals(true);
  ckLLOWallWireframe->blockSignals(true);
  ckRUOWallSurface->blockSignals(true);
  ckRUOWallWireframe->blockSignals(true);
  ckRLOWallSurface->blockSignals(true);
  ckRLOWallWireframe->blockSignals(true);
  ckOWallFloorSurface->blockSignals(true);
  ckOWallFloorWireframe->blockSignals(true);
  ckEnvirFloorSurface->blockSignals(true);
  ckEnvirFloorWireframe->blockSignals(true);

  ckLLaneSurface->setChecked(ckAllSurface->isChecked());
  ckLLaneWireframe->setChecked(ckAllWireframe->isChecked());
  ckRLaneSurface->setChecked(ckAllSurface->isChecked());
  ckRLaneWireframe->setChecked(ckAllWireframe->isChecked());
  ckLShoulderSurface->setChecked(ckAllSurface->isChecked());
  ckLShoulderWireframe->setChecked(ckAllWireframe->isChecked());
  ckRShoulderSurface->setChecked(ckAllSurface->isChecked());
  ckRShoulderWireframe->setChecked(ckAllWireframe->isChecked());
  ckLWallSurface->setChecked(ckAllSurface->isChecked());
  ckLWallWireframe->setChecked(ckAllWireframe->isChecked());
  ckRWallSurface->setChecked(ckAllSurface->isChecked());
  ckRWallWireframe->setChecked(ckAllWireframe->isChecked());
  ckRoofSurface->setChecked(ckAllSurface->isChecked());
  ckRoofWireframe->setChecked(ckAllWireframe->isChecked());
  ckLUOWallSurface->setChecked(ckAllSurface->isChecked());
  ckLUOWallWireframe->setChecked(ckAllWireframe->isChecked());
  ckLLOWallSurface->setChecked(ckAllSurface->isChecked());
  ckLLOWallWireframe->setChecked(ckAllWireframe->isChecked());
  ckRUOWallSurface->setChecked(ckAllSurface->isChecked());
  ckRUOWallWireframe->setChecked(ckAllWireframe->isChecked());
  ckRLOWallSurface->setChecked(ckAllSurface->isChecked());
  ckRLOWallWireframe->setChecked(ckAllWireframe->isChecked());
  ckOWallFloorSurface->setChecked(ckAllSurface->isChecked());
  ckOWallFloorWireframe->setChecked(ckAllWireframe->isChecked());
  ckEnvirFloorSurface->setChecked(ckAllSurface->isChecked());
  ckEnvirFloorWireframe->setChecked(ckAllWireframe->isChecked());

  ckLLaneSurface->blockSignals(false);
  ckLLaneWireframe->blockSignals(false);
  ckRLaneSurface->blockSignals(false);
  ckRLaneWireframe->blockSignals(false);
  ckLShoulderSurface->blockSignals(false);
  ckLShoulderWireframe->blockSignals(false);
  ckRShoulderSurface->blockSignals(false);
  ckRShoulderWireframe->blockSignals(false);
  ckLWallSurface->blockSignals(false);
  ckLWallWireframe->blockSignals(false);
  ckRWallSurface->blockSignals(false);
  ckRWallWireframe->blockSignals(false);
  ckRoofSurface->blockSignals(false);
  ckRoofWireframe->blockSignals(false);
  ckLUOWallSurface->blockSignals(false);
  ckLUOWallWireframe->blockSignals(false);
  ckLLOWallSurface->blockSignals(false);
  ckLLOWallWireframe->blockSignals(false);
  ckRUOWallSurface->blockSignals(false);
  ckRUOWallWireframe->blockSignals(false);
  ckRLOWallSurface->blockSignals(false);
  ckRLOWallWireframe->blockSignals(false);
  ckOWallFloorSurface->blockSignals(false);
  ckOWallFloorWireframe->blockSignals(false);
  ckEnvirFloorSurface->blockSignals(false);
  ckEnvirFloorWireframe->blockSignals(false);

  UpdatePreview();
}

//-------------------------------------------------------------------------------------------------

void CDisplaySettings::UpdatePreviewSelection()
{
  bool bAllSurfaceChecked = true;
  bool bAllWireframeChecked = true;
  bAllSurfaceChecked &= ckLLaneSurface->isChecked();
  bAllWireframeChecked &= ckLLaneWireframe->isChecked();
  bAllSurfaceChecked &= ckRLaneSurface->isChecked();
  bAllWireframeChecked &= ckRLaneWireframe->isChecked();
  bAllSurfaceChecked &= ckLShoulderSurface->isChecked();
  bAllWireframeChecked &= ckLShoulderWireframe->isChecked();
  bAllSurfaceChecked &= ckRShoulderSurface->isChecked();
  bAllWireframeChecked &= ckRShoulderWireframe->isChecked();
  bAllSurfaceChecked &= ckLWallSurface->isChecked();
  bAllWireframeChecked &= ckLWallWireframe->isChecked();
  bAllSurfaceChecked &= ckRWallSurface->isChecked();
  bAllWireframeChecked &= ckRWallWireframe->isChecked();
  bAllSurfaceChecked &= ckRoofSurface->isChecked();
  bAllWireframeChecked &= ckRoofWireframe->isChecked();
  bAllSurfaceChecked &= ckLUOWallSurface->isChecked();
  bAllWireframeChecked &= ckLUOWallWireframe->isChecked();
  bAllSurfaceChecked &= ckLLOWallSurface->isChecked();
  bAllWireframeChecked &= ckLLOWallWireframe->isChecked();
  bAllSurfaceChecked &= ckRUOWallSurface->isChecked();
  bAllWireframeChecked &= ckRUOWallWireframe->isChecked();
  bAllSurfaceChecked &= ckRLOWallSurface->isChecked();
  bAllWireframeChecked &= ckRLOWallWireframe->isChecked();
  bAllSurfaceChecked &= ckOWallFloorSurface->isChecked();
  bAllWireframeChecked &= ckOWallFloorWireframe->isChecked();
  bAllSurfaceChecked &= ckEnvirFloorSurface->isChecked();
  bAllWireframeChecked &= ckEnvirFloorWireframe->isChecked();

  ckAllSurface->blockSignals(true);
  ckAllWireframe->blockSignals(true);

  ckAllSurface->setChecked(bAllSurfaceChecked);
  ckAllWireframe->setChecked(bAllWireframeChecked);

  ckAllSurface->blockSignals(false);
  ckAllWireframe->blockSignals(false);

  UpdatePreview();
}

//-------------------------------------------------------------------------------------------------

void CDisplaySettings::UpdatePreview()
{
  uint32 uiShowModels = GetDisplaySettings();
  m_pTrackPreview->ShowModels(uiShowModels);
}

//-------------------------------------------------------------------------------------------------