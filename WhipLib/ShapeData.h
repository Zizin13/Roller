#ifndef _WHIPLIB_SHAPEDATA_H
#define _WHIPLIB_SHAPEDATA_H
//-------------------------------------------------------------------------------------------------
#include <glew.h>
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------
class CShader;
class CVertexBuffer;
class CIndexBuffer;
class CVertexArray;
class CTexture;
//-------------------------------------------------------------------------------------------------
class CShapeData
{
public:
  CShapeData(CVertexBuffer *pVertexBuf,
             CIndexBuffer *pIndexBuf,
             CVertexArray *pVertexArray,
             CShader *pShader,
             CTexture *pTexture = NULL,
             GLenum drawType = GL_TRIANGLES);
  ~CShapeData();

  void Draw(const glm::mat4 &worldToProjectionMatrix);
  //static CShapeData GenerateNormals(const CShapeData &data);

  glm::mat4 m_modelToWorldMatrix;

private:
  CVertexBuffer *m_pVertexBuf;
  CIndexBuffer *m_pIndexBuf;
  CVertexArray *m_pVertexArray;
  GLenum m_drawType;
  CShader *m_pShader; //owned by renderer
  CTexture *m_pTexture; //owned by track
};
//-------------------------------------------------------------------------------------------------
#endif