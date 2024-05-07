#ifndef _WHIPLIB_SHAPEFACTORY_H
#define _WHIPLIB_SHAPEFACTORY_H
//-------------------------------------------------------------------------------------------------
#include "Types.h"
#include <vector>
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
  CShapeFactory(CShapeFactory const &) = delete;
  void operator=(CShapeFactory const &) = delete;

  CShapeData *MakeAxes(CShader *pShader);
  CShapeData *MakeModel(CShader *pShader, CTexture *pTexture, eWhipModel model, int iSignSurfaceType = -1);
  CShapeData *MakeAudioMarker(CShader *pShader);
  CShapeData *MakeStuntMarker(CShader *pShader);
  CShapeData *MakeTrackSurface(CShader *pShader, CTrackData *pTrack, eShapeSection section, bool bAttachLast, bool bWireframe = false);
  CShapeData *MakeAILine(CShader *pShader, CTrackData *pTrack, eShapeSection section, bool bAttachLast);
  CShapeData *MakeSelectedChunks(CShader *pShader, CTrackData *pTrack, int iStart, int iEnd);
  void MakeSigns(CShader *pShader, CTrackData *pTrack, std::vector<CShapeData*> &signAy);
  void MakeAudio(CShader *pShader, CTrackData *pTrack, std::vector<CShapeData *> &audioAy);
  void MakeStunts(CShader *pShader, CTrackData *pTrack, std::vector<CShapeData *> &stuntAy);
  //static CShapeData GenerateNormals(const CShapeData &data);
  void GetCarPos(CTrackData *pTrack, int iChunk, eShapeSection aiLine, glm::mat4 &modelToWorldMatrix, bool bMillionPlus = false);

  float m_fScale;

private:
  CShapeFactory();

  tVertex *MakeVertsAxes(uint32 &uiNumVerts);
  uint32 *MakeIndicesAxes(uint32 &uiNumIndices);

  tVertex *MakeModelVerts(uint32 &numVertices, CTexture *pTexture, eWhipModel model, int iSignSurfaceType);
  uint32 *MakeModelIndices(uint32 &numIndices, eWhipModel model);

  tVertex *MakeVertsAudioMarker(uint32 &uiNumVerts);
  uint32 *MakeIndicesAudioMarker(uint32 &uiNumIndices);
  tVertex *MakeVertsStuntMarker(uint32 &uiNumVerts);
  uint32 *MakeIndicesStuntMarker(uint32 &uiNumIndices);

  float *GetCoords(eWhipModel model);
  int GetCoordsCount(eWhipModel model);
  tPolygon *GetPols(eWhipModel model);
  int GetPolsCount(eWhipModel model);
  tAnimation *GetAnms(eWhipModel model);
  int GetAnmsCount(eWhipModel model);

  tVertex *MakeVerts(uint32 &numVertices, eShapeSection section, CTrackData *pTrack, CTexture *pTexture);
  tVertex *MakeVertsEnvirFloor(uint32 &numVertices, CTrackData *pTrack);

  uint32 *MakeIndicesEnvirFloor(uint32 &numIndices);
  uint32 *MakeIndicesCenterline(uint32 &numIndices, CTrackData *pTrack);
  uint32 *MakeIndicesSurface(uint32 &numIndices, CTrackData *pTrack, bool bAttachLast);
  uint32 *MakeIndicesSurfaceWireframe(uint32 &numIndices, CTrackData *pTrack, bool bAttachLast);
  uint32 *MakeIndicesSingleSection(uint32 &numIndices, eShapeSection section, CTrackData *pTrack, bool bAttachLast);
  uint32 *MakeIndicesSelectedChunks(uint32 &numIndices, int iStart, int iEnd, CTrackData *pTrack);
  uint32 *MakeIndicesSingleSectionWireframe(uint32 &numIndices, CTrackData *pTrack, bool bAttachLast);

  void ApplyVerticesSingleSection(int i, tVertex *vertices, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);

  eWhipModel m_signAy[17] = {
    eWhipModel::SIGN_TOWER,
    eWhipModel::SIGN_TOWER2,
    eWhipModel::SIGN_SIGN01,
    eWhipModel::SIGN_SIGN02,
    eWhipModel::SIGN_BUILD,
    eWhipModel::SIGN_BUILD1,
    eWhipModel::SIGN_BUILD2,
    eWhipModel::SIGN_BUILD3,
    eWhipModel::SIGN_HEELBAR,
    eWhipModel::SIGN_BALLOON,
    eWhipModel::SIGN_TREE,
    eWhipModel::SIGN_ADVERT,
    eWhipModel::SIGN_ADVERT2,
    eWhipModel::SIGN_QUADBLD,
    eWhipModel::SIGN_BLD0,
    eWhipModel::SIGN_BALLOON2,
    eWhipModel::SIGN_ADVERT3,
  };
  int m_signAyCount = sizeof(m_signAy) / sizeof(m_signAy[0]);

  static CShapeFactory s_shapeFactory;
};

//-------------------------------------------------------------------------------------------------
#endif