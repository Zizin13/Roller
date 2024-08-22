#include "IndexBuffer.h"
#include "OpenGLDebug.h"
#include "ShapeFactory.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CIndexBuffer::CIndexBuffer(const uint32 *pData, uint32 uiCount, GLenum usage)
  : m_uiCount(uiCount)
  , m_usage(usage)
{
  GLCALL(glGenBuffers(1, &m_uiId));
  GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiId));
  GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, uiCount * sizeof(uint32), pData, usage));
}

//-------------------------------------------------------------------------------------------------

CIndexBuffer::~CIndexBuffer()
{
  if (!CShapeFactory::GetShapeFactory().m_bOglRunning)
    return;

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

void CIndexBuffer::Update(const uint32 *pData, uint32 uiCount)
{
  if (uiCount != m_uiCount) {
    assert(0);
    Logging::LogMessage("Error updating index buffer: count has changed");
    return;
  }

  Bind();
  GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, uiCount * sizeof(uint32), pData, m_usage));
}

//-------------------------------------------------------------------------------------------------