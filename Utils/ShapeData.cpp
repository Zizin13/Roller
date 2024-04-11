#include <glew.h>
#include "OpenGLDebug.h"
#include "ShapeData.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CShapeData::CShapeData(CVertexBuffer *pVertexBuf,
                       CIndexBuffer *pIndexBuf,
                       CVertexArray *pVertexArray,
                       CShader *pShader,
                       GLenum drawType)
  : m_pVertexBuf(pVertexBuf)
  , m_pIndexBuf(pIndexBuf)
  , m_pVertexArray(pVertexArray)
  , m_pShader(pShader)
  , m_drawType(drawType)
  , modelToWorldMatrix(glm::mat4(1))
{
}

//-------------------------------------------------------------------------------------------------

CShapeData::~CShapeData()
{
  if (m_pVertexBuf) {
    delete m_pVertexBuf;
    m_pVertexBuf = NULL;
  }
  if (m_pIndexBuf) {
    delete m_pIndexBuf;
    m_pIndexBuf = NULL;
  }
  if (m_pVertexArray) {
    delete m_pVertexArray;
    m_pVertexArray = NULL;
  }
}

//-------------------------------------------------------------------------------------------------

void CShapeData::Draw(const glm::mat4 &worldToProjectionMatrix)
{
  if (!m_pShader || !m_pVertexArray || !m_pIndexBuf)
    return;

  m_pShader->Bind();
  m_pVertexArray->Bind();
  m_pIndexBuf->Bind();

  glm::mat4 fullTransformMatrix = worldToProjectionMatrix * modelToWorldMatrix;
  m_pShader->SetUniformMat4("modelToProjectionMatrix", fullTransformMatrix);
  GLCALL(glDrawElements(m_drawType, m_pIndexBuf->GetCount(), GL_UNSIGNED_INT, 0));
}

//-------------------------------------------------------------------------------------------------