#include "OpenGLDebug.h"
#include "Shader.h"
#include <fstream>
#include <assert.h>
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CShader::CShader(const std::string &sVertexFile, const std::string &sFragmentFile)
{
  InstallShaders(m_uiId, sVertexFile.c_str(), sFragmentFile.c_str());
}

//-------------------------------------------------------------------------------------------------

CShader::~CShader()
{
  glDeleteProgram(m_uiId);
}

//-------------------------------------------------------------------------------------------------

void CShader::Bind() const
{
  GLCALL(glUseProgram(m_uiId));
}

//-------------------------------------------------------------------------------------------------

void CShader::Unbind() const
{
  GLCALL(glUseProgram(0));
}

//-------------------------------------------------------------------------------------------------

void CShader::SetUniformVec3(const std::string &sName, const glm::vec3 &vec)
{
  uint32 uiUniformLocation = glGetUniformLocation(m_uiId, sName.c_str());
  GLCALL(glUniform3fv(uiUniformLocation, 1, &vec[0]));
}

//-------------------------------------------------------------------------------------------------

void CShader::SetUniformVec4(const std::string &sName, const glm::vec4 &vec)
{
  uint32 uiUniformLocation = glGetUniformLocation(m_uiId, sName.c_str());
  GLCALL(glUniform4fv(uiUniformLocation, 1, &vec[0]));
}

//-------------------------------------------------------------------------------------------------

void CShader::SetUniformMat4(const std::string &sName, const glm::mat4 &matrix)
{
  uint32 uiUniformLocation = glGetUniformLocation(m_uiId, sName.c_str());
  GLCALL(glUniformMatrix4fv(uiUniformLocation, 1, GL_FALSE, &matrix[0][0]));
}

//-------------------------------------------------------------------------------------------------

bool CShader::CheckStatus(GLuint objectId,
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

    //TODO: LOGGING
    //g_pMainWindow->LogMessage(buffer);
    assert(0);

    delete[] buffer;
    return false;
  }
  return true;

}

//-------------------------------------------------------------------------------------------------

bool CShader::CheckShaderStatus(GLuint shaderId)
{
  return CheckStatus(shaderId, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

//-------------------------------------------------------------------------------------------------

bool CShader::CheckProgramStatus(GLuint programId)
{
  return CheckStatus(programId, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

//-------------------------------------------------------------------------------------------------

std::string CShader::ReadShaderCode(const char *filename)
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

void CShader::InstallShaders(GLuint &programId, const char *szVertexShader, const char *szFragmentShader)
{
  GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

  const char *adapter[1];
  std::string sTemp = ReadShaderCode(szVertexShader);
  adapter[0] = sTemp.c_str();
  GLCALL(glShaderSource(vertexShaderId, 1, adapter, 0));
  sTemp = ReadShaderCode(szFragmentShader);
  adapter[0] = sTemp.c_str();
  GLCALL(glShaderSource(fragmentShaderId, 1, adapter, 0));
  GLCALL(glCompileShader(vertexShaderId));
  GLCALL(glCompileShader(fragmentShaderId));
  if (!CheckShaderStatus(vertexShaderId) || !CheckShaderStatus(fragmentShaderId))
    return;
  programId = glCreateProgram();
  GLCALL(glAttachShader(programId, vertexShaderId));
  GLCALL(glAttachShader(programId, fragmentShaderId));
  GLCALL(glLinkProgram(programId));
  if (!CheckProgramStatus(programId))
    return;
  GLCALL(glDeleteShader(vertexShaderId));
  GLCALL(glDeleteShader(fragmentShaderId));
}

//-------------------------------------------------------------------------------------------------