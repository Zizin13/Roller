#include "ExportWizard.h"
#include "QtHelpers.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CExportWizard::CExportWizard(QWidget *pParent, eExportType exportType)
  : QDialog(pParent)
  , m_bExportSeparate(true)
  , m_bExportBacks(true)
  , m_bExportSigns(true)
  , m_exportType(exportType)
{
  setupUi(this);

  ckSections->setChecked(m_bExportSeparate);
  ckBacks->setChecked(m_bExportBacks);
  ckSigns->setChecked(m_bExportSigns);

  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  connect(ckSections, &QCheckBox::toggled,    this, &CExportWizard::OnSeparateChecked);
  connect(ckBacks,    &QCheckBox::toggled,    this, &CExportWizard::OnBacksChecked);
  connect(ckSigns,    &QCheckBox::toggled,    this, &CExportWizard::OnSignsChecked);
  connect(pbCancel,   &QPushButton::clicked,  this, &CExportWizard::close);
  connect(pbExport,   &QPushButton::clicked,  this, &CExportWizard::accept);
}

//-------------------------------------------------------------------------------------------------

CExportWizard::~CExportWizard()
{

}

//-------------------------------------------------------------------------------------------------

void CExportWizard::OnSeparateChecked(bool bChecked)
{
  m_bExportSeparate = bChecked;
}

//-------------------------------------------------------------------------------------------------

void CExportWizard::OnBacksChecked(bool bChecked)
{
  m_bExportBacks = bChecked;
}

//-------------------------------------------------------------------------------------------------

void CExportWizard::OnSignsChecked(bool bChecked)
{
  m_bExportSigns = bChecked;
}

//-------------------------------------------------------------------------------------------------