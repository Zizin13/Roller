#include "OpenGLDebug.h"
#include "VertexBuffer.h"
#include "Logging.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CVertexBuffer::CVertexBuffer(const tVertex *pData, uint32 uiCount, GLenum usage)
  : m_uiCount(uiCount)
  , m_usage(usage)
{
  GLCALL(glGenBuffers(1, &m_uiId));
  GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_uiId));
  GLCALL(glBufferData(GL_ARRAY_BUFFER, uiCount * sizeof(tVertex), pData, m_usage));
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

void CVertexBuffer::Update(const tVertex *pData, uint32 uiCount)
{
  if (uiCount != m_uiCount) {
    assert(0);
    Logging::LogMessage("Error updating vertex buffer: count has changed");
    return;
  }

  Bind();
  GLCALL(glBufferData(GL_ARRAY_BUFFER, uiCount * sizeof(tVertex), pData, m_usage));
}

//-------------------------------------------------------------------------------------------------