#pragma once
#include <glew.h>
#include "Vertex.h"

struct ShapeData
{
  ShapeData() : vertices(0), numVertices(0), indices(0), numIndices(0) {};
  Vertex *vertices;
  GLuint numVertices;
  GLushort *indices;
  GLuint numIndices;
  GLsizeiptr vertexBufSize() const
  {
    return numVertices * sizeof(Vertex);
  }
  GLsizeiptr indexBufSize() const
  {
    return numIndices * sizeof(GLushort);
  }
  void cleanup()
  {
    delete[] vertices;
    delete[] indices;
  }
};