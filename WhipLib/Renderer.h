#ifndef _WHIPLIB_RENDERER_H
#define _WHIPLIB_RENDERER_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include "glm.hpp"
#include <vector>
#include <map>
//-------------------------------------------------------------------------------------------------
class CShader;
class CShapeData;
class CCamera;
class CTrack;
class CTexture;
struct tCarShape;
//-------------------------------------------------------------------------------------------------
typedef std::map<int, tCarShape *> CCarShapeMap;
typedef std::vector<CShapeData *> CShapeAy;
//-------------------------------------------------------------------------------------------------

class CRenderer
{
public:
  CRenderer();
  ~CRenderer();

  bool Init(const std::string &sAppPath);
  bool Shutdown();
  void Draw(int iWindowWidth, int iWindowHeight, CCamera *pCamera);

  int MakeCarShape(CShapeData **pShape, eWhipModel model);
  bool DeleteCarShape(int iKey);
  void MakeTrackShape(CShapeData **pShape, CTrack *pTrack);
  void MakeSigns(CTrack *pTrack);
  void MakeEnvirFloor(CTrack *pTrack);
  void MakeDebugTri(CShapeData **pShape, CTexture *pTexture,
                    const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2);
  void MakeDebugLine(CShapeData **pShape, CTexture *pTexture,
                    const glm::vec3 &p0, const glm::vec3 &p1);

private:
  CShader *m_pShader;
  CCarShapeMap m_carShapeMap;
  int m_iNextCarShapeKey;
  CShapeAy m_shapeAy;
};

//-------------------------------------------------------------------------------------------------
#endif