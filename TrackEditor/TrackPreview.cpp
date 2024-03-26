#include "TrackPreview.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CTrackPreview::CTrackPreview(QWidget *pParent)
  : QGLWidget(QGLFormat(QGL::SampleBuffers), pParent)
{
  xRot = 0;
  yRot = 0;
  zRot = 0;
}

//-------------------------------------------------------------------------------------------------

CTrackPreview::~CTrackPreview()
{

}

//-------------------------------------------------------------------------------------------------

static void NormalizeAngle(int &iAngle)
{
  while (iAngle < 0)
    iAngle += 360 * 16;
  while (iAngle > 360)
    iAngle -= 360 * 16;
};

//-------------------------------------------------------------------------------------------------

void CTrackPreview::initializeGL()
{
  qglClearColor(Qt::black);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -10.0);
  glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
  glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
  glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
  Draw();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::resizeGL(int iWidth, int iHeight)
{
  int iSide = qMin(iWidth, iHeight);
  glViewport((iWidth - iSide) / 2, (iHeight - iSide) / 2, iSide, iSide);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
#ifdef QT_OPENGL_ES_1
  glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
  glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
  glMatrixMode(GL_MODELVIEW);
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::mousePressEvent(QMouseEvent *pEvent)
{
  lastPos = pEvent->pos();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::mouseMoveEvent(QMouseEvent *pEvent)
{
  int dx = pEvent->x() - lastPos.x();
  int dy = pEvent->y() - lastPos.y();

  if (pEvent->buttons() & Qt::LeftButton) {
    SetXRotation(xRot + 8 * dy);
    SetYRotation(yRot + 8 * dx);
  } else if (pEvent->buttons() & Qt::RightButton) {
    SetXRotation(xRot + 8 * dy);
    SetZRotation(zRot + 8 * dx);
  }

  lastPos = pEvent->pos();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::SetXRotation(int iAngle)
{
  NormalizeAngle(iAngle);
  if (iAngle != xRot) {
    xRot = iAngle;
    updateGL();
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::SetYRotation(int iAngle)
{
  NormalizeAngle(iAngle);
  if (iAngle != yRot) {
    yRot = iAngle;
    updateGL();
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::SetZRotation(int iAngle)
{
  NormalizeAngle(iAngle);
  if (iAngle != zRot) {
    zRot = iAngle;
    updateGL();
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::Draw()
{
  qglColor(Qt::red);
  glBegin(GL_QUADS);
  glNormal3f(0, 0, -1);
  glVertex3f(-1, -1, 0);
  glVertex3f(-1, 1, 0);
  glVertex3f(1, 1, 0);
  glVertex3f(1, -1, 0);

  glEnd();
  glBegin(GL_TRIANGLES);
  glNormal3f(0, -1, 0.707);
  glVertex3f(-1, -1, 0);
  glVertex3f(1, -1, 0);
  glVertex3f(0, 0, 1.2);
  glEnd();
  glBegin(GL_TRIANGLES);
  glNormal3f(1, 0, 0.707);
  glVertex3f(1, -1, 0);
  glVertex3f(1, 1, 0);
  glVertex3f(0, 0, 1.2);
  glEnd();
  glBegin(GL_TRIANGLES);
  glNormal3f(0, 1, 0.707);
  glVertex3f(1, 1, 0);
  glVertex3f(-1, 1, 0);
  glVertex3f(0, 0, 1.2);
  glEnd();
  glBegin(GL_TRIANGLES);
  glNormal3f(-1, 0, 0.707);
  glVertex3f(-1, 1, 0);
  glVertex3f(-1, -1, 0);
  glVertex3f(0, 0, 1.2);
  glEnd();
}

//-------------------------------------------------------------------------------------------------