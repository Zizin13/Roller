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
GLuint programId;
Camera camera;
GLint fullTransformUniformLocation;
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
  glDeleteProgram(programId);
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

  GLint ambientLightUniformLocation = glGetUniformLocation(programId, "ambientLight");
  glm::vec3 ambientLight(0.3f, 0.3f, 0.3f);
  glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);

  for (CShapeAy::iterator it = p->m_shapeAy.begin(); it != p->m_shapeAy.end(); ++it) {
    glBindVertexArray((*it).vertexArrayObjId);
    fullTransformMatrix = worldToProjectionMatrix * (*it).modelToWorldMatrix;
    glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
    glDrawElements(GL_TRIANGLES, (*it).numIndices, GL_UNSIGNED_SHORT, 0);
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::SetupVertexArrays()
{
  for (CShapeAy::iterator it = p->m_shapeAy.begin(); it != p->m_shapeAy.end(); ++it) {
    glGenVertexArrays(1, &(*it).vertexArrayObjId);

    glBindVertexArray((*it).vertexArrayObjId);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, (*it).vertexBufId);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 3));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*it).indexBufId);
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

void CTrackPreview::InstallShaders()
{
  GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

  const char *adapter[1];
  std::string sTemp = ReadShaderCode("VertexShaderCode.glsl");
  adapter[0] = sTemp.c_str();
  glShaderSource(vertexShaderId, 1, adapter, 0);
  sTemp = ReadShaderCode("FragmentShaderCode.glsl");
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

  glUseProgram(programId);
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

  tShapeData teapot = ShapeGenerator::MakeTeapot(20);
  teapot.modelToWorldMatrix =
    glm::translate(glm::vec3(-3.0f, 0.0f, -6.0f)) * 
    glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  tShapeData arrow = ShapeGenerator::MakeArrow();
  arrow.modelToWorldMatrix = glm::translate(glm::vec3(0.0f, 0.0f, -3.0f));
  tShapeData plane = ShapeGenerator::MakePlane(20);
  plane.modelToWorldMatrix =
    glm::translate(glm::vec3(0.0f, 1.0f, 0.0f));

  p->m_shapeAy.push_back(teapot);
  p->m_shapeAy.push_back(arrow);
  p->m_shapeAy.push_back(plane);

  SendDataToOpenGL();
  SetupVertexArrays();
  InstallShaders();

  fullTransformUniformLocation = glGetUniformLocation(programId, "fullTransformMatrix");
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