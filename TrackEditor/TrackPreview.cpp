#include "glew.h"
#include "TrackPreview.h"
#include "MainWindow.h"
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
#include "Camera.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "OpenGLDebug.h"
#include "Track.h"
#include "DisplaySettings.h"
#include "ShapeData.h"
#include "ShapeFactory.h"
#include "Texture.h"
#include "qevent.h"
#include "qdir.h"
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
    , m_pAILine1(NULL)
    , m_pAILine2(NULL)
    , m_pAILine3(NULL)
    , m_pAILine4(NULL)
    , m_pAxes(NULL)
    , m_pCar(NULL)
  {};
  ~CTrackPreviewPrivate()
  {
    DeleteModels();
    if (m_pCar) {
      delete m_pCar;
      m_pCar = NULL;
    }
    if (m_pShader) {
      delete m_pShader;
      m_pShader = NULL;
    }
    if (m_pAxes) {
      delete m_pAxes;
      m_pAxes = NULL;
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
    if (m_pAILine1) {
      delete m_pAILine1;
      m_pAILine1 = NULL;
    }
    if (m_pAILine2) {
      delete m_pAILine2;
      m_pAILine2 = NULL;
    }
    if (m_pAILine3) {
      delete m_pAILine3;
      m_pAILine3 = NULL;
    }
    if (m_pAILine4) {
      delete m_pAILine4;
      m_pAILine4 = NULL;
    }
    if (m_pSelection) {
      delete m_pSelection;
      m_pSelection = NULL;
    }
    for (std::vector<CShapeData *>::iterator it = m_signAy.begin(); it != m_signAy.end(); ++it) {
      delete *it;
    }
    m_signAy.clear();
    for (std::vector<CShapeData *>::iterator it = m_audioAy.begin(); it != m_audioAy.end(); ++it) {
      delete *it;
    }
    m_audioAy.clear();
    for (std::vector<CShapeData *>::iterator it = m_stuntAy.begin(); it != m_stuntAy.end(); ++it) {
      delete *it;
    }
    m_stuntAy.clear();
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
  CShapeData *m_pAILine1;
  CShapeData *m_pAILine2;
  CShapeData *m_pAILine3;
  CShapeData *m_pAILine4;
  CShapeData *m_pAxes;
  std::vector<CShapeData *> m_signAy;
  std::vector<CShapeData *> m_audioAy;
  std::vector<CShapeData *> m_stuntAy;
  CShader *m_pShader;
  CTrack *m_pTrack;

  CShapeData *m_pCar;
  CTexture m_carTex;
};

//-------------------------------------------------------------------------------------------------

CTrackPreview::CTrackPreview(QWidget *pParent)
  : QGLWidget(QGLFormat(QGL::SampleBuffers), pParent)
  , m_uiShowModels(0)
  , m_iFrom(0)
  , m_carModel(eWhipModel::CAR_ZIZIN)
  , m_carAILine(eShapeSection::AILINE1)
  , m_bMillionPlus(false)
  , m_bAttachLast(false)
  , m_iScale(1)
{
  p = new CTrackPreviewPrivate;

  connect(g_pMainWindow, &CMainWindow::UpdateGeometrySelectionSig, this, &CTrackPreview::UpdateGeometrySelection);
}

//-------------------------------------------------------------------------------------------------

CTrackPreview::~CTrackPreview()
{
  glUseProgram(0);
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::SetTrack(CTrack *pTrack)
{
  p->DeleteModels();
  p->m_pTrack = pTrack;

  CShapeFactory::GetShapeFactory().m_fScale = 10000.0f / m_iScale;
  if (p->m_pTrack && !p->m_pTrack->m_chunkAy.empty()) {
    p->m_pTrack->m_fScale = 10000.0f / m_iScale;
    p->m_pTrack->GenerateTrackMath();

    p->m_pLLaneSurf      = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::LLANE, m_bAttachLast);
    p->m_pLLaneWire      = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::LLANE, m_bAttachLast, true);
    p->m_pRLaneSurf      = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::RLANE, m_bAttachLast);
    p->m_pRLaneWire      = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::RLANE, m_bAttachLast, true);
    p->m_pLShoulderSurf  = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::LSHOULDER, m_bAttachLast);
    p->m_pLShoulderWire  = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::LSHOULDER, m_bAttachLast, true);
    p->m_pRShoulderSurf  = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::RSHOULDER, m_bAttachLast);
    p->m_pRShoulderWire  = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::RSHOULDER, m_bAttachLast, true);
    p->m_pLWallSurf      = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::LWALL, m_bAttachLast);
    p->m_pLWallWire      = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::LWALL, m_bAttachLast, true);
    p->m_pRWallSurf      = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::RWALL, m_bAttachLast);
    p->m_pRWallWire      = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::RWALL, m_bAttachLast, true);
    p->m_pRoofSurf       = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::ROOF, m_bAttachLast);
    p->m_pRoofWire       = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::ROOF, m_bAttachLast, true);
    p->m_pEnvirFloorSurf = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::ENVIRFLOOR, m_bAttachLast);
    p->m_pEnvirFloorWire = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::ENVIRFLOOR, m_bAttachLast, true);
    p->m_pOWallFloorSurf = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::OWALLFLOOR, m_bAttachLast);
    p->m_pOWallFloorWire = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::OWALLFLOOR, m_bAttachLast, true);
    p->m_pLLOWallSurf    = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::LLOWALL, m_bAttachLast);
    p->m_pLLOWallWire    = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::LLOWALL, m_bAttachLast, true);
    p->m_pRLOWallSurf    = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::RLOWALL, m_bAttachLast);
    p->m_pRLOWallWire    = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::RLOWALL, m_bAttachLast, true);
    p->m_pLUOWallSurf    = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::LUOWALL, m_bAttachLast);
    p->m_pLUOWallWire    = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::LUOWALL, m_bAttachLast, true);
    p->m_pRUOWallSurf    = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::RUOWALL, m_bAttachLast);
    p->m_pRUOWallWire    = CShapeFactory::GetShapeFactory().MakeTrackSurface(p->m_pShader, p->m_pTrack->m_pTex, p->m_pTrack, eShapeSection::RUOWALL, m_bAttachLast, true);
    p->m_pAILine1        = CShapeFactory::GetShapeFactory().MakeAILine(p->m_pShader, p->m_pTrack, eShapeSection::AILINE1, m_bAttachLast);
    p->m_pAILine2        = CShapeFactory::GetShapeFactory().MakeAILine(p->m_pShader, p->m_pTrack, eShapeSection::AILINE2, m_bAttachLast);
    p->m_pAILine3        = CShapeFactory::GetShapeFactory().MakeAILine(p->m_pShader, p->m_pTrack, eShapeSection::AILINE3, m_bAttachLast);
    p->m_pAILine4        = CShapeFactory::GetShapeFactory().MakeAILine(p->m_pShader, p->m_pTrack, eShapeSection::AILINE4, m_bAttachLast);
    CShapeFactory::GetShapeFactory().MakeSigns(p->m_pShader, p->m_pTrack->m_pBld, p->m_pTrack, p->m_signAy);
    CShapeFactory::GetShapeFactory().MakeAudio(p->m_pShader, p->m_pTrack, p->m_audioAy);
    CShapeFactory::GetShapeFactory().MakeStunts(p->m_pShader, p->m_pTrack, p->m_stuntAy);

    UpdateCar(m_carModel, m_carAILine, m_bMillionPlus);
    UpdateGeometrySelection(g_pMainWindow->GetSelFrom(), g_pMainWindow->GetSelTo());;
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
  m_iFrom = iFrom;

  if (p->m_pSelection)
    delete p->m_pSelection;
  if (p->m_pTrack) {
    p->m_pSelection = CShapeFactory::GetShapeFactory().MakeSelectedChunks(p->m_pShader, p->m_pTrack, iFrom, iTo);
  }

  if (p->m_pCar && p->m_pTrack)
    CShapeFactory::GetShapeFactory().GetCarPos(p->m_pTrack, m_iFrom, m_carAILine, p->m_pCar->m_modelToWorldMatrix, m_bMillionPlus);

  repaint();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::UpdateCar(eWhipModel carModel, eShapeSection aiLine, bool bMillionPlus)
{
  m_carModel = carModel;
  m_carAILine = aiLine;
  m_bMillionPlus = bMillionPlus;

  if (p->m_pCar) {
    delete p->m_pCar;
    p->m_pCar = NULL;
  }

  if (p->m_pTrack && p->m_pTrack->m_pPal && !g_pMainWindow->GetTrackFilesFolder().isEmpty()) {
    QString sTexName;
    switch (carModel) {
      case CAR_F1WACK:
        sTexName = "RED28.BM";
        break;
      case CAR_AUTO:
        sTexName = "XAUTO.BM";
        break;
      case CAR_DESILVA:
        sTexName = "XDESILVA.BM";
        break;
      case CAR_PULSE:
        sTexName = "XPULSE.BM";
        break;
      case CAR_GLOBAL:
        sTexName = "XGLOBAL.BM";
        break;
      case CAR_MILLION:
        sTexName = "XMILLION.BM";
        break;
      case CAR_MISSION:
        sTexName = "XMISSION.BM";
        break;
      case CAR_ZIZIN:
        sTexName = "XZIZIN.BM";
        break;
      case CAR_REISE:
        sTexName = "XREISE.BM";
        break;
    }
    QString sPal = g_pMainWindow->GetTrackFilesFolder() + QDir::separator() + "PALETTE.PAL";
    QString sTex = g_pMainWindow->GetTrackFilesFolder() + QDir::separator() + sTexName;
    p->m_carTex.LoadTexture(sTex.toLatin1().constData(), p->m_pTrack->m_pPal);
    p->m_pCar = CShapeFactory::GetShapeFactory().MakeModel(p->m_pShader, &p->m_carTex, carModel);
  }

  if (p->m_pCar && p->m_pTrack)
    CShapeFactory::GetShapeFactory().GetCarPos(p->m_pTrack, m_iFrom, m_carAILine, p->m_pCar->m_modelToWorldMatrix, m_bMillionPlus);

  repaint();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::SetScale(int iScale)
{
  m_iScale = iScale;
  SetTrack(p->m_pTrack);
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::AttachLast(bool bAttachLast)
{
  m_bAttachLast = bAttachLast;
  SetTrack(p->m_pTrack);
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

  glm::mat4 viewToProjectionMatrix = glm::perspective(glm::radians(60.0f), ((float)width()) / height(), 0.1f, 200.0f);
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
  if (m_uiShowModels & SHOW_AILINE_MODELS
      && p->m_pAILine1
      && p->m_pAILine2
      && p->m_pAILine3
      && p->m_pAILine4) {
    p->m_pAILine1->Draw(worldToProjectionMatrix);
    p->m_pAILine2->Draw(worldToProjectionMatrix);
    p->m_pAILine3->Draw(worldToProjectionMatrix);
    p->m_pAILine4->Draw(worldToProjectionMatrix);
  }
  if (m_uiShowModels & SHOW_TEST_CAR && p->m_pCar)
    p->m_pCar->Draw(worldToProjectionMatrix);
  if (m_uiShowModels & SHOW_SIGNS) {
    for (std::vector<CShapeData *>::iterator it = p->m_signAy.begin(); it != p->m_signAy.end(); ++it) {
      (*it)->Draw(worldToProjectionMatrix);
    }
  }
  if (m_uiShowModels & SHOW_AUDIO) {
    for (std::vector<CShapeData *>::iterator it = p->m_audioAy.begin(); it != p->m_audioAy.end(); ++it) {
      (*it)->Draw(worldToProjectionMatrix);
    }
  }
  if (m_uiShowModels & SHOW_STUNTS) {
    for (std::vector<CShapeData *>::iterator it = p->m_stuntAy.begin(); it != p->m_stuntAy.end(); ++it) {
      (*it)->Draw(worldToProjectionMatrix);
    }
  }
  //if (p->m_pAxes)
  //  p->m_pAxes->Draw(worldToProjectionMatrix);
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::Shutdown()
{
  if (p) {
    delete p;
    p = NULL;
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::ReloadCar()
{
  UpdateCar(m_carModel, m_carAILine, m_bMillionPlus);
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

  //p->m_pAxes = DebugShapes::MakeAxes(p->m_pShader);
  UpdateCar(m_carModel, m_carAILine, m_bMillionPlus);
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