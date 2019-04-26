#include "physics/face.h"

using namespace toaru;

Face::Face(Point &p0, Point &p1, Point &p2, Point &opposite) : p1(opposite), internal(false) {
  points.insert(points.end(), {&p0, &p1, &p2});

  // e1 = v1 - v0; e2 = v2 - v0; normal = e1 x e2;
  Vector3f e1 = p1.position - p0.position, e2 = p2.position - p0.position;

  restNormal = e1.cross(e2);
  restArea = restNormal.norm() / 2.0;
}

bool Face::isInternal() const {
  return internal;
}

Vector3f Face::getNormal() const {
  Point &p0 = *points[0], &p1 = *points[1], &p2 = *points[2];
  Vector3f u = p1.position - p0.position, v = p2.position - p0.position;
  return u.cross(v).normalized();
}

Point &Face::getOppositePoint() const {
  return p1;
}