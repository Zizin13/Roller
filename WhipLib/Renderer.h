#ifndef _WHIPLIB_RENDERER_H
#define _WHIPLIB_RENDERER_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include <vector>
//-------------------------------------------------------------------------------------------------
class CShader;
class CShapeData;
class CCamera;
class CTrack;
struct tCarShape;
//-------------------------------------------------------------------------------------------------
typedef std::vector<tCarShape *> CCarShapeAy;
typedef std::vector<CShapeData *> CShapeAy;
//-------------------------------------------------------------------------------------------------

class CRenderer
{
public:
  CRenderer();
  ~CRenderer();

  bool Init(const std::string &sFatDataDir);
  bool Shutdown();
  void Draw(int iWindowWidth, int iWindowHeight, CCamera *pCamera);

  CShapeData *MakeCarShape(eWhipModel model);
  CShapeData *MakeTrackShape(CTrack *pTrack);

private:
  std::string m_sFatDataDir;
  CShader *m_pShader;
  CCarShapeAy m_carShapeAy;
  CShapeAy m_shapeAy;
};

//-------------------------------------------------------------------------------------------------
#endif