#include "IndexBuffer.h"
#include "OpenGLDebug.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CIndexBuffer::CIndexBuffer(const uint32 *pData, uint32 uiCount)
  : m_uiCount(uiCount)
{
  GLCALL(glGenBuffers(1, &m_uiId));
  GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiId));
  GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, uiCount * sizeof(uint32), pData, GL_STATIC_DRAW));
}

//-------------------------------------------------------------------------------------------------

CIndexBuffer::~CIndexBuffer()
{
  glDeleteBuffers(1, &m_uiId);
}

//-------------------------------------------------------------------------------------------------

void CIndexBuffer::Bind() const
{
  GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiId));
}

//-------------------------------------------------------------------------------------------------

void CIndexBuffer::Unbind() const
{
  GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

//-------------------------------------------------------------------------------------------------