#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "adhoc/raytrace/ray.h"

namespace nash {
  class Intersection {
  public:
    bool hit;
    Vector3f position, normal;
    float t;
    const Ray *ray;

    Intersection(const Ray &ray);
    void update(Vector3f position);
  };
}

#endif
