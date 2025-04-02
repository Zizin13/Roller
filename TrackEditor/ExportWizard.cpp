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
  , m_bExportSigns(true)
  , m_exportType(exportType)
{
  setupUi(this);

  rbSeparate->setChecked(m_bExportSeparate);
  rbSingle->setChecked(!m_bExportSeparate);
  ckSigns->setChecked(m_bExportSigns);

  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  connect(rbSingle,       &QRadioButton::toggled, this, &CExportWizard::OnSingleObject);
  connect(rbSeparate,     &QRadioButton::toggled, this, &CExportWizard::OnSeparate);
  connect(ckSigns,        &QCheckBox::toggled,    this, &CExportWizard::OnSignsChecked);
  connect(pbCancel,       &QPushButton::clicked,  this, &CExportWizard::close);
  connect(pbExport,       &QPushButton::clicked,  this, &CExportWizard::accept);
}

//-------------------------------------------------------------------------------------------------

CExportWizard::~CExportWizard()
{

}

//-------------------------------------------------------------------------------------------------

void CExportWizard::OnSingleObject(bool bChecked)
{
  m_bExportSeparate = !bChecked;
  BLOCK_SIG_AND_DO(rbSeparate, setChecked(!bChecked));
}

//-------------------------------------------------------------------------------------------------

void CExportWizard::OnSeparate(bool bChecked)
{
  m_bExportSeparate = bChecked;
  BLOCK_SIG_AND_DO(rbSingle, setChecked(!bChecked));
}

//-------------------------------------------------------------------------------------------------

void CExportWizard::OnSignsChecked(bool bChecked)
{
  m_bExportSigns = bChecked;
}

//-------------------------------------------------------------------------------------------------