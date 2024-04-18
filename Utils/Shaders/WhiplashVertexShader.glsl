#version 430

in layout(location=0) vec4 vertexPositionModel;
in layout(location=1) vec3 vertexColor;
in layout(location=3) vec2 texCoords;
in layout(location=4) vec3 flags;

out vec3 vertexToFragmentColor;
out vec2 vertexToFragmentTexCoords;
out vec3 vertexToFragmentFlags;

uniform mat4 modelToProjectionMatrix;

void main()
{
  gl_Position = modelToProjectionMatrix * vertexPositionModel;
  vertexToFragmentColor = vertexColor;
  vertexToFragmentTexCoords = texCoords;
  vertexToFragmentFlags = flags;
}