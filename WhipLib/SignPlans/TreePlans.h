#ifndef _WHIPLIB_TREEPLANS_H
#define _WHIPLIB_TREEPLANS_H
//-------------------------------------------------------------------------------------------------
#include "Polygon.h"
#include "Animation.h"
//-------------------------------------------------------------------------------------------------
static float g_treeCoords[] = {
0.000000f,
372.300995f,
0.003000f,
0.000000f,
-372.300995f,
0.003000f,
0.000000f,
-372.300995f,
1712.557983f,
0.000000f,
372.300995f,
1712.557983f,
};
static int g_treeCoordsCount = sizeof(g_treeCoords) / sizeof(float); 
//-------------------------------------------------------------------------------------------------
static tPolygon g_treePols[] = {
{2,3,0,1,9473,0,0,0,0},
};
static int g_treePolsCount = sizeof(g_treePols)/sizeof(tPolygon);
//-------------------------------------------------------------------------------------------------
static uint32 g_treeBacks[] = {
0,
};
static int g_treeBacksCount = sizeof(g_treeBacks) / sizeof(uint32); 
//-------------------------------------------------------------------------------------------------
#endif
