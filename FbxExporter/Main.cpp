#include "ShapeFactory.h"
#include "FBXExporter.h"
#include "ShapeData.h"
#include "Texture.h"
#include "Palette.h"
#include "Logging.h"
#include "Track.h"
#include <string>
#include <fstream>
#include <filesystem>
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

  printf("Exporting");
  printf(sCarName.c_str());
  printf("...\n");

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

bool ExportTrack(CTrack *pTrack, std::string sOutDir)
{
  //make texture file
  std::string sTrackName = pTrack->m_sTrackFile;
  size_t pos = pTrack->m_sTrackFile.find_last_of('\\');
  if (pos == std::string::npos) {
    pos = pTrack->m_sTrackFile.find_last_of('/');
  }
  sTrackName = pTrack->m_sTrackFile.substr(pos + 1, pTrack->m_sTrackFile.size() - pos);
  pos = sTrackName.find_last_of('.');
  sTrackName = sTrackName.substr(0, pos);
  std::string sTexFile = sOutDir + "\\" + sTrackName + ".bmp";
  if (!pTrack || !pTrack->m_pTex || !pTrack->m_pBld)
    return false;

  printf("Exporting");
  printf(sTrackName.c_str());
  printf("...\n");
  {
    int iBmpSize;
    uint8 *pBmpData = pTrack->m_pTex->GenerateBitmapData(iBmpSize);
    std::ofstream out(sTexFile.c_str(), std::ios_base::binary);
    if (!out.is_open()) {
      //printf("failed to open bmp output file\n");
      return false;
    }
    for (int i = 0; i < iBmpSize; ++i) {
      out << pBmpData[i];
    }
    out.close();
    delete[] pBmpData;
  }

  //make sign texture file
  std::string sSignTexFile = sOutDir + "\\" + sTrackName + "_BLD.bmp";
  { //todo refactor into function
    int iSignBmpSize;
    uint8 *pSignBmpData = pTrack->m_pBld->GenerateBitmapData(iSignBmpSize);
    std::ofstream outSigns(sSignTexFile.c_str(), std::ios_base::binary);
    if (!outSigns.is_open()) {
      //printf("failed to open bmp output file\n");
      return false;
    }
    for (int i = 0; i < iSignBmpSize; ++i) {
      outSigns << pSignBmpData[i];
    }
    outSigns.close();
    delete[] pSignBmpData;
  }

  pTrack->m_fScale = 1.0f;
  pTrack->GenerateTrackMath();

  //generate models
  CShapeData *pExportTrack = NULL;
  std::vector<CShapeData *> signAy;
  pExportTrack = CShapeFactory::GetShapeFactory().MakeTrackSurface(pExportTrack,
                                                                   NULL,
                                                                   pTrack,
                                                                   eShapeSection::EXPORT,
                                                                   true);
  CShapeFactory::GetShapeFactory().MakeSigns(NULL, pTrack, signAy);
  //signs need to be moved to the right position on track, this is normally done in the shader
  for (std::vector<CShapeData *>::iterator it = signAy.begin(); it != signAy.end(); ++it)
    (*it)->TransformVertsForExport();

  //export
  std::string sFilename = sOutDir + std::string("\\") + sTrackName + std::string(".fbx");
  bool bExported = CFBXExporter::GetFBXExporter().ExportTrack(pExportTrack,
                                                              signAy,
                                                              sTrackName.c_str(),
                                                              sFilename.c_str(),
                                                              sTexFile.c_str(),
                                                              sSignTexFile.c_str());

  //cleanup
  if (pExportTrack)
    delete pExportTrack;
  for (std::vector<CShapeData *>::iterator it = signAy.begin(); it != signAy.end(); ++it)
    delete *it;
  return bExported;
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

  //export cars
  ExportCar(CAR_F1WACK, sWhipDir, sOutDir);
  ExportCar(CAR_XAUTO, sWhipDir, sOutDir);
  ExportCar(CAR_XDESILVA, sWhipDir, sOutDir);
  ExportCar(CAR_XPULSE, sWhipDir, sOutDir);
  ExportCar(CAR_XGLOBAL, sWhipDir, sOutDir);
  ExportCar(CAR_XMILLION, sWhipDir, sOutDir);
  ExportCar(CAR_XMISSION, sWhipDir, sOutDir);
  ExportCar(CAR_XZIZIN, sWhipDir, sOutDir);
  ExportCar(CAR_XREISE, sWhipDir, sOutDir);
  ExportCar(CAR_YAUTO, sWhipDir, sOutDir);
  ExportCar(CAR_YDESILVA, sWhipDir, sOutDir);
  ExportCar(CAR_YPULSE, sWhipDir, sOutDir);
  ExportCar(CAR_YGLOBAL, sWhipDir, sOutDir);
  ExportCar(CAR_YMILLION, sWhipDir, sOutDir);
  ExportCar(CAR_YMISSION, sWhipDir, sOutDir);
  ExportCar(CAR_YZIZIN, sWhipDir, sOutDir);
  ExportCar(CAR_YREISE, sWhipDir, sOutDir);
  ExportCar(CAR_DEATH, sWhipDir, sOutDir);

  //load all tracks in dir
  std::vector<CTrack *> trackAy;
  for (const auto &entry : std::filesystem::directory_iterator(sWhipDir)) {
    std::string sEntry = entry.path().string();
    size_t pos = sEntry.find_last_of('.');
    if (pos != std::string::npos) {
      std::string sExtension = sEntry.substr(pos, sEntry.size() - pos);
      if (sExtension.compare(".TRK") == 0) {
        CTrack *pNewTrack = new CTrack();
        printf("Loading %s ", sEntry.c_str());
        if (pNewTrack->LoadTrack(sEntry) && pNewTrack->LoadTextures()) {
          printf("success\n");
          trackAy.push_back(pNewTrack);
        } else {
          printf("failure\n");
          delete pNewTrack;
        }
      }
    }
  }
  //export tracks
  for (std::vector<CTrack *>::iterator it = trackAy.begin(); it != trackAy.end(); ++it) {
    CTrack *pTrack = *it;
    ExportTrack(pTrack, sOutDir);
    delete pTrack;
  }

  //exit
  return 0;
};

//-------------------------------------------------------------------------------------------------