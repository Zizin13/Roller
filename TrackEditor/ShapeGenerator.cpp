#include "glm.hpp"
#include "ShapeGenerator.h"
#include "Vertex.h"
//-------------------------------------------------------------------------------------------------
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)
//-------------------------------------------------------------------------------------------------

tShapeData ShapeGenerator::MakeTriangle()
{
  tShapeData ret;
  tVertex myTri[] = {
    glm::vec3(+0.0f, +1.0f, +0.0f),
    glm::vec3(+1.0f, +0.0f, +0.0f),
    glm::vec3(-1.0f, -1.0f, +0.0f),
    glm::vec3(+0.0f, +1.0f, +0.0f),
    glm::vec3(+1.0f, -1.0f, +0.0f),
    glm::vec3(+0.0f, +0.0f, +1.0f),
  };
  ret.numVertices = NUM_ARRAY_ELEMENTS(myTri);
  ret.vertices = new tVertex[ret.numVertices];
  memcpy(ret.vertices, myTri, sizeof(myTri));

  GLushort indices[] = { 0, 1, 2 };
  ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
  ret.indices = new GLushort[ret.numIndices];
  memcpy(ret.indices, indices, sizeof(indices));

  return ret;
}

//-------------------------------------------------------------------------------------------------

tShapeData ShapeGenerator::MakeCube()
{
  tShapeData ret;
  tVertex stackVerts[] = {
    glm::vec3(-1.0f, +1.0f, +1.0f), //0
    glm::vec3(+1.0f, +0.0f, +0.0f), //color
    glm::vec3(+1.0f, +1.0f, +1.0f), //1
    glm::vec3(+0.0f, +1.0f, +0.0f), //color
    glm::vec3(+1.0f, +1.0f, -1.0f), //2
    glm::vec3(+0.0f, +0.0f, +1.0f), //color
    glm::vec3(-1.0f, +1.0f, -1.0f), //3
    glm::vec3(+1.0f, +1.0f, +1.0f), //color

    glm::vec3(-1.0f, +1.0f, -1.0f), //4
    glm::vec3(+1.0f, +0.0f, +1.0f), //color
    glm::vec3(+1.0f, +1.0f, -1.0f), //5
    glm::vec3(+0.0f, +0.5f, +0.2f), //color
    glm::vec3(+1.0f, -1.0f, -1.0f), //6
    glm::vec3(+0.8f, +0.6f, +0.4f), //color
    glm::vec3(-1.0f, -1.0f, -1.0f), //7
    glm::vec3(+0.3f, +1.0f, +0.5f), //color

    glm::vec3(+1.0f, +1.0f, -1.0f), //8
    glm::vec3(+0.2f, +0.5f, +0.2f), //color
    glm::vec3(+1.0f, +1.0f, +1.0f), //9
    glm::vec3(+0.9f, +0.3f, +0.7f), //color
    glm::vec3(+1.0f, -1.0f, +1.0f), //10
    glm::vec3(+0.3f, +0.7f, +0.5f), //color
    glm::vec3(+1.0f, -1.0f, -1.0f), //11
    glm::vec3(+0.5f, +0.7f, +0.5f), //color

    glm::vec3(-1.0f, +1.0f, +1.0f), //12
    glm::vec3(+0.7f, +0.8f, +0.2f), //color
    glm::vec3(-1.0f, +1.0f, -1.0f), //13
    glm::vec3(+0.5f, +0.7f, +0.3f), //color
    glm::vec3(-1.0f, -1.0f, -1.0f), //14
    glm::vec3(+0.4f, +0.7f, +0.7f), //color
    glm::vec3(-1.0f, -1.0f, +1.0f), //15
    glm::vec3(+0.2f, +0.5f, +1.0f), //color

    glm::vec3(+1.0f, +1.0f, +1.0f), //16
    glm::vec3(+0.6f, +1.0f, +0.7f), //color
    glm::vec3(-1.0f, +1.0f, +1.0f), //17
    glm::vec3(+0.6f, +0.4f, +0.8f), //color
    glm::vec3(-1.0f, -1.0f, +1.0f), //18
    glm::vec3(+0.2f, +0.8f, +0.7f), //color
    glm::vec3(+1.0f, -1.0f, +1.0f), //19
    glm::vec3(+0.2f, +0.7f, +1.0f), //color

    glm::vec3(+1.0f, -1.0f, -1.0f), //20
    glm::vec3(+0.8f, +0.3f, +0.7f), //color
    glm::vec3(-1.0f, -1.0f, -1.0f), //21
    glm::vec3(+0.8f, +0.9f, +0.5f), //color
    glm::vec3(-1.0f, -1.0f, +1.0f), //22
    glm::vec3(+0.5f, +0.8f, +0.5f), //color
    glm::vec3(+1.0f, -1.0f, +1.0f), //23
    glm::vec3(+0.9f, +1.0f, +0.2f), //color
  };
  ret.numVertices = NUM_ARRAY_ELEMENTS(stackVerts);
  ret.vertices = new tVertex[ret.numVertices];
  memcpy(ret.vertices, stackVerts, sizeof(stackVerts));

  GLushort indices[] = { 
     0,  1,  2,  0,  2,  3, //top
     4,  5,  6,  4,  6,  7, //front
     8,  9, 10,  8, 10, 11, //right
    12, 13, 14, 12, 14, 15, //left
    16, 17, 18, 16, 18, 19, //back
    20, 22, 21, 20, 23, 22, //bottom
  };
  ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
  ret.indices = new GLushort[ret.numIndices];
  memcpy(ret.indices, indices, sizeof(indices));

  return ret;
}

