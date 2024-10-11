//-------------------------------------------------------------------------------------------------
#define GLFW_INCLUDE_NONE
#include <glfw3.h>
#include <glew.h>
#include <glm.hpp>
#include "gtc/matrix_transform.hpp"
#include "gtx/transform.hpp"
#include "Shader.h"
#include "Camera.h"
#include "ShapeFactory.h"
#include "ShapeData.h"
#include "Texture.h"
#include "Palette.h"
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(IS_WINDOWS)
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
//-------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
#if defined(_DEBUG) && defined(IS_WINDOWS)
  _set_error_mode(_OUT_TO_MSGBOX);
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
  GLFWwindow *window;

  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK)
    return -1;
  glEnable(GL_DEBUG_OUTPUT);
  //glDebugMessageCallback(GLErrorCb, 0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.24f);
  glLineWidth(3.0f);
  CShader shader("Shaders/WhiplashVertexShader.glsl", "Shaders/WhiplashFragmentShader.glsl");
  Camera camera;
  camera.m_position.z = 500.0f;
  camera.m_position.y = 1300.0f;
  camera.m_viewDirection.z = 0.05f;
  camera.m_viewDirection.y = -1.0f;
  CPalette *pPal = new CPalette();;
  pPal->LoadPalette("C:\\WHIP\\WHIPLASH\\FATDATA\\PALETTE.PAL");
  CTexture *pTex = new CTexture();
  pTex->LoadTexture("C:\\WHIP\\WHIPLASH\\FATDATA\\YZIZIN.BM", pPal);
  CShapeData *pShape = CShapeFactory::GetShapeFactory().MakeModel(&shader, pTex, eWhipModel::CAR_YZIZIN);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClearColor(0.03125f, 0.296875f, 0.984375f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    //glViewport(0, 0, width(), height());

    glm::mat4 viewToProjectionMatrix = glm::perspective(glm::radians(60.0f), ((float)640.0f) / 480.0f, 100.0f, 500000.0f);
    glm::mat4 worldToViewMatrix = camera.GetWorldToViewMatrix();
    glm::mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

    pShape->Draw(worldToProjectionMatrix, camera.m_position);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  delete pShape;
  delete pTex;
  delete pPal;

  glfwTerminate();

  return 0;
};

//-------------------------------------------------------------------------------------------------