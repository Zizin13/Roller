#include <fstream>
#include <sstream>
#include <vector>
#include <assert.h>
#include <algorithm>
#include "Polygon.h"
#include "Animation.h"
#include "Types.h"

//-------------------------------------------------------------------------------------------------
// just a little app to turn the "carplans.c.asm" file created by 
// wcdatool disassembly tool for watcom compiler into something 
// a little more usable
//-------------------------------------------------------------------------------------------------

std::vector<uint8> HexToBytes(const std::string &sBytes)
{
  std::vector<uint8> byteAy;

  for (uint32 i = 0; i < sBytes.length(); i += 2) {
    std::string sByte = sBytes.substr(i, 2);
    char byte = (char)strtol(sByte.c_str(), NULL, 16);
    byteAy.push_back(byte);
  }

  return byteAy;
}

//-------------------------------------------------------------------------------------------------

std::vector<float> BytesToFloats(const std::vector<uint8> bytes)
{
  std::vector<float> floatAy;

  int j = 0;
  uint32 uiData = 0;
  for (uint32 i = 0; i < bytes.size(); ++i) {
    uint32 uiTemp = 0;
    uint8 c = (uint8)bytes[i];
    uiTemp |= c;
    uiTemp = uiTemp << 8 * j;
    uiData |= uiTemp;
    ++j;

    if (j == 4) {
      float *pfData = reinterpret_cast<float *>(&uiData);
      floatAy.push_back(*pfData);
      uiData = 0;
      j = 0;
    }
  }

  return floatAy;
}

//-------------------------------------------------------------------------------------------------

std::vector<uint32> BytesToUInts(const std::vector<uint8> bytes)
{
  std::vector<uint32> intAy;

  int j = 0;
  uint32 uiData = 0;
  for (uint32 i = 0; i < bytes.size(); ++i) {
    uint32 uiTemp = 0;
    uint8 c = (uint8)bytes[i];
    uiTemp |= c;
    uiTemp = uiTemp << 8 * j;
    uiData |= uiTemp;
    ++j;

    if (j == 4) {
      intAy.push_back(uiData);
      uiData = 0;
      j = 0;
    }
  }

  return intAy;
}

//-------------------------------------------------------------------------------------------------

std::vector<tPolygon> BytesToPols(const std::vector<uint8> bytes)
{
  std::vector<tPolygon> polAy;

  int j = 0;
  tPolygon data;
  memset(&data, 0, sizeof(data));
  for (uint32 i = 0; i < bytes.size(); ++i) {
    uint8 c = bytes[i];
    uint32 uiTemp = 0;

    switch (j) {
      case 0:
        data.byVert1 = c;
        break;
      case 1:
        data.byVert2 = c;
        break;
      case 2:
        data.byVert3 = c;
        break;
      case 3:
        data.byVert4 = c;
        break;
      case 4:
      case 5:
      case 6:
      case 7:
        uiTemp |= c;
        uiTemp = uiTemp << 8 * (j - 4);
        data.uiTex |= uiTemp;
        break;
      case 8:
        data.byUnknown1 = c;
        break;
      case 9:
        data.byUnknown2 = c;
        break;
      case 10:
        data.byUnknown3 = c;
        break;
      case 11:
        data.byUnknown4 = c;
        break;
    }

    ++j;

    if (j == 12) {
      polAy.push_back(data);
      memset(&data, 0, sizeof(data));
      j = 0;
    }
  }

  return polAy;
}

//-------------------------------------------------------------------------------------------------

std::vector<tAnimation> UIntsToAnms(const std::vector<uint32> uints)
{
  std::vector<tAnimation> anmAy;

  int j = 0;
  tAnimation data;
  memset(&data, 0, sizeof(data));
  for (uint32 i = 0; i < uints.size(); ++i) {
    if (j == 0)
      data.uiCount = uints[i];
    else
      data.framesAy[j - 1] = uints[i];

    ++j;

    if (j == 17) {
      anmAy.push_back(data);
      memset(&data, 0, sizeof(data));
      j = 0;
    }
  }

  return anmAy;
}

//-------------------------------------------------------------------------------------------------

