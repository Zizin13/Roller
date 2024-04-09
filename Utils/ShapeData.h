#ifndef _TRACKEDITOR_SHAPEDATA_H
#define _TRACKEDITOR_SHAPEDATA_H
//-------------------------------------------------------------------------------------------------
#include <glew.h>
#include "glm.hpp"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
//-------------------------------------------------------------------------------------------------
class CShader;
//-------------------------------------------------------------------------------------------------
struct tShapeData
{
  tShapeData() 
    : pVertexBuf(NULL)
    , pIndexBuf(NULL)
    , pShader(NULL)
  {};
  GLuint vertexArrayObjId;
  glm::mat4 modelToWorldMatrix;

  CVertexBuffer *pVertexBuf;
  CIndexBuffer *pIndexBuf;
  CShader *pShader; //owned by renderer

  void Cleanup()
  {
    if (pVertexBuf) {
      delete pVertexBuf;
      pVertexBuf = NULL;
    }
    if (pIndexBuf) {
      delete pIndexBuf;
      pIndexBuf = NULL;
    }
  }
};
//-------------------------------------------------------------------------------------------------
#endif