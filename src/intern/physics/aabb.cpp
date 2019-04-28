#include "physics/aabb.h"

using namespace toaru;

AxisAlignedBoundingBox::AxisAlignedBoundingBox() : initialized(false) {}

void AxisAlignedBoundingBox::reset() {
  initialized = false;
}

void AxisAlignedBoundingBox::extend(const Vector3f &p) {
  if (!initialized) {
    minCorner = p;
    maxCorner = p;
    initialized = true;
  } else {
    minCorner = minVec(minCorner, p);
    maxCorner = maxVec(maxCorner, p);
  }
}

void AxisAlignedBoundingBox::extend(const Tetrahedron &tetra) {
  extend(tetra.getPoint(0).position);
  extend(tetra.getPoint(1).position);
  extend(tetra.getPoint(2).position);
  extend(tetra.getPoint(3).position);
}

void AxisAlignedBoundingBox::extend(const AxisAlignedBoundingBox &aabb) {
  extend(aabb.minCorner);
  extend(aabb.maxCorner);
}

bool AxisAlignedBoundingBox::isInside(const Vector3f &p) const {
  return p(0) > minCorner(0) && p(0) < maxCorner(0) &&
         p(1) > minCorner(1) && p(1) < maxCorner(1) &&
         p(2) > minCorner(2) && p(2) < maxCorner(2);
}

bool AxisAlignedBoundingBox::intersect(const AxisAlignedBoundingBox &other) const {
  Vector3f nmin = maxVec(minCorner, other.minCorner);
  Vector3f nmax = minVec(maxCorner, other.maxCorner);
  Vector3f diagnal = nmax - nmin;
  return diagnal.x() > 0 && diagnal.y() > 0 && diagnal.z() > 0;
}

bool AxisAlignedBoundingBox::intersect(const Ray &ray) const {

  // Calculate the intersections
  Vector3f t1, t2;
  for (int i = 0; i < 3; ++i) {
    t1(i) = ((minCorner[i] - ray.origin(i)) / ray.direction(i));
    t2(i) = ((maxCorner[i] - ray.origin(i)) / ray.direction(i));
  }
  
  // Calculate the t min max
  float tmin, tmax;
  tmin = fmaxf(fmaxf(fminf(t1(0), t2(0)), fminf(t1(1), t2(1))), fminf(t1(2), t2(2)));
  tmax = fminf(fminf(fmaxf(t1(0), t2(0)), fmaxf(t1(1), t2(1))), fmaxf(t1(2), t2(2)));
  
  // Check t
  return (tmin > 0 && tmin <= tmax) || (tmin < 0 && tmax > 0);
}

bool AxisAlignedBoundingBox::intersect(const Tetrahedron &tetra) const {
  return isInside(tetra.getPoint(0).position) || 
         isInside(tetra.getPoint(1).position) ||
         isInside(tetra.getPoint(2).position) ||
         isInside(tetra.getPoint(3).position);
}

Vector3f AxisAlignedBoundingBox::getDimension() const {
  return maxCorner - minCorner;
}

Vector3f AxisAlignedBoundingBox::minVec(Vector3f a, Vector3f b) {
  return Vector3f(fminf(a.x(), b.x()), fminf(a.y(), b.y()), fminf(a.z(), b.z()));
}

Vector3f AxisAlignedBoundingBox::maxVec(Vector3f a, Vector3f b) {
  return Vector3f(fmaxf(a.x(), b.x()), fmaxf(a.y(), b.y()), fmaxf(a.z(), b.z()));
}