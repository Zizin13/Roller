#include "glew.h"
#include "TrackPreview.h"
#include "MainWindow.h"
#include "ShapeGenerator.h"
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
#include "Camera.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "OpenGLDebug.h"
#include "Track.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

Camera camera;

//-------------------------------------------------------------------------------------------------

class CTrackPreviewPrivate
{
public:
  CTrackPreviewPrivate()
    : m_pPassThroughShader(NULL)
    , m_pTrack(NULL)
    , m_pTrackSurface(NULL)
    , m_pTrackWireframe(NULL)
  {};
  ~CTrackPreviewPrivate()
  {
    DeleteModels();
    if (m_pPassThroughShader) {
      delete m_pPassThroughShader;
      m_pPassThroughShader = NULL;
    }
  };
  void DeleteModels()
  {
    if (m_pTrackSurface) {
      delete m_pTrackSurface;
      m_pTrackSurface = NULL;
    }
    if (m_pTrackWireframe) {
      delete m_pTrackWireframe;
      m_pTrackWireframe = NULL;
    }
  }

  CShapeData *m_pTrackSurface;
  CShapeData *m_pTrackWireframe;
  CShader *m_pPassThroughShader;
  CTrack *m_pTrack;
};

//-------------------------------------------------------------------------------------------------

CTrackPreview::CTrackPreview(QWidget *pParent)
  : QGLWidget(QGLFormat(QGL::SampleBuffers), pParent)
  , m_bShowSurface(true)
  , m_bShowWireframe(false)
{
  p = new CTrackPreviewPrivate;
}

//-------------------------------------------------------------------------------------------------

CTrackPreview::~CTrackPreview()
{
  glUseProgram(0);
  if (p) {
    delete p;
    p = NULL;
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::SetTrack(CTrack *pTrack)
{
  p->DeleteModels();
  p->m_pTrack = pTrack;
  if (p->m_pTrack) {
    p->m_pTrackSurface = p->m_pTrack->MakeTrackSurface(p->m_pPassThroughShader);
    p->m_pTrackWireframe = p->m_pTrack->MakeTrackSurface(p->m_pPassThroughShader, true);
  }
  repaint();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::ShowModels(bool bShowSurface, bool bShowWireframe)
{
  m_bShowSurface = bShowSurface;
  m_bShowWireframe = bShowWireframe;
  repaint();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::paintGL()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, width(), height());

  glm::mat4 viewToProjectionMatrix = glm::perspective(30.0f, ((float)width()) / height(), 0.1f, 100.0f);
  glm::mat4 worldToViewMatrix = camera.GetWorldToViewMatrix();
  glm::mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

  if (m_bShowSurface && p->m_pTrackSurface)
    p->m_pTrackSurface->Draw(worldToProjectionMatrix);
  if (m_bShowWireframe && p->m_pTrackWireframe)
    p->m_pTrackWireframe->Draw(worldToProjectionMatrix);
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::initializeGL()
{
  setMouseTracking(false);
  if (glewInit() != GLEW_OK)
    assert(0);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(GLErrorCb, 0);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);

  if (!p->m_pPassThroughShader)
    p->m_pPassThroughShader = new CShader("Shaders/VertexShaderPassthroughCode.glsl", "Shaders/FragmentShaderPassthroughCode.glsl");
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::mouseMoveEvent(QMouseEvent *pEvent)
{
  setFocus();
  camera.MouseUpdate(glm::vec2(pEvent->x(), pEvent->y()));
  repaint();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::keyPressEvent(QKeyEvent *pEvent)
{
  switch (pEvent->key()) {
    case Qt::Key::Key_W:
      camera.MoveForward();
      break;
    case Qt::Key::Key_S:
      camera.MoveBackward();
      break;
    case Qt::Key::Key_A:
      camera.StrafeLeft();
      break;
    case Qt::Key::Key_D:
      camera.StrafeRight();
      break;
    case Qt::Key::Key_R:
      camera.MoveUp();
      break;
    case Qt::Key::Key_F:
      camera.MoveDown();
      break;
  }
  repaint();
}

//-------------------------------------------------------------------------------------------------