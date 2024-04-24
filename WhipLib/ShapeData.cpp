#include <glew.h>
#include "OpenGLDebug.h"
#include "ShapeData.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CShapeData::CShapeData(CVertexBuffer *pVertexBuf,
                       CIndexBuffer *pIndexBuf,
                       CVertexArray *pVertexArray,
                       CShader *pShader,
                       CTexture *pTexture,
                       GLenum drawType)
  : m_pVertexBuf(pVertexBuf)
  , m_pIndexBuf(pIndexBuf)
  , m_pVertexArray(pVertexArray)
  , m_pShader(pShader)
  , m_pTexture(pTexture)
  , m_drawType(drawType)
  , m_modelToWorldMatrix(glm::mat4(1))
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
  if (m_pTexture) {
    m_pTexture->Bind();
    m_pShader->SetUniform1i("textureSlot", 0);
  }

  glm::mat4 fullTransformMatrix = worldToProjectionMatrix * m_modelToWorldMatrix;
  m_pShader->SetUniformMat4("modelToProjectionMatrix", fullTransformMatrix);
  GLCALL(glDrawElements(m_drawType, m_pIndexBuf->GetCount(), GL_UNSIGNED_INT, 0));
}

//-------------------------------------------------------------------------------------------------

//CShapeData ShapeGenerator::GenerateNormals(const CShapeData &data)
//{
//  CShapeData ret;
//  ret.numVertices = data.numVertices * 2;
//  ret.vertices = new tVertex[ret.numVertices];
//  glm::vec3 white(1.0f, 1.0f, 1.0f);
//  for (int i = 0; i < data.numVertices; i++) {
//    GLuint vertIndex = i * 2;
//    tVertex &v1 = ret.vertices[vertIndex];
//    tVertex &v2 = ret.vertices[vertIndex + 1];
//    const tVertex &sourceVertex = data.vertices[i];
//    v1.position = sourceVertex.position;
//    v2.position = sourceVertex.position + sourceVertex.normal;
//    v1.color = v2.color = white;
//  }
//
//  ret.numIndices = ret.numVertices;
//  ret.indices = new GLushort[ret.numIndices];
//  for (int i = 0; i < ret.numIndices; i++)
//    ret.indices[i] = i;
//  return ret;
//}*/


//-------------------------------------------------------------------------------------------------