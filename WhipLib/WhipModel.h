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
enum eWhipModel
{
  CAR_F1WACK = 0,
  CAR_AUTO,
  CAR_DESILVA,
  CAR_PULSE,
  CAR_GLOBAL,
  CAR_MILLION,
  CAR_MISSION,
  CAR_ZIZIN,
  CAR_REISE,
  SIGN_TOWER,
  SIGN_TOWER2,
  SIGN_SIGN01,
  SIGN_SIGN02,
  SIGN_BUILD,
  SIGN_BUILD1,
  SIGN_BUILD2,
  SIGN_BUILD3,
  SIGN_HEELBAR,
  SIGN_BALLOON,
  SIGN_BALLOON2,
  SIGN_TREE,
  SIGN_ADVERT,
  SIGN_ADVERT2,
  SIGN_ADVERT3,
  SIGN_QUADBLD,
  SIGN_BLD0
};
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
