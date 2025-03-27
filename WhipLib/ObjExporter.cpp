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

bool CObjExporter::ExportTrack(std::vector<std::pair<std::string, CShapeData *>> trackSectionAy,
                               std::vector<CShapeData *> signAy,
                               const char *szFile)
{
  return false;
}

//-------------------------------------------------------------------------------------------------