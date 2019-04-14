#version 410

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoord;
layout(location=3) in vec3 color;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoord;
out vec3 fragColor;

uniform mat4 model = mat4(1);
uniform mat4 viewPersp = mat4(1);

void main() {
  mat4 mvp = viewPersp * model;
  gl_Position = mvp * vec4(position, 1);
  fragPosition = vec3(model * vec4(position, 1));
  fragNormal = vec3(transpose(inverse(model)) * vec4(normal, 0));
  fragTexCoord = texCoord;
  fragColor = color;
}
