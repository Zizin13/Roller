#ifndef _WHIPLIB_SHAPEFACTORY_H
#define _WHIPLIB_SHAPEFACTORY_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
//-------------------------------------------------------------------------------------------------
class CShapeData;
class CShader;
class CTexture;
class CTrackData;
struct tVertex;
struct tAnimation;
struct tPolygon;
//-------------------------------------------------------------------------------------------------

class CShapeFactory
{
public:
  static CShapeFactory &GetShapeFactory();
  ~CShapeFactory();

  CShapeData *MakeAxes(CShader *pShader);
  CShapeData *MakeModel(CShader *pShader, CTexture *pTexture, eWhipModel model);
  CShapeData *MakeTrackCenterline(CShader *pShader, CTrackData *pTrack);
  CShapeData *MakeTrackSurface(CShader *pShader, CTexture *pTexture, CTrackData *pTrack, eShapeSection section, bool bWireframe = false);
  CShapeData *MakeAILine(CShader *pShader, CTrackData *pTrack, eShapeSection section);
  CShapeData *MakeSelectedChunks(CShader *pShader, CTrackData *pTrack, int iStart, int iEnd);
  void GetCarPos(CTrackData *pTrack, int iChunk, eShapeSection aiLine, glm::mat4 &modelToWorldMatrix, bool bMillionPlus = false);

  float m_fScale;
  int m_iAILineHeight;

private:
  CShapeFactory();

  tVertex *MakeVertsAxes(uint32 &uiNumVerts);
  uint32 *MakeIndicesAxes(uint32 &uiNumIndices);

  tVertex *MakeModelVerts(uint32 &numVertices, CTexture *pTexture, eWhipModel model);
  uint32 *MakeModelIndices(uint32 &numIndices, eWhipModel model);

  float *GetCoords(eWhipModel model);
  int GetCoordsCount(eWhipModel model);
  tPolygon *GetPols(eWhipModel model);
  int GetPolsCount(eWhipModel model);
  tAnimation *GetAnms(eWhipModel model);
  int GetAnmsCount(eWhipModel model);

  tVertex *MakeVerts(uint32 &numVertices, eShapeSection section, CTrackData *pTrack, CTexture *pTexture);
  tVertex *MakeVertsEnvirFloor(uint32 &numVertices, CTrackData *pTrack);

  uint32 *MakeIndicesCenterline(uint32 &numIndices, CTrackData *pTrack);
  uint32 *MakeIndicesSurface(uint32 &numIndices, CTrackData *pTrack);
  uint32 *MakeIndicesSurfaceWireframe(uint32 &numIndices, CTrackData *pTrack);
  uint32 *MakeIndicesSingleSection(uint32 &numIndices, eShapeSection section, CTrackData *pTrack);
  uint32 *MakeIndicesSelectedChunks(uint32 &numIndices, int iStart, int iEnd, CTrackData *pTrack);
  bool ShouldMakeIndicesForChunk(int i, eShapeSection section, CTrackData *pTrack);
  bool ShouldDrawSurfaceType(int iSurfaceType);
  uint32 *MakeIndicesSingleSectionWireframe(uint32 &numIndices, CTrackData *pTrack);

  void GetCenter(CTrackData *pTrack, int i, glm::vec3 prevCenter,
                 glm::vec3 &center, glm::vec3 &pitchAxis, glm::vec3 &nextChunkPitched,
                 glm::mat4 &yawMat, glm::mat4 &pitchMat, glm::mat4 &rollMat);
  void GetLane(CTrackData *pTrack, int i, glm::vec3 center, glm::vec3 pitchAxis, glm::mat4 rollMat,
               glm::vec3 &lane, bool bLeft);
  void GetShoulder(CTrackData *pTrack, int i, glm::vec3 attach, glm::vec3 pitchAxis, glm::mat4 rollMat, glm::vec3 nextChunkPitched,
                   glm::vec3 &shoulder, bool bLeft, bool bIgnoreHeight = false);
  void GetEnvirFloor(CTrackData *pTrack, int i, glm::vec3 lShoulder, glm::vec3 rShoulder,
                     glm::vec3 &lEnvirFloor, glm::vec3 &rEnvirFloor);
  void GetOWallFloor(CTrackData *pTrack, int i, glm::vec3 lLane, glm::vec3 rLane, glm::vec3 pitchAxis, glm::vec3 nextChunkPitched,
                     glm::vec3 &lFloor, glm::vec3 &rFloor);
  void GetWall(CTrackData *pTrack, int i, glm::vec3 bottomAttach, glm::vec3 pitchAxis, glm::mat4 rollMat, glm::vec3 nextChunkPitched,
               glm::vec3 &lloWall, eShapeSection wallSection);
  void GetAILine(CTrackData *pTrack, int i, glm::vec3 center, glm::vec3 pitchAxis, glm::mat4 rollMat, glm::vec3 nextChunkPitched,
               glm::vec3 &aiLine, eShapeSection lineSection, int iHeight);
  void ApplyVerticesSingleSection(int i, tVertex *vertices, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);

  static CShapeFactory s_shapeFactory;
};

//-------------------------------------------------------------------------------------------------
#endif