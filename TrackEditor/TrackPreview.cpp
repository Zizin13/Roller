#include "glew.h"
#include "TrackPreview.h"
#include "qevent.h"
#include "MainWindow.h"
#include <fstream>
#include <string>
#include <sstream>
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

struct tShaderProgramSource
{
  std::string sVertexSource;
  std::string sFragmentSource;
};

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
    OutputDebugString(szOut);
    return false;
  }
  return true;
}

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

static tShaderProgramSource ParseShader(const std::string &sFile)
{
  std::ifstream stream(sFile);

  enum class ShaderType
  {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };

  std::string sLine;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;
  while (getline(stream, sLine)) {
    if (sLine.find("#shader") != std::string::npos) {
      if (sLine.find("vertex") != std::string::npos)
        type = ShaderType::VERTEX;
      else if (sLine.find("fragment") != std::string::npos)
        type = ShaderType::FRAGMENT;
    } else {
      ss[(int)type] << sLine << "\n";
    }
  }

  return { ss[0].str(), ss[1].str() };
}

//-------------------------------------------------------------------------------------------------

static unsigned int CompileShader(unsigned int uiType, const std::string &sSource)
{
  GLCALL(unsigned int uiShader = glCreateShader(uiType));
  const char *szSource = sSource.c_str();
  GLCALL(glShaderSource(uiShader, 1, &szSource, NULL));
  GLCALL(glCompileShader(uiShader));

  int iResult;
  GLCALL(glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iResult));
  if (iResult == GL_FALSE) {
    int iLength;
    GLCALL(glGetShaderiv(uiShader, GL_INFO_LOG_LENGTH, &iLength));
    char *szMsg = new char[iLength];
    GLCALL(glGetShaderInfoLog(uiShader, iLength, &iLength, szMsg));
    g_pMainWindow->LogMessage(QString("Failed to compile ") + 
                              QString((uiType == GL_VERTEX_SHADER) ? "vertex" : "fragment") + 
                              QString(" shader"));
    g_pMainWindow->LogMessage(szMsg);
    delete[] szMsg;
    GLCALL(glDeleteShader(uiShader));
    return 0;
  }

  return uiShader;
}

//-------------------------------------------------------------------------------------------------

static unsigned int CreateShader(const std::string &sVertexShader, const std::string &sFragmentShader)
{
  GLCALL(unsigned int uiProgram = glCreateProgram());
  unsigned int uiVertexShader = CompileShader(GL_VERTEX_SHADER, sVertexShader);
  unsigned int uiFragmentShader = CompileShader(GL_FRAGMENT_SHADER, sFragmentShader);
  GLCALL(glAttachShader(uiProgram, uiVertexShader));
  GLCALL(glAttachShader(uiProgram, uiFragmentShader));
  GLCALL(glLinkProgram(uiProgram));
  GLCALL(glValidateProgram(uiProgram));
  GLCALL(glDeleteShader(uiVertexShader));
  GLCALL(glDeleteShader(uiFragmentShader));
  return uiProgram;
}

//-------------------------------------------------------------------------------------------------

CTrackPreview::CTrackPreview(QWidget *pParent)
  : QGLWidget(QGLFormat(QGL::SampleBuffers), pParent)
{
  xRot = 0;
  yRot = 0;
  zRot = 0;
  fRed = 0.0f;
}

//-------------------------------------------------------------------------------------------------

CTrackPreview::~CTrackPreview()
{
  GLCALL(glDeleteProgram(m_uiShader));
}

//-------------------------------------------------------------------------------------------------

static void NormalizeAngle(int &iAngle)
{
  while (iAngle < 0)
    iAngle += 360 * 16;
  while (iAngle > 360)
    iAngle -= 360 * 16;
};

//-------------------------------------------------------------------------------------------------

