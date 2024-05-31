#ifndef _TRACKEDITOR_PREFERENCESDIALOG_H
#define _TRACKEDITOR_PREFERENCESDIALOG_H
//-------------------------------------------------------------------------------------------------
#include "ui_PreferencesDialog.h"
//-------------------------------------------------------------------------------------------------

class CPreferencesDialog : public QDialog, private Ui::PreferencesDialog
{
  Q_OBJECT

public:
  CPreferencesDialog(QWidget *pParent);
  ~CPreferencesDialog();

protected slots:
  void PasteNewToggled(bool bChecked);
  void PasteDataToggled(bool bChecked);
  void DialogEdited();

private:
  void UpdateText();
};

//-------------------------------------------------------------------------------------------------
#endif