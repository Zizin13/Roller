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
#include "CarData.h"
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
    , m_pLWallSurf(NULL)
    , m_pLWallWire(NULL)
    , m_pRWallSurf(NULL)
    , m_pRWallWire(NULL)
    , m_pRoofSurf(NULL)
    , m_pRoofWire(NULL)
    , m_pEnvirFloorSurf(NULL)
    , m_pEnvirFloorWire(NULL)
    , m_pOWallFloorSurf(NULL)
    , m_pOWallFloorWire(NULL)
    , m_pLLOWallSurf(NULL)
    , m_pLLOWallWire(NULL)
    , m_pRLOWallSurf(NULL)
    , m_pRLOWallWire(NULL)
    , m_pLUOWallSurf(NULL)
    , m_pLUOWallWire(NULL)
    , m_pRUOWallSurf(NULL)
    , m_pRUOWallWire(NULL)
    , m_pSelection(NULL)
    , m_pTestCar(NULL)
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
    if (m_pLWallSurf) {
      delete m_pLWallSurf;
      m_pLWallSurf = NULL;
    }
    if (m_pLWallWire) {
      delete m_pLWallWire;
      m_pLWallWire = NULL;
    }
    if (m_pRWallSurf) {
      delete m_pRWallSurf;
      m_pRWallSurf = NULL;
    }
    if (m_pRWallWire) {
      delete m_pRWallWire;
      m_pRWallWire = NULL;
    }
    if (m_pRoofSurf) {
      delete m_pRoofSurf;
      m_pRoofSurf = NULL;
    }
    if (m_pRoofWire) {
      delete m_pRoofWire;
      m_pRoofWire = NULL;
    }
    if (m_pEnvirFloorSurf) {
      delete m_pEnvirFloorSurf;
      m_pEnvirFloorSurf = NULL;
    }
    if (m_pEnvirFloorWire) {
      delete m_pEnvirFloorWire;
      m_pEnvirFloorWire = NULL;
    }
    if (m_pOWallFloorSurf) {
      delete m_pOWallFloorSurf;
      m_pOWallFloorSurf = NULL;
    }
    if (m_pOWallFloorWire) {
      delete m_pOWallFloorWire;
      m_pOWallFloorWire = NULL;
    }
    if (m_pLLOWallSurf) {
      delete m_pLLOWallSurf;
      m_pLLOWallSurf = NULL;
    }
    if (m_pLLOWallWire) {
      delete m_pLLOWallWire;
      m_pLLOWallWire = NULL;
    }
    if (m_pRLOWallSurf) {
      delete m_pRLOWallSurf;
      m_pRLOWallSurf = NULL;
    }
    if (m_pRLOWallWire) {
      delete m_pRLOWallWire;
      m_pRLOWallWire = NULL;
    }
    if (m_pLUOWallSurf) {
      delete m_pLUOWallSurf;
      m_pLUOWallSurf = NULL;
    }
    if (m_pLUOWallWire) {
      delete m_pLUOWallWire;
      m_pLUOWallWire = NULL;
    }
    if (m_pRUOWallSurf) {
      delete m_pRUOWallSurf;
      m_pRUOWallSurf = NULL;
    }
    if (m_pRUOWallWire) {
      delete m_pRUOWallWire;
      m_pRUOWallWire = NULL;
    }
    if (m_pSelection) {
      delete m_pSelection;
      m_pSelection = NULL;
    }
    if (m_pTestCar) {
      delete m_pTestCar;
      m_pTestCar = NULL;
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
  CShapeData *m_pLWallSurf;
  CShapeData *m_pLWallWire;
  CShapeData *m_pRWallSurf;
  CShapeData *m_pRWallWire;
  CShapeData *m_pRoofSurf;
  CShapeData *m_pRoofWire;
  CShapeData *m_pEnvirFloorSurf;
  CShapeData *m_pEnvirFloorWire;
  CShapeData *m_pOWallFloorSurf;
  CShapeData *m_pOWallFloorWire;
  CShapeData *m_pLLOWallSurf;
  CShapeData *m_pLLOWallWire;
  CShapeData *m_pRLOWallSurf;
  CShapeData *m_pRLOWallWire;
  CShapeData *m_pLUOWallSurf;
  CShapeData *m_pLUOWallWire;
  CShapeData *m_pRUOWallSurf;
  CShapeData *m_pRUOWallWire;
  CShapeData *m_pSelection;
  CShapeData *m_pTestCar;
  CShader *m_pShader;
  CTrack *m_pTrack;

  CCarData carData;
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
    p->m_pLWallSurf = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::LWALL);
    p->m_pLWallWire = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::LWALL, true);
    p->m_pRWallSurf = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::RWALL);
    p->m_pRWallWire = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::RWALL, true);
    p->m_pRoofSurf = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::ROOF);
    p->m_pRoofWire = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::ROOF, true);
    p->m_pEnvirFloorSurf = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::ENVIRFLOOR);
    p->m_pEnvirFloorWire = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::ENVIRFLOOR, true);
    p->m_pOWallFloorSurf = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::OWALLFLOOR);
    p->m_pOWallFloorWire = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::OWALLFLOOR, true);
    p->m_pLLOWallSurf = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::LLOWALL);
    p->m_pLLOWallWire = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::LLOWALL, true);
    p->m_pRLOWallSurf = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::RLOWALL);
    p->m_pRLOWallWire = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::RLOWALL, true);
    p->m_pLUOWallSurf = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::LUOWALL);
    p->m_pLUOWallWire = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::LUOWALL, true);
    p->m_pRUOWallSurf = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::RUOWALL);
    p->m_pRUOWallWire = p->m_pTrack->MakeTrackSurface(p->m_pShader, eShapeSection::RUOWALL, true);
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

