#ifndef _TRACKEDITOR_SHAPEDATA_H
#define _TRACKEDITOR_SHAPEDATA_H
//-------------------------------------------------------------------------------------------------
#include <glew.h>
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------
class CShader;
class CVertexBuffer;
class CIndexBuffer;
class CVertexArray;
//-------------------------------------------------------------------------------------------------
class CShapeData
{
public:
  CShapeData(CVertexBuffer *pVertexBuf,
             CIndexBuffer *pIndexBuf,
             CVertexArray *pVertexArray,
             CShader *pShader,
             GLenum drawType = GL_TRIANGLES);
  ~CShapeData();

  void Draw(const glm::mat4 &worldToProjectionMatrix);

  glm::mat4 modelToWorldMatrix;

private:
  CVertexBuffer *m_pVertexBuf;
  CIndexBuffer *m_pIndexBuf;
  CVertexArray *m_pVertexArray;
  GLenum m_drawType;
  CShader *m_pShader; //owned by renderer
};
//-------------------------------------------------------------------------------------------------
#endif