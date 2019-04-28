#ifndef TOARU_PHYSICS_AABB_H
#define TOARU_PHYSICS_AABB_H

#include "ray.h"
#include "tetrahedron.h"
#include "utility/math.h"

namespace toaru {
  class AxisAlignedBoundingBox {
  public:
    bool initialized;
    Vector3f minCorner, maxCorner;

    AxisAlignedBoundingBox();

    void reset();

    void extend(const Vector3f &p);
    void extend(const Tetrahedron &tetra);
    void extend(const AxisAlignedBoundingBox &aabb);

    bool isInside(const Vector3f &p) const;
    bool intersect(const AxisAlignedBoundingBox &other) const;
    bool intersect(const Ray &ray) const;
    bool intersect(const Tetrahedron &tetra) const;

    Vector3f getDimension() const;

    static Vector3f minVec(Vector3f a, Vector3f b);
    static Vector3f maxVec(Vector3f a, Vector3f b);
  };

  typedef AxisAlignedBoundingBox AABB;
}

#endif