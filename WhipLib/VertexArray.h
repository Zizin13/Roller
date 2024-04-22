#ifndef _UTILS_VERTEXARRAY_H
#define _UTILS_VERTEXARRAY_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
//-------------------------------------------------------------------------------------------------
class CVertexBuffer;
//-------------------------------------------------------------------------------------------------

class CVertexArray
{
public:
  CVertexArray(CVertexBuffer *pVertexBuf);
  ~CVertexArray();

  void Bind() const;
  void Unbind() const;

private:
  uint32 m_uiId;
};

//-------------------------------------------------------------------------------------------------
#endif