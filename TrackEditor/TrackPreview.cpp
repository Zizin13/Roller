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
#include "DisplaySettings.h"
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
    , m_pLLane(NULL)
    , m_pLLaneWireframe(NULL)
    , m_pRLane(NULL)
    , m_pRLaneWireframe(NULL)
    , m_pLShoulder(NULL)
    , m_pLShoulderWireframe(NULL)
    , m_pRShoulder(NULL)
    , m_pRShoulderWireframe(NULL)
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
    if (m_pLLane) {
      delete m_pLLane;
      m_pLLane = NULL;
    }
    if (m_pLLaneWireframe) {
      delete m_pLLaneWireframe;
      m_pLLaneWireframe = NULL;
    }
    if (m_pRLane) {
      delete m_pRLane;
      m_pRLane = NULL;
    }
    if (m_pRLaneWireframe) {
      delete m_pRLaneWireframe;
      m_pRLaneWireframe = NULL;
    }
    if (m_pLShoulder) {
      delete m_pLShoulder;
      m_pLShoulder = NULL;
    }
    if (m_pLShoulderWireframe) {
      delete m_pLShoulderWireframe;
      m_pLShoulderWireframe = NULL;
    }
    if (m_pRShoulder) {
      delete m_pRShoulder;
      m_pRShoulder = NULL;
    }
    if (m_pRShoulderWireframe) {
      delete m_pRShoulderWireframe;
      m_pRShoulderWireframe = NULL;
    }
  }

  CShapeData *m_pLLane;
  CShapeData *m_pLLaneWireframe;
  CShapeData *m_pRLane;
  CShapeData *m_pRLaneWireframe;
  CShapeData *m_pLShoulder;
  CShapeData *m_pLShoulderWireframe;
  CShapeData *m_pRShoulder;
  CShapeData *m_pRShoulderWireframe;
  CShader *m_pPassThroughShader;
  CTrack *m_pTrack;
};

//-------------------------------------------------------------------------------------------------

CTrackPreview::CTrackPreview(QWidget *pParent)
  : QGLWidget(QGLFormat(QGL::SampleBuffers), pParent)
  , m_uiShowModels(0)
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
    p->m_pLLane = p->m_pTrack->MakeTrackSurface(p->m_pPassThroughShader, eShapeSection::LLANE);
    p->m_pLLaneWireframe = p->m_pTrack->MakeTrackSurface(p->m_pPassThroughShader, eShapeSection::LLANE, true);
    p->m_pRLane = p->m_pTrack->MakeTrackSurface(p->m_pPassThroughShader, eShapeSection::RLANE);
    p->m_pRLaneWireframe = p->m_pTrack->MakeTrackSurface(p->m_pPassThroughShader, eShapeSection::RLANE, true);
    p->m_pLShoulder = p->m_pTrack->MakeTrackSurface(p->m_pPassThroughShader, eShapeSection::LSHOULDER);
    p->m_pLShoulderWireframe = p->m_pTrack->MakeTrackSurface(p->m_pPassThroughShader, eShapeSection::LSHOULDER, true);
    p->m_pRShoulder = p->m_pTrack->MakeTrackSurface(p->m_pPassThroughShader, eShapeSection::RSHOULDER);
    p->m_pRShoulderWireframe = p->m_pTrack->MakeTrackSurface(p->m_pPassThroughShader, eShapeSection::RSHOULDER, true);
  }
  repaint();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::ShowModels(uint32 uiShowModels)
{
  m_uiShowModels = uiShowModels;
  repaint();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::paintGL()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, width(), height());

  glm::mat4 viewToProjectionMatrix = glm::perspective(glm::radians(60.0f), ((float)width()) / height(), 0.1f, 100.0f);
  glm::mat4 worldToViewMatrix = camera.GetWorldToViewMatrix();
  glm::mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

  if (m_uiShowModels & SHOW_LLANE_SURF_MODEL && p->m_pLLane)
    p->m_pLLane->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_LLANE_WIRE_MODEL && p->m_pLLaneWireframe)
    p->m_pLLaneWireframe->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_RLANE_SURF_MODEL && p->m_pRLane)
    p->m_pRLane->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_RLANE_WIRE_MODEL && p->m_pRLaneWireframe)
    p->m_pRLaneWireframe->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_LSHOULDER_SURF_MODEL && p->m_pLShoulder)
    p->m_pLShoulder->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_LSHOULDER_WIRE_MODEL && p->m_pLShoulderWireframe)
    p->m_pLShoulderWireframe->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_RSHOULDER_SURF_MODEL && p->m_pRShoulder)
    p->m_pRShoulder->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_RSHOULDER_WIRE_MODEL && p->m_pRShoulderWireframe)
    p->m_pRShoulderWireframe->Draw(worldToProjectionMatrix);
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