#ifndef _TRACKEDITOR_TRACKPREVIEW_H
#define _TRACKEDITOR_TRACKPREVIEW_H
//-------------------------------------------------------------------------------------------------
#include <QtOpenGL\qgl.h>
#include "Types.h"
//-------------------------------------------------------------------------------------------------
#define DEFAULT_HISTORY_MAX_SIZE 256 //approx 200KB per saved track
struct tTrackHistory
{
  std::string sDescription;
  std::vector<uint8> byteAy;
};
typedef std::vector<tTrackHistory> CHistoryAy;
//-------------------------------------------------------------------------------------------------
class CTrackPreviewPrivate;
class CTrack;
//-------------------------------------------------------------------------------------------------
class CTrackPreview : public QGLWidget
{
  Q_OBJECT

public:
  CTrackPreview(QWidget *pParent, const QString &sTrackFile = "");
  ~CTrackPreview();

  bool LoadTrack(const QString &sFilename);
  void UpdateTrack();
  void ShowModels(uint32 uiShowModels);
  void UpdateCar(eWhipModel carModel, eShapeSection aiLine, bool bMillionPlus);
  void SetScale(int iScale);
  void AttachLast(bool bAttachLast);
  CTrack *GetTrack();
  bool SaveChangesAndContinue();
  bool SaveTrack();
  bool SaveTrackAs();
  QString GetTitle(bool bFullPath);
  const QString &GetFilename() { return m_sTrackFile; };
  void UpdateGeometrySelection();
  void SaveHistory(const QString &sDescription);
  void Undo();
  void Redo();
  void DeleteTrackModels();

  bool m_bUnsavedChanges;
  int m_iSelFrom;
  int m_iSelTo;
  bool m_bToChecked;

protected:
  void initializeGL();
  void paintGL();
  void mouseClickEvent(QMouseEvent *pEvent);
  void mouseMoveEvent(QMouseEvent *pEvent);
  void keyPressEvent(QKeyEvent *pEvent);

private:
  void LoadHistory(const tTrackHistory *pHistory);

  CTrackPreviewPrivate *p;
  uint32 m_uiShowModels;
  eWhipModel m_carModel;
  eShapeSection m_carAILine;
  bool m_bMillionPlus;
  bool m_bAttachLast;
  int m_iScale;
  bool m_bAlreadySaved;
  QString m_sTrackFile;
  QString m_sLastCarTex;
  int m_iHistoryIndex;
};

//-------------------------------------------------------------------------------------------------
#endif