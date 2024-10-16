#ifndef _WHIPLIB_RENDERER_H
#define _WHIPLIB_RENDERER_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include <vector>
//-------------------------------------------------------------------------------------------------
class CShader;
class CShapeData;
class CCamera;
struct tCarShape;
//-------------------------------------------------------------------------------------------------
typedef std::vector<tCarShape *> CCarShapeAy;
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

private:
  std::string m_sFatDataDir;
  CShader *m_pShader;
  CCarShapeAy m_carShapeAy;
};

//-------------------------------------------------------------------------------------------------
#endif