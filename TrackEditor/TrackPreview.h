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
  void resizeGL(int iWidth, int iHeight);
  void mousePressEvent(QMouseEvent *pEvent);
  void mouseMoveEvent(QMouseEvent *pEvent);
  void keyPressEvent(QKeyEvent *pEvent);

private:
  bool CheckStatus(GLuint objectId,
                   PFNGLGETSHADERIVPROC objectPropertyGetter,
                   PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
                   GLenum statusType);
  bool CheckShaderStatus(GLuint shaderId);
  bool CheckProgramStatus(GLuint programId);
  std::string ReadShaderCode(const char *filename);
  void InstallShaders(GLuint &programId, const char *szVertexShader, const char *szFragmentShader);
  void SetupVertexArrays();

  CTrackPreviewPrivate *p;
  tTestModel *m_pModel;
};

//-------------------------------------------------------------------------------------------------
#endif