#version 410

layout(location = 0) in vec3 position;

out vec3 texCoord;

uniform mat4 viewPersp = mat4(1);
uniform mat4 model = mat4(1);

void main() {
  texCoord = position;
  gl_Position = viewPersp * model * vec4(position, 1);
}