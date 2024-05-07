#ifndef _TRACKEDITOR_NEWTRACKDIALOG_H
#define _TRACKEDITOR_NEWTRACKDIALOG_H
//-------------------------------------------------------------------------------------------------
#include "ui_NewTrackDialog.h"
//-------------------------------------------------------------------------------------------------

class CNewTrackDialog : public QDialog, private Ui::NewTrackDialog
{
  Q_OBJECT

public:
  CNewTrackDialog(QWidget *pParent, int iNewTrackNum);
  ~CNewTrackDialog();

  QString GetFilename();
  QString GetTex();
  QString GetBld();

protected slots:
  void BrowseClicked();
  void OkClicked();

private:
  void UpdateDialog();
};

//-------------------------------------------------------------------------------------------------
#endif