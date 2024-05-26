#ifndef _TRACKEDITOR_ASSIGNBACKSDIALOG_H
#define _TRACKEDITOR_ASSIGNBACKSDIALOG_H
//-------------------------------------------------------------------------------------------------
#include "ui_AssignBacksDialog.h"
#include <vector>
//-------------------------------------------------------------------------------------------------
class CTrack;
class CBackWidget;
typedef std::vector<CBackWidget *> CBackWidgetAy;
//-------------------------------------------------------------------------------------------------
class CAssignBacksDialog : public QDialog, private Ui::AssignBacksDialog
{
  Q_OBJECT

public:
  CAssignBacksDialog(QWidget *pParent, CTrack *pTrack);
  ~CAssignBacksDialog();

protected slots:
  void OnApplyClicked();

private:
  CBackWidgetAy m_backWidgetAy;
  CTrack *m_pTrack;
};

//-------------------------------------------------------------------------------------------------
#endif