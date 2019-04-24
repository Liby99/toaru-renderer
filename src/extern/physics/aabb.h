#ifndef TOARU_PHYSICS_AABB_H
#define TOARU_PHYSICS_AABB_H

#include "utility/math.h"
#include "tetrahedron.h"
#include "ray.h"

namespace toaru {
  class AxisAlignedBoundingBox {
  public:
    bool initialized;
    Vector3f minCorner, maxCorner;

    AxisAlignedBoundingBox();

    void extend(Vector3f p);
    void extend(const Tetrahedron &tetra);
    void extend(const AxisAlignedBoundingBox &aabb);

    bool intersect(const AxisAlignedBoundingBox &other);
    bool intersect(const Ray &ray);

    static Vector3f minVec(Vector3f a, Vector3f b);
    static Vector3f maxVec(Vector3f a, Vector3f b);
  };

  typedef AxisAlignedBoundingBox AABB;
}

#endif