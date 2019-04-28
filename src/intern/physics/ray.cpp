#include "physics/ray.h"

using namespace toaru;

Ray::Ray(Vector3f origin, Vector3f direction) : origin(origin), direction(direction) {}

Vector3f Ray::getPoint(float t) const {
  return origin + t * direction;
}