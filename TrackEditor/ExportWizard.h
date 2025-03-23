#ifndef _TRACKEDITOR_EXPORTWIZARD_H
#define _TRACKEDITOR_EXPORTWIZARD_H
//-------------------------------------------------------------------------------------------------
#include "ui_ExportWizard.h"
//-------------------------------------------------------------------------------------------------

class CExportWizard : public QDialog, private Ui::ExportWizard
{
  Q_OBJECT

public:
  CExportWizard(QWidget *pParent);
  ~CExportWizard();

  bool m_bExportSeparate;

protected slots:
  void OnSingleObject();
  void OnSeparate();
};

//-------------------------------------------------------------------------------------------------
#endif