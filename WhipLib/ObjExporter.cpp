#include "ObjExporter.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

CObjExporter &CObjExporter::GetObjExporter()
{
  static CObjExporter s_objExporter;

  return s_objExporter;
}

//-------------------------------------------------------------------------------------------------

CObjExporter::CObjExporter()
{
}

//-------------------------------------------------------------------------------------------------

CObjExporter::~CObjExporter()
{
}

//-------------------------------------------------------------------------------------------------

bool CObjExporter::ExportShape(CShapeData *pShapeData, const char *szFile)
{
  return false;
}

//-------------------------------------------------------------------------------------------------

bool CObjExporter::ExportTrack(std::vector<std::pair<std::string, CShapeData *>> trackSectionAy,
                               std::vector<CShapeData *> signAy,
                               const char *szName)
{
  bool bSuccess = true;
  for (std::vector<std::pair<std::string, CShapeData *>>::iterator it = trackSectionAy.begin(); it != trackSectionAy.end(); ++it) {
    std::string sFilename = std::string(szName) + std::string("_") + it->first + std::string(".obj");
    bSuccess &= ExportShape(it->second, sFilename.c_str());
  }
  for (int i = 0; i < (int)signAy.size(); ++i) {
    std::string sFilename = std::string(szName) + std::string("_sign_") + std::to_string(i) + std::string(".obj");
    bSuccess &= ExportShape(signAy[i], sFilename.c_str());
  }
  return bSuccess;
}

//-------------------------------------------------------------------------------------------------