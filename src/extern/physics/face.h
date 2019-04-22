#ifndef TOARU_FACE_H
#define TOARU_FACE_H

#include "utility/math.h"

#include "point.h"
#include "tetrahedron.h"

#include <execution>
#include <memory>

namespace toaru {
  class Tetrahedron;

  class Face {
  public:
    // List of all faces
    static std::vector<std::shared_ptr<Face>> faces;

    float area;
    Vector3f normal;
    std::vector<std::shared_ptr<Point>> points;

    std::shared_ptr<Tetrahedron> t1;
    std::shared_ptr<Tetrahedron> t2 = nullptr;

    std::shared_ptr<Point> p1;
    std::shared_ptr<Point> p2 = nullptr;

    static std::tuple<bool, std::shared_ptr<Face>> getFace(
      std::initializer_list<std::shared_ptr<Point>> points);

    Face(std::initializer_list<std::shared_ptr<Point>> points);

    virtual bool operator==(const Face &other);

    virtual bool isInternalFace() const;

    virtual Vector3f getNormal(const std::shared_ptr<Tetrahedron> &t);
    virtual std::shared_ptr<Point> getOppositePoint(const std::shared_ptr<Tetrahedron> &t) const;

  private:
    virtual void updateNormal();
  };
}

#endif
