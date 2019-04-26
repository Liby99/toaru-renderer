#include "physics/face.h"

using namespace toaru;

Face::Face(Point &p0, Point &p1, Point &p2, Point &opposite) : p1(opposite), internal(false) {
  this->points.insert(this->points.end(), {&p0, &p1, &p2});
  updateNormal();
}

bool Face::isInternal() const {
  return internal;
}

Vector3f Face::getNormal() const {
  return normal;
}

Point &Face::getOppositePoint() const {
  return p1;
}

Vector3f Face::getCurrentNormal() const {
  Point &p0 = *points[0], &p1 = *points[1], &p2 = *points[2];
  Vector3f u = p1.position - p0.position, v = p2.position - p0.position;
  return u.cross(v).normalized();
}

void Face::updateNormal() {
  // e1 = v1 - v0; e2 = v2 - v0; normal = e1 x e2;
  Vector3f e1 = this->points[1]->position - this->points[0]->position;
  Vector3f e2 = this->points[2]->position - this->points[0]->position;

  this->normal = e1.cross(e2);
  this->area = this->normal.norm() / 2.0;
}