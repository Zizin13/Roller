#version 430

out vec4 daColor;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform vec3 lightPositionWorld;
uniform vec4 ambientLight;

void main()
{
  vec3 lightVector = normalize(lightPositionWorld - vertexPositionWorld);
  float brightness = dot(lightVector, normalize(normalWorld));
  vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);
  daColor = clamp(diffuseLight, 0, 1) + ambientLight;
}