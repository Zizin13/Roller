#ifndef _WHIPLIB_CARDATA_H
#define _WHIPLIB_CARDATA_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include "Texture.h"
//-------------------------------------------------------------------------------------------------
class CShapeData;
class CShader;
struct tVertex;
//-------------------------------------------------------------------------------------------------

class CCarData
{
public:
  CCarData();
  ~CCarData();

  CShapeData *MakeCar(CShader *pShader);

  float m_fScale;

private:
  tVertex *MakeVerts(uint32 &numVertices);
  uint32 *MakeIndices(uint32 &numIndices);
};

//-------------------------------------------------------------------------------------------------
#endif
