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

bool CObjExporter::ExportShape(CShapeData *pShapeData, const char *szFile)
{
  //open output file
  std::ofstream out(szFile);
  if (!out.is_open()) {
    Logging::LogMessage("Error: Could not open output file %s", szFile);
    return false;
  }

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
  if (pShapeData->m_drawType == GL_TRIANGLES) {
    out << "#pols\n";
    int iNumPols = (int)pShapeData->m_uiNumIndices / 3;
    for (int i = 0; i < iNumPols; ++i) {
      out << "f ";
      for (int j = 0; j < 3; ++j) {
        std::string sIndex = std::to_string(pShapeData->m_indices[i * 3 + j]);
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
  bool bSuccess = true;
  for (std::vector<std::pair<std::string, CShapeData *>>::iterator it = trackSectionAy.begin(); it != trackSectionAy.end(); ++it) {
    std::string sFilename = std::string (szFolder) + std::string("\\") + std::string(szName) + std::string(" ") + it->first + std::string(".obj");
    bSuccess &= ExportShape(it->second, sFilename.c_str());
  }
  for (int i = 0; i < (int)signAy.size(); ++i) {
    std::string sFilename = std::string(szFolder) + std::string("\\") + std::string(szName) + std::string(" Sign ") + std::to_string(i) + std::string(".obj");
    bSuccess &= ExportShape(signAy[i], sFilename.c_str());
  }
  return bSuccess;
}

//-------------------------------------------------------------------------------------------------