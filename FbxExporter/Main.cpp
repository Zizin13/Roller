#include "ShapeFactory.h"
#include "FBXExporter.h"
#include "ShapeData.h"
#include "Texture.h"
#include "Palette.h"
#include "Logging.h"
#include <string>
#include <fstream>
//-------------------------------------------------------------------------------------------------

static void LogMessageCbStatic(const char *szMsg, int iLen)
{
  (void)(iLen);
  printf(szMsg);
  printf("\n");
}

//-------------------------------------------------------------------------------------------------

bool ExportCar(eWhipModel carModel, std::string sWhipDir, std::string sOutDir)
{
  std::string sCarName = CarHelpers::GetCarNameFromModel(carModel);
  std::string sTex = CarHelpers::GetCarTextureFromModel(carModel);

  //load palette
  CPalette palette;
  if (!palette.LoadPalette(sWhipDir + "\\" + "PALETTE.PAL"))
    return false;

  //load texture
  CTexture carTex;
  if (!carTex.LoadTexture(sWhipDir + "\\" + sTex, &palette))
    return false;

  //make texture file
  int iBmpSize;
  uint8 *pBmpData = carTex.GenerateBitmapData(iBmpSize);
  std::string sTexFile = sOutDir + "\\" + sCarName + ".bmp";
  std::ofstream out(sTexFile.c_str(), std::ios_base::binary);
  if (!out.is_open()) {
    printf("failed to open bmp output file\n");
    return false;
  }
  for (int i = 0; i < iBmpSize; ++i) {
    out << pBmpData[i];
  }
  out.close();
  delete[] pBmpData;

  //create shape data
  CShapeData *pCar = CShapeFactory::GetShapeFactory().MakeModel(NULL, &carTex, carModel);
  if (!pCar)
    return false;

  std::string sFilename = sOutDir + std::string("\\") + sCarName + std::string(".fbx");
  bool bSuccess = CFBXExporter::GetFBXExporter().ExportShape(pCar, sCarName.c_str(), sFilename.c_str(), sTexFile.c_str());

  delete pCar;
  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  //not enough arguments
  if (argc < 3) {
    printf("Converts all whiplash car models and tracks to FBX format\n");
    printf("Usage: FbxExporter whiplash_dir output_dir\n");
    printf("       ex: FbxExporter C:\\WHIP\\WHIPLASH\\FATDATA C:\\WHIP\\output\n");
    return -1;
  }

  //setup
  Logging::SetWhipLibLoggingCallback(LogMessageCbStatic);
  std::string sWhipDir = argv[1];
  std::string sOutDir = argv[2];
  CShapeFactory::GetShapeFactory().m_fScale = 1.0f;
  CShapeFactory::GetShapeFactory().m_bOglRunning = false;

  //export models
  ExportCar(CAR_F1WACK, sWhipDir, sOutDir);
  ExportCar(CAR_XZIZIN, sWhipDir, sOutDir);

  //exit
  return 0;
};

//-------------------------------------------------------------------------------------------------