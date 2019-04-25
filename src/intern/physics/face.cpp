#include "physics/face.h"

using namespace toaru;

Vector3f Face::getNormal() {
  return normal;
}

Point &Face::getOppositePoint() const {
  return p1;
}

void Face::updateNormal() {
  // e1 = v1 - v0; e2 = v2 - v0; normal = e1 x e2;
  Vector3f e1 = this->points[1]->position - this->points[0]->position;
  Vector3f e2 = this->points[2]->position - this->points[0]->position;

  this->normal = e1.cross(e2);
  this->area = this->normal.norm() / 2.0;
}

Face::Face(Point &p0, Point &p1, Point &p2, Point &opposite)
  : p1(opposite) {
  this->points.insert(this->points.end(), {&p0, &p1, &p2});
}
