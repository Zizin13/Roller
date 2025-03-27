#ifndef _WHIPLIB_OBJEXPORTER_H
#define _WHIPLIB_OBJEXPORTER_H
//-------------------------------------------------------------------------------------------------
#include <vector>
#include <string>
//-------------------------------------------------------------------------------------------------
class CShapeData;
//-------------------------------------------------------------------------------------------------

class CObjExporter
{
public:
  static CObjExporter &GetObjExporter();
  ~CObjExporter();
  CObjExporter(CObjExporter const &) = delete;
  void operator=(CObjExporter const &) = delete;

  bool ExportMaterial(const char *szFolder, const char *szName);
  bool ExportShape(CShapeData *pShapeData, const char *szFile, const char *szMaterial);
  bool ExportTrack(std::vector<std::pair<std::string, CShapeData *>> trackSectionAy,
                   std::vector<CShapeData *> signAy,
                   const char *szFolder,
                   const char *szName);

private:
  CObjExporter();
};

//-------------------------------------------------------------------------------------------------
#endif