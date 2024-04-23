#ifndef _WHIPLIB_CARDATA_H
#define _WHIPLIB_CARDATA_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include <string>
//-------------------------------------------------------------------------------------------------
class CCarDataPrivate;
class CShapeData;
class CShader;
struct tVertex;
//-------------------------------------------------------------------------------------------------

class CCarData
{
public:
  CCarData();
  ~CCarData();

  void LoadTexture(const std::string &sPal, const std::string &sTex, bool bMangled);
  CShapeData *MakeCar(CShader *pShader);

  float m_fScale;

private:
  tVertex *MakeVerts(uint32 &numVertices);
  uint32 *MakeIndices(uint32 &numIndices);

  CCarDataPrivate *p;
};

//-------------------------------------------------------------------------------------------------
#endif
