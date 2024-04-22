#include <fstream>
#include <sstream>
#include <vector>
#include <assert.h>
#include <algorithm>

//-------------------------------------------------------------------------------------------------
// just a little app to turn the "carplans.c.asm" file created by 
// wcdatool disassembly tool for watcom compiler into something 
// a little more usable
//-------------------------------------------------------------------------------------------------

std::vector<char> HexToBytes(const std::string &sBytes)
{
  std::vector<char> byteAy;

  for (unsigned int i = 0; i < sBytes.length(); i += 2) {
    std::string sByte = sBytes.substr(i, 2);
    char byte = (char)strtol(sByte.c_str(), NULL, 16);
    byteAy.push_back(byte);
  }

  return byteAy;
}

//-------------------------------------------------------------------------------------------------

std::vector<float> BytesToFloats(const std::vector<char> bytes)
{
  std::vector<float> floatAy;

  int j = 0;
  unsigned int uiData = 0;
  for (unsigned int i = 0; i < bytes.size(); ++i) {

    if (j == 4) {
      float *pfData = reinterpret_cast<float *>(&uiData);
      floatAy.push_back(*pfData);
      uiData = 0;
      j = 0;
    }

    unsigned int uiTemp = 0;
    unsigned char c = (unsigned char)bytes[i];
    uiTemp |= c;
    uiTemp = uiTemp << 8 * j;
    uiData |= uiTemp;
    ++j;
  }

  return floatAy;
}

//-------------------------------------------------------------------------------------------------

std::vector<int> BytesToInts(const std::vector<char> bytes)
{
  std::vector<int> intAy;

  int j = 0;
  unsigned int uiData = 0;
  for (unsigned int i = 0; i < bytes.size(); ++i) {

    if (j == 4) {
      int *piData = reinterpret_cast<int *>(&uiData);
      intAy.push_back(*piData);
      uiData = 0;
      j = 0;
    }

    unsigned int uiTemp = 0;
    unsigned char c = (unsigned char)bytes[i];
    uiTemp |= c;
    uiTemp = uiTemp << 8 * j;
    uiData |= uiTemp;
    ++j;
  }

  return intAy;
}

//-------------------------------------------------------------------------------------------------

enum eCarPlansSection
{
  NONE = 0,
  ZIZIN_COORDS,
  ZIZIN_POLS
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
  int i = 0;
  while (std::getline(file, sLine)) {
    if (sLine.compare("_xzizin_coords:") == 0)
      section = eCarPlansSection::ZIZIN_COORDS;
    if (sLine.compare("_xzizin_pols:") == 0)
      section = eCarPlansSection::ZIZIN_POLS;
    if (sLine.compare("_xzizin_backs:") == 0)
      break; //stop here for now

    if (section == eCarPlansSection::ZIZIN_COORDS) {
      size_t pos = sLine.find("0x");
      if (pos != std::string::npos) {
        std::string sByte = sLine.substr(pos + 2, 2);
        sCoordsBytes += sByte;
      }
    }
    if (section == eCarPlansSection::ZIZIN_POLS) {
      size_t pos = sLine.find("0x");
      if (pos != std::string::npos) {
        std::string sByte = sLine.substr(pos + 2, 2);
        sPolsBytes += sByte;
      }
    }
  }
  file.close();

  printf("file loaded...\n");

  std::vector<char> bytes = HexToBytes(sCoordsBytes);
  std::vector<float> floats = BytesToFloats(bytes);
  bytes = HexToBytes(sPolsBytes);
  std::vector<int> ints = BytesToInts(bytes);

  printf("values converted...\n");

  //open output file
  std::ofstream out(argv[2]);
  if (!out.is_open()) {
    printf("failed to open output file\n");
    return -1;
  }

  printf("writing _xzizin_coords\n");

  out << "#ifndef _UTILS_CARPLANS_H\n";
  out << "#define _UTILS_CARPLANS_H\n";
  out << "//-------------------------------------------------------------------------------------------------\n";

  out << "float g_xzizinCoords[] = {\n";
  for (int i = 0; i < floats.size(); ++i) {
    out << floats[i] << ",\n";
  }
  out << "};\n";
  out << "int g_xzizinCoordsCount = sizeof(g_xzizinCoords)/sizeof(float);\n";

  out << "//-------------------------------------------------------------------------------------------------\n";
  out << "#endif\n";

  out.close();

  return 0;
}