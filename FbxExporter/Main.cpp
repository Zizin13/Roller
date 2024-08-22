#include "ShapeFactory.h"
#include "FBXExporter.h"
#include "ShapeData.h"
#include "Texture.h"
#include "Palette.h"
//-------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  CShapeFactory::GetShapeFactory().m_fScale = 1.0f;
  CShapeFactory::GetShapeFactory().m_bOglRunning = false;

  eWhipModel carModel = CAR_XZIZIN;
  CPalette palette;
  CTexture carTex;
  CShapeData *pCar = CShapeFactory::GetShapeFactory().MakeModel(NULL, &carTex, carModel);

  CFBXExporter::GetFBXExporter().ExportShape(pCar, "C:\\WHIP\\test.fbx");

  delete pCar;
  return 0;
};

//-------------------------------------------------------------------------------------------------