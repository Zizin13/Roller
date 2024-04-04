#ifndef _TRACKEDITOR_TRACKPREVIEW_H
#define _TRACKEDITOR_TRACKPREVIEW_H
//-------------------------------------------------------------------------------------------------
#include <QtOpenGL\qgl.h>
//-------------------------------------------------------------------------------------------------
class CTrackPreview : public QGLWidget
{
  Q_OBJECT

public:
  CTrackPreview(QWidget *pParent);
  ~CTrackPreview();

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int iWidth, int iHeight);
};

//-------------------------------------------------------------------------------------------------
#endif