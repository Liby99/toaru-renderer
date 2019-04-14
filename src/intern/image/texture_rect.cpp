#include "image/texture_rect.h"

using namespace nash;

TextureRect::TextureRect(const Image &image) : Texture(), image(&image) {}

void TextureRect::init() {
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_RECTANGLE, textureId);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image->data);
}

void TextureRect::bind(GLuint pos) const {
  Texture::bind(pos);
  glBindTexture(GL_TEXTURE_RECTANGLE, textureId);
}