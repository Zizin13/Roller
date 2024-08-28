#include "OpenGLDebug.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ShapeFactory.h"
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
  GLCALL(glEnableVertexAttribArray(5));
  GLCALL(glEnableVertexAttribArray(6));
  pVertexBuf->Bind();
  GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0));
  GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 3)));
  GLCALL(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 6)));
  GLCALL(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 10)));
  GLCALL(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 12)));
  GLCALL(glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 16)));
  GLCALL(glVertexAttribPointer(6, 4, GL_BYTE,  GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 18)));
}

//-------------------------------------------------------------------------------------------------

CVertexArray::~CVertexArray()
{
  if (!CShapeFactory::GetShapeFactory().m_bOglRunning)
    return;

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