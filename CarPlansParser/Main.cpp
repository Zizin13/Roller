#include <fstream>
#include <sstream>
#include <vector>
#include <assert.h>
#include <algorithm>
#include "Polygon.h"

//-------------------------------------------------------------------------------------------------
// just a little app to turn the "carplans.c.asm" file created by 
// wcdatool disassembly tool for watcom compiler into something 
// a little more usable
//-------------------------------------------------------------------------------------------------

std::vector<unsigned char> HexToBytes(const std::string &sBytes)
{
  std::vector<unsigned char> byteAy;

  for (unsigned int i = 0; i < sBytes.length(); i += 2) {
    std::string sByte = sBytes.substr(i, 2);
    char byte = (char)strtol(sByte.c_str(), NULL, 16);
    byteAy.push_back(byte);
  }

  return byteAy;
}

//-------------------------------------------------------------------------------------------------

std::vector<float> BytesToFloats(const std::vector<unsigned char> bytes)
{
  std::vector<float> floatAy;

  int j = 0;
  unsigned int uiData = 0;
  for (unsigned int i = 0; i < bytes.size(); ++i) {
    unsigned int uiTemp = 0;
    unsigned char c = (unsigned char)bytes[i];
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

std::vector<unsigned int> BytesToUInts(const std::vector<unsigned char> bytes)
{
  std::vector<unsigned int> intAy;

  int j = 0;
  unsigned int uiData = 0;
  for (unsigned int i = 0; i < bytes.size(); ++i) {
    unsigned int uiTemp = 0;
    unsigned char c = (unsigned char)bytes[i];
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

std::vector<unsigned short> BytesToUShorts(const std::vector<unsigned char> bytes)
{
  std::vector<unsigned short> shorAy;

  int j = 0;
  unsigned short unData = 0;
  for (unsigned int i = 0; i < bytes.size(); ++i) {
    unsigned int uiTemp = 0;
    unsigned char c = (unsigned char)bytes[i];
    uiTemp |= c;
    uiTemp = uiTemp << 8 * j;
    unData |= uiTemp;
    ++j;

    if (j == 2) {
      shorAy.push_back(unData);
      unData = 0;
      j = 0;
    }
  }

  return shorAy;
}

//-------------------------------------------------------------------------------------------------

std::vector<tPolygon> BytesToPols(const std::vector<unsigned char> bytes)
{
  std::vector<tPolygon> polAy;

  int j = 0;
  tPolygon data;
  memset(&data, 0, sizeof(data));
  for (unsigned int i = 0; i < bytes.size(); ++i) {
    unsigned char c = bytes[i];
    unsigned int uiTemp = 0;

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

std::string GetStringBytes(const std::string &sLine)
{
  std::string sRet;
  size_t colPos = sLine.find(":");
  
  if (colPos != std::string::npos) {
    sRet = sLine.substr(colPos + 1, 21);
  }
  
  sRet.erase(remove_if(sRet.begin(), sRet.end(), isspace), sRet.end());
  
  return sRet;
}

//-------------------------------------------------------------------------------------------------

enum eCarPlansSection
{
  NONE = 0,
  ZIZIN_COORDS,
  ZIZIN_POLS,
  ZIZIN_BACKS,
  ZIZIN_PLACES,
  ZIZIN_ANMS
};

//-------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  //must pass in filename
  if (argc < 3) {
    printf("need carplans.c.asm file location and output file\n");
    return -1;
  }

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
    if (sLine.compare("_xzizin_coords:") == 0)
      section = eCarPlansSection::ZIZIN_COORDS;
    if (sLine.compare("_xzizin_pols:") == 0)
      section = eCarPlansSection::ZIZIN_POLS;
    if (sLine.compare("_xzizin_backs:") == 0)
      section = eCarPlansSection::ZIZIN_BACKS;
    if (sLine.compare("_xzizin_places:") == 0)
      section = eCarPlansSection::ZIZIN_PLACES;
    if (sLine.compare("_xzizin_anms:") == 0)
      section = eCarPlansSection::ZIZIN_ANMS;
    if (sLine.compare("_xreise_coords:") == 0)
      break;

    if (section == eCarPlansSection::ZIZIN_COORDS)
      sCoordsBytes += GetStringBytes(sLine);
    if (section == eCarPlansSection::ZIZIN_POLS)
      sPolsBytes += GetStringBytes(sLine);
    if (section == eCarPlansSection::ZIZIN_BACKS)
      sBacksBytes += GetStringBytes(sLine);
    if (section == eCarPlansSection::ZIZIN_PLACES)
      sPlacesBytes += GetStringBytes(sLine);
    if (section == eCarPlansSection::ZIZIN_ANMS)
      sAnmsBytes += GetStringBytes(sLine);
  }
  file.close();

  printf("file loaded...\n");

  std::vector<unsigned char> bytes = HexToBytes(sCoordsBytes);
  std::vector<float> zizinCoordsFloats = BytesToFloats(bytes);
  printf("found %d floats in _xzizin_coords\n", (int)zizinCoordsFloats.size());

  bytes = HexToBytes(sPolsBytes);
  std::vector<tPolygon> zizinPols = BytesToPols(bytes);
  printf("found %d polygons in _xzizin_pols\n", (int)zizinPols.size());

  bytes = HexToBytes(sBacksBytes);
  std::vector<unsigned int> zizinBacksUInts = BytesToUInts(bytes);
  printf("found %d uints in _xzizin_backs\n", (int)zizinBacksUInts.size());

  bytes = HexToBytes(sPlacesBytes);
  std::vector<unsigned int> zizinPlacesUInts = BytesToUInts(bytes);
  printf("found %d uints in _xzizin_places\n", (int)zizinPlacesUInts.size());

  bytes = HexToBytes(sAnmsBytes);
  std::vector<unsigned int> zizinAnmsUInts = BytesToUInts(bytes);
  printf("found %d uints in _xzizin_anms\n", (int)zizinAnmsUInts.size());

  //open output file
  std::ofstream out(argv[2]);
  if (!out.is_open()) {
    printf("failed to open output file\n");
    return -1;
  }


  out << "#ifndef _WHIPLIB_ZIZINPLANS_H\n";
  out << "#define _WHIPLIB_ZIZINPLANS_H\n";
  out << "//-------------------------------------------------------------------------------------------------\n";
  out << "#include \"Polygon.h\"\n";
  out << "//-------------------------------------------------------------------------------------------------\n";


  printf("writing _xzizin_coords\n");
  out << "float g_xzizinCoords[] = {\n";
  for (int i = 0; i < zizinCoordsFloats.size(); ++i) {
    out << zizinCoordsFloats[i] << "f,\n";
  }
  out << "};\n";
  out << "int g_xzizinCoordsCount = sizeof(g_xzizinCoords)/sizeof(float);\n";
  out << "//-------------------------------------------------------------------------------------------------\n";


  printf("writing _xzizin_pols\n");
  out << "tPolygon g_xzizinPols[] = {\n";
  for (int i = 0; i < zizinPols.size(); ++i) {
    out << "{"
      << (int)zizinPols[i].byVert1 << ","
      << (int)zizinPols[i].byVert2 << ","
      << (int)zizinPols[i].byVert3 << ","
      << (int)zizinPols[i].byVert4 << ","
      << (int)zizinPols[i].uiTex << ","
      << (int)zizinPols[i].byUnknown1 << ","
      << (int)zizinPols[i].byUnknown2 << ","
      << (int)zizinPols[i].byUnknown3 << ","
      << (int)zizinPols[i].byUnknown4 << "},\n";
  }
  out << "};\n";
  out << "int g_xzizinPolsCount = sizeof(g_xzizinPols)/sizeof(tPolygon);\n";
  out << "//-------------------------------------------------------------------------------------------------\n";


  out << "#endif\n";

  out.close();

  return 0;
}