std::string GetStringBytes(const std::string &sLine)
{
  std::string sRet;
  size_t colPos = sLine.find(":\t");
  size_t hexPos = sLine.find("0x");
  
  if (colPos != std::string::npos && hexPos != std::string::npos) {
    sRet = sLine.substr(colPos + 2, hexPos - colPos);
    size_t dbPos = sRet.find("\tdb");

    sRet = sRet.substr(0, dbPos - 1);
  }
  
  sRet.erase(remove_if(sRet.begin(), sRet.end(), isspace), sRet.end());
  
  return sRet;
}

//-------------------------------------------------------------------------------------------------

enum eCarPlansSection
{
  NONE = 0,
  COORDS,
  POLS,
  BACKS,
  PLACES,
  ANMS
};

//-------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  //must pass in filename
  if (argc < 4) {
    printf("Usage: CarPlansParser input_file output_file car_name\n");
    printf("       ex: CarPlansParser FATAL.EXE_disasm_object_3_disassembly_formatted.asm ZizinPlans.h xzizin\n");
    return -1;
  }

  std::string sCarName = argv[3];
  std::string sCarNameLower = sCarName;
  std::transform(sCarNameLower.begin(), sCarNameLower.end(), sCarNameLower.begin(),
    [](unsigned char c) { return std::tolower(c); });
  std::string sCarNameUpper = sCarName;
  std::transform(sCarNameUpper.begin(), sCarNameUpper.end(), sCarNameUpper.begin(),
    [](unsigned char c) { return std::toupper(c); });

  //open input file
  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    printf("failed to open input file\n");
    return -1;
  }
  
  eCarPlansSection section = eCarPlansSection::NONE;
  std::string sLine;
  std::string sCoordsBytes;
  std::string sPolsBytes;
  std::string sBacksBytes;
  std::string sPlacesBytes;
  std::string sAnmsBytes;
  int i = 0;
  while (std::getline(file, sLine)) {
    std::string sCoordsComp = "_" + sCarNameLower + "_coords:";
    std::string sPolsComp = "_" + sCarNameLower + "_pols:";
    std::string sBacksComp = "_" + sCarNameLower + "_backs:";
    std::string sPlacesComp = "_" + sCarNameLower + "_places:";
    std::string sAnmsComp = "_" + sCarNameLower + "_anms:";
    if (sLine.compare(sCoordsComp) == 0)
      section = eCarPlansSection::COORDS;
    else if (sLine.compare(sPolsComp) == 0)
      section = eCarPlansSection::POLS;
    else if (sLine.compare(sBacksComp) == 0)
      section = eCarPlansSection::BACKS;
    else if (sLine.compare(sPlacesComp) == 0)
      section = eCarPlansSection::PLACES;
    else if (sLine.compare(sAnmsComp) == 0)
      section = eCarPlansSection::ANMS;
    else if (section != eCarPlansSection::NONE && sLine.at(0) == '_')
      break;

    if (section == eCarPlansSection::COORDS)
      sCoordsBytes += GetStringBytes(sLine);
    if (section == eCarPlansSection::POLS)
      sPolsBytes += GetStringBytes(sLine);
    if (section == eCarPlansSection::BACKS)
      sBacksBytes += GetStringBytes(sLine);
    if (section == eCarPlansSection::PLACES)
      sPlacesBytes += GetStringBytes(sLine);
    if (section == eCarPlansSection::ANMS)
      sAnmsBytes += GetStringBytes(sLine);
  }
  file.close();

  printf("file loaded...\n");

  std::vector<uint8> bytes = HexToBytes(sCoordsBytes);
  std::vector<float> coordsFloats = BytesToFloats(bytes);
  printf("found %d floats in _%s_coords\n", (int)coordsFloats.size(), sCarNameLower.c_str());

  bytes = HexToBytes(sPolsBytes);
  std::vector<tPolygon> pols = BytesToPols(bytes);
  printf("found %d polygons in _%s_pols\n", (int)pols.size(), sCarNameLower.c_str());

  bytes = HexToBytes(sBacksBytes);
  std::vector<uint32> backsUInts = BytesToUInts(bytes);
  printf("found %d uints in _%s_backs\n", (int)backsUInts.size(), sCarNameLower.c_str());

  bytes = HexToBytes(sPlacesBytes);
  std::vector<uint32> placesUInts = BytesToUInts(bytes);
  printf("found %d uints in _%s_places\n", (int)placesUInts.size(), sCarNameLower.c_str());

  bytes = HexToBytes(sAnmsBytes);
  std::vector<uint32> anmsUInts = BytesToUInts(bytes);
  std::vector<tAnimation> anms = UIntsToAnms(anmsUInts);
  printf("found %d animations in _%s_anms\n", (int)anms.size(), sCarNameLower.c_str());

  //open output file
  std::ofstream out(argv[2]);
  if (!out.is_open()) {
    printf("failed to open output file\n");
    return -1;
  }


  out << "#ifndef _WHIPLIB_" << sCarNameUpper << "PLANS_H\n";
  out << "#define _WHIPLIB_" << sCarNameUpper <<"PLANS_H\n";
  out << "//-------------------------------------------------------------------------------------------------\n";
  out << "#include \"Polygon.h\"\n";
  out << "#include \"Animation.h\"\n";
  out << "//-------------------------------------------------------------------------------------------------\n";


  printf("writing _%s_coords\n", sCarNameLower.c_str());
  out << "float g_" << sCarNameLower << "Coords[] = {\n";
  for (int i = 0; i < coordsFloats.size(); ++i) {
    out << coordsFloats[i] << "f,\n";
  }
  out << "};\n";
  out << "int g_" << sCarNameLower << "CoordsCount = sizeof(g_" << sCarNameLower << "Coords) / sizeof(float); \n";
  out << "//-------------------------------------------------------------------------------------------------\n";


  printf("writing _%s_pols\n", sCarNameLower.c_str());
  out << "tPolygon g_" << sCarNameLower << "Pols[] = {\n";
  for (int i = 0; i < pols.size(); ++i) {
    out << "{"
      << (int)pols[i].byVert1 << ","
      << (int)pols[i].byVert2 << ","
      << (int)pols[i].byVert3 << ","
      << (int)pols[i].byVert4 << ","
      << pols[i].uiTex << ","
      << (int)pols[i].byUnknown1 << ","
      << (int)pols[i].byUnknown2 << ","
      << (int)pols[i].byUnknown3 << ","
      << (int)pols[i].byUnknown4 << "},\n";
  }
  out << "};\n";
  out << "int g_" << sCarNameLower << "PolsCount = sizeof(g_" << sCarNameLower << "Pols)/sizeof(tPolygon);\n";
  out << "//-------------------------------------------------------------------------------------------------\n";


  printf("writing _%s_backs\n", sCarNameLower.c_str());
  out << "uint32 g_" << sCarNameLower << "Backs[] = {\n";
  for (int i = 0; i < backsUInts.size(); ++i) {
    out << backsUInts[i] << ",\n";
  }
  out << "};\n";
  out << "int g_" << sCarNameLower << "BacksCount = sizeof(g_" << sCarNameLower << "Backs) / sizeof(uint32); \n";
  out << "//-------------------------------------------------------------------------------------------------\n";


  printf("writing _%s_places\n", sCarNameLower.c_str());
  out << "uint32 g_" << sCarNameLower << "Places[] = {\n";
  for (int i = 0; i < placesUInts.size(); ++i) {
    out << placesUInts[i] << ",\n";
  }
  out << "};\n";
  out << "int g_" << sCarNameLower << "PlacesCount = sizeof(g_" << sCarNameLower << "Places) / sizeof(uint32); \n";
  out << "//-------------------------------------------------------------------------------------------------\n";


  printf("writing _%s_anms\n", sCarNameLower.c_str());
  out << "tAnimation g_" << sCarNameLower << "Anms[] = {\n";
  for (int i = 0; i < anms.size(); ++i) {
    out << "{" << anms[i].uiCount << ",{";
    for (int j = 0; j < 16; ++j) {
      out << anms[i].framesAy[j] << ",";
    }
    out << "}},\n";
  }
  out << "};\n";
  out << "int g_" << sCarNameLower << "AnmsCount = sizeof(g_" << sCarNameLower << "Anms) / sizeof(tAnimation); \n";
  out << "//-------------------------------------------------------------------------------------------------\n";
  

  out << "#endif\n";

  out.close();

  return 0;
}