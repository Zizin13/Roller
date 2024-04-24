#ifndef _WHIPLIB_SHAPEFACTORY_H
#define _WHIPLIB_SHAPEFACTORY_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
//-------------------------------------------------------------------------------------------------
class CShapeData;
class CShader;
class CTexture;
struct tVertex;
struct tAnimation;
struct tPolygon;
//-------------------------------------------------------------------------------------------------

class CShapeFactory
{
public:
  static CShapeFactory &GetShapeFactory();
  ~CShapeFactory();

  CShapeData *MakeAxes(CShader *pShader);
  CShapeData *MakeModel(CShader *pShader, CTexture *pTexture, eWhipModel model);

  float m_fScale;

private:
  CShapeFactory();

  tVertex *MakeVertsAxes(uint32 &uiNumVerts);
  uint32 *MakeIndicesAxes(uint32 &uiNumIndices);

  tVertex *MakeModelVerts(uint32 &numVertices, CTexture *pTexture, eWhipModel model);
  uint32 *MakeModelIndices(uint32 &numIndices, eWhipModel model);

  float *GetCoords(eWhipModel model);
  int GetCoordsCount(eWhipModel model);
  tPolygon *GetPols(eWhipModel model);
  int GetPolsCount(eWhipModel model);
  tAnimation *GetAnms(eWhipModel model);
  int GetAnmsCount(eWhipModel model);

  static CShapeFactory s_shapeFactory;
};

//-------------------------------------------------------------------------------------------------
#endif