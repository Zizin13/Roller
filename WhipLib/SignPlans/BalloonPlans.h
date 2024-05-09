#ifndef _WHIPLIB_BALLOONPLANS_H
#define _WHIPLIB_BALLOONPLANS_H
//-------------------------------------------------------------------------------------------------
#include "Polygon.h"
#include "Animation.h"
//-------------------------------------------------------------------------------------------------
static float g_balloonCoords[] = {
0.000000f,
846.767029f,
-84.984001f,
0.000000f,
-846.767029f,
-84.984001f,
0.000000f,
-846.767029f,
1614.759033f,
0.000000f,
846.767029f,
1614.759033f,
};
static int g_balloonCoordsCount = sizeof(g_balloonCoords) / sizeof(float); 
//-------------------------------------------------------------------------------------------------
static tPolygon g_balloonPols[] = {
{2,3,0,1,8704,0,0,0,0},
};
static int g_balloonPolsCount = sizeof(g_balloonPols)/sizeof(tPolygon);
//-------------------------------------------------------------------------------------------------
#endif