//-------------------------------------------------------------------------------------------------

tShapeData ShapeGenerator::MakeArrow()
{
  tShapeData ret;

  // Vert + Color
  tVertex verts[] =
  {
          // Top side of arrow head
          glm::vec3(+0.00f, +0.25f, -0.25f), glm::vec3(+1.00f, +0.00f, +0.00f), //vertice 0
  //glm::vec3(0, 1, 0),
          glm::vec3(+0.50f, +0.25f, -0.25f), glm::vec3(+1.00f, +0.00f, +0.00f), //vertice 1	  
  //glm::vec3(0, 1, 0),
          glm::vec3(+0.00f, +0.25f, -1.00f), glm::vec3(+1.00f, +0.00f, +0.00f), //vertice 2	  
  //glm::vec3(0, 1, 0),
          glm::vec3(-0.50f, +0.25f, -0.25f), glm::vec3(+1.00f, +0.00f, +0.00f), //vertice 3
  //glm::vec3(0, 1, 0),

          // Bottom side of arrow head
          glm::vec3(+0.00f, -0.25f, -0.25f), glm::vec3(+0.00f, +0.00f, +1.00f), //vertice 4	  
  //glm::vec3(0, -1, 0),
          glm::vec3(+0.50f, -0.25f, -0.25f), glm::vec3(+0.00f, +0.00f, +1.00f), //vertice 5	 
  //glm::vec3(0, -1, 0),
          glm::vec3(+0.00f, -0.25f, -1.00f), glm::vec3(+0.00f, +0.00f, +1.00f), //vertice 6	  
  //glm::vec3(0, -1, 0),
          glm::vec3(-0.50f, -0.25f, -0.25f), glm::vec3(+0.00f, +0.00f, +1.00f), //vertice 7
  //glm::vec3(0, -1, 0),

          // Right side of arrow tip
          glm::vec3(+0.50f, +0.25f, -0.25f), glm::vec3(+0.60f, +1.00f, +0.00f), //vertice 8	  
  //glm::vec3(0.83205032f, 0.00f, -0.55470026f),
          glm::vec3(+0.00f, +0.25f, -1.00f), glm::vec3(+0.60f, +1.00f, +0.00f), //vertice 9	  
  //glm::vec3(0.83205032f, 0.00f, -0.55470026f),
          glm::vec3(+0.00f, -0.25f, -1.00f), glm::vec3(+0.60f, +1.00f, +0.00f), //vertice 10  
  //glm::vec3(0.83205032f, 0.00f, -0.55470026f),
          glm::vec3(+0.50f, -0.25f, -0.25f), glm::vec3(+0.60f, +1.00f, +0.00f), //vertice 11
  //glm::vec3(0.83205032f, 0.00f, -0.55470026f),

          // Left side of arrow tip
          glm::vec3(+0.00f, +0.25f, -1.00f), glm::vec3(+0.00f, +1.00f, +0.00f), //vertice 12	  
  //glm::vec3(-0.55708605f, 0.00f, -0.37139067f),
          glm::vec3(-0.50f, +0.25f, -0.25f), glm::vec3(+0.00f, +1.00f, +0.00f), //vertice 13  
  //glm::vec3(-0.55708605f, 0.00f, -0.37139067f),
          glm::vec3(+0.00f, -0.25f, -1.00f), glm::vec3(+0.00f, +1.00f, +0.00f), //vertice 14  
  //glm::vec3(-0.55708605f, 0.00f, -0.37139067f),
          glm::vec3(-0.50f, -0.25f, -0.25f), glm::vec3(+0.00f, +1.00f, +0.00f), //vertice 15
  //glm::vec3(-0.55708605f, 0.00f, -0.37139067f),

          // Back side of arrow tip
          glm::vec3(-0.50f, +0.25f, -0.25f), glm::vec3(+0.50f, +0.50f, +0.50f), //vertice 16 		  
  //glm::vec3(0, 0, 1),
          glm::vec3(+0.50f, +0.25f, -0.25f), glm::vec3(+0.50f, +0.50f, +0.50f), //vertice 17  
  //glm::vec3(0, 0, 1),
          glm::vec3(-0.50f, -0.25f, -0.25f), glm::vec3(+0.50f, +0.50f, +0.50f), //vertice 18  
  //glm::vec3(0, 0, 1),
          glm::vec3(+0.50f, -0.25f, -0.25f), glm::vec3(+0.50f, +0.50f, +0.50f), //vertice 19
  //glm::vec3(0, 0, 1),

          // Top side of back of arrow
          glm::vec3(+0.25f, +0.25f, -0.25f), glm::vec3(+1.00f, +0.00f, +0.00f), //vertice 20	  
  //glm::vec3(0, 1, 0),
          glm::vec3(+0.25f, +0.25f, +1.00f), glm::vec3(+1.00f, +0.00f, +0.00f), //vertice 21  
  //glm::vec3(0, 1, 0),
          glm::vec3(-0.25f, +0.25f, +1.00f), glm::vec3(+1.00f, +0.00f, +0.00f), //vertice 22  
  //glm::vec3(0, 1, 0),
          glm::vec3(-0.25f, +0.25f, -0.25f), glm::vec3(+1.00f, +0.00f, +0.00f), //vertice 23 
  //glm::vec3(0, 1, 0),

          // Bottom side of back of arrow
          glm::vec3(+0.25f, -0.25f, -0.25f), glm::vec3(+0.00f, +0.00f, +1.00f), //vertice 24	  
  //glm::vec3(0, -1, 0),
          glm::vec3(+0.25f, -0.25f, +1.00f), glm::vec3(+0.00f, +0.00f, +1.00f), //vertice 25 
  //glm::vec3(0, -1, 0),
          glm::vec3(-0.25f, -0.25f, +1.00f), glm::vec3(+0.00f, +0.00f, +1.00f), //vertice 26 
  //glm::vec3(0, -1, 0),
          glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3(+0.00f, +0.00f, +1.00f), //vertice 27
  //glm::vec3(0, -1, 0),

          // Right side of back of arrow
          glm::vec3(+0.25f, +0.25f, -0.25f), glm::vec3(+0.60f, +1.00f, +0.00f), //vertice 28	  
  //glm::vec3(1, 0, 0),
          glm::vec3(+0.25f, -0.25f, -0.25f), glm::vec3(+0.60f, +1.00f, +0.00f), //vertice 29  
  //glm::vec3(1, 0, 0),
          glm::vec3(+0.25f, -0.25f, +1.00f), glm::vec3(+0.60f, +1.00f, +0.00f), //vertice 30  
  //glm::vec3(1, 0, 0),
          glm::vec3(+0.25f, +0.25f, +1.00f), glm::vec3(+0.60f, +1.00f, +0.00f), //vertice 31
  //glm::vec3(1, 0, 0),

          // Left side of back of arrow
          glm::vec3(-0.25f, +0.25f, -0.25f), glm::vec3(+0.00f, +1.00f, +0.00f), //vertice 32		  
  //glm::vec3(-1, 0, 0),
          glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3(+0.00f, +1.00f, +0.00f), //vertice 33	 
  //glm::vec3(-1, 0, 0),
          glm::vec3(-0.25f, -0.25f, +1.00f), glm::vec3(+0.00f, +1.00f, +0.00f), //vertice 34  
  //glm::vec3(-1, 0, 0),
          glm::vec3(-0.25f, +0.25f, +1.00f), glm::vec3(+0.00f, +1.00f, +0.00f), //vertice 35  
  //glm::vec3(-1, 0, 0),

          // Back side of back of arrow
          glm::vec3(-0.25f, +0.25f, +1.00f), glm::vec3(+0.50f, +0.50f, +0.50f), //vertice 36  
  //glm::vec3(0, 0, 1),
          glm::vec3(+0.25f, +0.25f, +1.00f), glm::vec3(+0.50f, +0.50f, +0.50f), //vertice 37  
  //glm::vec3(0, 0, 1),
          glm::vec3(-0.25f, -0.25f, +1.00f), glm::vec3(+0.50f, +0.50f, +0.50f), //vertice 38 
  //glm::vec3(0, 0, 1),
          glm::vec3(+0.25f, -0.25f, +1.00f), glm::vec3(+0.50f, +0.50f, +0.50f), //vertice 39  
  //glm::vec3(0, 0, 1)
  };

  ret.numVertices = NUM_ARRAY_ELEMENTS(verts);
  ret.vertices = new tVertex[ret.numVertices];
  memcpy(ret.vertices, verts, sizeof(verts));

  GLushort indices[] = {
          0, 1, 2, // Top
          0, 2, 3,
          4, 6, 5, // Bottom
          4, 7, 6,
          8, 10, 9, // Right side of arrow tip
          8, 11, 10,
          12, 15, 13, // Left side of arrow tip
          12, 14, 15,
          16, 19, 17, // Back side of arrow tip
          16, 18, 19,
          20, 22, 21, // Top side of back of arrow
          20, 23, 22,
          24, 25, 26, // Bottom side of back of arrow
          24, 26, 27,
          28, 30, 29, // Right side of back of arrow
          28, 31, 30,
          32, 33, 34, // Left side of back of arrow
          32, 34, 35,
          36, 39, 37, // Back side of back of arrow
          36, 38, 39,
  };

  ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
  ret.indices = new GLushort[ret.numIndices];
  memcpy(ret.indices, indices, sizeof(indices));

  return ret;
}

//-------------------------------------------------------------------------------------------------