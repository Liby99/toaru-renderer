#include "material/lambertian.h"

using namespace toaru;

const std::string Lambertian::DEFAULT_DIFFUSE_NAME = "diffuseColor";

const std::string Lambertian::DEFAULT_AMBIENT_NAME = "ambientColor";

const Vector3f Lambertian::DEFAULT_DIFFUSE_COLOR = Vector3f(0.8f, 0.8f, 0.8f);

const Vector3f Lambertian::DEFAULT_AMBIENT_COLOR = Vector3f(0.1f, 0.1f, 0.1f);

Lambertian::Lambertian() : Lambertian(Shader::get()) {}

Lambertian::Lambertian(Shader &shader)
    : diffuseColor(DEFAULT_DIFFUSE_COLOR), ambientColor(DEFAULT_AMBIENT_COLOR), Material(shader) {}

void Lambertian::prerender() {
  shader->setUniform(DEFAULT_DIFFUSE_NAME, diffuseColor);
  shader->setUniform(DEFAULT_AMBIENT_NAME, ambientColor);
}