#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include "Track.h"
//-------------------------------------------------------------------------------------------------

static std::string g_surfaceAy[13] = {
  "center",
  "lshoulder",
  "rshoulder",
  "lwall",
  "rwall",
  "roof",
  "luowall",
  "llowall",
  "ofloor",
  "rlowall",
  "ruowall",
  "efloor",
  "sign"
};
static int g_surfaceAyCount = 13;

//-------------------------------------------------------------------------------------------------

struct tTrackWithValues
{
  std::string sTrack;
  std::vector<int> valuesFound;
};

//-------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  //not enough arguments
  if (argc < 5) {
    printf("Usage: TrackAnalyzer whiplash_dir surface_type --option data\n");
    printf("       ex: TrackAnalyzer C:\\WHIP\\WHIPLASH\\FATDATA lshoulder --flag 9\n");
    printf("\n");
    printf("surfaces: ");
    for (int i = 0; i < g_surfaceAyCount; ++i) {
      printf("%s ", g_surfaceAy[i].c_str());
    }
    printf("\n");
    printf("options: --flag (integers 8-31), --value (number)\n");
    return -1;
  }

  std::string sPath = argv[1];
  std::string sSurface = argv[2];
  std::string sOptionType = argv[3];
  std::string sOptionVal = argv[4];

  //load all tracks in dir
  std::vector<CTrack *> trackAy;
  for (const auto &entry : std::filesystem::directory_iterator(sPath)) {
    std::string sEntry = entry.path().string();
    size_t pos = sEntry.find_last_of('.');
    if (pos != std::string::npos) {
      std::string sExtension = sEntry.substr(pos, sEntry.size() - pos);
      if (sExtension.compare(".TRK") == 0) {
        CTrack *pNewTrack = new CTrack();
        printf("Loading %s ", sEntry.c_str());
        if (pNewTrack->LoadTrack(sEntry)) {
          printf("success\n");
          trackAy.push_back(pNewTrack);
        } else {
          printf("failure\n");
          delete pNewTrack;
        }
      }
    }
  }

  //analyze
  for (std::vector<CTrack *>::iterator it = trackAy.begin(); it != trackAy.end(); ++it) {
    CTrack *pTrack = *it;

  }

  //cleanup
  for (std::vector<CTrack *>::iterator it = trackAy.begin(); it != trackAy.end(); ++it) {
    delete *it;
  }
  //exit
  return 0;
}

//-------------------------------------------------------------------------------------------------