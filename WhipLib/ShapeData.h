#ifndef _WHIPLIB_SHAPEDATA_H
#define _WHIPLIB_SHAPEDATA_H
//-------------------------------------------------------------------------------------------------
#include "Vertex.h"
#include "Types.h"
//-------------------------------------------------------------------------------------------------

struct tShapeData
{
  tShapeData()
    : m_vertices(NULL)
    , m_uiNumVerts(0)
    , m_indices(NULL)
    , m_uiNumIndices(0)
  {}
  ~tShapeData()
  {
    if (m_vertices) {
      delete[] m_vertices;
      m_vertices = NULL;
    }
    if (m_indices) {
      delete[] m_indices;
      m_indices = NULL;
    }
  }
  uint32 m_uiNumVerts;
  tVertex *m_vertices;
  uint32 m_uiNumIndices;
  uint32 *m_indices;
};

//-------------------------------------------------------------------------------------------------
#endif