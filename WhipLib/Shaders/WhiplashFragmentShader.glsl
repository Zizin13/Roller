#version 430

in vec3 vertexToFragmentColor;
in vec2 vertexToFragmentTexCoords;
in vec3 vertexToFragmentFlags;

out vec4 daColor;

uniform sampler2D textureSlot;

void main()
{
  if (vertexToFragmentFlags.x != 0) {
    daColor = vec4(vertexToFragmentColor, 1);
  } else {
    daColor = texture(textureSlot, vertexToFragmentTexCoords);
  }
  if (vertexToFragmentFlags.y > 0)
    daColor.a = vertexToFragmentFlags.y;
}