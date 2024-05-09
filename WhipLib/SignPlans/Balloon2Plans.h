#ifndef _WHIPLIB_BALLOON2PLANS_H
#define _WHIPLIB_BALLOON2PLANS_H
//-------------------------------------------------------------------------------------------------
#include "Polygon.h"
#include "Animation.h"
//-------------------------------------------------------------------------------------------------
static float g_balloon2Coords[] = {
0.000000f,
1520.671997f,
-152.619003f,
0.000000f,
-1520.671997f,
-152.619003f,
0.000000f,
-1520.671997f,
2899.875000f,
0.000000f,
1520.671997f,
2899.875000f,
};
static int g_balloon2CoordsCount = sizeof(g_balloon2Coords) / sizeof(float); 
//-------------------------------------------------------------------------------------------------
static tPolygon g_balloon2Pols[] = {
{2,3,0,1,8704,0,0,0,0},
};
static int g_balloon2PolsCount = sizeof(g_balloon2Pols)/sizeof(tPolygon);
//-------------------------------------------------------------------------------------------------
static uint32 g_balloon2Backs[] = {
0,
};
static int g_balloon2BacksCount = sizeof(g_balloon2Backs) / sizeof(uint32); 
//-------------------------------------------------------------------------------------------------
static tAnimation g_balloon2Anms[] = {
{0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}},
};
static int g_balloon2AnmsCount = sizeof(g_balloon2Anms) / sizeof(tAnimation); 
//-------------------------------------------------------------------------------------------------
#endif
