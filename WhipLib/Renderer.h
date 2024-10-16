#ifndef _WHIPLIB_RENDERER_H
#define _WHIPLIB_RENDERER_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include <vector>
//-------------------------------------------------------------------------------------------------
class CShader;
class CShapeData;
class CTexture;
class CPalette;
class Camera;
struct tCarShape;
//-------------------------------------------------------------------------------------------------
typedef std::vector<tCarShape *> CCarShapeAy;
//-------------------------------------------------------------------------------------------------

class CRenderer
{
public:
  CRenderer();
  ~CRenderer();

  bool Init();
  bool Shutdown();
  void Draw(int iWindowWidth, int iWindowHeight, Camera *pCamera);

  CShapeData *MakeCarShape(eWhipModel model);

private:
  CShader *m_pShader;
  CCarShapeAy m_carShapeAy;
};

//-------------------------------------------------------------------------------------------------
#endif