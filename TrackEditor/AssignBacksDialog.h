#ifndef _TRACKEDITOR_ASSIGNBACKSDIALOG_H
#define _TRACKEDITOR_ASSIGNBACKSDIALOG_H
//-------------------------------------------------------------------------------------------------
#include "ui_AssignBacksDialog.h"
//-------------------------------------------------------------------------------------------------
class CTrack;
//-------------------------------------------------------------------------------------------------
class CAssignBacksDialog : public QDialog, private Ui::AssignBacksDialog
{
  Q_OBJECT

public:
  CAssignBacksDialog(QWidget *pParent, CTrack *pTrack);
  ~CAssignBacksDialog();
};

//-------------------------------------------------------------------------------------------------
#endif