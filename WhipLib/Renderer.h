#ifndef _WHIPLIB_RENDERER_H
#define _WHIPLIB_RENDERER_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include "glm.hpp"
#include <vector>
//-------------------------------------------------------------------------------------------------
class CShader;
class CShapeData;
class CCamera;
class CTrack;
class CTexture;
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
  void UpdateTrackShape(CTrack *pTrack, CShapeData *pShape);
  void MakeSigns(CTrack *pTrack);
  void MakeEnvirFloor(CTrack *pTrack);
  CShapeData *MakeDebugTri(CShapeData *pShape, CTexture *pTexture,
                           const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2);

private:
  std::string m_sFatDataDir;
  CShader *m_pShader;
  CCarShapeAy m_carShapeAy;
  CShapeAy m_shapeAy;
};

//-------------------------------------------------------------------------------------------------
#endif