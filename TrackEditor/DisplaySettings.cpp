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
  ckHighlightSelection->setChecked(true);
  ckAILines->setChecked(true);

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
  connect(ckHighlightSelection, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);
  connect(ckAILines, &QCheckBox::toggled, this, &CDisplaySettings::UpdatePreviewSelection);

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
  if (ckRoofSurface->isChecked())         uiShowModels |= SHOW_ROOF_SURF_MODEL;
  if (ckRoofWireframe->isChecked())       uiShowModels |= SHOW_ROOF_WIRE_MODEL;
  if (ckLLOWallSurface->isChecked())      uiShowModels |= SHOW_LLOWALL_SURF_MODEL;
  if (ckLLOWallWireframe->isChecked())    uiShowModels |= SHOW_LLOWALL_WIRE_MODEL;
  if (ckLUOWallSurface->isChecked())      uiShowModels |= SHOW_LUOWALL_SURF_MODEL;
  if (ckLUOWallWireframe->isChecked())    uiShowModels |= SHOW_LUOWALL_WIRE_MODEL;
  if (ckRUOWallSurface->isChecked())      uiShowModels |= SHOW_RUOWALL_SURF_MODEL;
  if (ckRUOWallWireframe->isChecked())    uiShowModels |= SHOW_RUOWALL_WIRE_MODEL;
  if (ckRLOWallSurface->isChecked())      uiShowModels |= SHOW_RLOWALL_SURF_MODEL;
  if (ckRLOWallWireframe->isChecked())    uiShowModels |= SHOW_RLOWALL_WIRE_MODEL;
  if (ckOWallFloorSurface->isChecked())   uiShowModels |= SHOW_OWALLFLOOR_SURF_MODEL;
  if (ckOWallFloorWireframe->isChecked()) uiShowModels |= SHOW_OWALLFLOOR_WIRE_MODEL;
  if (ckEnvirFloorSurface->isChecked())   uiShowModels |= SHOW_ENVIRFLOOR_SURF_MODEL;
  if (ckEnvirFloorWireframe->isChecked()) uiShowModels |= SHOW_ENVIRFLOOR_WIRE_MODEL;
  if (ckHighlightSelection->isChecked())  uiShowModels |= SHOW_SELECTION_HIGHLIGHT;
  if (ckAILines->isChecked())             uiShowModels |= SHOW_AILINE_MODELS;
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
  ckHighlightSelection->blockSignals(true);
  ckAILines->blockSignals(true);

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
  ckRoofSurface->setChecked(        uiShowModels & SHOW_ROOF_SURF_MODEL);
  ckRoofWireframe->setChecked(      uiShowModels & SHOW_ROOF_WIRE_MODEL);
  ckLLOWallSurface->setChecked(     uiShowModels & SHOW_LLOWALL_SURF_MODEL);
  ckLLOWallWireframe->setChecked(   uiShowModels & SHOW_LLOWALL_WIRE_MODEL);
  ckLUOWallSurface->setChecked(     uiShowModels & SHOW_LUOWALL_SURF_MODEL);
  ckLUOWallWireframe->setChecked(   uiShowModels & SHOW_LUOWALL_WIRE_MODEL);
  ckRUOWallSurface->setChecked(     uiShowModels & SHOW_RUOWALL_SURF_MODEL);
  ckRUOWallWireframe->setChecked(   uiShowModels & SHOW_RUOWALL_WIRE_MODEL);
  ckRLOWallSurface->setChecked(     uiShowModels & SHOW_RLOWALL_SURF_MODEL);
  ckRLOWallWireframe->setChecked(   uiShowModels & SHOW_RLOWALL_WIRE_MODEL);
  ckOWallFloorSurface->setChecked(  uiShowModels & SHOW_OWALLFLOOR_SURF_MODEL);
  ckOWallFloorWireframe->setChecked(uiShowModels & SHOW_OWALLFLOOR_WIRE_MODEL);
  ckEnvirFloorSurface->setChecked(  uiShowModels & SHOW_ENVIRFLOOR_SURF_MODEL);
  ckEnvirFloorWireframe->setChecked(uiShowModels & SHOW_ENVIRFLOOR_WIRE_MODEL);
  ckHighlightSelection->setChecked( uiShowModels & SHOW_SELECTION_HIGHLIGHT);
  ckAILines->setChecked(            uiShowModels & SHOW_AILINE_MODELS);

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
  ckHighlightSelection->blockSignals(false);
  ckAILines->blockSignals(false);

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