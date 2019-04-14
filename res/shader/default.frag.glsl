#version 410

in vec3 fragPosition;
in vec3 fragNormal;

out vec4 finalColor;

uniform vec3 AmbientColor = vec3(0.3);
uniform vec3 LightDirection = normalize(vec3(1, 5, 2));
uniform vec3 LightColor = vec3(0.8);
uniform vec3 DiffuseColor = vec3(0.3);

void main() {
  vec3 irradiance = AmbientColor + LightColor * max(0, dot(LightDirection, fragNormal));
  vec3 reflectance = irradiance * DiffuseColor;
  finalColor = vec4(sqrt(reflectance), 1);
}
