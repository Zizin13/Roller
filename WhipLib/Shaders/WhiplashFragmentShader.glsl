#version 430

in vec4 vtfColor;
in vec2 vtfTexCoords;
in vec4 vtfBackColor;
in vec2 vtfBackTexCoords;
in vec4 vtfFlags;
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

  if (s > 0 || vtfNormalModel == vec3(0, 0, 0)) {
    if (vtfFlags.x != 0) {
      daColor = vtfColor;
    } else {
      daColor = texture(textureSlot, vtfTexCoords);
    }
  } else {
    if (vtfFlags.y != 0) {
      daColor = vtfBackColor;
    } else {
      daColor = texture(textureSlot, vtfBackTexCoords);
    }
  }
}