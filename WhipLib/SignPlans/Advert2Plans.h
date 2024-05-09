#ifndef _WHIPLIB_ADVERT2PLANS_H
#define _WHIPLIB_ADVERT2PLANS_H
//-------------------------------------------------------------------------------------------------
#include "Polygon.h"
#include "Animation.h"
//-------------------------------------------------------------------------------------------------
static float g_advert2Coords[] = {
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
static int g_advert2CoordsCount = sizeof(g_advert2Coords) / sizeof(float); 
//-------------------------------------------------------------------------------------------------
static tPolygon g_advert2Pols[] = {
{2,3,0,1,8704,0,0,0,0},
};
static int g_advert2PolsCount = sizeof(g_advert2Pols)/sizeof(tPolygon);
//-------------------------------------------------------------------------------------------------
static uint32 g_advert2Backs[] = {
0,
};
static int g_advert2BacksCount = sizeof(g_advert2Backs) / sizeof(uint32); 
//-------------------------------------------------------------------------------------------------
static tAnimation g_advert2Anms[] = {
{0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}},
};
static int g_advert2AnmsCount = sizeof(g_advert2Anms) / sizeof(tAnimation); 
//-------------------------------------------------------------------------------------------------
#endif
