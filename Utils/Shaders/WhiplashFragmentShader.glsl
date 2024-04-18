#version 430

in vec3 vertexToFragmentColor;
in vec2 vertexToFragmentTexCoords;
in vec3 vertexToFragmentFlags;

out vec4 daColor;

uniform sampler2D textureSlot;

void main()
{
  vec4 texColor = texture(textureSlot, vertexToFragmentTexCoords);
  if (vertexToFragmentFlags.x == 1)
	daColor = vec4(vertexToFragmentColor, 1);
  else
	daColor = texColor;
}