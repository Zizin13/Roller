#ifndef _WHIPLIB_SHAPECOMPONENT_H
#define _WHIPLIB_SHAPECOMPONENT_H
//-------------------------------------------------------------------------------------------------
#include "Component.h"
#include "Types.h"
#include <string>
//-------------------------------------------------------------------------------------------------
class CShapeData;
//-------------------------------------------------------------------------------------------------

class CShapeComponent : public IComponent
{
public:
  CShapeComponent();
  ~CShapeComponent();

  bool Init() override;
  bool Shutdown() override;
  void Update() override;

  void SetData(CShapeData *pShapeData);

private:
  CShapeData *m_pShapeData;
};

//-------------------------------------------------------------------------------------------------
#endif