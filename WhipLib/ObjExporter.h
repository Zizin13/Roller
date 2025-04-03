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

  bool ExportMaterial(const std::string &sFile, const std::vector<std::string> &texAy);
  bool ExportShapes(std::vector<std::pair<std::string, CShapeData *>> shapeAy, const std::string &sFile, const std::string &sMtlFile, const std::string &sMtlName);
  bool ExportTrack(std::vector<std::pair<std::string, CShapeData *>> trackSectionAy,
                   std::vector<CShapeData *> signAy,
                   std::vector<CShapeData *> signBackAy,
                   const std::string &sFolder,
                   const std::string &sName,
                   const std::string &sFile);

private:
  CObjExporter();
  void ExportShape_Internal(std::ofstream &out, int &iOffset, CShapeData *pShapeData, const std::string &sName, const std::string &sMtlName);
};

//-------------------------------------------------------------------------------------------------
#endif