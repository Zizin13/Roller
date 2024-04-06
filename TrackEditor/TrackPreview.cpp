#include "glew.h"
#include "TrackPreview.h"
#include "MainWindow.h"
#include <fstream>
#include "ShapeGenerator.h"
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
#include "Camera.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
  x;\
  ASSERT(GLLogCall(#x,__FILE__,__LINE__))
//-------------------------------------------------------------------------------------------------

static void GLErrorCb(GLenum source,
                      GLenum type,
                      GLuint id,
                      GLenum severity,
                      GLsizei length,
                      const GLchar *message,
                      const void *userParam)
{
  (void)(source); (void)(type); (void)(id); (void)(severity); (void)(length); (void)(userParam);
  g_pMainWindow->LogMessage("OpenGL Debug: " + QString(message));
}

//-------------------------------------------------------------------------------------------------

static void GLClearError()
{
  while (glGetError());
}

//-------------------------------------------------------------------------------------------------

static bool GLLogCall(const char *szFunction, const char *szFile, int iLine)
{
  while (GLenum error = glGetError()) {
    char szOut[100];
    snprintf(szOut, sizeof(szOut), "OpenGL Error (%d): %s %s %d", (int)error, szFunction, szFile, iLine);
    g_pMainWindow->LogMessage(szOut);
    return false;
  }
  return true;
}

//-------------------------------------------------------------------------------------------------

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 9;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint g_programId;
GLuint g_passThroughProgramId;
Camera camera;
GLint modelToProjectionMatrixUniformLocation;
GLint passThroughModelToProjectionMatrixUniformLocation;

//-------------------------------------------------------------------------------------------------
typedef std::vector<tShapeData> CShapeAy;
//-------------------------------------------------------------------------------------------------

class CTrackPreviewPrivate
{
public:
  CTrackPreviewPrivate() {};
  ~CTrackPreviewPrivate()
  {
    for (CShapeAy::iterator it = m_shapeAy.begin(); it != m_shapeAy.end(); ++it) {
      glDeleteBuffers(1, &(*it).vertexBufId);
      glDeleteBuffers(1, &(*it).indexBufId);
      (*it).Cleanup();
    }
  };

  CShapeAy m_shapeAy;
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
  if (p) {
    delete p;
    p = NULL;
  }
  glUseProgram(0);
  glDeleteProgram(g_programId);
  glDeleteProgram(g_passThroughProgramId);
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::SendDataToOpenGL()
{
  for (CShapeAy::iterator it = p->m_shapeAy.begin(); it != p->m_shapeAy.end(); ++it) {
    GLCALL(glGenBuffers(1, &(*it).vertexBufId));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, (*it).vertexBufId));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, (*it).VertexBufSize(), (*it).vertices, GL_STATIC_DRAW));

    GLCALL(glGenBuffers(1, &(*it).indexBufId));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*it).indexBufId));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*it).IndexBufSize(), (*it).indices, GL_STATIC_DRAW));
  }
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

  glm::vec3 eyePositionWorld = camera.GetPosition();

  for (CShapeAy::iterator it = p->m_shapeAy.begin(); it != p->m_shapeAy.end(); ++it) {
    GLCALL(glUseProgram((*it).shaderProgramId));
    GLCALL(glBindVertexArray((*it).vertexArrayObjId));
    fullTransformMatrix = worldToProjectionMatrix * (*it).modelToWorldMatrix;
    if ((*it).shaderProgramId == g_programId) {
      GLint ambientLightUniformLocation = glGetUniformLocation(g_programId, "ambientLight");
      glUniform4fv(ambientLightUniformLocation, 1, &ambientLight[0]);
      GLint lightPositionUniformLocation = glGetUniformLocation(g_programId, "lightPositionWorld");
      glUniform3fv(lightPositionUniformLocation, 1, &lightPositionWorld[0]);
      GLint eyePositionUniformLocation = glGetUniformLocation(g_programId, "eyePositionWorld");
      glUniform3fv(eyePositionUniformLocation, 1, &eyePositionWorld[0]);
      GLCALL(glUniformMatrix4fv(modelToProjectionMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]));
      GLint modelToWorldMatUniformLocation = glGetUniformLocation(g_programId, "modelToWorldMatrix");
      GLCALL(glUniformMatrix4fv(modelToWorldMatUniformLocation, 1, GL_FALSE, &(*it).modelToWorldMatrix[0][0]));
    } else {
      GLCALL(glUniformMatrix4fv(passThroughModelToProjectionMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]));
    }
    GLCALL(glDrawElements(GL_TRIANGLES, (*it).numIndices, GL_UNSIGNED_SHORT, 0));
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::SetupVertexArrays()
{
  for (CShapeAy::iterator it = p->m_shapeAy.begin(); it != p->m_shapeAy.end(); ++it) {
    GLCALL(glGenVertexArrays(1, &(*it).vertexArrayObjId));
    GLCALL(glBindVertexArray((*it).vertexArrayObjId));
    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glEnableVertexAttribArray(1));
    GLCALL(glEnableVertexAttribArray(2));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, (*it).vertexBufId));
    GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0));
    GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 3)));
    GLCALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 6)));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*it).indexBufId));
  }
}

