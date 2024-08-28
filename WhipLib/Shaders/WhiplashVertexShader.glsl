#version 430

in layout(location=0) vec4 vertexPositionModel;
in layout(location=1) vec4 vertexColor;
in layout(location=2) vec3 normalModel;
in layout(location=3) vec2 texCoords;
in layout(location=4) vec4 flags;
in layout(location=5) vec4 backColor;
in layout(location=6) vec2 backTexCoords;

out vec4 vtfColor;
out vec2 vtfTexCoords;
out vec4 vtfBackColor;
out vec2 vtfBackTexCoords;
out vec4 vtfFlags;
out vec3 vtfNormalWorld;
out vec3 vtfNormalModel;
out vec3 vtfVertexPositionWorld;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;

void main()
{
  gl_Position = modelToProjectionMatrix * vertexPositionModel;
  vtfColor = vertexColor;
  vtfTexCoords = texCoords;
  vtfBackColor = backColor;
  vtfBackTexCoords = backTexCoords;
  vtfFlags = flags;
  
  vtfNormalModel = normalModel;
  vtfNormalWorld = normalize(vec3(modelToWorldMatrix * vec4(normalModel, 0)));
  vtfVertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModel);
}