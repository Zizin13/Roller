#include "glew.h"
#include "IndexBuffer.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CIndexBuffer::CIndexBuffer(const uint32 *pData, uint32 uiCount)
  : m_uiCount(uiCount)
{
  glGenBuffers(1, &m_uiId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, uiCount * sizeof(uint32), pData, GL_STATIC_DRAW);
}

//-------------------------------------------------------------------------------------------------

CIndexBuffer::~CIndexBuffer()
{
  glDeleteBuffers(1, &m_uiId);
}

//-------------------------------------------------------------------------------------------------

void CIndexBuffer::Bind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiId);
}

//-------------------------------------------------------------------------------------------------

void CIndexBuffer::Unbind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//-------------------------------------------------------------------------------------------------