#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "adhoc/raytrace/intersection.h"
#include "adhoc/raytrace/ray.h"
#include <vector>

namespace nash {
  class Triangle {
  public:
    Triangle(const Vector3f a, const Vector3f b, const Vector3f c);

    Triangle(const std::initializer_list<Vector3f> vertices);

    bool intersect(const Ray &ray, Intersection &intersection);

  private:
    void calculateNormal();

    std::vector<Vector3f> vertices;
    Vector3f normal;
  };
}

#endif
