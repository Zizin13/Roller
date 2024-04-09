#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "glew.h"
//-------------------------------------------------------------------------------------------------

CVertexArray::CVertexArray(CVertexBuffer *pVertexBuf)
{
  glGenVertexArrays(1, &m_uiId);
  glBindVertexArray(m_uiId);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  pVertexBuf->Bind();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 3));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char *)(sizeof(float) * 6));\
}

//-------------------------------------------------------------------------------------------------

CVertexArray::~CVertexArray()
{
  glDeleteVertexArrays(1, &m_uiId);
}

//-------------------------------------------------------------------------------------------------

void CVertexArray::Bind() const
{
  glBindVertexArray(m_uiId);
}

//-------------------------------------------------------------------------------------------------

void CVertexArray::Unbind() const
{
  glBindVertexArray(0);
}

//-------------------------------------------------------------------------------------------------