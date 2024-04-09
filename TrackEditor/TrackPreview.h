#ifndef _TRACKEDITOR_TRACKPREVIEW_H
#define _TRACKEDITOR_TRACKPREVIEW_H
//-------------------------------------------------------------------------------------------------
#include <QtOpenGL\qgl.h>
//-------------------------------------------------------------------------------------------------
#include "glm.hpp"
struct tTestModel
{
  glm::vec3 lightPosition;
};
//-------------------------------------------------------------------------------------------------
class CTrackPreviewPrivate;
//-------------------------------------------------------------------------------------------------
class CTrackPreview : public QGLWidget
{
  Q_OBJECT

public:
  CTrackPreview(QWidget *pParent);
  ~CTrackPreview();

  void SetModel(tTestModel *pModel);

protected:
  void initializeGL();
  void paintGL();
  void mouseMoveEvent(QMouseEvent *pEvent);
  void keyPressEvent(QKeyEvent *pEvent);

private:
  CTrackPreviewPrivate *p;
  tTestModel *m_pModel;
};

//-------------------------------------------------------------------------------------------------
#endif