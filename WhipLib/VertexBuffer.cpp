#include "OpenGLDebug.h"
#include "VertexBuffer.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CVertexBuffer::CVertexBuffer(const tVertex *pData, uint32 uiCount)
{
  GLCALL(glGenBuffers(1, &m_uiId));
  GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_uiId));
  GLCALL(glBufferData(GL_ARRAY_BUFFER, uiCount * sizeof(tVertex), pData, GL_STATIC_DRAW));
}

//-------------------------------------------------------------------------------------------------

CVertexBuffer::~CVertexBuffer()
{
  glDeleteBuffers(1, &m_uiId);
}

//-------------------------------------------------------------------------------------------------

void CVertexBuffer::Bind() const
{
  GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_uiId));
}

//-------------------------------------------------------------------------------------------------

void CVertexBuffer::Unbind() const
{
  GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

//-------------------------------------------------------------------------------------------------