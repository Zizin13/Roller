#ifndef _WHIPLIB_WHIPMODEL_H
#define _WHIPLIB_WHIPMODEL_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include <string>
//-------------------------------------------------------------------------------------------------
class CWhipModelPrivate;
class CShapeData;
class CShader;
class CTexture;
struct tVertex;
struct tPolygon;
struct tAnimation;
//-------------------------------------------------------------------------------------------------

class CWhipModel
{
public:
  CWhipModel();
  ~CWhipModel();

  void MakeModel(CShader *pShader, CTexture *pTexture, eWhipModel model);

  float m_fScale;
  CShapeData *m_pShapeData;

private:
  tVertex *MakeVerts(uint32 &numVertices, CTexture *pTexture);
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
