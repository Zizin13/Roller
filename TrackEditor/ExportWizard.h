#ifndef _TRACKEDITOR_EXPORTWIZARD_H
#define _TRACKEDITOR_EXPORTWIZARD_H
//-------------------------------------------------------------------------------------------------
#include "ui_ExportWizard.h"
#include "TrackPreview.h"
//-------------------------------------------------------------------------------------------------

class CExportWizard : public QDialog, private Ui::ExportWizard
{
  Q_OBJECT

public:
  CExportWizard(QWidget *pParent, eExportType exportType);
  ~CExportWizard();

  bool m_bExportSeparate;
  bool m_bExportSigns;

protected slots:
  void OnSingleObject(bool bChecked);
  void OnSeparate(bool bChecked);
  void OnSignsChecked(bool bChecked);

private:
  eExportType m_exportType;
};

//-------------------------------------------------------------------------------------------------
#endif