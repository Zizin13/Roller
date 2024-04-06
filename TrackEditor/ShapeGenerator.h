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
  static tShapeData MakeTeapot(unsigned int uiTesselation = 10, const glm::mat4 &lidTransform = glm::mat4());
  static tShapeData GenerateNormals(const tShapeData &data);

private:
  static tShapeData MakePlaneVerts(unsigned int uiDimensions);
  static tShapeData MakePlaneIndices(unsigned int uiDimensions);

        // Teapot helpers
  static void generatePatches(float *v, float *n, float *tc, unsigned short *el, int grid);
  static void moveLid(int grid, float *v, glm::mat4 lidTransform);
  static void buildPatchReflect(int patchNum,
          float *B, float *dB,
          float *v, float *n,
          float *tc, unsigned short *el,
          int &index, int &elIndex, int &tcIndex, int grid,
          bool reflectX, bool reflectY);
  static void buildPatch(glm::vec3 patch[][4],
          float *B, float *dB,
          float *v, float *n, float *tc,
          unsigned short *el,
          int &index, int &elIndex, int &tcIndex,
          int grid, glm::mat3 reflect,
          bool invertNormal);
  static void getPatch(int patchNum, glm::vec3 patch[][4], bool reverseV);
  static void computeBasisFunctions(float *B, float *dB, int grid);
  static glm::vec3 evaluate(int gridU, int gridV, float *B, glm::vec3 patch[][4]);
  static glm::vec3 evaluateNormal(int gridU, int gridV,
          float *B, float *dB, glm::vec3 patch[][4]);
};
//-------------------------------------------------------------------------------------------------
#endif