void CTrackPreview::UpdateGeometrySelection(int iFrom, int iTo)
{
  if (p->m_pSelection)
    delete p->m_pSelection;
  if (p->m_pTrack) {
    p->m_pSelection = p->m_pTrack->MakeSelectedChunks(p->m_pShader, iFrom, iTo);
  }

  p->m_pTestCar = p->carData.MakeCar(p->m_pShader);

  repaint();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::paintGL()
{
  if (m_uiShowModels & SHOW_ENVIRFLOOR_SURF_MODEL)
    glClearColor(0.031f, 0.301f, 1.0f, 1.0f);
  else
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, width(), height());

  glm::mat4 viewToProjectionMatrix = glm::perspective(glm::radians(60.0f), ((float)width()) / height(), 0.1f, 100.0f);
  glm::mat4 worldToViewMatrix = camera.GetWorldToViewMatrix();
  glm::mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

  if (m_uiShowModels & SHOW_ENVIRFLOOR_SURF_MODEL && p->m_pEnvirFloorSurf)
    p->m_pEnvirFloorSurf->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_ENVIRFLOOR_WIRE_MODEL && p->m_pEnvirFloorWire)
    p->m_pEnvirFloorWire->Draw(worldToProjectionMatrix);
  //environment floor should never clip through track even when it is higher than it
  //so we draw it first then clear depth buffer bit
  glClear(GL_DEPTH_BUFFER_BIT);

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
  if (m_uiShowModels & SHOW_LWALL_SURF_MODEL && p->m_pLWallSurf)
    p->m_pLWallSurf->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_LWALL_WIRE_MODEL && p->m_pLWallWire)
    p->m_pLWallWire->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_RWALL_SURF_MODEL && p->m_pRWallSurf)
    p->m_pRWallSurf->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_RWALL_WIRE_MODEL && p->m_pRWallWire)
    p->m_pRWallWire->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_ROOF_SURF_MODEL && p->m_pRoofSurf)
    p->m_pRoofSurf->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_ROOF_WIRE_MODEL && p->m_pRoofWire)
    p->m_pRoofWire->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_OWALLFLOOR_SURF_MODEL && p->m_pOWallFloorSurf)
    p->m_pOWallFloorSurf->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_OWALLFLOOR_WIRE_MODEL && p->m_pOWallFloorWire)
    p->m_pOWallFloorWire->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_LLOWALL_SURF_MODEL && p->m_pLLOWallSurf)
    p->m_pLLOWallSurf->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_LLOWALL_WIRE_MODEL && p->m_pLLOWallWire)
    p->m_pLLOWallWire->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_RLOWALL_SURF_MODEL && p->m_pRLOWallSurf)
    p->m_pRLOWallSurf->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_RLOWALL_WIRE_MODEL && p->m_pRLOWallWire)
    p->m_pRLOWallWire->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_LUOWALL_SURF_MODEL && p->m_pLUOWallSurf)
    p->m_pLUOWallSurf->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_LUOWALL_WIRE_MODEL && p->m_pLUOWallWire)
    p->m_pLUOWallWire->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_RUOWALL_SURF_MODEL && p->m_pRUOWallSurf)
    p->m_pRUOWallSurf->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_RUOWALL_WIRE_MODEL && p->m_pRUOWallWire)
    p->m_pRUOWallWire->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_SELECTION_HIGHLIGHT && p->m_pSelection)
    p->m_pSelection->Draw(worldToProjectionMatrix);

  if (p->m_pTestCar)
    p->m_pTestCar->Draw(worldToProjectionMatrix);
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
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.7f);
  glLineWidth(3.0f);

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