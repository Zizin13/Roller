#ifndef _WHIPLIB_SHAPECOMPONENT_H
#define _WHIPLIB_SHAPECOMPONENT_H
//-------------------------------------------------------------------------------------------------
#include "Component.h"
#include "Types.h"
#include <string>
//-------------------------------------------------------------------------------------------------
class CShapeData;
class CTexture;
class CPalette;
class CShader;
//-------------------------------------------------------------------------------------------------

class CShapeComponent : public IComponent
{
public:
  CShapeComponent(CShader *pShader, const eWhipModel &model, const std::string &sTex, const std::string &sPal);
  ~CShapeComponent();

  bool Init() override;
  bool Shutdown() override;
  void Update() override;

private:
  eWhipModel m_model;
  std::string m_sTex;
  std::string m_sPal;
  CShapeData *m_pShapeData;
  CTexture *m_pTex;
  CPalette *m_pPal;
  CShader *m_pShader;
};

//-------------------------------------------------------------------------------------------------
#endif