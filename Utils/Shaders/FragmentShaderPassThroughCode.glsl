#version 430

in vec3 vertexToFragmentColor;
in vec2 vertexToFragmentTexCoords;

out vec4 daColor;

uniform sampler2D textureSlot;

void main()
{
  vec4 texColor = texture(textureSlot, vertexToFragmentTexCoords);
  daColor = texColor;  //vec4(vertexToFragmentColor, 1);
}