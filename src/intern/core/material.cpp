#include "core/material.h"

using namespace toaru;

Material::Material() : Material(Shader::get()) {}

Material::Material(Shader &shader) : shader(&shader), Component() {}

void Material::init() {
  shader->init();
}

void Material::prerender() {
  // Do nothing
}

Shader &Material::getShader() {
  return *shader;
}