#ifndef TOARU_LAMBERTIAN_H
#define TOARU_LAMBERTIAN_H

#include "core/material.h"

namespace toaru {
  class Lambertian : public Material {
  public:
    const static std::string DEFAULT_DIFFUSE_NAME, DEFAULT_AMBIENT_NAME;
    const static Vector3f DEFAULT_DIFFUSE_COLOR, DEFAULT_AMBIENT_COLOR;

    Vector3f diffuseColor, ambientColor;

    Lambertian();
    Lambertian(Shader &shader);

    virtual void prerender();
  };
}

#endif