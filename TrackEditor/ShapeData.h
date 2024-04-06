#ifndef _TRACKEDITOR_SHAPEDATA_H
#define _TRACKEDITOR_SHAPEDATA_H
//-------------------------------------------------------------------------------------------------
#include <glew.h>
#include "Vertex.h"
#include "glm.hpp"
//-------------------------------------------------------------------------------------------------
struct tShapeData
{
  tShapeData() : vertices(0), numVertices(0), indices(0), numIndices(0) {};
  tVertex *vertices;
  GLuint numVertices;
  GLushort *indices;
  GLuint numIndices;

  GLuint vertexBufId;
  GLuint indexBufId;
  GLuint vertexArrayObjId;
  GLuint shaderProgramId;

  glm::mat4 modelToWorldMatrix;

  GLsizeiptr VertexBufSize() const
  {
    return numVertices * sizeof(tVertex);
  }
  GLsizeiptr IndexBufSize() const
  {
    return numIndices * sizeof(GLushort);
  }
  void Cleanup()
  {
    delete[] vertices;
    delete[] indices;
  }
};
//-------------------------------------------------------------------------------------------------
#endif