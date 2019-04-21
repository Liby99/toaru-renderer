#include "image/texture.h"

using namespace nash;

void Texture::init() {}

void Texture::bind(GLuint pos) const { glActiveTexture(GL_TEXTURE0 + pos); }

void Texture::initAll() {
  for (int i = 0; i < textures.size(); i++) {
    textures[i]->init();
  }
}

Texture::Texture() : textureId(0) { textures.push_back(this); }

std::vector<Texture *> Texture::textures = std::vector<Texture *>();