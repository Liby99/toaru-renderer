#version 410

layout(location=0) in vec3 point;
layout(location=1) in vec3 color;

out vec3 fragColor;

uniform mat4 model = mat4(1);
uniform mat4 viewPersp = mat4(1);

void main() {
  gl_Position = viewPersp * model * vec4(point, 1);
  fragColor = color;
}