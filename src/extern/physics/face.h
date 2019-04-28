#ifndef TOARU_FACE_H
#define TOARU_FACE_H

#include "utility/math.h"
#include "point.h"
#include "ray.h"
#include <memory>

namespace toaru {
  class Tetrahedron;

  class Face {
  public:
    float restArea;
    Vector3f restNormal;

    bool internal;

    // Points on the faces
    std::vector<Point *> points;

    std::shared_ptr<Tetrahedron> t1;

    // Opposite points
    Point& p1;

    Face(Point &p0, Point &p1, Point &p2, Point &opposite);

    bool isInternal() const;
    Vector3f getNormal() const;
    Point &getPoint(int i) const;
    Point &getOppositePoint() const;

    /**
     * Test if this face intersects ray.
     * If not intersect, will return false, and the pos, normal remains as is
     * If is intersecting:
     *   return true
     *   pos <= intersection position
     *   normal <= face normal
     */
    bool intersect(const Ray &ray, Vector3f &pos, Vector3f &normal) const;

    bool isAtBackSide(const Vector3f &p) const;
  };
}

#endif
