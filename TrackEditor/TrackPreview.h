#ifndef _TRACKEDITOR_TRACKPREVIEW_H
#define _TRACKEDITOR_TRACKPREVIEW_H
//-------------------------------------------------------------------------------------------------
#include <QtOpenGL>
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

  void mousePressEvent(QMouseEvent *pEvent);
  void mouseMoveEvent(QMouseEvent *pEvent);

private:
  void SetXRotation(int iAngle);
  void SetYRotation(int iAngle);
  void SetZRotation(int iAngle);
  void Draw();

  int xRot;
  int yRot;
  int zRot;

  QPoint lastPos;
};

//-------------------------------------------------------------------------------------------------
#endif