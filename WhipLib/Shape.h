#ifndef _WHIPLIB_SHAPE_H
#define _WHIPLIB_SHAPE_H
//-------------------------------------------------------------------------------------------------
#include <glew.h>
#include "glm.hpp"
#include "Vertex.h"
#include "Types.h"
#include "ShapeData.h"
//-------------------------------------------------------------------------------------------------
class CShader;
class CVertexBuffer;
class CIndexBuffer;
class CVertexArray;
class CTexture;
//-------------------------------------------------------------------------------------------------
class CShape
{
public:
  CShape(CVertexBuffer *pVertexBuf,
         CIndexBuffer *pIndexBuf,
         CVertexArray *pVertexArray,
         CShader *pShader,
         CTexture *pTexture = NULL,
         GLenum drawType = GL_TRIANGLES);
  ~CShape();

  void Draw(const glm::mat4 &worldToProjectionMatrix, const glm::vec3 cameraPosition);
  void TransformVertsForExport();

  glm::mat4 m_modelToWorldMatrix;
  CIndexBuffer *m_pIndexBuf;
  CVertexBuffer *m_pVertexBuf;
  CTexture *m_pTexture; //owned by track or renderer
  tShapeData m_shapeData;

private:
  CVertexArray *m_pVertexArray;
  GLenum m_drawType;
  CShader *m_pShader; //owned by renderer
};
//-------------------------------------------------------------------------------------------------
#endif