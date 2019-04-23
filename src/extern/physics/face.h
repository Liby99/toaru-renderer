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

    // Points on the faces
    std::vector<std::shared_ptr<Point>> points;

    std::shared_ptr<Tetrahedron> t1;

    // Opposite points
    std::shared_ptr<Point> p1;

    Face(std::initializer_list<std::shared_ptr<Point>> points);

    virtual Vector3f getNormal(const std::shared_ptr<Tetrahedron> &t);
    virtual std::shared_ptr<Point> getOppositePoint(const std::shared_ptr<Tetrahedron> &t) const;

    virtual void updateNormal();
  };
}

#endif
