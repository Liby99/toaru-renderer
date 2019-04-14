#version 410

in vec3 texCoord;

out vec4 finalColor;

uniform samplerCube cubeMap;

void main() {
  finalColor = texture(cubeMap, texCoord);
}