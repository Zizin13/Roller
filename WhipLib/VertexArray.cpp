#include "OpenGLDebug.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CVertexArray::CVertexArray(CVertexBuffer *pVertexBuf)
{
  GLCALL(glGenVertexArrays(1, &m_uiId));
  GLCALL(glBindVertexArray(m_uiId));
  GLCALL(glEnableVertexAttribArray(0));
  GLCALL(glEnableVertexAttribArray(1));
  GLCALL(glEnableVertexAttribArray(2));
  GLCALL(glEnableVertexAttribArray(3));
  GLCALL(glEnableVertexAttribArray(4));
  pVertexBuf->Bind();
  GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0));
  GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 3)));
  GLCALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 6)));
  GLCALL(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 9)));
  GLCALL(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 11)));
}

//-------------------------------------------------------------------------------------------------

CVertexArray::~CVertexArray()
{
  glDeleteVertexArrays(1, &m_uiId);
}

//-------------------------------------------------------------------------------------------------

void CVertexArray::Bind() const
{
  GLCALL(glBindVertexArray(m_uiId));
}

//-------------------------------------------------------------------------------------------------

void CVertexArray::Unbind() const
{
  GLCALL(glBindVertexArray(0));
}

//-------------------------------------------------------------------------------------------------