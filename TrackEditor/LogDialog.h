#ifndef _TRACKEDITOR_LOGDIALOG_H
#define _TRACKEDITOR_LOGDIALOG_H
//-------------------------------------------------------------------------------------------------
#include "ui_LogDialog.h"
//-------------------------------------------------------------------------------------------------

class CLogDialog : public QDialog, private Ui::LogDialog
{
  Q_OBJECT

public:
  CLogDialog(QWidget *pParent);
  ~CLogDialog();

  void LogMessage(const QString &sMsg);
};

//-------------------------------------------------------------------------------------------------
#endif