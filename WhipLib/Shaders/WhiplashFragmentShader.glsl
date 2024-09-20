#version 430

in vec2 vtfTexCoords;
in vec3 vtfNormalModel;
in vec3 vtfNormalWorld;
in vec3 vtfVertexPositionWorld;

out vec4 daColor;

uniform sampler2D textureSlot;
uniform vec3 eyePositionWorld;

void main()
{
  vec3 eyeVectorWorld = normalize(eyePositionWorld - vtfVertexPositionWorld);
  float s = dot(vtfNormalWorld, eyeVectorWorld);

  daColor = texture(textureSlot, vtfTexCoords);
}