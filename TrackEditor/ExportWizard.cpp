#include "ExportWizard.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CExportWizard::CExportWizard(QWidget *pParent, eExportType exportType)
  : QDialog(pParent)
  , m_bExportSeparate(false)
  , m_bExportSigns(true)
  , m_exportType(exportType)
{
  setupUi(this);

  rbSeparate->setChecked(m_bExportSeparate);
  rbSingle->setChecked(!m_bExportSeparate);
  ckSigns->setChecked(m_bExportSigns);

  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  connect(rbSingle, &QRadioButton::clicked, this, &CExportWizard::OnSingleObject);
  connect(rbSeparate, &QRadioButton::clicked, this, &CExportWizard::OnSeparate);
  connect(ckSigns, &QCheckBox::toggled, this, &CExportWizard::OnSignsChecked);
  connect(pbCancel, &QPushButton::clicked, this, &CExportWizard::close);
  connect(pbExport, &QPushButton::clicked, this, &CExportWizard::accept);

  UpdateSignsCheckbox();
}

//-------------------------------------------------------------------------------------------------

CExportWizard::~CExportWizard()
{

}

//-------------------------------------------------------------------------------------------------

void CExportWizard::OnSingleObject()
{
  m_bExportSeparate = false;
  rbSeparate->setChecked(m_bExportSeparate);
  UpdateSignsCheckbox();
}

//-------------------------------------------------------------------------------------------------

void CExportWizard::OnSeparate()
{
  m_bExportSeparate = true;
  rbSingle->setChecked(!m_bExportSeparate);
  UpdateSignsCheckbox();
}

//-------------------------------------------------------------------------------------------------

void CExportWizard::OnSignsChecked(bool bChecked)
{
  m_bExportSigns = bChecked;
}

//-------------------------------------------------------------------------------------------------

void CExportWizard::UpdateSignsCheckbox()
{
  if (m_exportType == eExportType::EXPORT_OBJ) {
    if (!m_bExportSeparate) {
      ckSigns->setChecked(false);
      ckSigns->setEnabled(false);
    } else {
      ckSigns->setEnabled(true);
    }
  }
}

//-------------------------------------------------------------------------------------------------