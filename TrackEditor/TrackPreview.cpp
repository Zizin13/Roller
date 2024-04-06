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
const uint NUM_FLOATS_PER_VERTICE = 6;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programId;
Camera camera;
GLint fullTransformUniformLocation;

GLuint theBufferID;

GLuint teapotNumIndices;
GLuint arrowNumIndices;
GLuint planeNumIndices;
GLuint teapotVertexArrayObjectID;
GLuint arrowVertexArrayObjectID;
GLuint planeVertexArrayObjectID;
GLuint teapotIndexByteOffset;
GLuint arrowIndexByteOffset;
GLuint planeIndexByteOffset;

//-------------------------------------------------------------------------------------------------

CTrackPreview::CTrackPreview(QWidget *pParent)
  : QGLWidget(QGLFormat(QGL::SampleBuffers), pParent)
{
}

//-------------------------------------------------------------------------------------------------

CTrackPreview::~CTrackPreview()
{
  glDeleteBuffers(1, &theBufferID);
  glUseProgram(0);
  glDeleteProgram(programId);
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::SendDataToOpenGL()
{
  tShapeData teapot = ShapeGenerator::MakeTeapot(10);
  tShapeData arrow = ShapeGenerator::MakeArrow();
  tShapeData plane = ShapeGenerator::MakePlane(20);

  glGenBuffers(1, &theBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
  glBufferData(GL_ARRAY_BUFFER,
          teapot.VertexBufSize() + teapot.IndexBufSize() +
          arrow.VertexBufSize() + arrow.IndexBufSize() + 
          plane.VertexBufSize() + plane.IndexBufSize(), 0, GL_STATIC_DRAW);
  GLsizeiptr currentOffset = 0;
  glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.VertexBufSize(), teapot.vertices);
  currentOffset += teapot.VertexBufSize();
  glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.IndexBufSize(), teapot.indices);
  currentOffset += teapot.IndexBufSize();
  glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.VertexBufSize(), arrow.vertices);
  currentOffset += arrow.VertexBufSize();
  glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.IndexBufSize(), arrow.indices);
  currentOffset += arrow.IndexBufSize();
  glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.VertexBufSize(), plane.vertices);
  currentOffset += plane.VertexBufSize();
  glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.IndexBufSize(), plane.indices);

  teapotNumIndices = teapot.numIndices;
  arrowNumIndices = arrow.numIndices;
  planeNumIndices = plane.numIndices;

  glGenVertexArrays(1, &teapotVertexArrayObjectID);
  glGenVertexArrays(1, &arrowVertexArrayObjectID);
  glGenVertexArrays(1, &planeVertexArrayObjectID);

  glBindVertexArray(teapotVertexArrayObjectID);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

  glBindVertexArray(arrowVertexArrayObjectID);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
  GLuint arrowByteOffset = teapot.VertexBufSize() + teapot.IndexBufSize();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)arrowByteOffset);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(arrowByteOffset + sizeof(float) * 3));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

  glBindVertexArray(planeVertexArrayObjectID);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
  GLuint planeByteOffset = arrowByteOffset + arrow.VertexBufSize() + arrow.IndexBufSize();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)planeByteOffset);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(planeByteOffset + sizeof(float) * 3));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

  teapotIndexByteOffset = teapot.VertexBufSize();
  arrowIndexByteOffset = arrowByteOffset + arrow.VertexBufSize();
  planeIndexByteOffset = planeByteOffset + plane.VertexBufSize();

  teapot.Cleanup();
  arrow.Cleanup();
  plane.Cleanup();
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

  // teapot
  glBindVertexArray(teapotVertexArrayObjectID);
  glm::mat4 teapot1ModelToWorldMatrix = 
    glm::translate(glm::vec3(-3.0f, 0.0f, -6.0f)) * 
    glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  fullTransformMatrix = worldToProjectionMatrix * teapot1ModelToWorldMatrix;
  glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, teapotNumIndices, GL_UNSIGNED_SHORT, (void *)teapotIndexByteOffset);

  glm::mat4 teapot2ModelToWorldMatrix = 
    glm::translate(glm::vec3(3.0f, 0.0f, -6.75f)) * 
    glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  fullTransformMatrix = worldToProjectionMatrix * teapot2ModelToWorldMatrix;
  glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, teapotNumIndices, GL_UNSIGNED_SHORT, (void *)teapotIndexByteOffset);

  // Arrow
  glBindVertexArray(arrowVertexArrayObjectID);
  glm::mat4 arrowModelToWorldMatrix = glm::translate(glm::vec3(0.0f, 0.0f, -3.0f));
  fullTransformMatrix = worldToProjectionMatrix * arrowModelToWorldMatrix;
  glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, arrowNumIndices, GL_UNSIGNED_SHORT, (void *)arrowIndexByteOffset);

  // Plane
  glBindVertexArray(planeVertexArrayObjectID);
  glm::mat4 planeModelToWorldMatrix = glm::mat4();
  fullTransformMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;
  glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
  glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void *)planeIndexByteOffset);
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
  glEnable(GL_CULL_FACE);
  SendDataToOpenGL();
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