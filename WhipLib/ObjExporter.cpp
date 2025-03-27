#include "ObjExporter.h"
#include "ShapeData.h"
#include "Logging.h"
#include <fstream>
#include <sstream>
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

bool CObjExporter::ExportMaterial(const char *szFolder, const char *szName)
{
  //open output file
  std::string sOutFile = std::string(szFolder) + std::string("\\") + std::string(szName) + std::string(".mtl");
  std::ofstream out(sOutFile.c_str());
  if (!out.is_open()) {
    Logging::LogMessage("Error: Could not open output file %s", sOutFile.c_str());
    return false;
  }

  out << "newmtl " << szName << "\n";
  out << "map_Kd " << szName << ".png\n";
  out << "map_d " << szName << ".png\n";

  out.close();
  return true;
}

//-------------------------------------------------------------------------------------------------

bool CObjExporter::ExportShape(CShapeData *pShapeData, const char *szFile, const char *szMaterial)
{
  //open output file
  std::ofstream out(szFile);
  if (!out.is_open()) {
    Logging::LogMessage("Error: Could not open output file %s", szFile);
    return false;
  }

  out << "mtllib " << szMaterial << ".mtl\n";

  out << "#vertices\n";
  for (int i = 0; i < (int)pShapeData->m_uiNumVerts; ++i) {
    out << "v " 
      << std::to_string(pShapeData->m_vertices[i].position.x / 100.0f) << " " 
      << std::to_string(pShapeData->m_vertices[i].position.y / 100.0f) << " "
      << std::to_string(pShapeData->m_vertices[i].position.z / 100.0f) << "\n";
  }
  out << "#tex coords\n";
  for (int i = 0; i < (int)pShapeData->m_uiNumVerts; ++i) {
    out << "vt "
      << std::to_string(pShapeData->m_vertices[i].texCoords.x) << " "
      << std::to_string(pShapeData->m_vertices[i].texCoords.y) << "\n";
  }
  out << "#normals\n";
  for (int i = 0; i < (int)pShapeData->m_uiNumVerts; ++i) {
    out << "vn "
      << std::to_string(pShapeData->m_vertices[i].normal.x) << " "
      << std::to_string(pShapeData->m_vertices[i].normal.y) << " "
      << std::to_string(pShapeData->m_vertices[i].normal.z) << "\n";
  }
  out << "usemtl " << szMaterial << "\n";
  if (pShapeData->m_drawType == GL_TRIANGLES) {
    out << "#pols\n";
    int iNumPols = (int)pShapeData->m_uiNumIndices / 3;
    for (int i = 0; i < iNumPols; ++i) {
      out << "f ";
      for (int j = 0; j < 3; ++j) {
        std::string sIndex = std::to_string(pShapeData->m_indices[i * 3 + j] + 1);
        out << sIndex << "/" << sIndex << "/" << sIndex;
        if (j != 2)
          out << " ";
      }
      out << "\n";
    }
  } else if (pShapeData->m_drawType == GL_LINES) {
    out << "#lines\n";
    out << "l ";
    for (int i = 0; i < (int)pShapeData->m_uiNumVerts; ++i) {
      out << std::to_string(i) << " ";
    }
    out << "\n";
  }

  out.close();
  return true;
}

//-------------------------------------------------------------------------------------------------

bool CObjExporter::ExportTrack(std::vector<std::pair<std::string, CShapeData *>> trackSectionAy,
                               std::vector<CShapeData *> signAy,
                               const char *szFolder,
                               const char *szName)
{
  std::string sBld = std::string(szName) + std::string("_BLD");
  ExportMaterial(szFolder, szName);
  ExportMaterial(szFolder, sBld.c_str());
  bool bSuccess = true;
  for (std::vector<std::pair<std::string, CShapeData *>>::iterator it = trackSectionAy.begin(); it != trackSectionAy.end(); ++it) {
    std::string sFilename = std::string (szFolder) + std::string("\\") + std::string(szName) + std::string(" ") + it->first + std::string(".obj");
    bSuccess &= ExportShape(it->second, sFilename.c_str(), szName);
  }
  for (int i = 0; i < (int)signAy.size(); ++i) {
    std::string sFilename = std::string(szFolder) + std::string("\\") + std::string(szName) + std::string(" Sign ") + std::to_string(i) + std::string(".obj");
    bSuccess &= ExportShape(signAy[i], sFilename.c_str(), sBld.c_str());
  }
  return bSuccess;
}

//-------------------------------------------------------------------------------------------------