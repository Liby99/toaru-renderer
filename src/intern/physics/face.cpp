#include "physics/face.h"
#include "physics/tetrahedron.h"

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

Point &Face::getPoint(int i) const {
  return *points[i];
}

bool Face::intersect(const Ray &ray, Vector3f &pos, Vector3f &norm) const {
  // Pre cache the positions
  Vector3f &p0 = getPoint(0).position, &p1 = getPoint(1).position, &p2 = getPoint(2).position;
  Vector3f t0 = p1 - p0, t1 = p2 - p0;
  
  // Calculate normal
  Vector3f normal = t0.cross(t1).normalized();
  
  // Calculate t
  float t = (p0.dot(normal) - ray.origin.dot(normal)) / ray.direction.dot(normal);
  
  // Pre cache the position of the intersection
  Vector3f position = ray.getPoint(t);

  // Get the bary centric position
  Vector3f t2 = position - p0;
  float d00 = t0.dot(t0), d01 = t0.dot(t1), d11 = t1.dot(t1), d20 = t2.dot(t0), d21 = t2.dot(t1);
  float denom = d00 * d11 - d01 * d01;
  float bcx = (d11 * d20 - d01 * d21) / denom, bcy = (d00 * d21 - d01 * d20) / denom;
  float bcz = 1.0f - bcx - bcy;
  
  // Check if t is greater then 0 and the position is inside the triangle and need update
  if (t > 0 && bcx >= 0 && bcx <= 1 && bcy >= 0 && bcy <= 1 && bcz >= 0 && bcz <= 1) {
    // Intersects
    pos = position;
    norm = normal;
    return true;
  } else {
    return false;
  }
}

bool Face::isAtBackSide(const Vector3f &p) const {
  return (p - points[0]->position).dot(getNormal()) < 0;
}