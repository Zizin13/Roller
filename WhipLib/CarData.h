#ifndef _UTILS_CARDATA_H
#define _UTILS_CARDATA_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
//-------------------------------------------------------------------------------------------------
class CShapeData;
class CShader;
struct tVertex;
//-------------------------------------------------------------------------------------------------

class CCarData
{
  CCarData();
  ~CCarData();

  CShapeData *MakeCar(CShader *pShader);

private:
  tVertex *MakeVerts(uint32 &numVertices);
  uint32 *MakeIndices(uint32 &numIndices);
};

//-------------------------------------------------------------------------------------------------
#endif
