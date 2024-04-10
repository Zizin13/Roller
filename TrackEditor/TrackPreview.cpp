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
    m_trackModel.Cleanup();
    if (m_pPassThroughShader) {
      delete m_pPassThroughShader;
      m_pPassThroughShader = NULL;
    }
  };

  tShapeData m_trackModel;
  CShader *m_pPassThroughShader;
  CTrack *m_pTrack;
};

//-------------------------------------------------------------------------------------------------

CTrackPreview::CTrackPreview(QWidget *pParent)
  : QGLWidget(QGLFormat(QGL::SampleBuffers), pParent)
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
  p->m_trackModel.Cleanup();
  p->m_pTrack = pTrack;
  p->m_trackModel = p->m_pTrack->MakeTrackCenterline();
  p->m_trackModel.modelToWorldMatrix =
    glm::translate(glm::vec3(4.0f, -1.0f, -1.0f));
  p->m_trackModel.pShader = p->m_pPassThroughShader;
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

  if (!p->m_trackModel.pShader || !p->m_trackModel.pVertexArray || !p->m_trackModel.pIndexBuf)
    return;

  p->m_trackModel.pShader->Bind();
  p->m_trackModel.pVertexArray->Bind();
  p->m_trackModel.pIndexBuf->Bind();
  fullTransformMatrix = worldToProjectionMatrix * p->m_trackModel.modelToWorldMatrix;
  p->m_trackModel.pShader->SetUniformMat4("modelToProjectionMatrix", fullTransformMatrix);
  GLCALL(glDrawElements(GL_LINES, p->m_trackModel.pIndexBuf->GetCount(), GL_UNSIGNED_INT, 0));
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