//-------------------------------------------------------------------------------------------------

bool CTrackPreview::CheckStatus(GLuint objectId,
                 PFNGLGETSHADERIVPROC objectPropertyGetter,
                 PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
                 GLenum statusType)
{
  GLint status;
  objectPropertyGetter(objectId, statusType, &status);
  if (status != GL_TRUE) {
    GLint infoLogLength;
    objectPropertyGetter(objectId, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar *buffer = new GLchar[infoLogLength];
    GLsizei bufferSize;
    getInfoLogFunc(objectId, infoLogLength, &bufferSize, buffer);

    g_pMainWindow->LogMessage(buffer);
    assert(0);

    delete[] buffer;
    return false;
  }
  return true;

}

//-------------------------------------------------------------------------------------------------

bool CTrackPreview::CheckShaderStatus(GLuint shaderId)
{
  return CheckStatus(shaderId, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

//-------------------------------------------------------------------------------------------------

bool CTrackPreview::CheckProgramStatus(GLuint programId)
{
  return CheckStatus(programId, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

//-------------------------------------------------------------------------------------------------

std::string CTrackPreview::ReadShaderCode(const char *filename)
{
  std::ifstream stream(filename);
  if (!stream.good()) {
    assert(0);
  }
  return std::string(
    std::istreambuf_iterator<char>(stream),
    std::istreambuf_iterator<char>());
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::InstallShaders(GLuint &programId, const char *szVertexShader, const char *szFragmentShader)
{
  GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

  const char *adapter[1];
  std::string sTemp = ReadShaderCode(szVertexShader);
  adapter[0] = sTemp.c_str();
  glShaderSource(vertexShaderId, 1, adapter, 0);
  sTemp = ReadShaderCode(szFragmentShader);
  adapter[0] = sTemp.c_str();
  glShaderSource(fragmentShaderId, 1, adapter, 0);
  glCompileShader(vertexShaderId);
  glCompileShader(fragmentShaderId);
  if (!CheckShaderStatus(vertexShaderId) || !CheckShaderStatus(fragmentShaderId))
    return;
  programId = glCreateProgram();
  glAttachShader(programId, vertexShaderId);
  glAttachShader(programId, fragmentShaderId);
  glLinkProgram(programId);
  if (!CheckProgramStatus(programId))
    return;
  glDeleteShader(vertexShaderId);
  glDeleteShader(fragmentShaderId);
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::initializeGL()
{
  setMouseTracking(true);
  if (glewInit() != GLEW_OK)
    assert(0);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(GLErrorCb, 0);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);

  InstallShaders(g_programId,
                 "VertexShaderCode.glsl",
                 "FragmentShaderCode.glsl");
  InstallShaders(g_passThroughProgramId,
                 "VertexShaderPassthroughCode.glsl",
                 "FragmentShaderPassthroughCode.glsl");

  tShapeData teapot = ShapeGenerator::MakeTeapot(20);
  teapot.modelToWorldMatrix =
    glm::translate(glm::vec3(-3.0f, 1.0f, -6.0f)) * 
    glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  teapot.shaderProgramId = g_programId;
  tShapeData arrow = ShapeGenerator::MakeArrow();
  arrow.modelToWorldMatrix = 
    glm::translate(glm::vec3(0.0f, -2.0f, -8.0f)) *
    glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  arrow.shaderProgramId = g_programId;
  tShapeData plane = ShapeGenerator::MakePlane(20);
  plane.modelToWorldMatrix =
    glm::translate(glm::vec3(0.0f, 1.0f, 0.0f));
  plane.shaderProgramId = g_programId;
  tShapeData cube = ShapeGenerator::MakeCube();
  cube.modelToWorldMatrix =
    glm::translate(glm::vec3(0.0f, -3.0f, 0.0f)) *
    glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));
  cube.shaderProgramId = g_passThroughProgramId;

  p->m_shapeAy.push_back(teapot);
  p->m_shapeAy.push_back(arrow);
  p->m_shapeAy.push_back(plane);
  p->m_shapeAy.push_back(cube);

  SendDataToOpenGL();
  SetupVertexArrays();

  glUseProgram(g_passThroughProgramId);
  passThroughModelToProjectionMatrixUniformLocation = glGetUniformLocation(g_passThroughProgramId, "modelToProjectionMatrix");
  glUseProgram(g_programId);
  modelToProjectionMatrixUniformLocation = glGetUniformLocation(g_programId, "modelToProjectionMatrix");
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::resizeGL(int iWidth, int iHeight)
{
  //GLCALL(glViewport(0, 0, width(), height()));
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::mousePressEvent(QMouseEvent *pEvent)
{
  setFocus();
  //lastPos = pEvent->pos();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::mouseMoveEvent(QMouseEvent *pEvent)
{
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