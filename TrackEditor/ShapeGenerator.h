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
};
//-------------------------------------------------------------------------------------------------
#endif