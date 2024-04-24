#ifndef _WHIPLIB_WHIPMODEL_H
#define _WHIPLIB_WHIPMODEL_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include <string>
//-------------------------------------------------------------------------------------------------
class CWhipModelPrivate;
class CShapeData;
class CShader;
struct tVertex;
//-------------------------------------------------------------------------------------------------

class CWhipModel
{
public:
  CWhipModel();
  ~CWhipModel();

  void LoadTexture(const std::string &sPal, const std::string &sTex, bool bMangled);
  CShapeData *MakeCar(CShader *pShader);

  float m_fScale;

private:
  tVertex *MakeVerts(uint32 &numVertices);
  uint32 *MakeIndices(uint32 &numIndices);

  CWhipModelPrivate *p;
};

//-------------------------------------------------------------------------------------------------
#endif
