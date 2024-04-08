#ifndef _UTILS_INDEXBUFFER_H
#define _UTILS_INDEXBUFFER_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
//-------------------------------------------------------------------------------------------------

class CIndexBuffer
{
public:
  CIndexBuffer(const uint32 *pData, uint32 uiCount);
  ~CIndexBuffer();

  void Bind() const;
  void Unbind() const;
  inline uint32 GetCount() const { return m_uiCount; };

private:
  uint32 m_uiId;
  uint32 m_uiCount;
};

//-------------------------------------------------------------------------------------------------
#endif