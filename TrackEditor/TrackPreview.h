#ifndef _TRACKEDITOR_TRACKPREVIEW_H
#define _TRACKEDITOR_TRACKPREVIEW_H
//-------------------------------------------------------------------------------------------------
#include <QtOpenGL\qgl.h>
#include "Types.h"
//-------------------------------------------------------------------------------------------------
class CTrackPreviewPrivate;
class CTrack;
//-------------------------------------------------------------------------------------------------
class CTrackPreview : public QGLWidget
{
  Q_OBJECT

public:
  CTrackPreview(QWidget *pParent);
  ~CTrackPreview();

  void SetTrack(CTrack *pTrack);
  void ShowModels(uint32 uiShowModels);
  void UpdateCar(eWhipModel carModel, eShapeSection aiLine, bool bMillionPlus);
  void SetScale(int iScale);
  void AttachLast(bool bAttachLast);
  void ReloadCar();

protected slots:
  void UpdateGeometrySelection(int iFrom, int iTo);

protected:
  void initializeGL();
  void paintGL();
  void mouseMoveEvent(QMouseEvent *pEvent);
  void keyPressEvent(QKeyEvent *pEvent);

private:
  CTrackPreviewPrivate *p;
  uint32 m_uiShowModels;
  int m_iFrom;
  eWhipModel m_carModel;
  eShapeSection m_carAILine;
  bool m_bMillionPlus;
  bool m_bAttachLast;
  int m_iScale;
};

//-------------------------------------------------------------------------------------------------
#endif