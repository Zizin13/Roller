#include "glew.h"
#include "VertexBuffer.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CVertexBuffer::CVertexBuffer(const tVertex *pData, uint32 uiCount)
{
  glGenBuffers(1, &m_uiId);
  glBindBuffer(GL_ARRAY_BUFFER, m_uiId);
  glBufferData(GL_ARRAY_BUFFER, uiCount * sizeof(tVertex), pData, GL_STATIC_DRAW);
}

//-------------------------------------------------------------------------------------------------

CVertexBuffer::~CVertexBuffer()
{
  glDeleteBuffers(1, &m_uiId);
}

//-------------------------------------------------------------------------------------------------

void CVertexBuffer::Bind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, m_uiId);
}

//-------------------------------------------------------------------------------------------------

void CVertexBuffer::Unbind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-------------------------------------------------------------------------------------------------