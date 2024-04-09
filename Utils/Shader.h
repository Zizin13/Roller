#ifndef _UTILS_SHADER_H
#define _UTILS_SHADER_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include "glew.h"
#include "glm.hpp"
#include <string>
//-------------------------------------------------------------------------------------------------

class CShader
{
public:
  CShader(const std::string &sVertexFile, const std::string &sFragmentFile);
  ~CShader();

  void Bind() const;
  void Unbind() const;
  void SetUniformVec3(const std::string &sName, const glm::vec3 &vec);
  void SetUniformVec4(const std::string &sName, const glm::vec4 &vec);
  void SetUniformMat4(const std::string &sName, const glm::mat4 &matrix);

private:
  bool CheckStatus(GLuint objectId,
                   PFNGLGETSHADERIVPROC objectPropertyGetter,
                   PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
                   GLenum statusType);
  bool CheckShaderStatus(GLuint shaderId);
  bool CheckProgramStatus(GLuint programId);
  std::string ReadShaderCode(const char *filename);
  void InstallShaders(GLuint &programId, const char *szVertexShader, const char *szFragmentShader);

  uint32 m_uiId;
};

//-------------------------------------------------------------------------------------------------
#endif