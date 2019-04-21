#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "adhoc/raytrace/intersection.h"
#include "adhoc/raytrace/ray.h"
#include <float.h>
#include <vector>

namespace nash {
  class BoundingBox {
  public:
    BoundingBox(const Vector3f min, const Vector3f max);

    BoundingBox(const std::initializer_list<Vector3f> vertices);

    BoundingBox(const std::vector<Vector3f> vertices);

    bool intersect(const Ray &ray, Intersection &intersection);

    const std::vector<Vector3f> &getVertices_() const;
    const Vector3f &getExtents() const;
    const Vector3f &getCenter() const;

  private:
    void calculateBoundingBox();

    std::vector<Vector3f> vertices_;
    Vector3f extents;
    Vector3f center;
  };
}

#endif
