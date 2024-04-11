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
  {};
  ~CTrackPreviewPrivate()
  {
    m_trackSurface.Cleanup();
    m_trackWireframe.Cleanup();
    if (m_pPassThroughShader) {
      delete m_pPassThroughShader;
      m_pPassThroughShader = NULL;
    }
  };

  tShapeData m_trackSurface;
  tShapeData m_trackWireframe;
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
  p->m_trackSurface.Cleanup();
  p->m_pTrack = pTrack;
  p->m_trackSurface = p->m_pTrack->MakeTrackSurface();
  p->m_trackSurface.modelToWorldMatrix = glm::mat4(1.0);
  p->m_trackSurface.pShader = p->m_pPassThroughShader;
  p->m_trackWireframe = p->m_pTrack->MakeTrackSurface(true);
  p->m_trackWireframe.modelToWorldMatrix = glm::mat4(1.0);
  p->m_trackWireframe.pShader = p->m_pPassThroughShader;
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

  glm::mat4 fullTransformMatrix;
  glm::mat4 viewToProjectionMatrix = glm::perspective(30.0f, ((float)width()) / height(), 0.1f, 100.0f);
  glm::mat4 worldToViewMatrix = camera.GetWorldToViewMatrix();
  glm::mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

  if (!p->m_trackSurface.pShader || !p->m_trackSurface.pVertexArray || !p->m_trackSurface.pIndexBuf)
    return;
  if (!p->m_trackWireframe.pShader || !p->m_trackWireframe.pVertexArray || !p->m_trackWireframe.pIndexBuf)
    return;

  if (m_bShowSurface) {
    p->m_trackSurface.pShader->Bind();
    p->m_trackSurface.pVertexArray->Bind();
    p->m_trackSurface.pIndexBuf->Bind();
    fullTransformMatrix = worldToProjectionMatrix * p->m_trackSurface.modelToWorldMatrix;
    p->m_trackSurface.pShader->SetUniformMat4("modelToProjectionMatrix", fullTransformMatrix);
    GLCALL(glDrawElements(GL_TRIANGLES, p->m_trackSurface.pIndexBuf->GetCount(), GL_UNSIGNED_INT, 0));
  }

  if (m_bShowWireframe) {
    p->m_trackWireframe.pShader->Bind();
    p->m_trackWireframe.pVertexArray->Bind();
    p->m_trackWireframe.pIndexBuf->Bind();
    fullTransformMatrix = worldToProjectionMatrix * p->m_trackWireframe.modelToWorldMatrix;
    p->m_trackWireframe.pShader->SetUniformMat4("modelToProjectionMatrix", fullTransformMatrix);
    GLCALL(glDrawElements(GL_LINES, p->m_trackWireframe.pIndexBuf->GetCount(), GL_UNSIGNED_INT, 0));
  }
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