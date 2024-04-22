#ifndef _UTILS_SHAPEGENERATOR_H
#define _UTILS_SHAPEGENERATOR_H
//-------------------------------------------------------------------------------------------------
#include "ShapeData.h"
#include "Vertex.h"
//-------------------------------------------------------------------------------------------------
class ShapeGenerator
{
public:
  //static CShapeData MakeTriangle();
  //static CShapeData MakeCube();
  //static CShapeData MakeArrow();
  //static CShapeData MakePlane(unsigned int uiDimensions = 10);
  //static CShapeData MakeSphere(GLuint uiTesselation = 20);
  //static CShapeData MakeTorus(GLuint uiTesselation = 20);
  //static CShapeData MakeTeapot(unsigned int uiTesselation = 10, const glm::mat4 &lidTransform = glm::mat4());
  //static CShapeData GenerateNormals(const CShapeData &data);

  static glm::vec3 RandomColor();

private:
  static tVertex *MakePlaneVerts(uint32 &numVertices, uint32 uiDimensions);
  static uint32 *MakePlaneIndices(uint32 &numIndices, uint32 uiDimensions);
  //static CIndexBuffer *MakePlaneUnseamedIndices(GLuint uiTesselation);
  //
  //      // Teapot helpers
  //static void generatePatches(float *v, float *n, float *tc, unsigned short *el, int grid);
  //static void moveLid(int grid, float *v, glm::mat4 lidTransform);
  //static void buildPatchReflect(int patchNum,
  //        float *B, float *dB,
  //        float *v, float *n,
  //        float *tc, unsigned short *el,
  //        int &index, int &elIndex, int &tcIndex, int grid,
  //        bool reflectX, bool reflectY);
  //static void buildPatch(glm::vec3 patch[][4],
  //        float *B, float *dB,
  //        float *v, float *n, float *tc,
  //        unsigned short *el,
  //        int &index, int &elIndex, int &tcIndex,
  //        int grid, glm::mat3 reflect,
  //        bool invertNormal);
  //static void getPatch(int patchNum, glm::vec3 patch[][4], bool reverseV);
  //static void computeBasisFunctions(float *B, float *dB, int grid);
  //static glm::vec3 evaluate(int gridU, int gridV, float *B, glm::vec3 patch[][4]);
  //static glm::vec3 evaluateNormal(int gridU, int gridV,
  //        float *B, float *dB, glm::vec3 patch[][4]);
};
//-------------------------------------------------------------------------------------------------
#endif