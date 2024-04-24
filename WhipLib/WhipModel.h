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
struct tPolygon;
struct tAnimation;
//-------------------------------------------------------------------------------------------------

class CWhipModel
{
public:
  CWhipModel();
  ~CWhipModel();

  void LoadTexture(const std::string &sPal, const std::string &sTex, bool bMangled);
  CShapeData *MakeModel(CShader *pShader, eWhipModel model);

  float m_fScale;

private:
  tVertex *MakeVerts(uint32 &numVertices);
  uint32 *MakeIndices(uint32 &numIndices);

  float *GetCoords();
  int GetCoordsCount();
  tPolygon *GetPols();
  int GetPolsCount();
  tAnimation *GetAnms();
  int GetAnmsCount();

  CWhipModelPrivate *p;
};

//-------------------------------------------------------------------------------------------------
#endif
