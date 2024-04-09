#ifndef _TRACKEDITOR_SHAPEDATA_H
#define _TRACKEDITOR_SHAPEDATA_H
//-------------------------------------------------------------------------------------------------
#include <glew.h>
#include "glm.hpp"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
//-------------------------------------------------------------------------------------------------
class CShader;
//-------------------------------------------------------------------------------------------------
struct tShapeData
{
  tShapeData() 
    : pVertexBuf(NULL)
    , pIndexBuf(NULL)
    , pVertexArray(NULL)
    , pShader(NULL)
  {};
  glm::mat4 modelToWorldMatrix;

  CVertexBuffer *pVertexBuf;
  CIndexBuffer *pIndexBuf;
  CVertexArray *pVertexArray;
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
    if (pVertexArray) {
      delete pVertexArray;
      pVertexArray = NULL;
    }
  }
};
//-------------------------------------------------------------------------------------------------
#endif