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
  void mouseMoveEvent(QMouseEvent *pEvent);

private:
  void sendDataToOpenGL();
  bool checkStatus(GLuint objectId,
                   PFNGLGETSHADERIVPROC objectPropertyGetter,
                   PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
                   GLenum statusType);
  bool checkShaderStatus(GLuint shaderId);
  bool checkProgramStatus(GLuint programId);
  std::string readShaderCode(const char *filename);
  void installShaders();
};

//-------------------------------------------------------------------------------------------------
#endif