void CTrackPreview::initializeGL()
{
  if (glewInit() != GLEW_OK)
    assert(0);

  GLCALL(glEnable(GL_DEBUG_OUTPUT));
  GLCALL(glDebugMessageCallback(GLErrorCb, 0));

  //qglClearColor(Qt::black);
  //glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  //glShadeModel(GL_SMOOTH);
  //glEnable(GL_LIGHTING);
  //glEnable(GL_LIGHT0);

  //static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
  //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

  float positions[] = {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f
  };

  unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
  };

  m_uiVertAyObj;
  GLCALL(glGenVertexArrays(1, &m_uiVertAyObj));
  GLCALL(glBindVertexArray(m_uiVertAyObj));

  unsigned int uiBufId;
  GLCALL(glGenBuffers(1, &uiBufId));
  GLCALL(glBindBuffer(GL_ARRAY_BUFFER, uiBufId));
  GLCALL(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), &positions, GL_STATIC_DRAW));

  GLCALL(glEnableVertexAttribArray(0));
  GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

  GLCALL(glGenBuffers(1, &m_uiBo));
  GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiBo));
  GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices, GL_STATIC_DRAW));

  tShaderProgramSource source = ParseShader("shaders/Basic.shader");

  m_uiShader = CreateShader(source.sVertexSource, source.sFragmentSource);
  GLCALL(glUseProgram(m_uiShader));

  GLCALL(int iLocation = glGetUniformLocation(m_uiShader, "u_Color"));
  assert(iLocation != -1);
  GLCALL(glUniform4f(iLocation, 0.8f, 0.3f, 0.8f, 1.0f));

  GLCALL(glBindVertexArray(0));
  GLCALL(glUseProgram(0));
  GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::paintGL()
{
  GLCALL(glClear(GL_COLOR_BUFFER_BIT));// | GL_DEPTH_BUFFER_BIT);
  //glLoadIdentity();
  //glTranslatef(0.0, 0.0, -10.0);
  //glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
  //glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
  //glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
  Draw();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::resizeGL(int iWidth, int iHeight)
{
  int iSide = qMin(iWidth, iHeight);
  GLCALL(glViewport((iWidth - iSide) / 2, (iHeight - iSide) / 2, iSide, iSide));

//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
//#ifdef QT_OPENGL_ES_1
//  glOrthof(-2, +2, -2, +2, 1.0, 15.0);
//#else
//  glOrtho(-2, +2, -2, +2, 1.0, 15.0);
//#endif
//  glMatrixMode(GL_MODELVIEW);
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::mousePressEvent(QMouseEvent *pEvent)
{
  lastPos = pEvent->pos();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::mouseMoveEvent(QMouseEvent *pEvent)
{
  int dx = pEvent->x() - lastPos.x();
  int dy = pEvent->y() - lastPos.y();

  if (pEvent->buttons() & Qt::LeftButton) {
    SetXRotation(xRot + 8 * dy);
    SetYRotation(yRot + 8 * dx);
  } else if (pEvent->buttons() & Qt::RightButton) {
    SetXRotation(xRot + 8 * dy);
    SetZRotation(zRot + 8 * dx);
  }

  lastPos = pEvent->pos();
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::SetXRotation(int iAngle)
{
  NormalizeAngle(iAngle);
  if (iAngle != xRot) {
    xRot = iAngle;
    updateGL();
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::SetYRotation(int iAngle)
{
  NormalizeAngle(iAngle);
  if (iAngle != yRot) {
    yRot = iAngle;
    updateGL();
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::SetZRotation(int iAngle)
{
  NormalizeAngle(iAngle);
  if (iAngle != zRot) {
    zRot = iAngle;
    updateGL();
  }
}

//-------------------------------------------------------------------------------------------------

void CTrackPreview::Draw()
{
  GLCALL(glUseProgram(m_uiShader));

  GLCALL(int iLocation = glGetUniformLocation(m_uiShader, "u_Color"));
  assert(iLocation != -1);
  GLCALL(glUniform4f(iLocation, fRed, 0.3f, 0.8f, 1.0f));

  GLCALL(glBindVertexArray(m_uiVertAyObj));
  GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiBo));

  GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

  float fInc = 0.05f;
  if (fRed > 1.0f) {
    fInc = -0.05f;
  } else if (fRed < 0.0f) {
    fInc = 0.05f;
  }
  fRed += fInc;

  //qglColor(Qt::red);
  //glBegin(GL_QUADS);
  //glNormal3f(0, 0, -1);
  //glVertex3f(-1, -1, 0);
  //glVertex3f(-1, 1, 0);
  //glVertex3f(1, 1, 0);
  //glVertex3f(1, -1, 0);
  //
  //glEnd();
  //glBegin(GL_TRIANGLES);
  //glNormal3f(0, -1, 0.707f);
  //glVertex3f(-1, -1, 0);
  //glVertex3f(1, -1, 0);
  //glVertex3f(0, 0, 1.2f);
  //glEnd();
  //glBegin(GL_TRIANGLES);
  //glNormal3f(1, 0, 0.707f);
  //glVertex3f(1, -1, 0);
  //glVertex3f(1, 1, 0);
  //glVertex3f(0, 0, 1.2f);
  //glEnd();
  //glBegin(GL_TRIANGLES);
  //glNormal3f(0, 1, 0.707f);
  //glVertex3f(1, 1, 0);
  //glVertex3f(-1, 1, 0);
  //glVertex3f(0, 0, 1.2f);
  //glEnd();
  //glBegin(GL_TRIANGLES);
  //glNormal3f(-1, 0, 0.707f);
  //glVertex3f(-1, 1, 0);
  //glVertex3f(-1, -1, 0);
  //glVertex3f(0, 0, 1.2f);
  //glEnd();
}

//-------------------------------------------------------------------------------------------------