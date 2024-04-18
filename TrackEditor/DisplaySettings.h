#ifndef _TRACKEDITOR_DISPLAYSETTINGS_H
#define _TRACKEDITOR_DISPLAYSETTINGS_H
//-------------------------------------------------------------------------------------------------
#include "ui_DisplaySettings.h"
#include "Types.h"
//-------------------------------------------------------------------------------------------------
class CTrackPreview;
//-------------------------------------------------------------------------------------------------
#define SHOW_LLANE_SURF_MODEL      0x00000001
#define SHOW_LLANE_WIRE_MODEL      0x00000002
#define SHOW_RLANE_SURF_MODEL      0x00000004
#define SHOW_RLANE_WIRE_MODEL      0x00000008
#define SHOW_LSHOULDER_SURF_MODEL  0x00000010
#define SHOW_LSHOULDER_WIRE_MODEL  0x00000020
#define SHOW_RSHOULDER_SURF_MODEL  0x00000040
#define SHOW_RSHOULDER_WIRE_MODEL  0x00000080
#define SHOW_LWALL_SURF_MODEL      0x00000100
#define SHOW_LWALL_WIRE_MODEL      0x00000200
#define SHOW_RWALL_SURF_MODEL      0x00000400
#define SHOW_RWALL_WIRE_MODEL      0x00000800
#define SHOW_ROOF_SURF_MODEL       0x00001000
#define SHOW_ROOF_WIRE_MODEL       0x00002000
#define SHOW_ENVIRFLOOR_SURF_MODEL 0x00004000
#define SHOW_ENVIRFLOOR_WIRE_MODEL 0x00008000
//-------------------------------------------------------------------------------------------------

class CDisplaySettings : public QWidget, private Ui::DisplaySettings
{
  Q_OBJECT

public:
  CDisplaySettings(QWidget *pParent, CTrackPreview *pTrackPreview);
  ~CDisplaySettings();

  uint32 GetDisplaySettings();
  void SetDisplaySettings(uint32 uiShowModels);

protected slots:
  void UpdateAll();
  void UpdatePreviewSelection();

private:
  void UpdatePreview();

  CTrackPreview *m_pTrackPreview;
};

//-------------------------------------------------------------------------------------------------
#endif