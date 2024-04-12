#ifndef _TRACKEDITOR_GLOBALTRACKSETTINGS_H
#define _TRACKEDITOR_GLOBALTRACKSETTINGS_H
//-------------------------------------------------------------------------------------------------
#include "ui_GlobalTrackSettings.h"
//-------------------------------------------------------------------------------------------------
class CGlobalTrackSettingsPrivate;
class CTrack;
class CTexture;
//-------------------------------------------------------------------------------------------------
class CGlobalTrackSettings : public QWidget, private Ui::GlobalTrackSettings
{
  Q_OBJECT

public:
  CGlobalTrackSettings(QWidget *pParent, CTrack *pTrack);
  ~CGlobalTrackSettings();

protected slots:
  void OnUpdateWindow();
  void OnApplyInfoClicked();
  void OnCancelInfoClicked();
  void UpdateInfoEditMode();

private:
  void UpdateInfoSelection();
  void RevertInfo();

  CGlobalTrackSettingsPrivate *p;
};

//-------------------------------------------------------------------------------------------------
#endif