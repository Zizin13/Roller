#version 430

in layout(location=0) vec3 vertexPositionModel;
in layout(location=1) vec3 normalModel;
in layout(location=2) vec2 texCoords;

out vec2 vtfTexCoords;
out vec3 vtfNormalWorld;
out vec3 vtfNormalModel;
out vec3 vtfVertexPositionWorld;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;

void main()
{
  gl_Position = modelToProjectionMatrix * vec4(vertexPositionModel, 1);
  vtfTexCoords = texCoords;
  
  vtfNormalModel = normalModel;
  vtfNormalWorld = normalize(vec3(modelToWorldMatrix * vec4(normalModel, 0)));
  vtfVertexPositionWorld = vec3(modelToWorldMatrix * vec4(vertexPositionModel, 1));
}