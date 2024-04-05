#ifndef _TRACKEDITOR_SHAPEGENERATOR_H
#define _TRACKEDITOR_SHAPEGENERATOR_H
//-------------------------------------------------------------------------------------------------
#include "ShapeData.h"
//-------------------------------------------------------------------------------------------------
class ShapeGenerator
{
public:
  static tShapeData MakeTriangle();
  static tShapeData MakeCube();
  static tShapeData MakeArrow();
  static tShapeData MakePlane(unsigned int uiDimensions = 10);

private:
  static tShapeData MakePlaneVerts(unsigned int uiDimensions);
  static tShapeData MakePlaneIndices(unsigned int uiDimensions);
};
//-------------------------------------------------------------------------------------------------
#endif