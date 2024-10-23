#include "ShapeFactory.h"
#include "FBXExporter.h"
#include "ShapeData.h"
#include "Texture.h"
#include "Palette.h"
#include "Logging.h"
#include "Track.h"
#include "CarHelpers.h"
#include <string>
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

  //make texture file
  std::string sTexFile = sOutDir + "\\" + sCarName + ".png";
  carTex.ExportToPngFile(sTexFile);
  printf("Exporting ");
  printf(sTexFile.c_str());
  printf("...\n");

  //create shape data
  CShapeData *pCar = NULL;
  CShapeFactory::GetShapeFactory().MakeModel(&pCar, NULL, &carTex, carModel);
  if (!pCar)
    return false;

  std::string sFilename = sOutDir + std::string("\\") + sCarName + std::string(".fbx");
  printf("Exporting ");
  printf(sFilename.c_str());
  printf("...\n");
  bool bSuccess = CFBXExporter::GetFBXExporter().ExportShape(pCar, sCarName.c_str(), sFilename.c_str(), sTexFile.c_str());

  delete pCar;
  return bSuccess;
}

//-------------------------------------------------------------------------------------------------

bool ExportTrack(CTrack *pTrack, std::string sOutDir)
{
  if (!pTrack || !pTrack->m_pTex || !pTrack->m_pBld)
    return false;

  //get track name
  std::string sTrackName = pTrack->m_sTrackFile;
  size_t pos = pTrack->m_sTrackFile.find_last_of('\\');
  if (pos == std::string::npos) {
    pos = pTrack->m_sTrackFile.find_last_of('/');
  }
  sTrackName = pTrack->m_sTrackFile.substr(pos + 1, pTrack->m_sTrackFile.size() - pos);
  pos = sTrackName.find_last_of('.');
  sTrackName = sTrackName.substr(0, pos);

  //make texture files
  std::string sTexFile = sOutDir + "\\" + sTrackName + ".png";
  printf("Exporting ");
  printf(sTexFile.c_str());
  printf("...\n");
  pTrack->m_pTex->ExportToPngFile(sTexFile);
  std::string sSignTexFile = sOutDir + "\\" + sTrackName + "_BLD.png";
  printf("Exporting ");
  printf(sSignTexFile.c_str());
  printf("...\n");
  pTrack->m_pBld->ExportToPngFile(sSignTexFile);

  //setup
  pTrack->GenerateTrackMath();

  //generate models
  CShapeData *pExportTrack = NULL;
  std::vector<CShapeData *> signAy;
  pExportTrack = NULL;
  CShapeFactory::GetShapeFactory().MakeTrackSurface(&pExportTrack,
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
  printf("Exporting ");
  printf(sFilename.c_str());
  printf("...\n");
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
  CShapeFactory::GetShapeFactory().m_bOglRunning = false;

  //export cars
  ExportCar(eWhipModel::CAR_F1WACK, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_XAUTO, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_XDESILVA, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_XPULSE, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_XGLOBAL, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_XMILLION, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_XMISSION, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_XZIZIN, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_XREISE, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_YAUTO, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_YDESILVA, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_YPULSE, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_YGLOBAL, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_YMILLION, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_YMISSION, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_YZIZIN, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_YREISE, sWhipDir, sOutDir);
  ExportCar(eWhipModel::CAR_DEATH, sWhipDir, sOutDir);

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