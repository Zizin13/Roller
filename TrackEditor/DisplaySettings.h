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
#define SHOW_OWALLFLOOR_SURF_MODEL 0x00010000
#define SHOW_OWALLFLOOR_WIRE_MODEL 0x00020000
#define SHOW_LLOWALL_SURF_MODEL    0x00040000
#define SHOW_LLOWALL_WIRE_MODEL    0x00080000
#define SHOW_RLOWALL_SURF_MODEL    0x00100000
#define SHOW_RLOWALL_WIRE_MODEL    0x00200000
#define SHOW_LUOWALL_SURF_MODEL    0x00400000
#define SHOW_LUOWALL_WIRE_MODEL    0x00800000
#define SHOW_RUOWALL_SURF_MODEL    0x01000000
#define SHOW_RUOWALL_WIRE_MODEL    0x02000000
#define SHOW_SELECTION_HIGHLIGHT   0x04000000
#define SHOW_AILINE_MODELS         0x08000000
#define SHOW_TEST_CAR              0x10000000
#define SHOW_SIGNS                 0x20000000
#define SHOW_AUDIO                 0x40000000
#define SHOW_STUNTS                0x80000000
//-------------------------------------------------------------------------------------------------

class CDisplaySettings : public QWidget, private Ui::DisplaySettings
{
  Q_OBJECT

public:
  CDisplaySettings(QWidget *pParent, CTrackPreview *pTrackPreview);
  ~CDisplaySettings();

  uint32 GetDisplaySettings(eWhipModel &carModel, eShapeSection &aiLine, bool &bMillionPlus);
  void SetDisplaySettings(uint32 uiShowModels,eWhipModel carModel, eShapeSection aiLine, bool bMillionPlus);
  bool GetAttachLast();
  void SetAttachLast(bool bAttachLast);
  int GetScale();
  void SetScale(int iScale);

protected slots:
  void UpdateAllSurface();
  void UpdateAllWireframe();
  void UpdatePreviewSelection();
  void OnAttachLastChecked();
  void OnSetScale(int iValue);

private:
  void UpdatePreview();

  CTrackPreview *m_pTrackPreview;
};

//-------------------------------------------------------------------------------------------------
#endif