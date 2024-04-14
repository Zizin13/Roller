#ifndef _TRACKEDITOR_DISPLAYSETTINGS_H
#define _TRACKEDITOR_DISPLAYSETTINGS_H
//-------------------------------------------------------------------------------------------------
#include "ui_DisplaySettings.h"
//-------------------------------------------------------------------------------------------------
class CTrackPreview;
//-------------------------------------------------------------------------------------------------
#define SHOW_LLANE_SURF_MODEL     0x00000001
#define SHOW_LLANE_WIRE_MODEL     0x00000002
#define SHOW_RLANE_SURF_MODEL     0x00000004
#define SHOW_RLANE_WIRE_MODEL     0x00000008
#define SHOW_LSHOULDER_SURF_MODEL 0x00000010
#define SHOW_LSHOULDER_WIRE_MODEL 0x00000020
#define SHOW_RSHOULDER_SURF_MODEL 0x00000040
#define SHOW_RSHOULDER_WIRE_MODEL 0x00000080
//-------------------------------------------------------------------------------------------------

class CDisplaySettings : public QWidget, private Ui::DisplaySettings
{
  Q_OBJECT

public:
  CDisplaySettings(QWidget *pParent, CTrackPreview *pTrackPreview);
  ~CDisplaySettings();

protected slots:
  void UpdateAll();
  void UpdatePreviewSelection();

private:
  void UpdatePreview();

  CTrackPreview *m_pTrackPreview;
};

//-------------------------------------------------------------------------------------------------
#endif