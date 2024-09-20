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
  pVertexBuf->Bind();
  GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0));
  GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 3)));
  GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 6)));
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