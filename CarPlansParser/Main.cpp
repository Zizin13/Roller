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
  
  bool bFoundZizinCoords = false;
  std::string sLine;
  std::string sCoordsBytes;
  int i = 0;
  while (std::getline(file, sLine)) {
    if (sLine.compare("_xzizin_coords:") == 0)
      bFoundZizinCoords = true;
    if (sLine.compare("_xzizin_pols:") == 0)
      break; //stop here for now

    if (bFoundZizinCoords) {
      size_t pos = sLine.find("0x");
      if (pos != std::string::npos) {
        std::string sByte = sLine.substr(pos + 2, 2);
        sCoordsBytes += sByte;
      }
    }
  }
  file.close();

  std::vector<char> bytes = HexToBytes(sCoordsBytes);
  std::vector<float> floats = BytesToFloats(bytes);

  //open output file
  std::ofstream out(argv[2]);
  if (!out.is_open()) {
    printf("failed to open output file\n");
    return -1;
  }

  out << "_xzizin_coords:\n";
  for (int i = 0; i < floats.size(); ++i) {
    out << floats[i] << "\n";
  }

  out.close();

  return 0;
}