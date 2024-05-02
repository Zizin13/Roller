#ifndef _WHIPLIB_OPENGLDEBUG_H
#define _WHIPLIB_OPENGLDEBUG_H
//-------------------------------------------------------------------------------------------------
#include "glew.h"
#include "stdio.h"
#include <assert.h>
#include "Logging.h"
//-------------------------------------------------------------------------------------------------
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
  x;\
  ASSERT(GLLogCall(#x,__FILE__,__LINE__))
//-------------------------------------------------------------------------------------------------

static void GLErrorCb(GLenum source,
                      GLenum type,
                      GLuint id,
                      GLenum severity,
                      GLsizei length,
                      const GLchar * message,
                      const void *userParam)
{
  (void)(source); (void)(type); (void)(id); (void)(severity); (void)(length); (void)(userParam);
  Logging::LogMessage(message);
}

//-------------------------------------------------------------------------------------------------

static void GLClearError()
{
  int iTest = 0;
  while (GLenum error = glGetError()) {
    iTest++;
    if (iTest > 100) {
      assert(0);
      return;
    }
  }
}

//-------------------------------------------------------------------------------------------------

static bool GLLogCall(const char *szFunction, const char *szFile, int iLine)
{
  while (GLenum error = glGetError()) {
    Logging::LogMessage("OpenGL Error (%d): %s %s %d", (int)error, szFunction, szFile, iLine);
    return false;
  }
  return true;
}

//-------------------------------------------------------------------------------------------------
#endif