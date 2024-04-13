#ifndef _TRACKEDITOR_DISPLAYSETTINGS_H
#define _TRACKEDITOR_DISPLAYSETTINGS_H
//-------------------------------------------------------------------------------------------------
#include "ui_DisplaySettings.h"
//-------------------------------------------------------------------------------------------------
class CTrackPreview;
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
  CTrackPreview *m_pTrackPreview;
};

//-------------------------------------------------------------------------------------------------
#endif