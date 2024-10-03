#ifndef _WHIPLIB_SHAPEFACTORY_H
#define _WHIPLIB_SHAPEFACTORY_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include <vector>
#include <glm.hpp>
//-------------------------------------------------------------------------------------------------
class CShape;
class CShader;
class CTexture;
class CTrack;
struct tVertex;
struct tAnimation;
struct tPolygon;
//-------------------------------------------------------------------------------------------------

class CShapeFactory
{
public:
  static CShapeFactory &GetShapeFactory();
  ~CShapeFactory();
  CShapeFactory(CShapeFactory const &) = delete;
  void operator=(CShapeFactory const &) = delete;

  CShape *MakeAxes(CShader *pShader);
  CShape *MakeModel(CShader *pShader, CTexture *pTexture, eWhipModel model, int iSignSurfaceType = -1);
  CShape *MakeAudioMarker(CShader *pShader, CTexture *pTexture);
  CShape *MakeStuntMarker(CShader *pShader, CTexture *pTexture);
  CShape *MakeTrackSurface(CShape *pShape, CShader *pShader, CTrack *pTrack, eShapeSection section, bool bAttachLast, bool bWireframe = false);
  CShape *MakeEnvirFloor(CShape *pShape, CShader *pShader, CTrack *pTrack, int iIndex);
  CShape *MakeAILine(CShape *pShape, CShader *pShader, CTrack *pTrack, eShapeSection section, bool bAttachLast);
  CShape *MakeSelectedChunks(CShape *pShape, CShader *pShader, CTrack *pTrack, int iStart, int iEnd);
  void MakeSigns(CShader *pShader, CTrack *pTrack, std::vector<CShape*> &signAy);
  void MakeAudio(CShader *pShader, CTrack *pTrack, std::vector<CShape *> &audioAy);
  void MakeStunts(CShader *pShader, CTrack *pTrack, std::vector<CShape *> &stuntAy);
  CShape *MakeNormalsTest(const CShape &data, CShader *pShader);
  void GetCarPos(CTrack *pTrack, int iChunk, eShapeSection aiLine, glm::mat4 &modelToWorldMatrix, bool bMillionPlus = false);

  bool m_bOglRunning;

private:
  CShapeFactory();

  tVertex *MakeVertsAxes(uint32 &uiNumVerts);
  uint32 *MakeIndicesAxes(uint32 &uiNumIndices);

  tVertex *MakeModelVerts(uint32 &numVertices, CTexture *pTexture, eWhipModel model, int iSignSurfaceType);
  uint32 *MakeModelIndices(uint32 &numIndices, eWhipModel model);

  tVertex *MakeVertsAudioMarker(uint32 &uiNumVerts, CTexture *pTexture);
  uint32 *MakeIndicesAudioMarker(uint32 &uiNumIndices);
  tVertex *MakeVertsStuntMarker(uint32 &uiNumVert, CTexture *pTextures);
  uint32 *MakeIndicesStuntMarker(uint32 &uiNumIndices);

  float *GetCoords(eWhipModel model);
  int GetCoordsCount(eWhipModel model);
  tPolygon *GetPols(eWhipModel model);
  int GetPolsCount(eWhipModel model);
  tAnimation *GetAnms(eWhipModel model);
  int GetAnmsCount(eWhipModel model);
  uint32 *GetBacks(eWhipModel model);
  int GetBacksCount(eWhipModel model);

  tVertex *MakeVerts(uint32 &numVertices, eShapeSection section, CTrack *pTrack, CTexture *pTexture);
  tVertex *MakeVertsEnvirFloor(uint32 &numVertices, CTrack *pTrack, CTexture *pTexture, int iIndex);

  uint32 *MakeIndicesEnvirFloor(uint32 &numIndices);
  uint32 *MakeIndicesCenterline(uint32 &numIndices, CTrack *pTrack, bool bAttachLast);
  uint32 *MakeIndicesSurface(uint32 &numIndices, CTrack *pTrack, bool bAttachLast);
  uint32 *MakeIndicesSurfaceWireframe(uint32 &numIndices, CTrack *pTrack, bool bAttachLast);
  uint32 *MakeIndicesSingleSection(uint32 &numIndices, eShapeSection section, CTrack *pTrack, bool bAttachLast);
  uint32 *MakeIndicesExport(uint32 &numIndices, CTrack *pTrack);
  uint32 *MakeIndicesSelectedChunks(uint32 &numIndices, int iStart, int iEnd, CTrack *pTrack);
  uint32 *MakeIndicesSingleSectionWireframe(uint32 &numIndices, eShapeSection section, CTrack *pTrack, bool bAttachLast);

  void ApplyVerticesSingleSection(int i, tVertex *vertices,
                                  const glm::vec3 &v0,
                                  const glm::vec3 &v1,
                                  const glm::vec3 &v2,
                                  const glm::vec3 &v3);
  void MakeNormals(tVertex &topLeft, tVertex &topRight, tVertex &bottomLeft, tVertex &bottomRight);

  static CShapeFactory s_shapeFactory;
};

//-------------------------------------------------------------------------------------------------
#endif