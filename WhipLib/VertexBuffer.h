#ifndef _UTILS_VERTEXBUFFER_H
#define _UTILS_VERTEXBUFFER_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include "Vertex.h"
//-------------------------------------------------------------------------------------------------

class CVertexBuffer
{
public:
  CVertexBuffer(const tVertex *pData, uint32 uiCount);
  ~CVertexBuffer();

  void Bind() const;
  void Unbind() const;

private:
  uint32 m_uiId;
};

//-------------------------------------------------------------------------------------------------
#endif