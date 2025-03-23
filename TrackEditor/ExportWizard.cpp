#include "ExportWizard.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
  #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CExportWizard::CExportWizard(QWidget *pParent)
  : QDialog(pParent)
  , m_bExportSeparate(false)
{
  setupUi(this);
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  connect(pbSingle, &QPushButton::clicked, this, &CExportWizard::OnSingleObject);
  connect(pbSeparate, &QPushButton::clicked, this, &CExportWizard::OnSeparate);
}

//-------------------------------------------------------------------------------------------------

CExportWizard::~CExportWizard()
{

}

//-------------------------------------------------------------------------------------------------

void CExportWizard::OnSingleObject()
{
  m_bExportSeparate = false;
  close();
}

//-------------------------------------------------------------------------------------------------

void CExportWizard::OnSeparate()
{
  m_bExportSeparate = true;
  close();
}

//-------------------------------------------------------------------------------------------------