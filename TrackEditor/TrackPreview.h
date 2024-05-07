#ifndef _TRACKEDITOR_TRACKPREVIEW_H
#define _TRACKEDITOR_TRACKPREVIEW_H
//-------------------------------------------------------------------------------------------------
#include <QtOpenGL\qgl.h>
#include "Types.h"
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

  bool m_bUnsavedChanges;
  int m_iSelFrom;
  int m_iSelTo;
  bool m_bToChecked;

protected:
  void initializeGL();
  void paintGL();
  void mouseMoveEvent(QMouseEvent *pEvent);
  void keyPressEvent(QKeyEvent *pEvent);

private:
  CTrackPreviewPrivate *p;
  uint32 m_uiShowModels;
  eWhipModel m_carModel;
  eShapeSection m_carAILine;
  bool m_bMillionPlus;
  bool m_bAttachLast;
  int m_iScale;
  bool m_bAlreadySaved;
  QString m_sTrackFile;
};

//-------------------------------------------------------------------------------------------------
#endif