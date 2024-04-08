#ifndef _TRACKEDITOR_SHAPEDATA_H
#define _TRACKEDITOR_SHAPEDATA_H
//-------------------------------------------------------------------------------------------------
#include <glew.h>
#include "glm.hpp"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
//-------------------------------------------------------------------------------------------------
struct tShapeData
{
  tShapeData() 
    : pVertexBuf(NULL)
    , pIndexBuf(NULL)
  {};
  GLuint vertexArrayObjId;
  GLuint shaderProgramId;
  glm::mat4 modelToWorldMatrix;

  CVertexBuffer *pVertexBuf;
  CIndexBuffer *pIndexBuf;

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