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
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

Camera camera;
typedef std::vector<tShapeData> CShapeAy;

//-------------------------------------------------------------------------------------------------

class CTrackPreviewPrivate
{
public:
  CTrackPreviewPrivate()
    : m_pLightingShader(NULL)
    , m_pPassThroughShader(NULL)
  {};
  ~CTrackPreviewPrivate()
  {
    for (CShapeAy::iterator it = m_shapeAy.begin(); it != m_shapeAy.end(); ++it) {
      (*it).Cleanup();
    }
    if (m_pLightingShader) {
      delete m_pLightingShader;
      m_pLightingShader = NULL;
    }
    if (m_pPassThroughShader) {
      delete m_pPassThroughShader;
      m_pPassThroughShader = NULL;
    }
  };

  CShapeAy m_shapeAy;
  CShader *m_pLightingShader;
  CShader *m_pPassThroughShader;
};

//-------------------------------------------------------------------------------------------------

CTrackPreview::CTrackPreview(QWidget *pParent)
  : QGLWidget(QGLFormat(QGL::SampleBuffers), pParent)
  , m_pModel(NULL)
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

void CTrackPreview::SetModel(tTestModel *pModel)
{
  m_pModel = pModel;
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::paintGL()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, width(), height());

  glm::mat4 fullTransformMatrix;
  glm::mat4 viewToProjectionMatrix = glm::perspective(30.0f, ((float)width()) / height(), 0.1f, 20.0f);
  glm::mat4 worldToViewMatrix = camera.GetWorldToViewMatrix();
  glm::mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

  glm::vec4 ambientLight(0.1f, 0.1f, 0.1f, 1.0f);

  glm::vec3 lightPositionWorld = glm::vec3(0.0f, -3.0f, 0.0f);
  if (m_pModel) {
    lightPositionWorld = m_pModel->lightPosition;
    p->m_shapeAy[5].modelToWorldMatrix =
      glm::translate(lightPositionWorld) *
      glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));
  }

  glm::vec3 eyePositionWorld = camera.GetPosition();

  for (CShapeAy::iterator it = p->m_shapeAy.begin(); it != p->m_shapeAy.end(); ++it) {
    (*it).pShader->Bind();
    (*it).pVertexArray->Bind();
    (*it).pIndexBuf->Bind();
    fullTransformMatrix = worldToProjectionMatrix * (*it).modelToWorldMatrix;
    (*it).pShader->SetUniformMat4("modelToProjectionMatrix", fullTransformMatrix);
    (*it).pShader->SetUniformVec4("ambientLight", ambientLight);
    (*it).pShader->SetUniformVec3("lightPositionWorld", lightPositionWorld);
    (*it).pShader->SetUniformVec3("eyePositionWorld", eyePositionWorld);
    (*it).pShader->SetUniformMat4("modelToWorldMatrix", (*it).modelToWorldMatrix);
    GLCALL(glDrawElements(GL_TRIANGLES, (*it).pIndexBuf->GetCount(), GL_UNSIGNED_INT, 0));
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

  if (!p->m_pLightingShader)
    p->m_pLightingShader = new CShader("Shaders/VertexShaderCode.glsl", "Shaders/FragmentShaderCode.glsl");
  if (!p->m_pPassThroughShader)
    p->m_pPassThroughShader = new CShader("Shaders/VertexShaderPassthroughCode.glsl", "Shaders/FragmentShaderPassthroughCode.glsl");

  tShapeData teapot = ShapeGenerator::MakeTeapot(20);
  teapot.modelToWorldMatrix =
    glm::translate(glm::vec3(-3.0f, 1.0f, -6.0f)) * 
    glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  teapot.pShader = p->m_pLightingShader;
  tShapeData arrow = ShapeGenerator::MakeArrow();
  arrow.modelToWorldMatrix = 
    glm::translate(glm::vec3(0.0f, -2.0f, -8.0f)) *
    glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  arrow.pShader = p->m_pLightingShader;
  tShapeData plane = ShapeGenerator::MakePlane(20);
  plane.modelToWorldMatrix =
    glm::translate(glm::vec3(0.0f, 1.0f, 0.0f));
  plane.pShader = p->m_pLightingShader;
  tShapeData cube = ShapeGenerator::MakeCube();
  cube.modelToWorldMatrix =
    glm::translate(glm::vec3(0.0f, -3.0f, 0.0f)) *
    glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));
  cube.pShader = p->m_pPassThroughShader;
  tShapeData torus = ShapeGenerator::MakeTorus();
  torus.modelToWorldMatrix =
    glm::translate(glm::vec3(3.0f, -2.0f, -5.0f));
  torus.pShader = p->m_pLightingShader;
  tShapeData sphere = ShapeGenerator::MakeSphere();
  sphere.modelToWorldMatrix =
    glm::translate(glm::vec3(4.0f, -1.0f, -1.0f));
  sphere.pShader = p->m_pLightingShader;

  p->m_shapeAy.push_back(teapot);
  p->m_shapeAy.push_back(arrow);
  p->m_shapeAy.push_back(plane);
  p->m_shapeAy.push_back(torus);
  p->m_shapeAy.push_back(sphere);
  p->m_shapeAy.push_back(cube);
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