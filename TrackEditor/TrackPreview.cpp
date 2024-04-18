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
    : m_pShader(NULL)
    , m_pTrack(NULL)
    , m_pLLaneSurf(NULL)
    , m_pLLaneWire(NULL)
    , m_pRLaneSurf(NULL)
    , m_pRLaneWire(NULL)
    , m_pLShoulderSurf(NULL)
    , m_pLShoulderWire(NULL)
    , m_pRShoulderSurf(NULL)
    , m_pRShoulderWire(NULL)
  {};
  ~CTrackPreviewPrivate()
  {
    DeleteModels();
    if (m_pShader) {
      delete m_pShader;
      m_pShader = NULL;
    }
  };
  void DeleteModels()
  {
    if (m_pLLaneSurf) {
      delete m_pLLaneSurf;
      m_pLLaneSurf = NULL;
    }
    if (m_pLLaneWire) {
      delete m_pLLaneWire;
      m_pLLaneWire= NULL;
    }
    if (m_pRLaneSurf) {
      delete m_pRLaneSurf;
      m_pRLaneSurf = NULL;
    }
    if (m_pRLaneWire) {
      delete m_pRLaneWire;
      m_pRLaneWire= NULL;
    }
    if (m_pLShoulderSurf) {
      delete m_pLShoulderSurf;
      m_pLShoulderSurf = NULL;
    }
    if (m_pLShoulderWire) {
      delete m_pLShoulderWire;
      m_pLShoulderWire= NULL;
    }
    if (m_pRShoulderSurf) {
      delete m_pRShoulderSurf;
      m_pRShoulderSurf = NULL;
    }
    if (m_pRShoulderWire) {
      delete m_pRShoulderWire;
      m_pRShoulderWire= NULL;
    }
  }

  CShapeData *m_pLLaneSurf;
  CShapeData *m_pLLaneWire;
  CShapeData *m_pRLaneSurf;
  CShapeData *m_pRLaneWire;
  CShapeData *m_pLShoulderSurf;
  CShapeData *m_pLShoulderWire;
  CShapeData *m_pRShoulderSurf;
  CShapeData *m_pRShoulderWire;
  CShader *m_pShader;
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
    p->m_pLLaneSurf = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::LLANE);
    p->m_pLLaneWire= p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::LLANE, true);
    p->m_pRLaneSurf = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::RLANE);
    p->m_pRLaneWire = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::RLANE, true);
    p->m_pLShoulderSurf = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::LSHOULDER);
    p->m_pLShoulderWire = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::LSHOULDER, true);
    p->m_pRShoulderSurf = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::RSHOULDER);
    p->m_pRShoulderWire = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::RSHOULDER, true);
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

  if (m_uiShowModels & SHOW_LLANE_SURF_MODEL && p->m_pLLaneSurf)
    p->m_pLLaneSurf->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_LLANE_WIRE_MODEL && p->m_pLLaneWire)
    p->m_pLLaneWire->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_RLANE_SURF_MODEL && p->m_pRLaneSurf)
    p->m_pRLaneSurf->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_RLANE_WIRE_MODEL && p->m_pRLaneWire)
    p->m_pRLaneWire->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_LSHOULDER_SURF_MODEL && p->m_pLShoulderSurf)
    p->m_pLShoulderSurf->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_LSHOULDER_WIRE_MODEL && p->m_pLShoulderWire)
    p->m_pLShoulderWire->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_RSHOULDER_SURF_MODEL && p->m_pRShoulderSurf)
    p->m_pRShoulderSurf->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_RSHOULDER_WIRE_MODEL && p->m_pRShoulderWire)
    p->m_pRShoulderWire->Draw(worldToProjectionMatrix);
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
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (!p->m_pShader)
    p->m_pShader = new CShader("Shaders/WhiplashVertexShader.glsl", "Shaders/WhiplashFragmentShader.glsl");
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