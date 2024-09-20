#version 430

in vec2 vtfTexCoords;
in vec2 vtfBackTexCoords;
in vec3 vtfNormalModel;
in vec3 vtfNormalWorld;
in vec3 vtfVertexPositionWorld;
in float vtfAlpha;

out vec4 daColor;

uniform sampler2D textureSlot;
uniform vec3 eyePositionWorld;

void main()
{
  vec3 eyeVectorWorld = normalize(eyePositionWorld - vtfVertexPositionWorld);
  float s = dot(vtfNormalWorld, eyeVectorWorld);

  if (s > 0 || vtfNormalModel == vec3(0, 0, 0)) {
    daColor = texture(textureSlot, vtfTexCoords);
  } else {
    daColor = texture(textureSlot, vtfBackTexCoords);
  }
  daColor.a = vtfAlpha;
}