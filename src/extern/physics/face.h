#ifndef TOARU_FACE_H
#define TOARU_FACE_H

#include "utility/math.h"

#include "point.h"
#include "tetrahedron.h"

#include <memory>

namespace toaru {
  class Tetrahedron;

  class Face {
  public:
    float area;
    Vector3f normal;

    bool internal;

    // Points on the faces
    std::vector<Point *> points;

    std::shared_ptr<Tetrahedron> t1;

    // Opposite points
    Point& p1;

    Face(Point &p0, Point &p1, Point &p2, Point &opposite);

    bool isInternal() const;
    Vector3f getNormal() const;
    Vector3f getCurrentNormal() const;
    Point & getOppositePoint() const;
    void updateNormal();
  };
}

#endif
