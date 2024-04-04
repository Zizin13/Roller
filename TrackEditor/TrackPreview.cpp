#include "glew.h"
#include "TrackPreview.h"
#include "MainWindow.h"
#include <fstream>
#include "ShapeGenerator.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
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

CTrackPreview::CTrackPreview(QWidget *pParent)
  : QGLWidget(QGLFormat(QGL::SampleBuffers), pParent)
{
}

//-------------------------------------------------------------------------------------------------

CTrackPreview::~CTrackPreview()
{

}

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programId;

void sendDataToOpenGL()
{
  ShapeData tri = ShapeGenerator::makeTriangle();

  GLuint vertexBufId;
  glGenBuffers(1, &vertexBufId);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufId);
  glBufferData(GL_ARRAY_BUFFER, tri.vertexBufSize(), tri.vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 3));

  GLuint indexBufId;
  glGenBuffers(1, &indexBufId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri.indexBufSize(), tri.indices, GL_STATIC_DRAW);

  tri.cleanup();
}

bool checkStatus(GLuint objectId,
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

bool checkShaderStatus(GLuint shaderId)
{
  return checkStatus(shaderId, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programId)
{
  return checkStatus(programId, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string readShaderCode(const char *filename)
{
  std::ifstream stream(filename);
  if (!stream.good()) {
    assert(0);
  }
  return std::string(
    std::istreambuf_iterator<char>(stream),
    std::istreambuf_iterator<char>());
}

void installShaders()
{
  GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

  const char *adapter[1];
  std::string sTemp = readShaderCode("VertexShaderCode.glsl");
  adapter[0] = sTemp.c_str();
  glShaderSource(vertexShaderId, 1, adapter, 0);
  sTemp = readShaderCode("FragmentShaderCode.glsl");
  adapter[0] = sTemp.c_str();
  glShaderSource(fragmentShaderId, 1, adapter, 0);

  glCompileShader(vertexShaderId);
  glCompileShader(fragmentShaderId);

  if (!checkShaderStatus(vertexShaderId) || !checkShaderStatus(fragmentShaderId))
    return;

  programId = glCreateProgram();
  glAttachShader(programId, vertexShaderId);
  glAttachShader(programId, fragmentShaderId);
  glLinkProgram(programId);

  if (!checkProgramStatus(programId))
    return;

  glUseProgram(programId);
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::initializeGL()
{
  if (glewInit() != GLEW_OK)
    assert(0);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(GLErrorCb, 0);
  glEnable(GL_DEPTH_TEST);
  sendDataToOpenGL();
  installShaders();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::paintGL()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, width(), height());

  GLint dominatingColorUniformLocation = glGetUniformLocation(programId, "dominatingColor");
  GLint yFlipUniformLocation = glGetUniformLocation(programId, "yFlip");
  glm::vec3 dominatingColor(1.0f, 0.0f, 0.0f);

  glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
  glUniform1f(yFlipUniformLocation, 1.0f);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

  dominatingColor.r = 0;
  dominatingColor.b = 1;
  glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
  glUniform1f(yFlipUniformLocation, -1.0f);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::resizeGL(int iWidth, int iHeight)
{
}

//-------------------------------------------------------------------------------------------------