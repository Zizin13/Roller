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
  , m_bDefinedBacksOnly(true)
  , m_exportType(exportType)
{
  setupUi(this);

  rbSeparate->setChecked(m_bExportSeparate);
  rbSingle->setChecked(!m_bExportSeparate);
  ckSigns->setChecked(m_bExportSigns);
  rbBacksAll->setChecked(!m_bDefinedBacksOnly);
  rbBacksDefined->setChecked(m_bDefinedBacksOnly);
  rbBacksAll->hide();
  rbBacksDefined->hide();
  lineBacks->hide();

  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  connect(rbSingle,       &QRadioButton::toggled, this, &CExportWizard::OnSingleObject);
  connect(rbSeparate,     &QRadioButton::toggled, this, &CExportWizard::OnSeparate);
  connect(rbBacksAll,     &QRadioButton::toggled, this, &CExportWizard::OnAllBacks);
  connect(rbBacksDefined, &QRadioButton::toggled, this, &CExportWizard::OnDefinedBacksOnly);
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

void CExportWizard::OnDefinedBacksOnly(bool bChecked)
{
  m_bDefinedBacksOnly = bChecked;
  BLOCK_SIG_AND_DO(rbBacksAll, setChecked(!bChecked));
}

//-------------------------------------------------------------------------------------------------

void CExportWizard::OnAllBacks(bool bChecked)
{
  m_bDefinedBacksOnly = !bChecked;
  BLOCK_SIG_AND_DO(rbBacksDefined, setChecked(!bChecked));
}

//-------------------------------------------------------------------------------------------------