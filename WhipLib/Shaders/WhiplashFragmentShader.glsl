#version 430

in vec3 vtfColor;
in vec2 vtfTexCoords;
in vec3 vtfBackColor;
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
      daColor = vec4(vtfColor, 1);
    } else {
      daColor = texture(textureSlot, vtfTexCoords);
    }
    if (vtfFlags.y > 0)
      daColor.a = vtfFlags.y;
  } else {
    if (vtfFlags.z != 0) {
      daColor = vec4(vtfBackColor, 1);
    } else {
      daColor = texture(textureSlot, vtfBackTexCoords);
    }
    if (vtfFlags.a > 0)
      daColor.a = vtfFlags.a;
  }
}